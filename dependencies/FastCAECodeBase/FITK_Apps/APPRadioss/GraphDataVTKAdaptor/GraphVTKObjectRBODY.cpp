/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectRBODY.h"
#include "GraphVTKObjectHighlighting.h"
#include "GraphDataSetCompomentManager.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/fitkradiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkAppendFilter.h>
#include <vtkPoints.h>
#include <vtkVertex.h>
#include <vtkLine.h>

namespace Exchange
{
    GraphVTKObjectRBODY::GraphVTKObjectRBODY(Radioss::FITKConnectionRBODY* dataObj)
        : GraphVTKObjectModelBase(dataObj)
    {
        init();

        if (!generateDataSet()) {
            return;
        }

        generateGraph();

        updateVisibility();
    }

    GraphVTKObjectRBODY::~GraphVTKObjectRBODY()
    {
        deleteVtkObj(_grid);
    }

    void GraphVTKObjectRBODY::updateModelColor(QColor color)
    {
        // 检查对象是否有效
        Radioss::FITKConnectionRBODY* rbody = dynamic_cast<Radioss::FITKConnectionRBODY*>(_dataObj);
        if (rbody == nullptr || _actor == nullptr) return;

        // 如果颜色无效，使用默认蓝色
        if (!color.isValid()) {
            color = Qt::blue;
        }

        // 更新演员颜色 - 用户可以设置任何颜色，包括红色
        _actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _actor->GetProperty()->SetOpacity(color.alphaF());
    }

    bool GraphVTKObjectRBODY::getVisibility()
    {
        if (_actor == nullptr) {
            return false;
        }
        if (_actor->GetVisibility() && _dataObj->isEnable()) {
            return true;
        }
    }

    void GraphVTKObjectRBODY::updateVisibility()
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(getVisibility());
    }

    void GraphVTKObjectRBODY::setVisible(bool visibility)
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(visibility);
    }

    void GraphVTKObjectRBODY::update(bool forceUpdate)
    {
        //更新可见
        updateVisibility();
        //判断是否需要刷新数据
        if (forceUpdate == false)
        {
            return;
        }
        if (!generateDataSet()) {
            return;
        }
        _actor->setInputDataObject(_grid);
    }

    void GraphVTKObjectRBODY::init()
    {
        _grid = vtkUnstructuredGrid::New();
        _grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
    }

    bool GraphVTKObjectRBODY::generateDataSet()
    {
        if (_grid == nullptr) {
            return false;
        }

        Radioss::FITKConnectionRBODY* rbody = dynamic_cast<Radioss::FITKConnectionRBODY*>(_dataObj);
        if (rbody == nullptr) {
            return false;
        }

        GraphDataSetCompomentManager* manager = GraphDataSetCompomentManager::getInstance();
        if (manager == nullptr) {
            return false;
        }
        vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();

        // 用于存储主节点坐标
        double masterCoord[3] = { 0.0, 0.0, 0.0 };
        bool masterCoordInitialized = false;

        // 获取主节点id
        int masterNodeID = rbody->getMainNodeID();

        // 获得主节点
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return -1;
        Radioss::FITKRadiossMeshModel* meshModel = caseObj->getMeshModel();
        if (!meshModel) return -1;
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        Core::FITKNode* masterNode = nodes->getNodeByID(masterNodeID);
        if (masterNode)
        {
            // 获取主节点坐标
            masterNode->getCoor(masterCoord);
            masterCoordInitialized = true;
            // 创建主节点的可视化数据
            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            points->InsertNextPoint(masterCoord);
            vtkSmartPointer<vtkVertex> vertex = vtkSmartPointer<vtkVertex>::New();
            vertex->GetPointIds()->SetId(0, 0);
            vtkSmartPointer<vtkUnstructuredGrid> pointGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
            pointGrid->SetPoints(points);
            pointGrid->InsertNextCell(vertex->GetCellType(), vertex->GetPointIds());
            appendFilter->AddInputData(pointGrid);

        }

        // 获取从节点组并创建连接线
        int setId = rbody->getSecondaryNodesGroupID();
        Interface::FITKModelSet* secondarySet = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
        if (secondarySet)
        {
            int modelId = secondarySet->getAbsModelID();
            vtkUnstructuredGrid* setGrid = manager->getSetGridData(setId, modelId);
            if (setGrid) {
                appendFilter->AddInputData(setGrid);
            }
            if (masterCoordInitialized) {
                // 创建线数据
                vtkSmartPointer<vtkPoints> linePoints = vtkSmartPointer<vtkPoints>::New();
                vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
                vtkIdType masterPointId = linePoints->InsertNextPoint(masterCoord);

                for (vtkIdType i = 0; i < setGrid->GetNumberOfPoints(); ++i)
                {
                    double secondaryCoord[3];
                    setGrid->GetPoint(i, secondaryCoord);
                    vtkIdType secondaryPointId = linePoints->InsertNextPoint(secondaryCoord);

                    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
                    line->GetPointIds()->SetId(0, masterPointId);
                    line->GetPointIds()->SetId(1, secondaryPointId);
                    lines->InsertNextCell(line);
                }

                vtkSmartPointer<vtkUnstructuredGrid> lineGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
                lineGrid->SetPoints(linePoints);
                lineGrid->SetCells(VTK_LINE, lines);
                appendFilter->AddInputData(lineGrid);
            }
        }

        appendFilter->Update();
        _grid->DeepCopy(appendFilter->GetOutput());

        return true;
    }

    void GraphVTKObjectRBODY::generateGraph()
    {
        _actor = FITKGraphActor::New();
        _actor->SetPickable(false);
        _actor->setGraphObject(this);
        _actor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
        _actor->GetProperty()->SetPointSize(GraphVTKCommons::_highlightPointSize);
        this->setRenderLayer(0);
        // 设置默认颜色
        QColor defaultColor = Qt::green;
        _actor->GetProperty()->SetColor(defaultColor.redF(), defaultColor.greenF(), defaultColor.blueF());
        _actor->setRelativeCoincidentTopologyPolygonOffsetParameters(-10);
        _actor->setInputDataObject(_grid);
        addActor(_actor);

        m_graphObjHighlight->addModelInputDataObject(_grid);
        m_graphObjHighlight->setVertexSize(GraphVTKCommons::_highlightPointSize);
        m_graphObjHighlight->setActorType(2);
    }
}
