/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPick.h"

#include "GUIWidget/PickedData.h"
#include "PostGraphAdaptor/PostGraphObjectPick.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
// Render
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

// Pick data provider
#include "GUIWidget/PickedDataProvider.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "GUIWidget/GUIPickInfo.h"

namespace GUIOper
{
    OperGraphPick::OperGraphPick()
    {
        // 初始化拾取数据管理器。
        m_pickedProvider = GraphData::PickedDataProvider::getInstance();
    }

    OperGraphPick::~OperGraphPick()
    {

    }

    void OperGraphPick::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (!graphWindow || !actor)return;

        GUI::GUIPickInfo::PickObjType pickType = GUI::GUIPickInfo::GetPickInfo()._pickObjType;
        m_pickedProvider->addPicked(actor, index, pickedWorldPos);

        Interface::PostGraphObjectPick* graphObjPick = m_pickedProvider->getPickDatas()->getGraphPickObject();
        if (m_pickedProvider == nullptr)return;
        graphObjPick->setColor(QColor(255, 0, 0));
        graphObjPick->update();

        // 设置演员样式。
        //@{
        switch (pickType)
        {
        case GUI::GUIPickInfo::POBJVert:
            graphObjPick->setPickedType(1);
            break;
        case GUI::GUIPickInfo::POBJEdge:
            graphObjPick->setPickedType(2);
            break;
        case GUI::GUIPickInfo::POBJFace:
        case GUI::GUIPickInfo::POBJSolid:
            graphObjPick->setPickedType(3);
            break;
        default:
            graphObjPick->setVisible(false);
            return;
        }
        //@}

        // 添加到渲染窗口。
        graphObjPick->setVisible(true);
        addGraphObjectToWidget(graphObjPick, graphWindow);

        // 刷新窗口。
        graphWindow->reRender();
    }

    void OperGraphPick::addGraphObjectToWidget(Interface::PostGraphObjectPick* obj, Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        if (!obj || !graphWidget)
        {
            return;
        }

        // 添加可视化对象。
        //@{
        obj->removeFromGraphWidget();
        graphWidget->addObject(obj->getRenderLayer(), obj, true);
        //}

        // 刷新窗口。
        //@{
        graphWidget->reRender();
        //@}
    }

    void OperGraphPick::setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId)
    {
        //GraphData::GraphProviderManager::getInstance()->setActorStateByPickInfo(pickObjType, pickMethod, dataObjId);
    }
}  // namespace GUIOper