/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectBCS.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkDoubleArray.h>
#include <vtkDistanceToCamera.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
#include <vtkConeSource.h>

#include <QVector>

namespace Exchange
{
    GraphVTKObjectBCS::GraphVTKObjectBCS(Radioss::FITKRadiossBCS* bcData)
        : GraphVTKObjectBCBase(dynamic_cast<Radioss::FITKAbstractBCS*>(bcData))
    {
        if (bcData == nullptr) {
            return;
        }
        init();
        initActor();
    }

    GraphVTKObjectBCS::~GraphVTKObjectBCS()
    {
        // 删除T组VTK对象
        for (int i = 0; i < 3; i++) {
            deleteVtkObj(_polyDataT[i]);
            deleteVtkObj(_normalArrayT[i]);
        }
        // 删除W组VTK对象
        for (int i = 0; i < 3; i++) {
            deleteVtkObj(_polyDataW[i]);
            deleteVtkObj(_normalArrayW[i]);
        }
    }

    void GraphVTKObjectBCS::update(bool forceUpdate)
    {
        if (!forceUpdate) {
            return;
        }
        // 设置计算相机距离
        for (int i = 0; i < 3; i++) {
            _disCameraT[i]->SetRenderer(_render);
            _disCameraW[i]->SetRenderer(_render);
        }
        updateGraphData();
    }

    void GraphVTKObjectBCS::init()
    {
        // 创建箭头数据源并反转箭头，使 tip 位于原点
        vtkSmartPointer<vtkConeSource> arrowSource = vtkSmartPointer<vtkConeSource>::New();
        arrowSource->SetHeight(0.25);
        arrowSource->SetRadius(0.125);
        arrowSource->SetResolution(6);

        vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
        coneSource->SetHeight(0.25);
        coneSource->SetRadius(0.125);
        coneSource->SetResolution(6);
        coneSource->SetCenter(-coneSource->GetHeight() * 0.5, 0, 0);

        auto createGlyphForAxis = [](vtkPolyData*& polyData, vtkDoubleArray*& normals,
            vtkDistanceToCamera*& distance, vtkGlyph3D*& glyph, vtkPolyDataAlgorithm* alg)
            {
                polyData = vtkPolyData::New();
                polyData->SetPoints(vtkSmartPointer<vtkPoints>::New());
                polyData->SetVerts(vtkSmartPointer<vtkCellArray>::New());
                normals = vtkDoubleArray::New();
                normals->SetNumberOfComponents(3);
                polyData->GetPointData()->SetNormals(normals);
                distance = vtkDistanceToCamera::New();
                distance->SetInputDataObject(polyData);
                distance->SetScreenSize(60);
                glyph = vtkGlyph3D::New();
                glyph->SetSourceConnection(alg->GetOutputPort());
                glyph->SetInputConnection(distance->GetOutputPort());
                glyph->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
                    distance->GetDistanceArrayName());
                glyph->SetVectorModeToUseNormal();
                if (distance->GetOutput()->GetNumberOfPoints() == 0) {
                    std::cerr << "Error: vtkDistanceToCamera did not generate any points" << std::endl;
                    return false;
                }
            };

        // 为T组和W组的每个方向（索引0->X, 1->Y, 2->Z）创建VTK对象
        for (int i = 0; i < 3; i++) {
            createGlyphForAxis(_polyDataT[i], _normalArrayT[i], _disCameraT[i], _glyphT[i], arrowSource);
            createGlyphForAxis(_polyDataW[i], _normalArrayW[i], _disCameraW[i], _glyphW[i], coneSource);
        }
    }

    void GraphVTKObjectBCS::initActor()
    {
        // 设置颜色：T组蓝色，W组黄色
        double blue[3] = { 0.0, 0.0, 1.0 };
        double yellow[3] = { 1.0, 1.0, 0.0 };

        // lambda函数：为指定方向创建actor并设置属性
        auto createActorForAxis = [this](vtkGlyph3D* glyph, FITKGraphActor*& actor, double color[3])
            {
                actor = FITKGraphActor::New();
                actor->setGraphObject(this);
                actor->SetPickable(false);
                actor->SetVisibility(false);
                actor->GetProperty()->SetPointSize(5);
                actor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
                actor->GetProperty()->SetColor(color);
                actor->setInputConnection(glyph->GetOutputPort());
                actor->GetMapper()->ScalarVisibilityOff();
                actor->setRelativeCoincidentTopologyPolygonOffsetParameters(m_polygonOffset);
                addActor(actor);
            };

        // 为T组和W组分别为每个方向创建actor
        for (int i = 0; i < 3; i++) {
            createActorForAxis(_glyphT[i], _actorT[i], blue);
            createActorForAxis(_glyphW[i], _actorW[i], yellow);
        }

        // 对W组的glyph设置缩放因子
        for (int i = 0; i < 3; i++) {
            _glyphW[i]->SetScaleFactor(0.8);
        }
    }


    bool GraphVTKObjectBCS::updateGraphData()
    {
        // 清空T组和W组的polyData数据
        for (int i = 0; i < 3; i++) {
            _polyDataT[i]->Reset();
            _polyDataW[i]->Reset();
        }
        // 为两组数据点均生成数据（此处以原点为例）
        for (int i = 0; i < 3; i++) {
            generatePolyData(_polyDataT[i]);
            generatePolyData(_polyDataW[i]);
        }
        updateDirection();
        return true;
    }

    void GraphVTKObjectBCS::updateDirection()
    {

        Radioss::FITKRadiossBCS* radBc =
            dynamic_cast<Radioss::FITKRadiossBCS*>(_dataObj);
        if (radBc == nullptr) {
            return;
        }
        //获取坐标系id
        int coordId = radBc->getCoordinateSystemID();
        //获取坐标

        //暂时：
        bool stateT[3] = { radBc->isFreedomFixed(1),radBc->isFreedomFixed(2),radBc->isFreedomFixed(3) };
        bool stateW[3] = { radBc->isFreedomFixed(4),radBc->isFreedomFixed(5),radBc->isFreedomFixed(6) };
        for (int axis = 0; axis < 3; axis++) {
            _normalArrayT[axis]->Reset();
            _normalArrayW[axis]->Reset();

            // 单位法向量：X:(1,0,0)，Y:(0,1,0)，Z:(0,0,1)
            double normal[3] = { 0, 0, 0 };
            normal[axis] = 1;

            int numPointsT = _polyDataT[axis]->GetNumberOfPoints();
            for (int i = 0; i < numPointsT; i++) {
                _normalArrayT[axis]->InsertNextTuple3(normal[0], normal[1], normal[2]);
            }
            int numPointsW = _polyDataW[axis]->GetNumberOfPoints();
            for (int i = 0; i < numPointsW; i++) {
                _normalArrayW[axis]->InsertNextTuple3(normal[0], normal[1], normal[2]);
            }
            // 根据状态设置actor的可见性
            _actorT[axis]->SetVisibility(stateT[axis]);
            _actorW[axis]->SetVisibility(stateW[axis]);
        }
    }
}
