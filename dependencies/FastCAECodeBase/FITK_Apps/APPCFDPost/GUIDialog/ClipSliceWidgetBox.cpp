/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ClipSliceWidgetBox.h"
#include "ui_ClipSliceWidgetBox.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "PostGraphAdaptor/PostGraphWidgetBox.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostClipBox.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceBox.h"

namespace GUI
{
    ClipSliceWidgetBox::ClipSliceWidgetBox(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget* parent) :
        ClipSliceWidgetBase(widgetType, parent), _parentDataID(parentDataID)
    {
        _currentDataID = currentDataID;

        _ui = new Ui::ClipSliceWidgetBox();
        _ui->setupUi(this);

        MainWindow* mainWin = FITKAPP->getGlobalData()->getMainWindowT<MainWindow>();
        if (mainWin == nullptr)return;
        QWidget* currentWidget = mainWin->getRenderWidget()->getRenderWidget();
        _boxWidget = new Interface::PostGraphWidgetBox(currentWidget);

        connect(_boxWidget, SIGNAL(sigValueChange(double*)), this, SLOT(slotVTKWidgetValueChange(double*)));
        connect(_ui->lineEdit_Point1X, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point1Y, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point1Z, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2X, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2Y, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2Z, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
    }

    ClipSliceWidgetBox::~ClipSliceWidgetBox()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }

        if (_boxWidget) {
            delete _boxWidget;
            _boxWidget = nullptr;
        }
    }

    void ClipSliceWidgetBox::getDataFromWidget(Interface::FITKAbstractCFDPostData * postData)
    {
        double bounds[6] = { 0,0,0,0,0,0 };
        bounds[0] = _ui->lineEdit_Point1X->value();
        bounds[1] = _ui->lineEdit_Point2X->value();
        bounds[2] = _ui->lineEdit_Point1Y->value();
        bounds[3] = _ui->lineEdit_Point2Y->value();
        bounds[4] = _ui->lineEdit_Point1Z->value();
        bounds[5] = _ui->lineEdit_Point2Z->value();

        switch (_widgetType){
        case GUI::ClipSlice::None:break;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipBox* currentData = dynamic_cast<Interface::FITKCFDPostClipBox*>(postData);
            if (currentData == nullptr)return;
            currentData->setValue(bounds);
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSliceBox* currentData = dynamic_cast<Interface::FITKCFDPostSliceBox*>(postData);
            if (currentData == nullptr)return;
            currentData->setValue(bounds);
            break;
        }
        }
    }

    void ClipSliceWidgetBox::setDataToWidget(Interface::FITKAbstractCFDPostData* postParentData, Interface::FITKAbstractCFDPostData* postData)
    {
        double bounds[6] = { 0,0,0,0,0,0 };

        if (postParentData) {
            postParentData->getBounds(bounds);
        }

        switch (_widgetType){
        case GUI::ClipSlice::None:return;
        case GUI::ClipSlice::Clip: {
            Interface::FITKCFDPostClipBox* currentData = dynamic_cast<Interface::FITKCFDPostClipBox*>(postData);
            if (currentData) {
                currentData->getValue(bounds);
            }
            break;
        }
        case GUI::ClipSlice::Slice: {
            Interface::FITKCFDPostSliceBox* currentData = dynamic_cast<Interface::FITKCFDPostSliceBox*>(postData);
            if (currentData) {
                currentData->getValue(bounds);
            }
            break;
        }
        }

        _ui->lineEdit_Point1X->setText(QString::number(bounds[0]));
        _ui->lineEdit_Point2X->setText(QString::number(bounds[1]));
        _ui->lineEdit_Point1Y->setText(QString::number(bounds[2]));
        _ui->lineEdit_Point2Y->setText(QString::number(bounds[3]));
        _ui->lineEdit_Point1Z->setText(QString::number(bounds[4]));
        _ui->lineEdit_Point2Z->setText(QString::number(bounds[5]));

        _boxWidget->setValue(bounds);
    }

    void ClipSliceWidgetBox::showEvent(QShowEvent * event)
    {
        if (_boxWidget) {
            _boxWidget->setIsShow(true);
            reRender();
        }
        ClipSliceWidgetBase::showEvent(event);
    }

    void ClipSliceWidgetBox::closeEvent(QCloseEvent * event)
    {
        if (_boxWidget) {
            _boxWidget->setIsShow(false);
            delete _boxWidget;
            _boxWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::closeEvent(event);
    }

    void ClipSliceWidgetBox::hideEvent(QHideEvent * event)
    {
        if (_boxWidget) {
            _boxWidget->setIsShow(false);
            delete _boxWidget;
            _boxWidget = nullptr;
            reRender();
        }
        ClipSliceWidgetBase::hideEvent(event);
    }

    void ClipSliceWidgetBox::slotVTKWidgetValueChange(double* bounds)
    {
        if (_ui == nullptr)return;
        _ui->lineEdit_Point1X->setText(QString::number(bounds[0]));
        _ui->lineEdit_Point2X->setText(QString::number(bounds[1]));
        _ui->lineEdit_Point1Y->setText(QString::number(bounds[2]));
        _ui->lineEdit_Point2Y->setText(QString::number(bounds[3]));
        _ui->lineEdit_Point1Z->setText(QString::number(bounds[4]));
        _ui->lineEdit_Point2Z->setText(QString::number(bounds[5]));
    }

    void ClipSliceWidgetBox::slotDataChange()
    {
        if (_boxWidget == nullptr)return;
        double bounds[6] = { 0,0,0,0,0,0 };
        bounds[0] = _ui->lineEdit_Point1X->value();
        bounds[1] = _ui->lineEdit_Point2X->value();
        bounds[2] = _ui->lineEdit_Point1Y->value();
        bounds[3] = _ui->lineEdit_Point2Y->value();
        bounds[4] = _ui->lineEdit_Point1Z->value();
        bounds[5] = _ui->lineEdit_Point2Z->value();

        _boxWidget->setValue(bounds);
        reRender();
    }
}

