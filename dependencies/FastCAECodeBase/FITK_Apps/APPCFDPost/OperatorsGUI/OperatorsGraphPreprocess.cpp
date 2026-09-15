/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsGraphPreprocess.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "PostGraphAdaptor/PostGraphObjectAgent.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectLight.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLightManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLight.h"

#include <QAction>

namespace OperGUI
{
    OperatorsGraphPreprocess::OperatorsGraphPreprocess()
    {
        _mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        _postGraphManager = Interface::PostGraphObjectManager::getInstance();
        _postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        _postListManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostLightManager();

        connect(_mainWin, SIGNAL(sigMainWindowClose()), this, SLOT(slotMainWindowClose()));
    }

    void OperatorsGraphPreprocess::updateAll()
    {

    }

    void OperatorsGraphPreprocess::updateSingle(int postDataID)
    {
        Interface::FITKAbstractCFDPostData* postData = _postDataManager->getDataByID(postDataID);
        if (postData == nullptr)return;

        Comp::FITKGraph3DWindowVTK* graph3DWidget = getGraphWidget();
        if (graph3DWidget == nullptr)return;

        Interface::PostGraphObjectBase* graphObj = nullptr;
        for (int i = 0; i < _postGraphManager->getDataCount(); i++) {
            Interface::PostGraphObjectAgent* agent = _postGraphManager->getDataByIndex(i);
            if(agent == nullptr)continue;
            Interface::PostGraphObjectBase* obj = agent->getGraphObject();
            if(obj == nullptr)continue;
            if (obj->getPostID() == postDataID)graphObj = obj;
        }

        //后处理数据对应的渲染对象不存在则创建
        if (graphObj == nullptr) {
            graphObj = new Interface::PostGraphObjectBase(postData, graph3DWidget);
            graphObj->create();
            graph3DWidget->addObject(graphObj->getRenderLayer(), graphObj);
            //渲染对象管理器添加
            Interface::PostGraphObjectAgent* graphAgentObj = new Interface::PostGraphObjectAgent(graphObj);
            _postGraphManager->appendDataObj(graphAgentObj);
        }

        postData->update();
        graph3DWidget->reRender();
    }

    void OperatorsGraphPreprocess::updateShow(int postDataID, bool isShow)
    {
        Interface::FITKAbstractCFDPostData* data = _postDataManager->getDataByID(postDataID);
        if (data == nullptr)return;
        Interface::PostGraphObjectManager* manager = Interface::PostGraphObjectManager::getInstance();
        if (manager == nullptr) return;
        Interface::PostGraphObjectBase* graphData = manager->getGraphObjectBase(data);
        if (graphData == nullptr)return;
        if (isShow == data->isEnable())return;
        graphData->setIsShow(isShow);
    }

    void OperatorsGraphPreprocess::updateLight(int dataID)
    {
        Interface::FITKCFDPostLight* light = _postListManager->getDataByID(dataID);
        if (light == nullptr)return;
        Interface::PostGraphLightManager* lightManager = Interface::PostGraphObjectManager::getInstance()->getGraphLightManager();
        if (lightManager == nullptr)return;

        Comp::FITKGraph3DWindowVTK* graph3DWidget = getGraphWidget();
        if (graph3DWidget == nullptr)return;

        Interface::PostGraphObjectLight* lightObject = nullptr;

        //判断当前数据对应的light是否存在
        for (int i = 0; i < lightManager->getDataCount(); i++) {
            Interface::PostGraphObjectLight* object = lightManager->getDataByIndex(i);
            if(object == nullptr)continue;
            if (light == object->getLightData()) {
                lightObject = object;
            }
        }

        //创建数据
        if (lightObject == nullptr) {
            lightObject = new Interface::PostGraphObjectLight(light, graph3DWidget);
            lightManager->appendDataObj(lightObject);
        }

        lightObject->update();
        reRender();
    }

    void OperatorsGraphPreprocess::updateAllRotateVis(bool isVis)
    {
        for (int i = 0; i < _postGraphManager->getDataCount(); i++) {
            Interface::PostGraphObjectAgent* agent = _postGraphManager->getDataByIndex(i);
            if (agent == nullptr)continue;
            Interface::PostGraphObjectBase* obj = agent->getGraphObject();
            if (obj == nullptr)continue;
            obj->updateRotateVis(isVis);
        }
        reRender();
    }

    void OperatorsGraphPreprocess::reRender()
    {
        Comp::FITKGraph3DWindowVTK* graph3DWidget = getGraphWidget();
        if (graph3DWidget == nullptr)return;
        graph3DWidget->reRender();
    }

    void OperatorsGraphPreprocess::slotMainWindowClose()
    {
        //主窗口关闭时，将渲染窗口中的所有数据对象清除，防止渲染窗口与渲染数据对象对actor重复释放

        //获取所有数据对象id
        QList<int> postDataIds = {};
        for (int i = 0; i < _postDataManager->getDataCount(); i++) {
            Interface::FITKAbstractCFDPostData* postData = _postDataManager->getDataByIndex(i);
            if (postData == nullptr)continue;
            postDataIds.append(postData->getDataObjectID());
        }
        //清除
        QAction dataDelectAction;
        dataDelectAction.setObjectName("actionGraphDelete");
        auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionGraphDelete");
        if (acOper == nullptr)return;
        acOper->setEmitter(&dataDelectAction);
        for (int id : postDataIds) {
            acOper->setArgs(objID_String, id);
            acOper->actionTriggered();
        }

        //获取所有灯光数据对象id
        QList<int> postLightDataIds = {};
        for (int i = 0; i < _postListManager->getDataCount(); i++) {
            Interface::FITKCFDPostLight* postData = _postListManager->getDataByIndex(i);
            if (postData == nullptr)continue;
            postLightDataIds.append(postData->getDataObjectID());
        }
        //清除
        QAction lightDeleteAction;
        lightDeleteAction.setObjectName("actionLightDelete");
        acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionLightDelete");
        if (acOper == nullptr)return;
        acOper->setEmitter(&lightDeleteAction);
        for (int id : postLightDataIds) {
            acOper->setArgs(objID_String, id);
            acOper->actionTriggered();
        }
    }

    Comp::FITKGraph3DWindowVTK * OperatorsGraphPreprocess::getGraphWidget()
    {
        GUI::RenderWidget* renderWidget = _mainWin->getRenderWidget();
        if (renderWidget == nullptr)return nullptr;
        Comp::FITKGraph3DWindowVTK* graph3DWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(renderWidget->getRenderWidget());
        return graph3DWidget;
    }
}
