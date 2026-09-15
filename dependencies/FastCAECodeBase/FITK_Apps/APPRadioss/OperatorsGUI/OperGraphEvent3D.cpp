/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphEvent3D.h"

#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphModelProvider.h"
#include "GraphDataProvider/GraphMarkProvider.h"
#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/CentralWidget.h"
#include "GUIWidget/GraphArea.h"
#include "GUIWidget/GraphMdiArea.h"
#include "GUIWidget/PostGraphWidget.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementList.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
namespace GUIOper {
    void OperGraphEvent3D::render()
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget) {
            graphWidget->reRender();
        }

        graphWidget = getGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidget) {
            graphWidget->reRender();
        }
    }

    void OperGraphEvent3D::fitView()
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget) {
            graphWidget->fitView();
        }

        graphWidget = getGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidget) {
            graphWidget->fitView();
        }
    }

    GUI::GraphArea* OperGraphEvent3D::getGraphWidget(GUI::GraphAreaType type)
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) {
            return nullptr;
        }

        // 获取中心区域
        GUI::CentralWidget* centralWidget = mainWindow->getCentralWidget();
        if (!centralWidget) {
            return nullptr;
        }

        GUI::GraphMdiArea* graphArea = centralWidget->getGraphMidArea();
        if (!graphArea) {
            return nullptr;
        }
        return graphArea->getFirstGraphArea(type);
    }

    GUI::GraphArea* OperGraphEvent3D::createGraphWidget(GUI::GraphAreaType type)
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) {
            return nullptr;
        }
        // 获取中心区域
        GUI::CentralWidget* centralWidget = mainWindow->getCentralWidget();
        if (!centralWidget) {
            return nullptr;
        }
        GUI::GraphMdiArea* graphArea = centralWidget->getGraphMidArea();
        if (!graphArea) {
            return nullptr;
        }
        int id = graphArea->addSubWidget(new GUI::PostGraphWidget(centralWidget), "post");
        centralWidget->addPostWidgetId(id);
        
        return graphArea->getGraphAreaByID(id);
       
    }

    QList<GUI::GraphArea*> OperGraphEvent3D::getAllGraphWidget(GUI::GraphAreaType type)
    { 
        QList<GUI::GraphArea*> graphWidgets;
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) {
            return graphWidgets;
        }
        // 获取中心区域
        GUI::CentralWidget* centralWidget = mainWindow->getCentralWidget();
        if (!centralWidget) {
            return graphWidgets;
        }
        GUI::GraphMdiArea* graphArea = centralWidget->getGraphMidArea();
        if (!graphArea) {
            return graphWidgets;
        }
        return graphArea->getAllGraphArea(type);
    }

    QList<Comp::FITKGraph3DWindowVTK*> OperGraphEvent3D::getGraphWindows()
    {
        // 获取全部视口。
        QList<Comp::FITKGraph3DWindowVTK*> windows;

        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) {
            return windows;
        }

        // 获取中心区域
        GUI::CentralWidget* centralWidget = mainWindow->getCentralWidget();
        if (!centralWidget) {
            return windows;
        }

        GUI::GraphMdiArea* graphArea = centralWidget->getGraphMidArea();
        if (!graphArea) {
            return windows;
        }
        QList<GUI::GraphArea*> graphAreas = graphArea->getAllGraphArea(GUI::GraphAreaType::GATPre);
        for (GUI::GraphArea* area : graphAreas) {
            if (area) {
                Comp::FITKGraph3DWindowVTK* graphWindow = qobject_cast<Comp::FITKGraph3DWindowVTK*>(area);
                if (graphWindow) {
                    windows.append(graphWindow);
                }
            }
        }
        QList<GUI::GraphArea*> postGraphAreas = graphArea->getAllGraphArea(GUI::GraphAreaType::GATPost);
        for (GUI::GraphArea* area : postGraphAreas) {
            if (area) {
                Comp::FITKGraph3DWindowVTK* graphWindow = qobject_cast<Comp::FITKGraph3DWindowVTK*>(area);
                if (graphWindow) {
                    windows.append(graphWindow);
                }
            }
        }

        return windows;
    }

    void OperGraphEvent3D::addGraphObjectToWidget(Exchange::GraphVTKObject3D* obj, GUI::GraphArea* graphWidget, bool fitView)
    {
        if (!obj || !graphWidget)
        {
            return;
        }

        // 添加可视化对象。
        obj->removeFromGraphWidget();
        graphWidget->addObject(obj->getRenderLayer(), obj, false);
        obj->setRenderer(graphWidget->getRenderWindow()->getRenderer(obj->getRenderLayer())->getRenderer());


        //刷新窗口
        if (fitView) {
            graphWidget->fitView();
        }
        else {
            graphWidget->reRender();
        }
    }

    void OperGraphEvent3D::addGraphHightlightObjectToWidget(Exchange::GraphVTKObject3D * obj, GUI::GraphArea * graphWidget)
    {
        if (obj == nullptr || graphWidget == nullptr) return;
        //添加可视化对象
        obj->removeFromGraphWidget();

        //获得附加可视化对象链表
        QList<Exchange::GraphVTKObject3D*> addinObjs = obj->getAddinGraphObjs();
        for (Exchange::GraphVTKObject3D* addinObj : addinObjs)
        {
            addinObj->removeFromGraphWidget();
        }
        //获取图层
        int layer = obj->getRenderLayer();
        graphWidget->addObject(layer, obj, false);
        //添加至高亮图层
        for(Exchange::GraphVTKObject3D* addinObj : addinObjs)
        {
            int layer = addinObj->getRenderLayer();
            graphWidget->addObject(layer, addinObj, false);
        }
       
        //刷新窗口
        graphWidget->reRender();
    }

    QList<Exchange::GraphVTKObject3D*> OperGraphEvent3D::getGraphObjectsByDataId(int dataObjId, QVariant otherInfo)
    {
        QList<Exchange::GraphVTKObject3D*> objs;

        // 获取可视化窗口。
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) {
            return objs;
        }

        // 获取模型与符号可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getModelProvider(graphWidget->getRenderWindow());
        GraphData::GraphMarkProvider* markProvider = GraphData::GraphProviderManager::getInstance()->getMarkProvider(graphWidget->getRenderWindow());
        if (!modelProvider || !markProvider) {
            return objs;
        }
        Core::FITKAbstractDataObject* dataObj = FITKDATAREPO->getTDataByID<Core::FITKAbstractDataObject>(dataObjId);
        if (dataObj == nullptr) {
            return objs;
        }

        //检查是否为几何part对象
        Radioss::FITKRadiossGeomPart* geoPart = dynamic_cast<Radioss::FITKRadiossGeomPart*>(dataObj);
        if (geoPart) {
            //objs << modelProvider->getPartGeoGraphObject(dataObjId);
        }

        // 检查是否为几何命令对象
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(dataObj);
        if (geoCommand) {
            // 直接使用几何命令ID获取可视化对象
            Exchange::GraphVTKObject3D* obj = modelProvider->getModelGraphObject(dataObjId);
            if (obj) {
                objs.append(obj);
            }
        }
        //检查是否为part对象
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(dataObj);
        if (part) {
            objs << modelProvider->getPartGraphObject(dataObjId);
        }
        //检查是否为刚体墙对象
        Radioss::FITKAbstractRWall* rwall = dynamic_cast<Radioss::FITKAbstractRWall*>(dataObj);
        if (rwall) {
            objs << modelProvider->getRWallGraphObject(dataObjId);
        }
        //检查是否为重力载荷对象
        Radioss::FITKGravity* gravity = dynamic_cast<Radioss::FITKGravity*>(dataObj);
        if (gravity) {
            objs << modelProvider->getGravityGraphObject(dataObjId);
            //检查重力载荷内是否有集合
            /*Interface::FITKModelSet* set = Core::FITKDataRepo::getInstance()->getTDataByID <Interface::FITKModelSet>(gravity->getNodeGroupID());
            if(set)
            {
                if (!otherInfo.isValid()) {
                    otherInfo = -1;
                }
                objs << modelProvider->getGraphObjSetOrSurface(set->getDataObjectID(),otherInfo.toInt());
            };*/
        }
        //判断是不是初始化场
        Radioss::FITKAbstractInitialField* initField = dynamic_cast<Radioss::FITKAbstractInitialField*>(dataObj);
        if(initField) {
            objs << modelProvider->getFiledGraphObject(dataObjId);
        }
        //判断是不是边界条件
        Radioss::FITKAbstractBCS* bc = dynamic_cast<Radioss::FITKAbstractBCS*>(dataObj);
        if (bc) {
            objs << modelProvider->getBCSGraphObject(dataObjId);
        }
        //判断是不是相互作用
        Radioss::FITKAbstractInteraction* interaction = dynamic_cast<Radioss::FITKAbstractInteraction*>(dataObj);
        if (interaction) {
            objs << modelProvider->getInteractionGraphObject(dataObjId);
        }
        //判断是不是连接
        Radioss::FITKAbstractConnection* connection = dynamic_cast<Radioss::FITKAbstractConnection*>(dataObj);
        if (connection) {
            objs << modelProvider->getConnectionGraphObject(dataObjId);
        }
        //判断是不是探针
        Radioss::FITKAbstractProbe* probe = dynamic_cast<Radioss::FITKAbstractProbe*>(dataObj);
        if (probe)
        {
            objs << modelProvider->getProbeGraphObject(dataObjId);
        }
        //判断是否是集合或表面
        Interface::FITKModelSet* modelSet = dynamic_cast<Interface::FITKModelSet*>(dataObj);
        Interface::FITKMeshSurface* surface = dynamic_cast<Interface::FITKMeshSurface*>(dataObj);
        if (modelSet || surface && objs.isEmpty())
        {

            //第一位为instance数据ID，可以为-1。
            if (!otherInfo.isValid()) {
                otherInfo = -1;
            }

            objs << modelProvider->getGraphObjSetOrSurface(dataObjId, otherInfo.toInt());
        }
        return objs;
    }
    
    QList<Exchange::GraphVTKObject3D*> OperGraphEvent3D::getPartGeoGraphObjectsByDataId(int dataObjId)
    {
        QList<Exchange::GraphVTKObject3D*> objs;
        //获取可视化窗口
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) return objs;

        //获取模型与符号可视化对象管理器
        GraphData::GraphModelProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getModelProvider(graphWidget->getRenderWindow());
        if (modelProvider == nullptr) return objs;

        Interface::FITKAbsGeoCommand* geoCommand = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKAbsGeoCommand>(dataObjId);
        if (geoCommand) {
            objs << modelProvider->getModelGraphObject(dataObjId);
        }

        //检查是否为part对象
        Radioss::FITKRadiossPart* part = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKRadiossPart>(dataObjId);
        if (part) {
            objs << modelProvider->getPartGraphObject(dataObjId);
        }
        return objs;
    }

    QList<Exchange::GraphVTKObject3D*> OperGraphEvent3D::getAllGraphObject()
    {
        QList<Exchange::GraphVTKObject3D*> objs = {};
        // 获取可视化窗口。
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) {
            return objs;
        }

        // 获取模型与符号可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getModelProvider(graphWidget->getRenderWindow());
        GraphData::GraphMarkProvider* markProvider = GraphData::GraphProviderManager::getInstance()->getMarkProvider(graphWidget->getRenderWindow());
        if (modelProvider == nullptr || markProvider == nullptr) {
            return objs;
        }

        objs.append(modelProvider->getAllGraphObjects());
        return objs;
    }

    QList<Exchange::GraphVTKObject3D*> OperGraphEvent3D::getAllGeoGraphObject()
    {
        QList<Exchange::GraphVTKObject3D*> objs = {};
        // 获取可视化窗口。
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) {
            return objs;
        }

        // 获取模型与符号可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getModelProvider(graphWidget->getRenderWindow());
        GraphData::GraphMarkProvider* markProvider = GraphData::GraphProviderManager::getInstance()->getMarkProvider(graphWidget->getRenderWindow());
        if (modelProvider == nullptr || markProvider == nullptr) {
            return objs;
        }
        objs.append(modelProvider->getAllGeoGraphObjects());
        return objs;
    }

    QList<Exchange::GraphVTKObject3D*> OperGraphEvent3D::getAllMeshGraphObject()
    {
        QList<Exchange::GraphVTKObject3D*> objs = {};
        // 获取可视化窗口。
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) {
            return objs;
        }

        // 获取模型与符号可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getModelProvider(graphWidget->getRenderWindow());
        GraphData::GraphMarkProvider* markProvider = GraphData::GraphProviderManager::getInstance()->getMarkProvider(graphWidget->getRenderWindow());
        if (modelProvider == nullptr || markProvider == nullptr) {
            return objs;
        }
        objs.append(modelProvider->getAllMeshGraphObjects());
        return objs;
    }
    

    void OperGraphEvent3D::updateGraphRecursively(int dataObjId, int refLayer, bool recDown)
    {
        // 检查数据ID。
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(dataObjId);
        if (!cmd) {
            return;
        }

        // 更新可视化对象。
        updateGraph(dataObjId);

        // 更新引用数据对象。
        if (refLayer != 0)
        {
            int subLayer = refLayer == -1 ? -1 : refLayer - 1;

            if (recDown)
            {
                int nRefedCmd = cmd->getReferencedCmdCount();
                for (int i = 0; i < nRefedCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferencedCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
            else
            {
                int nRefCmd = cmd->getReferenceCmdCount();
                for (int i = 0; i < nRefCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferenceCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
        }
    }


}


