/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSketchManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include "GUIFrame/RenderWidget.h"

#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

#include <cmath>

#include <QAction>

namespace ModelOper
{
    // 完成拾取平面，进入草绘
    void OperSketchManager::startSketchSlot() {

        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        // 解析拾取数据
        auto pickedList = pickDataProvider->getPickedList();
        if (pickedList.size() != 1) return;
        auto pickData = pickedList.at(0);
        if (pickData == nullptr) return;
        // 尝试获取虚拓扑
        auto virtualTop = pickData->getVirtualTopo();

        int planeId = pickData->getDatumID();
        if (planeId < 1 && virtualTop)
        {
            planeId = virtualTop->getDataObjectID();
        }
        // 解析数据失败
        if (planeId < 1) return;
        // 清除状态
        GUI::GUIPickInfo::ClearPickInfo();

        pickDataProvider->disconnect(this);
        // 进入草绘
        auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            bool result = graphPreprocessOper->enableSketchMode(true, planeId);
            // 成功进入草绘，创建新的草绘对象
            if (result) {
                auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
                auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
                auto sketchCmd = fac->createCommandT<Interface::FITKAbsGeoSketch2D>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D);
                if (sketchCmd == nullptr) return;
                sketchCmd->setDataObjectName(geoCmdList->checkName(QString("Sketch-%1").arg(geoCmdList->getDataCount() + 1)));

                // 获取草绘平面参数
                auto renderWidget = m_MainWindow->getRenderWidget();
                if (renderWidget == nullptr) return;
                auto graph3DWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(renderWidget->getGraph3DWidget());
                if (graph3DWidget == nullptr) return;
                auto points = graph3DWidget->getSketchPoint();
                double planeOri[3];
                double planeDir[3];
                double planeYDir[3];
                graph3DWidget->getSketchPln(planeOri, planeDir, planeYDir);
                sketchCmd->setPosition(planeOri);
                sketchCmd->setNormal(planeDir);
                sketchCmd->setUp(planeYDir);

                geoCmdList->appendDataObj(sketchCmd);

                // 激活当前草图
                geoCmdList->setCurrentSketchID(sketchCmd->getDataObjectID());

                // 置顶草图可视化对象。
                graphPreprocessOper->setSketchTopMost(sketchCmd->getDataObjectID());

                // 禁用主页面按钮。
                m_MainWindow->setSketchMode(true);

                // 开启点拾取模式进行吸附辅助。
                GUI::GUIPickInfoStru pickInfo;
                pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
                pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJVert;
                pickInfo._enableModelPick = true;
                pickInfo._enableSketchPick = true;
                GUI::GUIPickInfo::SetPickInfo(pickInfo);

                if (m_MainWindow != nullptr) m_MainWindow->updateGeometryTree();
            }
        }
    }
    bool OperSketchManager::setSketchMode(QString actionName)
    {
        // 获取当前草绘
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto currentSketch = geoCmdList->getCurrentSketchData();
        if (currentSketch == nullptr) return false;

        auto renderWidget = m_MainWindow->getRenderWidget();
        if (renderWidget == nullptr) return false;
        auto graph3DWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(renderWidget->getGraph3DWidget());
        if (graph3DWidget == nullptr) return false;

        graph3DWidget->enableSketch(false);
        m_SketchModeType = CommonOCC::SketchShapeType::None;

        static QMetaObject::Connection conn;
        disconnect(conn);

        if (actionName == "actionSketchLine") {
            m_SketchModeType = CommonOCC::SketchShapeType::Draft_Line;
        }
        else if (actionName == "actionSketchWire") {
            m_SketchModeType = CommonOCC::SketchShapeType::Draft_Wire;
        }
        else if (actionName == "actionSketchCircle") {
            m_SketchModeType = CommonOCC::SketchShapeType::Draft_Circle;
        }
        else if (actionName == "actionSketchCircleArc") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_ArcTools; }
        else if (actionName == "actionSketchEllipse") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_Ellipse; }
        else if (actionName == "actionSketchHyperbolaArc") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_Hyperbola; }
        else if (actionName == "actionSketchParabolaArc") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_Parabola; }
        else if (actionName == "actionSketchRectangle") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_Rectangle; }
        else if (actionName == "actionSketchBezier") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_BezierTools; }
        else if (actionName == "actionSketchBspline") { m_SketchModeType = CommonOCC::SketchShapeType::Draft_Bspline; }
        else return false;

        conn = connect(graph3DWidget, SIGNAL(sig_parametersSatisfy()), this, SLOT(endSketchMode()), Qt::UniqueConnection);
        graph3DWidget->enableSketch(true, m_SketchModeType);

        // 提示信息。
        AppFrame::FITKMessageNormal(tr("Press Mouse-R to exit the interactive mode, press Ctrl to snap to the grid node."));

        return true;
    }
    void OperSketchManager::endSketchMode()
    {
        // 获取当前草绘
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto currentSketch = geoCmdList->getCurrentSketchData();
        if (currentSketch == nullptr) return;

        // 获取草绘平面参数
        auto renderWidget = m_MainWindow->getRenderWidget();
        if (renderWidget == nullptr) return;
        auto graph3DWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(renderWidget->getGraph3DWidget());
        if (graph3DWidget == nullptr) return;
        auto points = graph3DWidget->getSketchPoint();
        double planeOri[3];
        double planeDir[3];
        double planeYDir[3];
        graph3DWidget->getSketchPln(planeOri, planeDir, planeYDir);

        // 处理不同的草绘模式
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        switch (m_SketchModeType)
        {
        case CommonOCC::SketchShapeType::Draft_Line:
        {
            // 起点、终点
            if (points.size() != 2) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelSegment>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSegment);
            cmd->setStartPoint(points.at(0)[0], points.at(0)[1], points.at(0)[2]);
            cmd->setEndPoint(points.at(1)[0], points.at(1)[1], points.at(1)[2]);
            cmd->setDataObjectName(currentSketch->checkName(QString("Segment-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Wire:
        {
            // 拐点列表
            if (points.size() < 2) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelWire>(Interface::FITKGeoEnum::FITKGeometryComType::FGTWire);
            for (int i = 0; i < points.size(); ++i) {
                cmd->addPoint(points.at(i)[0], points.at(i)[1], points.at(i)[2]);
                //printLog(2, QString("Point%1: %2, %3, %4").arg(i + 1).arg(points.at(i)[0]).arg(points.at(i)[1]).arg(points.at(i)[2]));
            }
            cmd->setDataObjectName(currentSketch->checkName(QString("Wire-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Circle:
        {
            // 圆心、圆上任意一点
            if (points.size() != 2) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelCircle>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircle);


            auto x1 = points.at(0)[0];
            auto y1 = points.at(0)[1];
            auto z1 = points.at(0)[2];

            auto x2 = points.at(1)[0];
            auto y2 = points.at(1)[1];
            auto z2 = points.at(1)[2];

            cmd->setOrigin(x1, y1, z1);
            cmd->setYDirection(planeYDir[0], planeYDir[1], planeYDir[2]);
            cmd->setZDirection(planeDir[0], planeDir[1], planeDir[2]);

            cmd->setRadius(std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2)));

            cmd->setDataObjectName(currentSketch->checkName(QString("Circle-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_ArcTools:
        {
            // 圆心，圆弧起点、圆弧终点
            if (points.size() != 3 && points.size() != 4) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelCircleArc>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircleArc);

            auto x1 = points.at(0)[0];
            auto y1 = points.at(0)[1];
            auto z1 = points.at(0)[2];

            auto x2 = points.at(1)[0];
            auto y2 = points.at(1)[1];
            auto z2 = points.at(1)[2];

            auto x3 = points.at(2)[0];
            auto y3 = points.at(2)[1];
            auto z3 = points.at(2)[2];

            cmd->setOrigin(x1, y1, z1);
            cmd->setYDirection(planeYDir[0], planeYDir[1], planeYDir[2]);
            cmd->setZDirection(planeDir[0], planeDir[1], planeDir[2]);
            cmd->setRadius(std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2)));

            cmd->setStartPoint(x2, y2, z2);
            cmd->setEndPoint(x3, y3, z3);
            cmd->setDataObjectName(currentSketch->checkName(QString("ArcOfCircle-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Ellipse:
        {
            // 圆心，圆弧起点、圆弧终点
            if (points.size() != 3 && points.size() != 4) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelEllipse>(Interface::FITKGeoEnum::FITKGeometryComType::FGTEllipse);

            auto x1 = points.at(0)[0];
            auto y1 = points.at(0)[1];
            auto z1 = points.at(0)[2];

            auto x2 = points.at(1)[0];
            auto y2 = points.at(1)[1];
            auto z2 = points.at(1)[2];

            auto x3 = points.at(2)[0];
            auto y3 = points.at(2)[1];
            auto z3 = points.at(2)[2];

            cmd->setOrigin(x1, y1, z1);

            cmd->setZDirection(planeDir[0], planeDir[1], planeDir[2]);

            auto radius1 = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));
            auto radius2 = std::sqrt(std::pow(x3 - x1, 2) + std::pow(y3 - y1, 2) + std::pow(z3 - z1, 2));

            if (radius1 > radius2) {
                cmd->setMajorRadius(radius1);
                cmd->setMinorRadius(radius2);
                cmd->setYDirection(x3 - x1, y3 - y1, z3 - z1);
            }
            else {
                cmd->setMajorRadius(radius2);
                cmd->setMinorRadius(radius1);
                cmd->setYDirection(x2 - x1, y2 - y1, z2 - z1);
            }

            cmd->setDataObjectName(currentSketch->checkName(QString("Ellipse-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Rectangle:
        {
            // 四个顶点
            if (points.size() != 4) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelWire>(Interface::FITKGeoEnum::FITKGeometryComType::FGTWire);
            for (int i = 0; i < points.size(); ++i) {
                cmd->addPoint(points.at(i)[0], points.at(i)[1], points.at(i)[2]);
            }
            cmd->addPoint(points.at(0)[0], points.at(0)[1], points.at(0)[2]);

            cmd->setDataObjectName(currentSketch->checkName(QString("Rectangle-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Hyperbola:
        {
            // 焦点、数轴交点、顶点、起始点、终止点
            if (points.size() != 5) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelHyperbolaArc>(Interface::FITKGeoEnum::FITKGeometryComType::FGTHyperbolaArc);
            /* 设置局部坐标系 */
            /*@{*/
            cmd->setOrigin(points.at(1)[0], points.at(1)[1], points.at(1)[2]);
            cmd->setZDirection(planeDir[0], planeDir[1], planeDir[2]);
            std::array<double, 3> xDirection = { points.at(0)[0] - points.at(1)[0],points.at(0)[1] - points.at(1)[1],points.at(0)[2] - points.at(1)[2] };
            std::array<double, 3> yDirection = {
                planeDir[1] * xDirection[2] - xDirection[1] * planeDir[2],
                xDirection[0] * planeDir[2] - planeDir[0] * xDirection[2],
                planeDir[0] * xDirection[1] - xDirection[0] * planeDir[1] };
            cmd->setYDirection(yDirection);
            /*@}*/

            /* 计算实轴和虚轴 */
            /*@{*/
            double a = std::sqrt(std::pow(points.at(2)[0] - points.at(1)[0], 2) + std::pow(points.at(2)[1] - points.at(1)[1], 2) + std::pow(points.at(2)[2] - points.at(1)[2], 2));
            double c = std::sqrt(std::pow(points.at(0)[0] - points.at(1)[0], 2) + std::pow(points.at(0)[1] - points.at(1)[1], 2) + std::pow(points.at(0)[2] - points.at(1)[2], 2));
            double b = std::sqrt(std::pow(c, 2) - std::pow(a, 2));
            cmd->setMajorRadius(a);
            cmd->setMinorRadius(b);
            /*@}*/

            cmd->setStartPoint(points.at(3)[0], points.at(3)[1], points.at(3)[2]);
            cmd->setEndPoint(points.at(4)[0], points.at(4)[1], points.at(4)[2]);

            cmd->setDataObjectName(currentSketch->checkName(QString("ArcOfHyper-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);

            break;
        }
        case CommonOCC::SketchShapeType::Draft_Parabola:
        {
            // 焦点、顶点、起始点、终止点
            if (points.size() != 4) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelParabolaArc>(Interface::FITKGeoEnum::FITKGeometryComType::FGTParabolaArc);
            /* 设置局部坐标系 */
            /*@{*/
            cmd->setOrigin(points.at(1)[0], points.at(1)[1], points.at(1)[2]);
            cmd->setZDirection(planeDir[0], planeDir[1], planeDir[2]);
            std::array<double, 3> xDirection = { points.at(0)[0] - points.at(1)[0],points.at(0)[1] - points.at(1)[1],points.at(0)[2] - points.at(1)[2] };
            std::array<double, 3> yDirection = {
                planeDir[1] * xDirection[2] - xDirection[1] * planeDir[2],
                xDirection[0] * planeDir[2] - planeDir[0] * xDirection[2],
                planeDir[0] * xDirection[1] - xDirection[0] * planeDir[1] };
            cmd->setYDirection(yDirection);
            /*@}*/

            /* 计算实轴和虚轴 */
            /*@{*/
            double focal = std::sqrt(std::pow(points.at(0)[0] - points.at(1)[0], 2) + std::pow(points.at(0)[1] - points.at(1)[1], 2) + std::pow(points.at(0)[2] - points.at(1)[2], 2));
            cmd->setFocalLength(focal);
            /*@}*/

            cmd->setStartPoint(points.at(2)[0], points.at(2)[1], points.at(2)[2]);
            cmd->setEndPoint(points.at(3)[0], points.at(3)[1], points.at(3)[2]);

            cmd->setDataObjectName(currentSketch->checkName(QString("ArcOfPara-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_Bspline:
        {
            // 通过B样条曲线的点列表
            if (points.size() < 2) return;

            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelBsplineByThroughPoints>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBSplineByThroughPoints);
            for (auto xyz : points) {
                cmd->addThroughPoint(xyz[0], xyz[1], xyz[2]);
            }
            cmd->setDataObjectName(currentSketch->checkName(QString("BSpline-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        case CommonOCC::SketchShapeType::Draft_BezierTools:
        {
            // 贝塞尔曲线的控制点列表
            if (points.size() < 2) return;
            auto cmd = fac->createCommandT<Interface::FITKAbsGeoModelBeizeByControlPoints>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBezierByControlPoints);
            for (auto xyz : points) {
                cmd->addControlPoint(xyz[0], xyz[1], xyz[2]);
            }
            cmd->setDataObjectName(currentSketch->checkName(QString("Beize-%1").arg(currentSketch->getDataCount() + 1)));
            if (!cmd->update()) return;
            currentSketch->appendDataObj(cmd);
            break;
        }
        default:
            return;
            break;
        }
        // 合并草绘数据
        if (!currentSketch->update()) return;
        auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->updateGraph(currentSketch->getDataObjectID());
        }

        execProfession();
        graph3DWidget->enableSketch(false, m_SketchModeType);
        graph3DWidget->disconnect(this);
    }
    /**
      * @brief 界面逻辑，生成UI交互
      * @return 执行成功返回true，否则返回false
      * @author yanzhihui (chanyuantiandao@126.com)
      * @date 2023-08-15
      */
    bool OperSketchManager::execGUI()
    {
        auto action = qobject_cast<QAction*>(_emitter);
        if (action == nullptr) return false;

        // 先关闭所有窗口。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* iDlg : dlgs)
        {
            if (iDlg)
            {
                iDlg->close();
            }
        }

        if (setSketchMode(action->objectName())) false;

        // 尝试获取草图数据。
        Interface::FITKAbsGeoSketch2D* cmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>
            (FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getDataByID(m_Id));

        switch (m_OperType)
        {
        case Create:
        {
            auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
            // 按下草绘按钮，进入拾取草绘平面状态
            if (action->isChecked()) {
                GUI::GUIPickInfoStru pickInfo{};
                pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
                pickInfo._pickObjType = GUI::GUIPickInfo::POBJFace;
                pickInfo._enableDatumPick = true;
                GUI::GUIPickInfo::SetPickInfo(pickInfo);
                connect(pickDataProvider, SIGNAL(sig_dataPicked()), this, SLOT(startSketchSlot()));

                // 提示信息。
                AppFrame::FITKMessageNormal(tr("Waiting for a plane for sketcher..."));
            }
            else {
                // 清理资源
                GUI::GUIPickInfo::ClearPickInfo();
                auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
                pickDataProvider->disconnect(this);
                // 关闭草绘模式
                auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->enableSketchMode(false, -1);
                }
                // 如果草绘对象为空，则删除
                auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
                auto sketch = geoCmdList->getCurrentSketchData();
                if (sketch != nullptr && sketch->getDataCount() == 0) {
                    geoCmdList->removeDataObj(sketch);
                    //delete sketch;
                }
                // 取消激活当前草图
                geoCmdList->setCurrentSketchID(-1);

                // 取消置顶草图可视化对象。
                graphPreprocessOper->clearSketchTopMost();

                // 启用主页面按钮。
                m_MainWindow->setSketchMode(false);

                // 取消草图按钮勾选状态。
                QAction* sketchAct = m_MainWindow->getSketchAction();
                if (sketchAct)
                {
                    sketchAct->setChecked(false);
                }

                if (m_MainWindow != nullptr) m_MainWindow->updateGeometryTree();

                // 提示信息。
                AppFrame::FITKMessageNormal(tr("Exit sketcher."));
            }
            break;
        }
        case Edit:
        {
            if (!cmd)
            {
                return false;
            }

            // 查询草图按钮，并设置
            QList<QAction*> actionList = m_MainWindow->findChildren<QAction*>();
            for (QAction* action : actionList)
            {
                if (action && action->objectName() == "actionSketch2DCreate")
                {
                    action->setChecked(true);
                    break;
                }
            }

            // 进入草绘
            auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphPreprocessOper != nullptr)
            {
                double pos3[3]{ 0.,0.,0. }, nor3[3]{ 0.,0.,0. }, up3[3]{ 0.,0.,0. };
                cmd->getPlane(pos3, nor3, up3);

                bool result = graphPreprocessOper->enableSketchMode(true, pos3, nor3, up3);
                if (result)
                {
                    // 激活当前草图。
                    Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
                    geoCmdList->setCurrentSketchID(cmd->getDataObjectID());

                    // 置顶草图可视化对象。
                    graphPreprocessOper->setSketchTopMost(cmd->getDataObjectID());

                    // 禁用主页面按钮。
                    m_MainWindow->setSketchMode(true);
                }
            }

            break;
        }
        case Delete:
        {
            // 尝试获取草图数据。
            Interface::FITKAbsGeoSketch2D* cmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>
                (FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getDataByID(m_Id));

            // 判断是否为激活中的草图。
            auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
            if (geoCmdList->getCurrentSketchData() == cmd)
            {
                // 清理资源
                GUI::GUIPickInfo::ClearPickInfo();
                auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
                pickDataProvider->disconnect(this);

                // 关闭草绘模式
                auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->enableSketchMode(false, -1);
                }
                // 如果草绘对象为空，则删除

                auto sketch = geoCmdList->getCurrentSketchData();
                if (sketch != nullptr && sketch->getDataCount() == 0) {
                    geoCmdList->removeDataObj(sketch);
                    //delete sketch;
                }

                // 取消激活当前草图
                geoCmdList->setCurrentSketchID(-1);

                // 取消置顶草图可视化对象。
                graphPreprocessOper->clearSketchTopMost();

                // 启用主页面按钮。
                m_MainWindow->setSketchMode(false);

                // 取消草图按钮勾选状态。
                QAction* sketchAct = m_MainWindow->getSketchAction();
                if (sketchAct)
                {
                    sketchAct->setChecked(false);
                }
            }

            // 清除高亮。
            auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphPreprocessOper)
            {
                graphPreprocessOper->clearHighlight();
            }

            // 移除数据。
            deleteCmd(m_Id);

            return true;
        }
        case Hide:
        {
            if (!cmd)
            {
                return false;
            }

            // 启用数据。
            cmd->enable(false);

            // 更新可视化对象。
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                graphOper->clearHighlight();
                graphOper->updateGraph(m_Id);    
            }

            break;
        }
        case Show:
        {
            if (!cmd)
            {
                return false;
            }

            // 启用数据。
            cmd->enable(true);

            // 更新可视化对象。
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                graphOper->clearHighlight();
                graphOper->updateGraph(m_Id);
            }

            break;
        }
        default:
            break;
        }
        return false;
    }
    /**
     * @brief 业务处理逻辑，在execGUI后执行
     * @return 执行成功返回true，否则返回false
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-15
     */
    bool OperSketchManager::execProfession()
    {
        if (m_MainWindow != nullptr)
            m_MainWindow->updateGeometryTree();
        return true;
    }
} // namespace ModelOper
