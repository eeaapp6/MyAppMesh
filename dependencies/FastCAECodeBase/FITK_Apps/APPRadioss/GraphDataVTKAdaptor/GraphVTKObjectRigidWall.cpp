/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectRigidWall.h"
#include "GraphVTKCommons.h"
#include "GraphVTKObjectHighlighting.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkProperty.h>
#include <vtkCylinderSource.h>
#include <vtkTriangleFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>

namespace Exchange
{
    GraphVTKObjectRigidWall::GraphVTKObjectRigidWall(Radioss::FITKAbstractRWall* wall)
        : GraphVTKObjectModelBase(wall)
    {
        if (wall == nullptr) return;

        init();
        initActor();
        updateGraphData();
    }

    GraphVTKObjectRigidWall::~GraphVTKObjectRigidWall()
    {
        deleteVtkObj(_meshCellData);
        if (_feature != nullptr) {
            delete _feature;
            _feature = nullptr;
        }
    }

    void GraphVTKObjectRigidWall::init()
    {
        Radioss::FITKAbstractRWall* wall = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (wall == nullptr) return;

        _meshCellData = vtkUnstructuredGrid::New();
        _meshCellData->SetPoints(vtkSmartPointer<vtkPoints>::New());
    }

    void GraphVTKObjectRigidWall::initActor()
    {
        Radioss::FITKAbstractRWall* wall = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (wall == nullptr) return;

        this->setRenderLayer(0);

        // 创建单元演员 - 主要显示墙面
        _meshCellActor = FITKGraphActor::New();
        _meshCellActor->SetPickable(false);
        _meshCellActor->setGraphObject(this);
        _meshCellActor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _meshCellActor->GetProperty()->SetEdgeVisibility(false);
        _meshCellActor->GetProperty()->SetOpacity(1);
        _meshCellActor->setInputDataObject(_meshCellData);

        // 设置正面颜色为灰色，背面颜色为黑
        _meshCellActor->setFrontFaceColor(QColor(224, 224, 224));
        _meshCellActor->setBackFaceColor(QColor(0, 0, 0));
        _meshCellActor->setEnableBackFaceColor(true);

        _meshCellActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_rigidWallGraphOffset);
        addActor(_meshCellActor);
        setLightPropertiesForSurface(_meshCellActor);
        // 更新特征边
        _feature = new Interface::FITKMeshFeatureVTK(_meshCellData);
        m_graphObjHighlight->setModelInputConnection(_feature->getFeatureEdgeFilter()->GetOutputPort());
        m_graphObjHighlight->setActorType(-1);

    }

    bool GraphVTKObjectRigidWall::updateGraphData()
    {
        // 首先完全清理所有数据
        if (_meshCellData) {
            _meshCellData->Reset();
            _meshCellData->GetPointData()->Reset();
            _meshCellData->GetCellData()->Reset();
        }
        // 重新创建单元数据
        createCellGrid();

        // 强制更新所有 Actor 的输入数据
        if (_meshCellActor) {
            _meshCellActor->Modified();
        }

        if (_feature)
        {
            _feature->update();
        }

        return true;
    }
    void GraphVTKObjectRigidWall::createCellGrid()
    {
        Radioss::FITKAbstractRWall* wall = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (!wall) return;

        switch (wall->getType())
        {
        case Radioss::FITKAbstractRWall::RWallPLANE:
            calculateWallCornerPoints();
            break;
        case Radioss::FITKAbstractRWall::RWallCYL:
            calculateCylinderWall();
            break;
        default:
            // 其他类型暂不支持
            _meshCellData->Reset();
            break;
        }
    }
    void GraphVTKObjectRigidWall::update(bool forceUpdate)
    {
        updateVisibility();
        if (!forceUpdate) return;

        createCellGrid();

        // 通知VTK数据已更新
        if (_meshCellData) {
            _meshCellData->Modified();
        }
        if (_feature)
        {
            _feature->update();
        }
    }

    void GraphVTKObjectRigidWall::setVisible(bool isVisible)
    {
        _meshCellActor->SetVisibility(isVisible);
    }

    bool GraphVTKObjectRigidWall::getVisibility()
    {
        Radioss::FITKAbstractRWall* wall = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (wall == nullptr) {
            return false;
        }
        return this->Superclass::getVisibility();
    }

    void GraphVTKObjectRigidWall::updateVisibility()
    {
        if (_meshCellActor == nullptr) return;

        bool isVisible = getVisibility();
        _meshCellActor->SetVisibility(isVisible);
    }

    vtkDataSet* GraphVTKObjectRigidWall::getMesh(GraphVTKCommons::ShapePickMode pickType)
    {
        switch (pickType) {
        case GraphVTKCommons::ShapePickMode::PickMeshCell:
            return _meshCellData;
        }
        return nullptr;
    }

    bool GraphVTKObjectRigidWall::calculateWallCornerPoints()
    {
        Radioss::FITKRWallPlane* plane = dynamic_cast<Radioss::FITKRWallPlane*>(_dataObj);
        if (!plane) return false;

        // 获取 meshModel 以获取包围盒信息
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!radiossCase) return false;

        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (!meshModel) return false;

        // 获取节点包围盒
        double bndBox[6] = { 0.0 }; // xmin, xmax, ymin, ymax, zmin, zmax
        meshModel->getNodeBndBox(bndBox);

        // 计算包围盒尺寸
        double width = bndBox[1] - bndBox[0];   // x方向
        double height = bndBox[3] - bndBox[2];  // y方向
        double depth = bndBox[5] - bndBox[4];   // z方向

        // 使用包围盒的体对角线长度作为墙体的尺寸
        double wallSize = std::sqrt(width * width + height * height + depth * depth);
        if (wallSize < 1e-6) wallSize = 1.0; // Avoid zero size

        // 获取平面基点和法向量
        double refPoint[3], normal[3];
        plane->getBasicPoint(refPoint[0], refPoint[1], refPoint[2]);
        plane->getNormalVector(normal[0], normal[1], normal[2]);

        Core::FITKPoint referencePoint(refPoint[0], refPoint[1], refPoint[2]);
        Core::FITKVec3D normalVector(normal[0], normal[1], normal[2]);
        normalVector.normalize();

        Core::FITKPoint bboxCenter(
            (bndBox[0] + bndBox[1]) * 0.5,
            (bndBox[2] + bndBox[3]) * 0.5,
            (bndBox[4] + bndBox[5]) * 0.5
        );

        Core::FITKPoint projectedCenter = Core::ProjectPointToAPlane(bboxCenter, referencePoint, normalVector);

        Core::FITKVec3D u(0.0, 0.0, 0.0), v(0.0, 0.0, 0.0);
        Core::FITKVec3D temp(1.0, 0.0, 0.0);
        if (std::abs(Core::DotProduct(normalVector, temp)) > 0.9) {
            temp = Core::FITKVec3D(0.0, 1.0, 0.0);
        }
        u = Core::CrossProduct(normalVector, temp);
        u.normalize();
        v = Core::CrossProduct(normalVector, u);
        v.normalize();

        Core::FITKPoint corners[4];
        double halfSize = wallSize * 0.5;
        corners[0] = Core::Add(Core::Add(projectedCenter, u, 1.0, halfSize), v, 1.0, halfSize);
        corners[1] = Core::Add(Core::Add(projectedCenter, u, 1.0, -halfSize), v, 1.0, halfSize);
        corners[2] = Core::Add(Core::Add(projectedCenter, u, 1.0, -halfSize), v, 1.0, -halfSize);
        corners[3] = Core::Add(Core::Add(projectedCenter, u, 1.0, halfSize), v, 1.0, -halfSize);

        // 创建网格
        _meshCellData->Reset();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        for (int i = 0; i < 4; i++) {
            points->InsertNextPoint(corners[i].x(), corners[i].y(), corners[i].z());
        }
        _meshCellData->SetPoints(points);

        vtkSmartPointer<vtkIdList> quad = vtkSmartPointer<vtkIdList>::New();
        for (int i = 0; i < 4; i++) quad->InsertNextId(i);
        _meshCellData->InsertNextCell(VTK_QUAD, quad);

        vtkSmartPointer<vtkIntArray> cellID = vtkSmartPointer<vtkIntArray>::New();
        cellID->SetName(GraphVTKCommons::_dataCellIDArratName.toUtf8().constData());
        cellID->InsertNextValue(plane->getDataObjectID());
        _meshCellData->GetCellData()->AddArray(cellID);

        return true;
    }

    bool GraphVTKObjectRigidWall::calculateCylinderWall()
    {
        Radioss::FITKAbstractRWall* cyl = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (!cyl) return false;

        if (cyl->getType() != Radioss::FITKAbstractRWall::RWallType::RWallCYL) {
            return false;
        }
        Radioss::FITKRWallCylinder* cylWall = dynamic_cast<Radioss::FITKRWallCylinder*>(cyl);
        if (!cylWall) return false;

        // 获取模型包围盒
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!radiossCase) return false;
        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (!meshModel) return false;
        double bndBox[6];
        double minBndBox[3];
        double maxBndBox[3];
        meshModel->getNodeBndBox(bndBox);

        minBndBox[0] = bndBox[0]; minBndBox[1] = bndBox[2]; minBndBox[2] = bndBox[4];
        maxBndBox[0] = bndBox[1]; maxBndBox[1] = bndBox[3]; maxBndBox[2] = bndBox[5];

        // 计算包围盒尺寸
        double width = bndBox[1] - bndBox[0];   // x方向
        double height = bndBox[3] - bndBox[2];  // y方向
        double depth = bndBox[5] - bndBox[4];   // z方向

        // 使用包围盒的体对角线长度作为墙体的尺寸
        double wallSize = std::sqrt(width * width + height * height + depth * depth);

        double refPoint[3], normal[3], central[3], m1[3];
        cylWall->getBasicPoint(refPoint[0], refPoint[1], refPoint[2]);
        cylWall->getAxisVector(normal[0], normal[1], normal[2]);

        //计算墙体中心点
        m1[0] = refPoint[0] + normal[0];
        m1[1] = refPoint[1] + normal[1];
        m1[2] = refPoint[2] + normal[2];

        //获取实际圆柱顶点和底点
        bool value = cylWall->calcBoxInterPoint(refPoint, m1, minBndBox, maxBndBox);

        central[0] = refPoint[0] + (m1[0] - refPoint[0]) * 0.5;
        central[1] = refPoint[1] + (m1[1] - refPoint[1]) * 0.5;
        central[2] = refPoint[2] + (m1[2] - refPoint[2]) * 0.5;

        Core::FITKPoint referencePoint(refPoint[0], refPoint[1], refPoint[2]);
        Core::FITKVec3D normalVector(normal[0], normal[1], normal[2]);
        normalVector.normalize();
        double diameter = cyl->getDiameter();

        vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
        cylinderSource->SetRadius(diameter / 2.0);
        cylinderSource->SetHeight(wallSize);
        cylinderSource->SetResolution(50); // 分段数，可以调整以获得更平滑的曲面
        cylinderSource->CappingOn(); // 打开顶盖和底盖
        cylinderSource->Update();

        // 默认圆柱体方向为 Y 轴
        double defaultAxis[3] = { 0.0, 1.0, 0.0 };
        double targetAxis[3] = { normalVector.x(), normalVector.y(), normalVector.z() };

        // 计算旋转轴和旋转角度
        double rotationAxis[3];
        vtkMath::Cross(defaultAxis, targetAxis, rotationAxis);
        double rotationAngle = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(defaultAxis, targetAxis)));

        // 创建变换
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->RotateWXYZ(rotationAngle, rotationAxis);
        transform->Translate(central[0], central[1], central[2]);


        // 应用变换
        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        transformFilter->SetInputData(cylinderSource->GetOutput());
        transformFilter->SetTransform(transform);
        transformFilter->Update();

        // 计算法线以实现平滑着色
        vtkSmartPointer<vtkPolyDataNormals> normalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
        normalsFilter->SetInputConnection(transformFilter->GetOutputPort());
        normalsFilter->ComputePointNormalsOn();
        normalsFilter->ComputeCellNormalsOff();
        normalsFilter->SplittingOff();
        normalsFilter->Update();

        // 将PolyData转换为UnstructuredGrid
        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(normalsFilter->GetOutputPort());
        triangleFilter->Update();

        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        grid->SetPoints(triangleFilter->GetOutput()->GetPoints());
        grid->GetPointData()->SetNormals(triangleFilter->GetOutput()->GetPointData()->GetNormals());
        grid->SetCells(VTK_TRIANGLE, triangleFilter->GetOutput()->GetPolys());

        // 为每个单元格添加ID
        vtkSmartPointer<vtkIntArray> cellID = vtkSmartPointer<vtkIntArray>::New();
        cellID->SetName(GraphVTKCommons::_dataCellIDArratName.toUtf8().constData());
        cellID->SetNumberOfComponents(1);
        int wallId = cyl->getDataObjectID();
        for (vtkIdType i = 0; i < grid->GetNumberOfCells(); ++i)
        {
            cellID->InsertNextValue(wallId);
        }
        grid->GetCellData()->AddArray(cellID);

        _meshCellData->DeepCopy(grid);

        return true;
    }
}