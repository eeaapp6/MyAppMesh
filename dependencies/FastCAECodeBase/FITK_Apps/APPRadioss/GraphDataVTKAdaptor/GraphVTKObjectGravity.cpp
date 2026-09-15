/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectGravity.h"
#include "GraphDataSetCompomentManager.h"
#include "GraphVTKCommons.h"
#include "GraphVTKObjectModelBase.h"
#include "GraphVTKObjectHighlighting.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <algorithm>
#include <cmath>

namespace Exchange
{
    GraphVTKObjectGravity::GraphVTKObjectGravity(Radioss::FITKGravity* dataObj) :
        GraphVTKObjectModelBase(dataObj)
    {
        init();

        initActor();
        if (!generateDataSet()) {
            return;
        }
        generateGraph();

        updateVisibility();
    }

    GraphVTKObjectGravity::~GraphVTKObjectGravity()
    {
        // 清理VTK对象
        if (_arrowData) {
            _arrowData->Delete();
        }
        if (_points) {
            _points->Delete();
        }
        if (_vectors) {
            _vectors->Delete();
        }
        if (_arrowSource) {
            _arrowSource->Delete();
        }
        if (_glyph3D) {
            _glyph3D->Delete();
        }

        // 清理载荷点
        for (double* point : _loadPoints) {
            delete[] point;
        }
        _loadPoints.clear();
    }

    void GraphVTKObjectGravity::update(bool forceUpdate)
    {
        updateVisibility();

        if (!forceUpdate) return;
        // 检查是否需要重新初始化数据点
        bool needReinitPoints = false;

        // 比较当前载荷点数量和预期数量
        Radioss::FITKGravity* gravity = dynamic_cast<Radioss::FITKGravity*>(_dataObj);
        if (gravity) {
            int nodesId = gravity->getNodeGroupID();
            if (nodesId == 0) {
                // 检查包围盒中心点是否变化
                needReinitPoints = checkAndUpdateGlobalCenter();
            }
            else {
                // 集合节点，检查集合成员是否变化
                Interface::FITKModelSet* set = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKModelSet>(nodesId);
                if (set) {
                    if (_prevLoadSet != set)
                        needReinitPoints = true;
                }
            }
        }

        // 如果需要重新初始化点，重建数据集
        if (needReinitPoints) {
            // 清理旧的VTK对象
            if (_arrowData) {
                _arrowData->Delete();
                _arrowData = nullptr;
            }
            if (_points) {
                _points->Delete();
                _points = nullptr;
            }
            if (_vectors) {
                _vectors->Delete();
                _vectors = nullptr;
            }
            if (_arrowSource) {
                _arrowSource->Delete();
                _arrowSource = nullptr;
            }
            if (_glyph3D) {
                _glyph3D->Delete();
                _glyph3D = nullptr;
            }

            // 重新初始化
            init();

            if (!_loadPoints.isEmpty()) {
                if (generateDataSet()) {
                    generateGraph();
                }
            }
        }
        else {
            // 只更新属性，不重建数据集
            updateRenderingProperties();
        }
        
    }

    bool GraphVTKObjectGravity::getVisibility()
    {
        if (!_arrowActor) {
            return false;
        }
        if (_arrowActor->GetVisibility() && _dataObj->isEnable())
            return true;
        
    }

    void GraphVTKObjectGravity::updateVisibility()
    {
        if (_arrowActor) {
            _arrowActor->SetVisibility(getVisibility());
        }
    }

    void GraphVTKObjectGravity::setVisible(bool isVisible)
    {
        if (_arrowActor) {
            _arrowActor->SetVisibility(isVisible);
        }
    }

