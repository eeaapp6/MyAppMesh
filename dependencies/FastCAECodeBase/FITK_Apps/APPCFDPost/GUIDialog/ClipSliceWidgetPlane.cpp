/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ClipSliceWidgetPlane.h"
#include "ui_ClipSliceWidgetPlane.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "PostGraphAdaptor/PostGraphWidgetPlane.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostClipPlane.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSlicePlane.h"

namespace GUI
{
    ClipSliceWidgetPlane::ClipSliceWidgetPlane(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget* parent) :
        ClipSliceWidgetBase(widgetType, parent), _parentDataID(parentDataID)
    {
        _currentDataID = currentDataID;

        _ui = new Ui::ClipSliceWidgetPlane();
        _ui->setupUi(this);

        MainWindow* mainWin = FITKAPP->getGlobalData()->getMainWindowT<MainWindow>();
        if (mainWin == nullptr)return;
        QWidget* currentWidget = mainWin->getRenderWidget()->getRenderWidget();
        _planeWidget = new Interface::PostGraphWidgetPlane(currentWidget);

        connect(_planeWidget, SIGNAL(sigValueChange(double*, double*)), this, SLOT(slotVTKWidgetValueChange(double*, double*)));
        connect(_ui->lineEdit_OriginX, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_OriginY, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_OriginZ, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_NormalX, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_NormalY, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_NormalZ, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
    }

    ClipSliceWidgetPlane::~ClipSliceWidgetPlane()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }

        if (_planeWidget) {
            delete _planeWidget;
            _planeWidget = nullptr;
        }
    }

    void ClipSliceWidgetPlane::getDataFromWidget(Interface::FITKAbstractCFDPostData * postData)
    {
        double origin[3] = { 0,0,0 };
        double normal[3] = { 0,0,0 };
        origin[0] = _ui->lineEdit_OriginX->value();
        origin[1] = _ui->lineEdit_OriginY->value();
        origin[2] = _ui->lineEdit_OriginZ->value();
        normal[0] = _ui->lineEdit_NormalX->value();
        normal[1] = _ui->lineEdit_NormalY->value();
        normal[2] = _ui->lineEdit_NormalZ->value();

        switch (_widgetType){
        case GUI::ClipSlice::None:break;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipPlane* currentData = dynamic_cast<Interface::FITKCFDPostClipPlane*>(postData);
            if (currentData) {
                currentData->setValue(origin, normal);
            }
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSlicePlane* currentData = dynamic_cast<Interface::FITKCFDPostSlicePlane*>(postData);
            if (currentData) {
                currentData->setValue(origin, normal);
            }
            break;
        }
        }
    }

    void ClipSliceWidgetPlane::setDataToWidget(Interface::FITKAbstractCFDPostData* postParentData, Interface::FITKAbstractCFDPostData* postData)
    {
        //获取边界、中心点、轴向
        double bounds[6] = { 0,0,0,0,0,0 };
        double origin[3] = { 0,0,0 };
        double normal[3] = { 1,0,0 };

        if (postParentData) {
            postParentData->getOrigin(origin);
            postParentData->getBounds(bounds);
        }

        switch (_widgetType){
        case GUI::ClipSlice::None:return;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipPlane* currentData = dynamic_cast<Interface::FITKCFDPostClipPlane*>(postData);
            if (currentData) {
                currentData->getValue(origin, normal);
            }
            break;
        }
        case GUI::ClipSlice::Slice:
            Interface::FITKCFDPostSlicePlane* currentData = dynamic_cast<Interface::FITKCFDPostSlicePlane*>(postData);
            if (currentData) {
                currentData->getValue(origin, normal);
            }
            break;
        }

        _ui->lineEdit_OriginX->setText(QString::number(origin[0]));
        _ui->lineEdit_OriginY->setText(QString::number(origin[1]));
        _ui->lineEdit_OriginZ->setText(QString::number(origin[2]));
        _ui->lineEdit_NormalX->setText(QString::number(normal[0]));
        _ui->lineEdit_NormalY->setText(QString::number(normal[1]));
        _ui->lineEdit_NormalZ->setText(QString::number(normal[2]));

        _planeWidget->setBounds(bounds);
        _planeWidget->setValue(origin, normal);
    }

    void ClipSliceWidgetPlane::showEvent(QShowEvent * event)
    {
        if (_planeWidget) {
            _planeWidget->setIsShow(true);
            reRender();
        }
        ClipSliceWidgetBase::showEvent(event);
    }

    void ClipSliceWidgetPlane::closeEvent(QCloseEvent * event)
    {
        if (_planeWidget) {
            _planeWidget->setIsShow(false);
            delete _planeWidget;
            _planeWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::closeEvent(event);
    }

    void ClipSliceWidgetPlane::hideEvent(QHideEvent * event)
    {
        if (_planeWidget) {
            _planeWidget->setIsShow(false);
            delete _planeWidget;
            _planeWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::hideEvent(event);
    }

    void ClipSliceWidgetPlane::slotVTKWidgetValueChange(double * origin, double * normal)
    {
        if (_ui == nullptr)return;
        _ui->lineEdit_OriginX->setText(QString::number(origin[0]));
        _ui->lineEdit_OriginY->setText(QString::number(origin[1]));
        _ui->lineEdit_OriginZ->setText(QString::number(origin[2]));
        _ui->lineEdit_NormalX->setText(QString::number(normal[0]));
        _ui->lineEdit_NormalY->setText(QString::number(normal[1]));
        _ui->lineEdit_NormalZ->setText(QString::number(normal[2]));
    }

    void ClipSliceWidgetPlane::slotDataChange()
    {
        if (_planeWidget == nullptr)return;
        double origin[3] = { 0,0,0 };
        double normal[3] = { 0,0,0 };
        origin[0] = _ui->lineEdit_OriginX->value();
        origin[1] = _ui->lineEdit_OriginY->value();
        origin[2] = _ui->lineEdit_OriginZ->value();
        normal[0] = _ui->lineEdit_NormalX->value();
        normal[1] = _ui->lineEdit_NormalY->value();
        normal[2] = _ui->lineEdit_NormalZ->value();
        _planeWidget->setValue(origin, normal);
        reRender();
    }
}

