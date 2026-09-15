/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ClipSliceWidgetSphere.h"
#include "ui_ClipSliceWidgetSphere.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "PostGraphAdaptor/PostGraphWidgetSphere.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostClipSphere.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceSphere.h"

namespace GUI
{
    ClipSliceWidgetSphere::ClipSliceWidgetSphere(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget* parent) :
        ClipSliceWidgetBase(widgetType, parent), _parentDataID(parentDataID)
    {
        _currentDataID = currentDataID;

        _ui = new Ui::ClipSliceWidgetSphere();
        _ui->setupUi(this);

        MainWindow* mainWin = FITKAPP->getGlobalData()->getMainWindowT<MainWindow>();
        if (mainWin == nullptr)return;
        QWidget* currentWidget = mainWin->getRenderWidget()->getRenderWidget();
        _sphereWidget = new Interface::PostGraphWidgetSphere(currentWidget);

        connect(_sphereWidget, SIGNAL(sigValueChange(double*, double)), this, SLOT(slotVTKWidgetValueChange(double*, double)));
        connect(_ui->lineEdit_CenterX, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_CenterY, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_CenterZ, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Radius, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
    }

    ClipSliceWidgetSphere::~ClipSliceWidgetSphere()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }

        if (_sphereWidget) {
            delete _sphereWidget;
            _sphereWidget = nullptr;
        }
    }

    void ClipSliceWidgetSphere::getDataFromWidget(Interface::FITKAbstractCFDPostData * postData)
    {
        double origin[3] = { 0,0,0 };
        double radius = 0;
        origin[0] = _ui->lineEdit_CenterX->value();
        origin[1] = _ui->lineEdit_CenterY->value();
        origin[2] = _ui->lineEdit_CenterZ->value();
        radius = _ui->lineEdit_Radius->value();

        switch (_widgetType){
        case GUI::ClipSlice::None:break;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipSphere* currentData = dynamic_cast<Interface::FITKCFDPostClipSphere*>(postData);
            if (currentData == nullptr)return;
            currentData->setValue(origin, radius);
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSliceSphere* currentData = dynamic_cast<Interface::FITKCFDPostSliceSphere*>(postData);
            if (currentData == nullptr)return;
            currentData->setValue(origin, radius);
            break;
        }
        }
    }

    void ClipSliceWidgetSphere::setDataToWidget(Interface::FITKAbstractCFDPostData* postParentData, Interface::FITKAbstractCFDPostData* postData)
    {
        //获取边界、中心点、轴向
        double bounds[6] = { 0,0,0,0,0,0 };
        double origin[3] = { 0,0,0 };
        double radius = 0;

        if (postParentData) {
            postParentData->getOrigin(origin);
            postParentData->getBounds(bounds);
            Core::FITKPoint point1(bounds[0], bounds[2], bounds[4]);
            Core::FITKPoint point2(bounds[1], bounds[3], bounds[5]);
            radius = Core::Distance(point1, point2) / 2.0;
        }

        switch (_widgetType){
        case GUI::ClipSlice::None:return;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipSphere* currentData = dynamic_cast<Interface::FITKCFDPostClipSphere*>(postData);
            if (currentData) {
                currentData->getValue(origin, radius);
            }
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSliceSphere* currentData = dynamic_cast<Interface::FITKCFDPostSliceSphere*>(postData);
            if (currentData) {
                currentData->getValue(origin, radius);
            }
            break;
        }
        }

        _ui->lineEdit_CenterX->setText(QString::number(origin[0]));
        _ui->lineEdit_CenterY->setText(QString::number(origin[1]));
        _ui->lineEdit_CenterZ->setText(QString::number(origin[2]));
        _ui->lineEdit_Radius->setText(QString::number(radius));

        _sphereWidget->setValue(origin, radius);
    }

    void ClipSliceWidgetSphere::showEvent(QShowEvent * event)
    {
        if (_sphereWidget) {
            _sphereWidget->setIsShow(true);
            reRender();
        }
        ClipSliceWidgetBase::showEvent(event);
    }

    void ClipSliceWidgetSphere::closeEvent(QCloseEvent * event)
    {
        if (_sphereWidget) {
            _sphereWidget->setIsShow(false);
            delete _sphereWidget;
            _sphereWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::closeEvent(event);
    }

    void ClipSliceWidgetSphere::hideEvent(QHideEvent * event)
    {
        if (_sphereWidget) {
            _sphereWidget->setIsShow(false);
            delete _sphereWidget;
            _sphereWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::hideEvent(event);
    }

    void ClipSliceWidgetSphere::slotVTKWidgetValueChange(double* center, double radius)
    {
        if (_ui == nullptr)return;
        _ui->lineEdit_CenterX->setText(QString::number(center[0]));
        _ui->lineEdit_CenterY->setText(QString::number(center[1]));
        _ui->lineEdit_CenterZ->setText(QString::number(center[2]));
        _ui->lineEdit_Radius->setText(QString::number(radius));
    }

    void ClipSliceWidgetSphere::slotDataChange()
    {
        if (_sphereWidget == nullptr)return;
        double origin[3] = { 0,0,0 };
        double radius = 0.0;
        origin[0] = _ui->lineEdit_CenterX->value();
        origin[1] = _ui->lineEdit_CenterY->value();
        origin[2] = _ui->lineEdit_CenterZ->value();
        radius = _ui->lineEdit_Radius->value();
        _sphereWidget->setValue(origin, radius);
        reRender();
    }
}