    void GraphVTKObjectGravity::init()
    {
        // 清理之前的载荷点
        for (double* point : _loadPoints) {
            delete[] point;
        }
        _loadPoints.clear();

        Radioss::FITKGravity* gravity = dynamic_cast<Radioss::FITKGravity*>(_dataObj);
        if (gravity == nullptr) return;

        //获取集合
        int nodesId = gravity->getNodeGroupID();
        if (nodesId == 0)
        {
            //全局节点 - 在包围盒中心渲染重力箭头
            this->setRenderLayer(1);
            Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (radiossCase == nullptr) return;

            Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
            if (meshModel == nullptr) return;

            // 获取模型的包围盒
            double bndBox[6]; // xmin, xmax, ymin, ymax, zmin, zmax
            meshModel->getNodeBndBox(bndBox);

            // 计算包围盒中心点
            double centerPos[3];
            centerPos[0] = (bndBox[0] + bndBox[1]) * 0.5; // x center
            centerPos[1] = (bndBox[2] + bndBox[3]) * 0.5; // y center  
            centerPos[2] = (bndBox[4] + bndBox[5]) * 0.5; // z center

            // 在包围盒中心插入重力箭头点
            insertNextPoint(centerPos);
        }
        else {
            this->setRenderLayer(0);
            Interface::FITKModelSet* set = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKModelSet>(nodesId);
            if (set == nullptr) return;
            _prevLoadSet = set;
            //获取模型
            Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(set->getAbsModelID());
            Radioss::FITKRadiossMeshModel* meshModel = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossMeshModel>(set->getAbsModelID());
            QList<int> members = set->getAbsoluteMember();

            if (part)
            {
                Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
                if (radiossCase == nullptr) return;
                Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
                if (meshModel == nullptr) return;
                Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
                if (partMgr == nullptr) return;
                Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
                if (nodes == nullptr) return;
                int partIndex = GraphDataSetCompomentManager::getPartNodeIdByRadiossNodes(part);
                if (partIndex < 0) return;
                QList<int> partNodeIDs = part->getAllNodeIDs();
                int partNodeCount = partNodeIDs.count();
                if (partNodeCount <= 0) return;
                for (int i = 0; i < members.size(); i++)
                {
                    int nodeId = members.at(i);
                    Core::FITKNode* node = nodes->getNodeByID(partIndex + nodeId);
                    if (node == nullptr) continue;
                    double pos[3] = { 0 };
                    node->getCoor(pos);
                    if (pos == nullptr) continue;
                    insertNextPoint(pos);
                }
            }
            if (meshModel)
            {
                Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
                if (nodes == nullptr) return;
                for (int i = 0; i < members.size(); i++)
                {
                    int nodeId = members.at(i);
                    Core::FITKNode* node = nodes->getNodeByID(nodeId);
                    if (node == nullptr) continue;
                    double pos[3] = { 0 };
                    node->getCoor(pos);
                    if (pos == nullptr) continue;
                    insertNextPoint(pos);
                }
            }
        }
    }

