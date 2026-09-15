/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ClipSliceWidgetCylinder.h"
#include "ui_ClipSliceWidgetCylinder.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "PostGraphAdaptor/PostGraphWidgetCylinder.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostClipCylinder.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceCylinder.h"

#include <iostream>

namespace GUI
{
    ClipSliceWidgetCylinder::ClipSliceWidgetCylinder(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget * parent) :
        ClipSliceWidgetBase(widgetType, parent), _parentDataID(parentDataID)
    {
        _currentDataID = currentDataID;

        _ui = new Ui::ClipSliceWidgetCylinder();
        _ui->setupUi(this);

        MainWindow* mainWin = FITKAPP->getGlobalData()->getMainWindowT<MainWindow>();
        if (mainWin == nullptr)return;
        QWidget* currentWidget = mainWin->getRenderWidget()->getRenderWidget();
        _cylinderWidget = new Interface::PostGraphWidgetCylinder(currentWidget);

        connect(_cylinderWidget, SIGNAL(sigValueChange(double*,double*,double)), this, SLOT(slotVTKWidgetValueChange(double*, double*, double)));
        connect(_ui->lineEdit_CenterX, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_CenterY, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_CenterZ, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_AxisX, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_AxisY, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_AxisZ, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Radius, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
    }

    ClipSliceWidgetCylinder::~ClipSliceWidgetCylinder()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }

        if (_cylinderWidget) {
            delete _cylinderWidget;
            _cylinderWidget = nullptr;
        }
    }

    void ClipSliceWidgetCylinder::getDataFromWidget(Interface::FITKAbstractCFDPostData * postData)
    {
        double center[3] = { 0,0,0 }, axis[3] = { 0,1,0 }, radius = 0;
        center[0] = _ui->lineEdit_CenterX->value();
        center[1] = _ui->lineEdit_CenterY->value();
        center[2] = _ui->lineEdit_CenterZ->value();
        axis[0] = _ui->lineEdit_AxisX->value();
        axis[1] = _ui->lineEdit_AxisY->value();
        axis[2] = _ui->lineEdit_AxisZ->value();
        radius = _ui->lineEdit_Radius->value();

        switch (_widgetType) {
        case GUI::ClipSlice::None:break;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipCylinder* currentData = dynamic_cast<Interface::FITKCFDPostClipCylinder*>(postData);
            if (currentData) {
                currentData->setValue(center, axis, radius);
            }
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSliceCylinder* currentData = dynamic_cast<Interface::FITKCFDPostSliceCylinder*>(postData);
            if (currentData) {
                currentData->setValue(center, axis, radius);
            }
            break;
        }
        }
    }

    void ClipSliceWidgetCylinder::setDataToWidget(Interface::FITKAbstractCFDPostData * postParentData, Interface::FITKAbstractCFDPostData * postData)
    {
        //获取边界、中心点、轴向
        double bounds[6] = { 0,0,0,0,0,0 };
        double center[3] = { 0,0,0 }, axis[3] = { 0,1,0 }, radius = 0;

        if (postParentData) {
            postParentData->getOrigin(center);
            postParentData->getBounds(bounds);
            double X = std::abs(bounds[1] - bounds[0]) / 2.0;
            double Z = std::abs(bounds[5] - bounds[4]) / 2.0;
            radius = (X < Z) ? X : Z;
        }

        switch (_widgetType) {
        case GUI::ClipSlice::None:return;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipCylinder* currentData = dynamic_cast<Interface::FITKCFDPostClipCylinder*>(postData);
            if (currentData) {
                currentData->getValue(center, axis, radius);
            }
            break;
        }
        case GUI::ClipSlice::Slice:
            Interface::FITKCFDPostSliceCylinder* currentData = dynamic_cast<Interface::FITKCFDPostSliceCylinder*>(postData);
            if (currentData) {
                currentData->getValue(center, axis, radius);
            }
            break;
        }

        _ui->lineEdit_CenterX->setValue(center[0]);
        _ui->lineEdit_CenterY->setValue(center[1]);
        _ui->lineEdit_CenterZ->setValue(center[2]);
        _ui->lineEdit_AxisX->setValue(axis[0]);
        _ui->lineEdit_AxisY->setValue(axis[1]);
        _ui->lineEdit_AxisZ->setValue(axis[2]);
        _ui->lineEdit_Radius->setValue(radius);

        _cylinderWidget->setBounds(bounds);
        _cylinderWidget->setValue(center, axis, radius);
    }

    void ClipSliceWidgetCylinder::showEvent(QShowEvent * event)
    {
        if (_cylinderWidget) {
            _cylinderWidget->setIsShow(true);
            reRender();
        }
        ClipSliceWidgetBase::showEvent(event);
    }

    void ClipSliceWidgetCylinder::closeEvent(QCloseEvent * event)
    {
        if (_cylinderWidget) {
            _cylinderWidget->setIsShow(false);
            delete _cylinderWidget;
            _cylinderWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::closeEvent(event);
    }

    void ClipSliceWidgetCylinder::hideEvent(QHideEvent * event)
    {
        if (_cylinderWidget) {
            _cylinderWidget->setIsShow(false);
            delete _cylinderWidget;
            _cylinderWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::hideEvent(event);
    }

    void ClipSliceWidgetCylinder::slotVTKWidgetValueChange(double* center, double* axis, double radius)
    {
        if (_ui == nullptr)return;
        _ui->lineEdit_CenterX->setValue(center[0]);
        _ui->lineEdit_CenterY->setValue(center[1]);
        _ui->lineEdit_CenterZ->setValue(center[2]);
        _ui->lineEdit_AxisX->setValue(axis[0]);
        _ui->lineEdit_AxisY->setValue(axis[1]);
        _ui->lineEdit_AxisZ->setValue(axis[2]);
        _ui->lineEdit_Radius->setValue(radius);
    }

    void ClipSliceWidgetCylinder::slotDataChange()
    {
        if (_cylinderWidget == nullptr)return;
        double center[3] = { 0,0,0 }, axis[3] = { 0,0,0 }, radius = 0;
        center[0] = _ui->lineEdit_CenterX->value();
        center[1] = _ui->lineEdit_CenterY->value();
        center[2] = _ui->lineEdit_CenterZ->value();
        axis[0] = _ui->lineEdit_AxisX->value();
        axis[1] = _ui->lineEdit_AxisY->value();
        axis[2] = _ui->lineEdit_AxisZ->value();
        radius = _ui->lineEdit_Radius->value();
        _cylinderWidget->setValue(center, axis, radius);
        reRender();
    }
}