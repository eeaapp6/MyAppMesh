/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPreprocess.h"
#include "OperGraphEvent3D.h"
#include "GUIWidget/GraphArea.h"
#include "GUIWidget/GUIEnumType.h"

#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GraphDataVTKAdaptor/GraphVTKObjectPick.h"
#include "GraphDataVTKAdaptor/GraphVTKObjectPickPreview.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

namespace GUIOper
{
    void OperGraphPreprocess::updateGraph(int dataObjId, bool forceUpdate, bool isFitView)
    {
        //获取可视化窗口
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget)
        {
            return;
        }

        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs = getGraphObjectsByDataId(dataObjId);

        //添加至三维可视化窗口
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }
            addGraphObjectToWidget(obj, graphWidget, false);
            obj->update(forceUpdate);
        }
        if (isFitView) {
            graphWidget->fitView();
        }
        else {
            graphWidget->reRender();
        }
    }

    void OperGraphPreprocess::updateAll(bool forceUpdate)
    {
        updateAllModel(forceUpdate);
    }

    void OperGraphPreprocess::updateColor(int dataObjId, QColor color, QVariant otherPar)
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) {
            return;
        }
        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs;
        if (dataObjId == -1)
        {
            objs = getAllGeoGraphObject();
        }
        else if (dataObjId == -2)
        {
            objs = getAllMeshGraphObject();
        }
        else
        {
            objs = getGraphObjectsByDataId(dataObjId);
        }
        //刷新颜色
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateSubColor(color,otherPar);
        }
        // 刷新渲染。
        render();
    }

    void OperGraphPreprocess::updateAllColor(int dataObjId, QColor color)
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) {
            return;
        }

        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs = getPartGeoGraphObjectsByDataId(dataObjId);

        //刷新颜色
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateModelColor(color);
        }

        // 刷新渲染。
        render();
    }
    void OperGraphPreprocess::updateAllModel(bool forceUpdate)
    {
        //获取可视化窗口
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) return;
        QList<Exchange::GraphVTKObject3D*>objs = {};

        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr) return;
        //获取所有几何
        Interface::FITKGeoCommandList* geoList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (geoList)
        {
            for (int i = 0; i < geoList->getDataCount(); i++)
            {
                Interface::FITKAbsGeoCommand* geoCmd = geoList->getDataByIndex(i);
                if (geoCmd == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*>modelObjs = getGraphObjectsByDataId(geoCmd->getDataObjectID());
                objs << modelObjs;
            }
        }
        //获取所有部件
        Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr) {
           for (int i = 0; i < partMgr->getDataCount(); i++)
            {
                Radioss::FITKRadiossPart* part = partMgr->getDataByIndex(i);
                if (part == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(part->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有刚性壁
        Radioss::FITKRadiossRWallManager* rwallMgr = caseData->getRWallManager();
        if (rwallMgr) {
            for (int i = 0; i < rwallMgr->getDataCount(); i++)
            {
                Radioss::FITKAbstractRWall* rwall = rwallMgr->getDataByIndex(i);
                if (rwall == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(rwall->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有的重力
        Radioss::FITKGravityManager* gravityMgr = caseData->getGravityManager();
        if (gravityMgr)
        {
            for (int i = 0; i < gravityMgr->getDataCount(); i++)
            {
                Radioss::FITKGravity* gravity = gravityMgr->getDataByIndex(i);
                if (gravity == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(gravity->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有的场
        Radioss::FITKInitialFieldManager* fieldMgr = caseData->getInitialFieldManager();
        if (fieldMgr)
        {
            for (int i = 0; i < fieldMgr->getDataCount(); i++)
            {
                Radioss::FITKAbstractInitialField* field = fieldMgr->getDataByIndex(i);
                if (field == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(field->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有的边界条件
        Radioss::FITKRadiossBCSManager* bcsMgr = caseData->getBCSManager();
        if (bcsMgr)
        {
            for (int i = 0; i < bcsMgr->getDataCount(); i++)
            {
                Radioss::FITKAbstractBCS* bcs = bcsMgr->getDataByIndex(i);
                if (bcs == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(bcs->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有的相互作用
        Radioss::FITKInteractionManager* interactionMgr = caseData->getInteractionManager();
        if (interactionMgr)
        {
            for (int i = 0; i < interactionMgr->getDataCount(); i++)
            {
                Radioss::FITKAbstractInteraction* interaction = interactionMgr->getDataByIndex(i);
                if (interaction == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(interaction->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获得所有的刚体连接
        Radioss::FITKConnectionManager* connection = caseData->getConnectionManager();
        if (connection){
            for (int i = 0; i < connection->getDataCount(); i++)
            {
                Radioss::FITKAbstractConnection* conn = connection->getDataByIndex(i);
                if (conn == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(conn->getDataObjectID());
                objs << modeObjs;
            }
        }
        //获取所有的探针
        Radioss::FITKProbeManager* probeMgr = caseData->getProbeManager();
        if (probeMgr){
            for (int i = 0; i < probeMgr->getDataCount(); i++)
            {
                Radioss::FITKAbstractProbe* probe = probeMgr->getDataByIndex(i);
                if (probe == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> modeObjs = getGraphObjectsByDataId(probe->getDataObjectID());
                objs << modeObjs;
            }
        }
        //添加至三维窗口
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (obj == nullptr) continue;
            addGraphObjectToWidget(obj, graphWidget, true);
            obj->update(forceUpdate);
        }
        graphWidget->reRender();
        graphWidget->fitView();
    }
    void OperGraphPreprocess::setModelVisible(int dataObjId, bool visibility, bool isFitView)
    {
        //获取或创建可视化对象
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) return;
        Comp::FITKGraph3DWindowVTK* renderWindow = graphWidget->getRenderWindow();
        if (renderWindow == nullptr) return;

        QList<Exchange::GraphVTKObject3D*> objs = getGraphObjectsByDataId(dataObjId);

        //隐藏对象
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) continue;
            obj->setVisible(visibility);
        }

        render();
        if (isFitView) {
            fitView();
        }
    }
    void OperGraphPreprocess::setPointVisible(bool visibility, bool isPart, bool isFitView)
    {
        //获取或创建可视化对象
        if (isPart)
        {
            Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseData == nullptr) return;
            Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
            if (meshModel == nullptr) return;
            //获取所有部件
            Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
            if (partMgr) {
                for (int i = 0; i < partMgr->getDataCount(); i++)
                {
                    Radioss::FITKRadiossPart* part = partMgr->getDataByIndex(i);
                    if (part == nullptr) continue;
                    QList<Exchange::GraphVTKObject3D*> objs = getPartGeoGraphObjectsByDataId(part->getDataObjectID());
                    //隐藏对象
                    for (Exchange::GraphVTKObject3D* obj : objs) {
                        if (obj == nullptr) continue;
                        obj->setPointVisibility(visibility);
                    }
                }
            }
        }
        else
        {
            Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->
                getGeometryData<Interface::FITKGeoCommandList>();
            if (geometryData == nullptr) return;
            //获取所有的几何体
            for (int i = 0; i < geometryData->getDataCount(); i++)
            {
                Interface::FITKAbsGeoCommand* geoCmd = geometryData->getDataByIndex(i);
                if (geoCmd == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> objs = getGraphObjectsByDataId(geoCmd->getDataObjectID());
                //隐藏对象
                for (Exchange::GraphVTKObject3D* obj : objs) {
                    if (obj == nullptr) continue;
                    obj->setPointVisibility(visibility);
                }
            }
        }

        render();
        if (isFitView) {
            fitView();
        }
    }

    void OperGraphPreprocess::setLineVisible(bool visibility, bool isPart, bool isFitView)
    {
        //获取或创建可视化对象
        if (isPart)
        {
            Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseData == nullptr) return;
            Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
            if (meshModel == nullptr) return;
            //获取所有部件
            Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
            if (partMgr) {
                for (int i = 0; i < partMgr->getDataCount(); i++)
                {
                    Radioss::FITKRadiossPart* part = partMgr->getDataByIndex(i);
                    if (part == nullptr) continue;
                    QList<Exchange::GraphVTKObject3D*> objs = getPartGeoGraphObjectsByDataId(part->getDataObjectID());
                    //隐藏对象
                    for (Exchange::GraphVTKObject3D* obj : objs) {
                        if (obj == nullptr) continue;
                        obj->setLineVisibility(visibility);
                    }
                }
            }
        }
        else
        {
            Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->
                getGeometryData<Interface::FITKGeoCommandList>();
            if (geometryData == nullptr) return;
            //获取所有的几何体
            for (int i = 0; i < geometryData->getDataCount(); i++)
            {
                Interface::FITKAbsGeoCommand* geoCmd = geometryData->getDataByIndex(i);
                if (geoCmd == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> objs = getGraphObjectsByDataId(geoCmd->getDataObjectID());
                //隐藏对象
                for (Exchange::GraphVTKObject3D* obj : objs) {
                    if (obj == nullptr) continue;
                    obj->setLineVisibility(visibility);
                }
            }
        }

        render();
        if (isFitView) {
            fitView();
        }
    }

    void OperGraphPreprocess::setAllModelVisible(bool visibility, bool isPart, bool isFitView)
    {
        //获取或创建可视化对象
        if (isPart)
        {
            Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseData == nullptr) return;
            Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
            if (meshModel == nullptr) return;
            //获取所有部件
            Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
            if (partMgr) {
                for (int i = 0; i < partMgr->getDataCount(); i++)
                {
                    Radioss::FITKRadiossPart* part = partMgr->getDataByIndex(i);
                    if (part == nullptr) continue;
                    QList<Exchange::GraphVTKObject3D*> objs = getPartGeoGraphObjectsByDataId(part->getDataObjectID());
                    //隐藏对象
                    for (Exchange::GraphVTKObject3D* obj : objs) {
                        if (obj == nullptr) continue;
                        obj->setCellVisible(visibility);
                    }
                }
            }
        }
        else
        {
            Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->
                getGeometryData<Interface::FITKGeoCommandList>();
            if (geometryData == nullptr) return;
            //获取所有的几何体
            for (int i = 0; i < geometryData->getDataCount(); i++)
            {
                Interface::FITKAbsGeoCommand* geoCmd = geometryData->getDataByIndex(i);
                if (geoCmd == nullptr) continue;
                QList<Exchange::GraphVTKObject3D*> objs = getGraphObjectsByDataId(geoCmd->getDataObjectID());
                //隐藏对象
                for (Exchange::GraphVTKObject3D* obj : objs) {
                    if (obj == nullptr) continue;
                    obj->setCellVisible(visibility);
                }
            }
        }

        render();
        if (isFitView) {
            fitView();
        }
    }

    void OperGraphPreprocess::hightObject(int dataObjId, QColor color)
    {
        //获取可视化窗口
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) return;
        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs;
        objs = getGraphObjectsByDataId(dataObjId);
        if (objs.isEmpty()) return;

        //添加至三维窗口
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (obj == nullptr || obj->isHighlighting() == true) continue;

            obj->highlight(color);
            addGraphHightlightObjectToWidget(obj, graphWidget);
        }
    }
    void OperGraphPreprocess::clearAllHight()
    {
        // 获取可视化窗口。
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (!graphWidget) return;

        QList<Exchange::GraphVTKObject3D*> objs = getAllGraphObject();
        if (objs.isEmpty())return;

        // 添加至三维窗口。
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr || obj->isHighlighting() == false) continue;

            obj->disHighlight();
            addGraphObjectToWidget(obj, graphWidget);
        }
    }
    void OperGraphPreprocess::updatePointSize(int dataObjId, int pointSize)
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) {
            return;
        }

        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs;
        if (dataObjId == -1)
        {
            objs = getAllGeoGraphObject();
        }
        else if(dataObjId == -2)
        {
            objs = getAllMeshGraphObject();
        }
        else {
            objs = getGraphObjectsByDataId(dataObjId);
        }
        //刷新颜色
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updatePointSize(pointSize);
        }

        // 刷新渲染。
        render();
    }
    void OperGraphPreprocess::updateLineWidth(int dataObjId, int lineWidth)
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) {
            return;
        }

        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs;
        if(dataObjId == -1)
        {
            objs = getAllGeoGraphObject();
        }
        else if(dataObjId == -2)
        {
            objs = getAllMeshGraphObject();
        }
        else {
            objs = getGraphObjectsByDataId(dataObjId);
        }
        //刷新颜色
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateLineWidth(lineWidth);
        }
        // 刷新渲染。
        render();
    }
    void OperGraphPreprocess::updateSHPSize(int dataObjId, double size)
    {
        GUI::GraphArea* graphWidget = getGraphWidget(GUI::GraphAreaType::GATPre);
        if (graphWidget == nullptr) {
            return;
        }

        //获取或创建可视化对象
        QList<Exchange::GraphVTKObject3D*> objs;
        if (dataObjId == -1)
        {
            objs = getAllGeoGraphObject();
        }
        else if (dataObjId == -2)
        {
            objs = getAllMeshGraphObject();
        }
        else {
            objs = getGraphObjectsByDataId(dataObjId);
        }
        //刷新颜色
        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateSHPSize(size);
        }
        // 刷新渲染。
        render();
    }
    void OperGraphPreprocess::finalize()
    {
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPre);
        for (GUI::GraphArea* area : graphWidgets) {
            if (area) {
                Comp::FITKGraph3DWindowVTK* graphWindow = qobject_cast<Comp::FITKGraph3DWindowVTK*>(area);
                if (graphWindow) {
                    graphWindow->clear();
                }
            }
        }
    }
}