    void GraphVTKObjectGravity::initActor()
    {
        if (_loadPoints.isEmpty()) {
            return;
        }
        // 创建箭头演员
        _arrowActor = FITKGraphActor::New();
        _arrowActor->SetPickable(false);
        _arrowActor->setGraphObject(this);
        _arrowActor->GetProperty()->SetAmbient(1);
        _arrowActor->GetProperty()->SetDiffuse(1);
        _arrowActor->GetProperty()->SetSpecular(1);
        _arrowActor->GetProperty()->SetSpecularPower(64.);
        _arrowActor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
        _arrowActor->GetProperty()->SetColor(1.0, 1.0, 0.0); // 黄色
        _arrowActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_mesh2DGraphOffset);
        addActor(_arrowActor);
    }

    bool GraphVTKObjectGravity::generateDataSet()
    {
        if (_loadPoints.isEmpty()) {
            return false;
        }

        // 创建VTK数据对象
        _arrowData = vtkPolyData::New();
        _points = vtkPoints::New();
        _vectors = vtkFloatArray::New();
        _arrowSource = vtkArrowSource::New();
        _glyph3D = vtkGlyph3D::New();

        // 设置向量数组
        _vectors->SetNumberOfComponents(3);
        _vectors->SetName("GravityVectors");

        // 获取重力方向
        double direction[3];
        getGravityDirectionVector(direction);

        // 添加点和向量
        for (const double* point : _loadPoints) {
            _points->InsertNextPoint(point);
            _vectors->InsertNextTuple3(direction[0], direction[1], direction[2]);
        }

        // 设置点数据
        _arrowData->SetPoints(_points);
        _arrowData->GetPointData()->SetVectors(_vectors);

        // 计算模型包围盒和合适的箭头大小
        double arrowScale = calculateArrowScale();

        // 配置箭头源 - 使用固定比例，通过SetScaleFactor控制整体大小
        _arrowSource->SetShaftResolution(8);
        _arrowSource->SetTipResolution(8);
        _arrowSource->SetTipLength(0.25);     // 箭头尖端占总长度的25%
        _arrowSource->SetTipRadius(0.08);     // 箭头尖端半径
        _arrowSource->SetShaftRadius(0.03);   // 箭头轴半径

        // 配置Glyph3D
        _glyph3D->SetInputData(_arrowData);
        _glyph3D->SetSourceConnection(_arrowSource->GetOutputPort());
        _glyph3D->SetVectorModeToUseVector();
        _glyph3D->SetScaleModeToDataScalingOff();
        _glyph3D->SetScaleFactor(arrowScale); // 设置整体缩放因子
        _glyph3D->OrientOn();
        _glyph3D->Update();

        return true;
    }

    void GraphVTKObjectGravity::generateGraph()
    {
        if (_arrowActor && _glyph3D) {
            _arrowActor->setInputConnection(_glyph3D->GetOutputPort());

            m_graphObjHighlight->addModelInputConnection(_glyph3D->GetOutputPort());
            m_graphObjHighlight->setActorType(2);
        }
    }

    void GraphVTKObjectGravity::insertNextPoint(double* pos)
    {
        if (!pos) return;
        // 复制点坐标
        double* newPoint = new double[3];
        newPoint[0] = pos[0];
        newPoint[1] = pos[1];
        newPoint[2] = pos[2];
        _loadPoints.append(newPoint);
    }

    void GraphVTKObjectGravity::getGravityDirectionVector(double direction[3])
    {
        // 初始化方向向量
        direction[0] = 0.0;
        direction[1] = 0.0;
        direction[2] = 0.0;

        Radioss::FITKGravity* gravity = dynamic_cast<Radioss::FITKGravity*>(_dataObj);
        if (gravity == nullptr) return;

        // 根据重力方向枚举设置方向向量
        Radioss::FITKGravity::GravityDirection gravityDir = gravity->getGravityDirection();
        int dir = gravity->getFScaleY(); // 获取X方向缩放因子，作为方向的强度
        switch (gravityDir) {
        case Radioss::FITKGravity::GRAVITY_X:
            direction[0] = dir; // X方向
            break;
        case Radioss::FITKGravity::GRAVITY_Y:
            direction[1] = dir; // Y方向
            break;
        case Radioss::FITKGravity::GRAVITY_Z:
            direction[2] = dir; // Z方向
            break;
        }
    }

    double GraphVTKObjectGravity::calculateArrowScale()
    {
        // 获取网格模型
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) {
            return 1.0; // 默认缩放
        }

        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr) {
            return 1.0; // 默认缩放
        }

        // 获取模型的包围盒
        double bndBox[6]; // xmin, xmax, ymin, ymax, zmin, zmax
        meshModel->getNodeBndBox(bndBox);

        // 计算包围盒对角线长度
        double dx = bndBox[1] - bndBox[0]; // xmax - xmin
        double dy = bndBox[3] - bndBox[2]; // ymax - ymin  
        double dz = bndBox[5] - bndBox[4]; // zmax - zmin

        double diagonalLength = std::sqrt(dx * dx + dy * dy + dz * dz);

        // 根据包围盒大小计算合适的箭头缩放因子
        double scaleRatio = 0.1;
        double scale = diagonalLength * scaleRatio;

        // 动态设置最小和最大缩放限制
        double minScale = diagonalLength * 0.01; // 最小为对角线长度的1%
        double maxScale = diagonalLength * 0.5;  // 最大为对角线长度的50%

        // 应用缩放限制
        scale = std::max(minScale, std::min(scale, maxScale));

        return scale;
    }

    void GraphVTKObjectGravity::updateRenderingProperties()
    {
        if (!_vectors || !_glyph3D || !_arrowActor) {
            return;
        }

        // 更新重力方向向量
        double direction[3];
        getGravityDirectionVector(direction);

        // 更新所有向量
        for (vtkIdType i = 0; i < _vectors->GetNumberOfTuples(); i++) {
            _vectors->SetTuple3(i, direction[0], direction[1], direction[2]);
        }
        _vectors->Modified();

        // 更新箭头缩放
        double arrowScale = calculateArrowScale();
        _glyph3D->SetScaleFactor(arrowScale);

        // 标记数据已修改并更新
        _arrowData->Modified();
        _glyph3D->Update();
    }
    bool GraphVTKObjectGravity::checkAndUpdateGlobalCenter()
    {
        if (!_points || _loadPoints.isEmpty()) {
            return false;
        }

        // 获取当前包围盒中心
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return false;

        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr) return false;

        // 获取模型的包围盒
        double bndBox[6]; // xmin, xmax, ymin, ymax, zmin, zmax
        meshModel->getNodeBndBox(bndBox);

        // 计算包围盒中心点
        double newCenterPos[3];
        newCenterPos[0] = (bndBox[0] + bndBox[1]) * 0.5; // x center
        newCenterPos[1] = (bndBox[2] + bndBox[3]) * 0.5; // y center  
        newCenterPos[2] = (bndBox[4] + bndBox[5]) * 0.5; // z center

        // 比较当前载荷点坐标和新的包围盒中心坐标
        double* currentPos = _loadPoints.first();
        const double tolerance = 1e-10; // 容差值

        bool needUpdate = (std::abs(currentPos[0] - newCenterPos[0]) > tolerance ||
            std::abs(currentPos[1] - newCenterPos[1]) > tolerance ||
            std::abs(currentPos[2] - newCenterPos[2]) > tolerance);

        if (needUpdate) {
            // 更新载荷点坐标
            currentPos[0] = newCenterPos[0];
            currentPos[1] = newCenterPos[1];
            currentPos[2] = newCenterPos[2];

            // 更新VTK点坐标（索引0是第一个也是唯一的点）
            _points->SetPoint(0, newCenterPos);
            _points->Modified();

            // 标记数据已修改
            _arrowData->Modified();

            return true;
        }

        return false;
    }
}