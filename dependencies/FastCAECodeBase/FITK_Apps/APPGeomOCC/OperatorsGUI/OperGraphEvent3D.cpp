/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphEvent3D.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// Graph widget
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Graph data manager
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphModelProvider.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

namespace GUIOper
{
    Render::FITKGraph3DWindowOCC* OperGraphEvent3D::getGraphWidget()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow)
        {
            return nullptr;
        }

        // 获取可视化区。
        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (!renderWidget)
        {
            return nullptr;
        }
        // 获取当前窗口，尝试转换为三维窗口。
        auto w = renderWidget->getGraph3DWidget();
        Render::FITKGraph3DWindowOCC* graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(w);

        return graphWidget;
    }

    void OperGraphEvent3D::addGraphObjectToWidget(Exchange::FITKOCCGraphObject3D* obj, Render::FITKGraph3DWindowOCC* graphWidget, bool fitView)
    {
        if (!obj || !graphWidget)
        {
            return;
        }

        // 添加可视化对象。
        //@{
        // obj->removeFromGraphWidget();
        obj->setGraphWidget(graphWidget);
        graphWidget->addObject(obj, false);
        //}

        // 刷新窗口。
        //@{
        if (fitView)
        {
            graphWidget->fitView();
        }
        //@}
    }

    QList<Exchange::FITKOCCGraphObject3D*> OperGraphEvent3D::getGraphObjectsByDataId(int dataObjId)
    {
        // 获取或创建可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        QList<Exchange::FITKOCCGraphObject3D*> objs;
        bool isValid = false;

        // 检查数据仓库是否存在该数据。
        Core::FITKAbstractDataObject* dataObj = FITKDATAREPO->getDataByID(dataObjId);
        if (!dataObj)
        {
            return objs;
        }

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return objs;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return objs;
        }

        // 检查数据ID是否为草图。
        Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(dataObj);
        if (sketch && !isValid)
        {
            obj = modelProvider->getSketch2DGraphObject(dataObjId);
            isValid = true;
        }

        // 检查数据ID是否为基准元素。
        Interface::FITKAbsGeoDatum* datum = dynamic_cast<Interface::FITKAbsGeoDatum*>(dataObj);
        if (datum && !isValid)
        {
            obj = modelProvider->getDatumGraphObject(dataObjId);
            isValid = true;
        }

        // 检查数据ID是否为模型。
        Interface::FITKAbsGeoCommand* model = dynamic_cast<Interface::FITKAbsGeoCommand*>(dataObj);
        if (model && !isValid)
        {
            obj = modelProvider->getModelGraphObject(dataObjId);
            isValid = true;
        }

        if (obj)
        {
            objs.push_back(obj);
        }

        return objs;
    }
}  // namespace GUIOper