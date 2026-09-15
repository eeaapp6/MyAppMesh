/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGUIView.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QFileDialog>
namespace GUIOper
{
    bool OperGUIView::execGUI()
    {

        //获取渲染窗口
        auto mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr) return false;
        auto renderView = mainWindow->getRenderWidget();
        if (renderView == nullptr) return false;
        //获取显示区域
        auto graph3DWidget = renderView->getGraph3DWidget();
        if (graph3DWidget == nullptr) return false;

        auto actionName = _emitter->objectName();
        if (actionName == "actionGUIViewFront") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVZPos);
        }
        else if (actionName == "actionGUIViewBack") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVZNeg);
        }
        else if (actionName == "actionGUIViewTop") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVYPos);
        }
        else if (actionName == "actionGUIViewBottom") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVYNeg);
        }
        else if (actionName == "actionGUIViewLeft") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVXPos);
        }
        else if (actionName == "actionGUIViewRight") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVXNeg);
        }
        else if (actionName == "actionGUIViewISO") {
            graph3DWidget->setView(Core::FITKGraph3DStandardView::F3DVISO);
        }
        else if (actionName == "actionGUIViewAutoFit") {
            graph3DWidget->fitView();
        }
        else if (actionName == "actionGUIViewOrthogonal") {
            graph3DWidget->setParallelProjection(true);
        }
        else if (actionName == "actionGUIViewPerspective") {
            graph3DWidget->setParallelProjection(false);
        }
        else if (actionName == "actionGUIViewSaveImage") {
            QString filePath = QFileDialog::getSaveFileName(nullptr, tr("Save Image"), "",
                "All(*.png;*.bmp;*.ppm;*.jpg;*.jpe;*.jpeg;*.tif;*.tiff;*.exr;*.tga);;"
                "PNG(Portable Network Graphics)(*.png);;"
                "BMP(Bitmap)(*.bmp);;"
                "PPM(Portable Pixmap Format)(*.ppm);;"
                "JPEG/JIFF(Joint Photographic Experts Group)(*.jpg;*.jpe;*.jpeg);;"
                "TIFF(Tagged Image File Format)(*.tif;*.tiff);;"
                "OpenEXR high dynamic-range(*.exr);;"
                "TGA(Truevision Targa Graphic)(*.tga)", nullptr,
                QFileDialog::DontUseNativeDialog);
            if (filePath.isEmpty()) return false;
            graph3DWidget->saveImage(filePath);
        }
        else if (actionName == "actionGUIShowDatums") {
            // 显示初始基准元素。
            EventOper::GraphEventOperator* operGraph = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (operGraph)
            {
                auto act = qobject_cast<QAction*>(_emitter);
                if (act == nullptr) return false;
                operGraph->setDatumsVisibility(act->isChecked());
            }
        }
        else {
            return false;
        }
        return true;
    }

    bool OperGUIView::execProfession()
    {

        return true;
    }
}
