/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PreWindowInitializer.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/CentralWidget.h"
#include "GUIWidget/GraphMdiArea.h"
#include "GUIWidget/GraphArea.h"
#include "OperatorsInterface/GraphInteractionOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/PickedDataProvider.h"


#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphAreaPicker.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

#include <vtkCallbackCommand.h>
#include <vtkInteractorObserver.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkAreaPicker.h>
#include <vtkPlanes.h>
#include <vtkProp3DCollection.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>

PreWindowInitializer::PreWindowInitializer()
{
    // 初始化渲染层数。
    this->setLayerCount(3);

    //背景颜色
    //QColor top = QColor::fromRgb(27, 45, 70);
    //QColor buttom = QColor::fromRgb(160, 174, 195);
    //this->setValue(BackGroundColorTop, top);
    //this->setValue(BackGroundColorButtom, buttom);
}

Comp::FITKGraphInteractionStyle* PreWindowInitializer::getStyle()
{
    return PreWindowInteractionStyle::New();
}

PreWindowInteractionStyle* PreWindowInteractionStyle::New()
{
    return new PreWindowInteractionStyle;
}

PreWindowInteractionStyle::PreWindowInteractionStyle()
{
    // 初始化定时器属性。
    m_timerForPrePick.setInterval(100);
    m_timerForPrePick.setSingleShot(true);

    connect(&m_timerForPrePick, &QTimer::timeout, this, &PreWindowInteractionStyle::slot_prePick);
    connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::setPickableObjTypeSig,
        this, &PreWindowInteractionStyle::slot_pickActivated);
    // 初始化操作器。
    //@{
    auto operatorRepo = Core::FITKOperatorRepo::getInstance();
    m_operPreview = operatorRepo->getOperatorT<EventOper::GraphInteractionOperator>("GraphPickPreview");
    m_operPick = operatorRepo->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
    m_operGraph = operatorRepo->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    //@}
}

void PreWindowInteractionStyle::OnLeftButtonDown()
{
    // TEST.
    //@{
    //GUI::GUIPickInfoStru info = GUI::GUIPickInfo::GetPickInfo();
    //info._pickObjType = GUI::GUIPickInfo::PickObjType::POBJVert;
    //info._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
    //GUI::GUIPickInfo::SetPickInfo(info);
    //@}

    // TEST2.
    //@{
    // GUI::MainTreeEnum::MainTree_MeshPoint = 9
    //GraphOperParam param;
    //param.HighlightMode = HighlightLevel::AdvHighlight;
    //param.AdvHighlightIndice = QVector<int>{ 1 };
    //param.Visibility = true;
    //m_operGraph->updateGraphByType(9, param);
    //@}

    this->Interactor->GetEventPosition(m_leftButtonDowmPos);

    if (m_areaPick != nullptr)
    {
        m_areaPick->setLeftButtonDownPos(m_leftButtonDowmPos);
        m_areaPick->enable(true);
    }
}

void PreWindowInteractionStyle::OnLeftButtonUp()
{
    vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
    this->Interactor->GetEventPosition(m_leftButtonUpPos);

    // 只有在逐个选择模式下支持框选。
    bool canAreaPick = GUI::GUIPickInfo::GetPickInfo()._pickMethod == GUI::GUIPickInfo::PickMethod::PMIndividually;

    if (m_areaPick != nullptr && isMouseMoved())
    {
        if (m_areaPick->isEnable() && canAreaPick)
            m_areaPick->pick();
        m_areaPick->enable(false);
    }
    else
    {
        pick();
    }

    AppFrame::FITKComponentInterface* fcInterface
        = FITKAPP->getComponents()->getComponentByName("Graph3DWindowVTK");
    if (fcInterface)
    {
        CallBackFuns fun = fcInterface->getCallBackFuns(1);
        if (fun != nullptr)
            fun(nullptr);
    }
}

void PreWindowInteractionStyle::OnMiddleButtonDown()
{
    this->Interactor->GetEventPosition(m_leftButtonDowmPos);

    if (FITKAPP->getGlobalData()->getKeyMouseStates()->keyPressed(Qt::Key_Shift))
        vtkInteractorStyleRubberBandPick::OnMiddleButtonDown();
    else
        vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
}

void PreWindowInteractionStyle::OnMiddleButtonUp()
{
    this->Interactor->GetEventPosition(m_leftButtonDowmPos);

    vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
    vtkInteractorStyleRubberBandPick::OnMiddleButtonUp();
    if (m_areaPick != nullptr)
    {
        m_areaPick->enable(false);
    }
}

void PreWindowInteractionStyle::OnMouseMove()
{
    if (m_timerForPrePick.isActive())
    {
        m_timerForPrePick.stop();
    }

    m_timerForPrePick.start();

    this->Interactor->GetEventPosition(m_leftButtonUpPos);

    vtkInteractorStyleRubberBandPick::OnMouseMove();
    bool isMoved = this->isMouseMoved();
    const bool lbd = FITKAPP->getGlobalData()->getKeyMouseStates()->mousePressed(Qt::LeftButton);

    // 非单独拾取，且非算法拾取或非拾取状态时可绘制橡皮筋。
    bool needDrawRect = (GUI::GUIPickInfo::GetPickInfo()._pickMethod == GUI::GUIPickInfo::PickMethod::PMIndividually ||
        GUI::GUIPickInfo::GetPickInfo()._pickMethod == GUI::GUIPickInfo::PickMethod::PMNone);
   
    if (lbd && isMoved && needDrawRect)
    {
        m_areaPick->drawRectangle();
        return;
    }
}

