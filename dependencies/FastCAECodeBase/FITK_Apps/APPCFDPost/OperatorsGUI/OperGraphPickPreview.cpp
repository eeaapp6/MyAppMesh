/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPickPreview.h"

#include "GUIWidget/PickedDataProvider.h"
#include "GUIWidget/PickedData.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "GUIWidget/GUIPickInfo.h"
#include "PostGraphAdaptor/PostGraphObjectPick.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Graph
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"

// Render
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include <vtkActor.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>

namespace GUIOper
{
    OperGraphPickPreview::OperGraphPickPreview()
    {
        // 初始化拾取数据管理器。
        m_pickedProvider = GraphData::PickedDataProvider::getInstance();
    }

    OperGraphPickPreview::~OperGraphPickPreview()
    {

    }

    void OperGraphPickPreview::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (!graphWindow || !actor)return;

        auto pickType = GUI::GUIPickInfo::GetPickInfo()._pickObjType;

        bool needUpdate = m_pickedProvider->setPreviewPicked(actor, index, pickedWorldPos);
        if (!needUpdate)return;

        Interface::PostGraphObjectPick* graphObjPreview = m_pickedProvider->getPickedPreview()->getGraphPickObject();
        if (graphObjPreview == nullptr)return;
        graphObjPreview->setColor(QColor(255, 100, 0));
        graphObjPreview->update();

        switch (pickType)
        {
        case GUI::GUIPickInfo::POBJVert:
            graphObjPreview->setPickedType(1);
            break;
        case GUI::GUIPickInfo::POBJEdge:
            graphObjPreview->setPickedType(2);
            break;
        case GUI::GUIPickInfo::POBJFace:
        case GUI::GUIPickInfo::POBJSolid:
            graphObjPreview->setPickedType(3);
            break;
        default:
            graphObjPreview->setVisible(false);
            return;
        }
        //@}

        // 添加到渲染窗口。
        graphObjPreview->setVisible(true);
        addGraphObjectToWidget(graphObjPreview, graphWindow);

        // 刷新窗口。
        graphWindow->reRender();
    }

    void OperGraphPickPreview::addGraphObjectToWidget(Interface::PostGraphObjectPick* obj, Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        if (!obj || !graphWidget)return;

        obj->removeFromGraphWidget();
        graphWidget->addObject(obj->getRenderLayer(), obj, true);
        graphWidget->reRender();
    }
} 