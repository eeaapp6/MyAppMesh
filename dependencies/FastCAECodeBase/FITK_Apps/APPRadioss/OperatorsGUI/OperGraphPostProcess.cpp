/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPostProcess.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/CentralWidget.h"
#include "GUIWidget/GraphMdiArea.h"
#include "GraphDataProvider/GraphPostProvider.h"
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"

#include <vtkScalarBarActor.h>
#include <vtkLookupTable.h>

namespace GUIOper
{
    void OperGraphPostProcess::updateAll(bool forceUpdate)
    {
        Radioss::FITKRadiossPostData* postData = FITKAPP->getGlobalData()->getPostData<Radioss::FITKRadiossPostData>();
        if (postData == nullptr) return;
        GUI::MainWindow* mainWin = GUI::MainWindow::GetMainWindowFromFramework();
        GUI::CentralWidget* centralWidget = mainWin->getCentralWidget();
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        Interface::FITKStruPost3DManager* post3DManager = postData->getPost3DManager();
        int postObjCount = post3DManager->getDataCount();

        for (int i = 0; i < postObjCount; ++i)
        {
            Interface::FITKAbstractStructuralPostVTK* postObj = post3DManager->getDataByIndex(i);
            if (postObj == nullptr) continue;

            GUI::GraphArea* graphW = nullptr;
            if (i < graphWidgets.size())
            {
                graphW = graphWidgets.at(i);
            }
            else
            {
                graphW = createGraphWidget(GUI::GraphAreaType::GATPost);
                if (graphW != nullptr)
                    graphWidgets.append(graphW);
            }

            if (graphW == nullptr) continue;

            GraphData::GraphPostProvider* postProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphW->getRenderWindow());
            if (postProvider == nullptr) continue;

            Exchange::GraphVTKObject3D* graphObj = postProvider->getModelGraphObject(postObj->getDataObjectID());
            if (graphObj == nullptr) continue;

            graphObj->update();
            addGraphObjectToWidget(graphObj, graphW, false);

            //刷新
            render();
            //自适应
            if (forceUpdate) fitView();
        }
        centralWidget->setCurGraphWidgetToPost();
    }
    void OperGraphPostProcess::updateGraph(int dataObjId, bool forceUpdate, bool isFitView)
    {
        Radioss::FITKRadiossPostData* postData = FITKAPP->getGlobalData()->getPostData<Radioss::FITKRadiossPostData>();
        if (postData == nullptr) return;
        GUI::MainWindow* mainWin = GUI::MainWindow::GetMainWindowFromFramework();
        GUI::CentralWidget* centralWidget = mainWin->getCentralWidget();

        // 遍历所有后处理窗口
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) continue;
            GraphData::GraphPostProvider* postProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(),false);
            if (postProvider == nullptr) continue;
            Exchange::GraphVTKObject3D* graphObj = postProvider->getModelGraphObject(dataObjId, false);
            if (graphObj != nullptr)
            {
                centralWidget->setCurGraphWidget(graphWidget);
                graphObj->update();
                return;
            }
        }
        render();
        if (forceUpdate) fitView();
    }
    void OperGraphPostProcess::changeField(int dataId, int type, QString fieldName, int compIndex)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(),false);
            if (modelProvider == nullptr) {
                return;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(dataId, false);
            if (graphObj != nullptr) {

                graphObj->changeField(type, fieldName, compIndex);
            }

        }

        render();
    }
    void OperGraphPostProcess::updateTextInfo(int dataId, QString info)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(dataId,false);
            if (graphObj != nullptr) {

                graphObj->updateTextInfo(info);
            }

        }
        render();
    }
    void OperGraphPostProcess::setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency, int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            if(postId == -1)
            {
                QList<Exchange::GraphVTKObject3D*> graphObjs = modelProvider->getCurrentGraphObjs();
                for (Exchange::GraphVTKObject3D* graphObj : graphObjs)
                {
                    if (graphObj != nullptr) {
                        graphObj->setRangeColor(isOpenOutRanger, minRangeColor, maxRangeColor, transparency);
                    }
                }
                continue;
            }
            else {
                Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
                if (graphObj != nullptr) {

                    graphObj->setRangeColor(isOpenOutRanger, minRangeColor, maxRangeColor, transparency);
                }
            }
        }
        render();
    }

    void OperGraphPostProcess::getRangeColor(bool isOutRanger, int postId, QColor & minRangeColor, QColor & maxRangeColor)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
            if (graphObj != nullptr) {

                graphObj->getRangeColor(isOutRanger, minRangeColor, maxRangeColor);
            }
        }
    }

    void OperGraphPostProcess::setRange(int postId, double * range)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
            if (graphObj != nullptr) {

                graphObj->setRange(range);
            }
        }
    }

    double* OperGraphPostProcess::getRange(int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return nullptr;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
            if (graphObj != nullptr) {

                return graphObj->getRange();
            }
        }
        return nullptr;
    }


    void OperGraphPostProcess::setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum, int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }
            if(postId == -1)
            {
                QList<Exchange::GraphVTKObject3D*> graphObjs = modelProvider->getCurrentGraphObjs();
                for (Exchange::GraphVTKObject3D* graphObj : graphObjs)
                {
                    if (graphObj != nullptr) {
                        graphObj->setOutRangerColor(isOpenOutRanger, aboveRGBA, belowRGBA, colorNum);
                        vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                        scalarBarActor->SetMaximumNumberOfColors(colorNum);
                    }
                }
                continue;
            }
            else {
                Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
                if (graphObj != nullptr) {

                    graphObj->setOutRangerColor(isOpenOutRanger, aboveRGBA, belowRGBA, colorNum);

                    vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                    scalarBarActor->SetMaximumNumberOfColors(colorNum);
                }
            }
        }
        render();
    }
    void OperGraphPostProcess::setRangerColorSetFlag(bool isSet)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

           QList<Exchange::GraphVTKObject3D*> graphObjs = modelProvider->getCurrentGraphObjs();
           for (Exchange::GraphVTKObject3D* graphObj : graphObjs)
           {
               if (graphObj != nullptr) {
                   graphObj->setRangerColorSetFlag(isSet);
               }
           }
        }
        render();
    }
    bool OperGraphPostProcess::getRangerColorSetFlag(int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return false;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }

            Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
            if (graphObj != nullptr) {

                return graphObj->getRangerColorSetFlag();
            }
        }
        return false;
    }

    void OperGraphPostProcess::setRangerColorNum(bool isOpenOutRanger, int colorNum, int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }
            if (postId == -1)
            {
                QList<Exchange::GraphVTKObject3D*> graphObjs = modelProvider->getCurrentGraphObjs();
                for (Exchange::GraphVTKObject3D* graphObj : graphObjs)
                {
                    if (graphObj != nullptr) {
                        vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(scalarBarActor->GetLookupTable());
                        lookupTable->SetNumberOfColors(colorNum);
                    }
                }
                continue;
            }
            else {
                Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
                if (graphObj != nullptr) {
                    vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                    vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(scalarBarActor->GetLookupTable());
                    lookupTable->SetNumberOfColors(colorNum);
                }
            }
        }
        render();
    }
    int OperGraphPostProcess::getRangerColorNum(int postId)
    {
        //获取后处理渲染界面
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
        if (graphWidgets.isEmpty()) {
            return -1;
        }
        for (GUI::GraphArea* graphWidget : graphWidgets)
        {
            if (graphWidget == nullptr) {
                continue;
            }

            GraphData::GraphPostProvider* modelProvider = GraphData::GraphProviderManager::getInstance()->getPostProvider(graphWidget->getRenderWindow(), false);
            if (modelProvider == nullptr) {
                continue;
            }
            if (postId == -1)
            {
                QList<Exchange::GraphVTKObject3D*> graphObjs = modelProvider->getCurrentGraphObjs();
                for (Exchange::GraphVTKObject3D* graphObj : graphObjs)
                {
                    vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                    vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(scalarBarActor->GetLookupTable());
                    return lookupTable->GetNumberOfColors();
                }
            }
            else {
                Exchange::GraphVTKObject3D* graphObj = modelProvider->getModelGraphObject(postId, false);
                if (graphObj != nullptr) {
                    vtkScalarBarActor* scalarBarActor = graphObj->getScalarBarActor();
                    vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(scalarBarActor->GetLookupTable());
                    return lookupTable->GetNumberOfColors();
                }
            }
        }
    }
    void OperGraphPostProcess::finalize()
    {
        QList<GUI::GraphArea*> graphWidgets = getAllGraphWidget(GUI::GraphAreaType::GATPost);
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
