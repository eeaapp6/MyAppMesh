/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropertyWidget.h"
#include "ui_PropertyWidget.h"

#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace GUI
{
    PropertyWidget::PropertyWidget(QWidget * parent) :
        GUIWidgetBase(parent)
    {
        _ui = new Ui::PropertyWidget();
        _ui->setupUi(this);
        init();
    }

    PropertyWidget::~PropertyWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void PropertyWidget::init()
    {
        //关闭滑块跟踪
        _ui->horizontalSlider_Opacity->setTracking(false);
        updateCurrentGraphObj(-1);
    }

    void PropertyWidget::updateCurrentGraphObj(int postDataID)
    {
        _currentDataID = postDataID;
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(postDataID);
        _ui->label_NameInfo->setText("");
        if (currentData) {
            _ui->label_NameInfo->setText(currentData->getDataObjectName());
        }
        updateOpacity();
    }

    void PropertyWidget::on_horizontalSlider_Opacity_valueChanged(int value)
    {
        Q_UNUSED(value);
        QSignalBlocker blocker(_ui->spinBox_Opacity);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        double opacity = _ui->horizontalSlider_Opacity->value();
        currentGraphObj->getProperty()->setOpacity(opacity / 100);
        _ui->spinBox_Opacity->setValue(opacity);
        reRender();
    }

    void PropertyWidget::on_spinBox_Opacity_editingFinished()
    {
        QSignalBlocker blocker(_ui->horizontalSlider_Opacity);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        double opacity = _ui->spinBox_Opacity->value();
        currentGraphObj->getProperty()->setOpacity(opacity / 100);
        _ui->horizontalSlider_Opacity->setValue(opacity);
        reRender();
    }

    void PropertyWidget::updateOpacity()
    {
        QSignalBlocker blocker1(_ui->horizontalSlider_Opacity);
        QSignalBlocker blocker2(_ui->spinBox_Opacity);
        _ui->horizontalSlider_Opacity->setValue(100);
        _ui->spinBox_Opacity->setValue(100);

        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        double opacity = currentGraphObj->getProperty()->getOpacity();
        _ui->horizontalSlider_Opacity->setValue(opacity * 100);
        _ui->spinBox_Opacity->setValue(opacity * 100);
    }
}