void PreWindowInteractionStyle::OnMouseWheelForward()
{
    this->FindPokedRenderer(
        this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->StartDolly();
    // double factor = this->MotionFactor * -0.2 * this->MouseWheelMotionFactor;
    this->Dolly(0.98); // pow(1.1, factor)
    this->EndDolly();
    this->ReleaseFocus();
}

void PreWindowInteractionStyle::OnMouseWheelBackward()
{
    this->FindPokedRenderer(
        this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->StartDolly();
    // double factor = this->MotionFactor * 0.2 * this->MouseWheelMotionFactor;
    this->Dolly(1.05); // pow(1.1, factor)
    this->EndDolly();
    this->ReleaseFocus();
}

void PreWindowInteractionStyle::OnRightButtonDown()
{
    this->Interactor->GetEventPosition(m_leftButtonDowmPos);
}

void PreWindowInteractionStyle::OnRightButtonUp()
{
    this->Interactor->GetEventPosition(m_leftButtonUpPos);
}

void PreWindowInteractionStyle::areaPick(int* startPos, int* endPos)
{
    // 获取当前三维窗口。
    Comp::FITKGraph3DWindowVTK* graphWindow = getCurrentGraphWindow();
    if (!graphWindow || !m_operPick)
    {
        return;
    }

    // 所有渲染层均执行一次框选并获取拾取演员。
    //@{
    QList<vtkActor*> actors;
    vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();

    for (int i = 0; i < graphWindow->getRenderCount(); i++)
    {
        vtkRenderer* renderer = graphWindow->getRenderer(i)->getRenderer();
        areaPicker->AreaPick(startPos[0], startPos[1], endPos[0], endPos[1], renderer);
        vtkProp3DCollection* props = areaPicker->GetProp3Ds();
        props->InitTraversal();

        const int nActors = props->GetNumberOfItems();
        for (vtkIdType i = 0; i < nActors; i++)
        {
            vtkProp3D* prop = props->GetNextProp3D();
            vtkActor* actor = vtkActor::SafeDownCast(prop);
            if (actor)
            {
                actors.append(actor);
            }
        }
    }
    //@}

    // 没有拾取到，且不是Shift与Ctrl拾取模式则清除拾取。
    AppFrame::FITKKeyMouseStates* settings = FITKGLODATA->getKeyMouseStates();
    bool shiftOrCtrlPick = (settings->keyPressed(Qt::Key_Shift) && !settings->keyPressed(Qt::Key_Control))
        || (!settings->keyPressed(Qt::Key_Shift) && settings->keyPressed(Qt::Key_Control));
    if (actors.isEmpty() && !shiftOrCtrlPick)
    {
        m_operPick->clear(graphWindow);
        return;
    }

    vtkPlanes* cutPlane = areaPicker->GetFrustum();

    // 操作器执行高亮。
    m_operPick->picked(graphWindow, actors, cutPlane);
}

void PreWindowInteractionStyle::pick(bool isPreview)
{
    if (!m_operPreview || !m_operPick){
        return;
    }

    // 获取当前鼠标位置进行拾取。
    vtkRenderWindowInteractor* interactor = this->GetInteractor();
    if (!interactor){
        return;
    }

    Comp::FITKGraph3DWindowVTK* graphWindow = getCurrentGraphWindow();
    if (!graphWindow){
        return;
    }

    int* pos = interactor->GetEventPosition();

    // 获取当前拾取类型。
    GUI::GUIPickInfo::PickObjType type = GUI::GUIPickInfo::GetPickInfo()._pickObjType;
    switch (type){
    case GUI::GUIPickInfo::POBGeoVert:
    case GUI::GUIPickInfo::POBGeoEdge:
    case GUI::GUIPickInfo::POBGeoFace:
    case GUI::GUIPickInfo::POBGeoSolid:
    case GUI::GUIPickInfo::POBMeshCell:
    case GUI::GUIPickInfo::POBMeshCellSurf:
    case GUI::GUIPickInfo::POBMeshVert:
    case GUI::GUIPickInfo::POBMeshVertSurf:
        pickCell(graphWindow, pos, isPreview);
        break;
    default:
        return;
    }
}

void PreWindowInteractionStyle::pickPoint(Comp::FITKGraph3DWindowVTK* graphWindow, int* pos, bool isPreview, double tol)
{
    // 节点拾取器。
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    vtkSmartPointer<vtkPropPicker> pickerProp = vtkSmartPointer<vtkPropPicker>::New();

    picker->SetTolerance(tol);

    // 优先拾取最后一层，反向遍历。
    for (int i = graphWindow->getRenderCount() - 1; i >= 0; i--)
    {
        vtkRenderer* renderer = graphWindow->getRenderer(i)->getRenderer();

        int ret = picker->Pick(pos[0], pos[1], 0, renderer);

        // 是否拾取到对象。
        if (!ret)
        {
            continue;
        }

        // 获取拾取节点索引。
        vtkActor* actor = picker->GetActor();
        int index = picker->GetPointId();
        if (index < 0)
        {
            continue;
        }

        double* pickedWorldPos = picker->GetPickPosition();

        if (isPreview)
        {
            m_operPreview->picked(graphWindow, actor, index, pickedWorldPos);
        }
        else
        {
            m_operPick->picked(graphWindow, actor, index, pickedWorldPos);
        }

        return;
    }

    // 没有拾取到则清除拾取。
    if (isPreview)
    {
        m_operPreview->clear(graphWindow);
    }
    else
    {
        // 没有拾取到，且不是Shift与Ctrl拾取模式则清除拾取。
        AppFrame::FITKKeyMouseStates* settings = FITKGLODATA->getKeyMouseStates();
        bool shiftOrCtrlPick = (settings->keyPressed(Qt::Key_Shift) && !settings->keyPressed(Qt::Key_Control))
            || (!settings->keyPressed(Qt::Key_Shift) && settings->keyPressed(Qt::Key_Control));
        if (!shiftOrCtrlPick)
        {
            m_operPick->clear(graphWindow);
        }
    }
}

void PreWindowInteractionStyle::pickCell(Comp::FITKGraph3DWindowVTK* graphWindow, int* pos, bool isPreview, double tol)
{
    // 单元拾取器。
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();

    picker->SetTolerance(tol);

    // 优先拾取最后一层，反向遍历。
    for (int i = graphWindow->getRenderCount() - 1; i >= 0 ; i--)
    {
        vtkRenderer* renderer = graphWindow->getRenderer(i)->getRenderer();

        // 优先进行参考点标签与符号拾取。
        int ret = picker->Pick(pos[0], pos[1], 0, renderer);

        // 是否拾取到对象。
        if (!ret)
        {
            continue;
        }

        // 获取拾取单元索引。
        vtkActor* actor = picker->GetActor();
        int index = picker->GetCellId();
        if (index < 0)
        {
            continue;
        }

        double* pickedWorldPos = picker->GetPickPosition();

        //判断是否预选状态
        if (isPreview){
            m_operPreview->picked(graphWindow, actor, index, pickedWorldPos);
        }
        else{
            m_operPick->picked(graphWindow, actor, index, pickedWorldPos);
        }

        return;
    }

    // 没有拾取到则清除拾取。
    if (isPreview)
    {
        m_operPreview->clear(graphWindow);
    }
    else
    {
        // 没有拾取到，且不是Shift与Ctrl拾取模式则清除拾取。
        AppFrame::FITKKeyMouseStates* settings = FITKGLODATA->getKeyMouseStates();
        bool shiftOrCtrlPick = (settings->keyPressed(Qt::Key_Shift) && !settings->keyPressed(Qt::Key_Control))
            || (!settings->keyPressed(Qt::Key_Shift) && settings->keyPressed(Qt::Key_Control));
        if (!shiftOrCtrlPick)
        {
            m_operPick->clear(graphWindow);
        }
    }
}

void PreWindowInteractionStyle::slot_pickActivated(int pickType)
{
    // 根据pickType设置拾取对象类型
    GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

    // 根据传入的pickType值设置对应的拾取类型
    switch (pickType)
    {
    case -1: // 关闭拾取
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBNone;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
        break;
    case 3001: // 网格节点
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBMeshVert;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        break;
    case 3002: // 网格单元
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBMeshCell;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        break;
    case 3003: // 几何点
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBGeoVert;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        break;
    case 3004: // 几何线
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBGeoEdge;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        break;
    case 3005: // 几何面
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBGeoFace;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        break;
    case 3006: // 几何体
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBGeoSolid;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        break;
    default:
        // 默认关闭拾取
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBNone;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
        break;
    }
    // 应用拾取设置
    GUI::GUIPickInfo::SetPickInfo(pickInfo);

    GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
    if (!pickProvider) return;
    pickProvider->clearPickedData();
}

void PreWindowInteractionStyle::slot_prePick()
{
    // 预选高亮。
    pick(true);
}

Comp::FITKGraph3DWindowVTK* PreWindowInteractionStyle::getCurrentGraphWindow()
{
    GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
    if (mainWindow == nullptr) return nullptr;

    // 获取可视化区。
    GUI::CentralWidget* renderWidget = mainWindow->getCentralWidget();
    if (!renderWidget) return nullptr;

    GUI::GraphMdiArea* mdiArea = renderWidget->getGraphMidArea();
    if (!mdiArea)return nullptr;

    GUI::GraphArea* curGraphArea = mdiArea->getCurrentGraphWidget<GUI::GraphArea>();
    if (curGraphArea == nullptr)return nullptr;

    //获取当前三维窗口
    Comp::FITKGraph3DWindowVTK* graphWindow = curGraphArea->getRenderWindow();
    return graphWindow;
}