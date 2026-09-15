/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SliceDialog.h"
#include "ui_SliceDialog.h"
#include "ClipSliceWidgetBase.h"
#include "ClipSliceWidgetPlane.h"
#include "ClipSliceWidgetSphere.h"
#include "ClipSliceWidgetCylinder.h"
#include "ClipSliceWidgetBox.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphWidgetPlane.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSlicePlane.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceSphere.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceCylinder.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSliceBox.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(Interface::FITKPostDataType)

namespace GUI
{
    SliceDialog::SliceDialog(int dataID, EventOper::ParaWidgetInterfaceOperator * oper, bool isCreate) :
        GUIDialogBase(oper, "SliceDialog", FITKAPP->getGlobalData()->getMainWindow()),
        _isCreate(isCreate)
    {
        _ui = new Ui::SliceDialog();
        _ui->setupUi(this);
        if (isCreate) {
            _parentDataID = dataID;
        }
        else {
            _currentDataID = dataID;
            if (_postDataManager->getDataByID(_currentDataID)) {
                _parentDataID = _postDataManager->getDataByID(_currentDataID)->getCFDPostParentID();
            }
        }
        init();
    }

    SliceDialog::~SliceDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void SliceDialog::init()
    {
        QSignalBlocker blocker(_ui->comboBox_SliceType);

        _ui->comboBox_SliceType->addItem(tr("Plane"), QVariant::fromValue(Interface::FITKPostDataType::Post_SlicePlane));
        _ui->comboBox_SliceType->addItem(tr("Sphere"), QVariant::fromValue(Interface::FITKPostDataType::Post_SliceSphere));
        _ui->comboBox_SliceType->addItem(tr("Cylinder"), QVariant::fromValue(Interface::FITKPostDataType::Post_SliceCylinder));
        _ui->comboBox_SliceType->addItem(tr("Box"), QVariant::fromValue(Interface::FITKPostDataType::Post_SliceBox));
        _ui->comboBox_SliceType->setCurrentIndex(0);

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("Slice Create"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("Slice Edit"));
        }
    }

    void SliceDialog::on_pushButton_OK_clicked()
    {
        if (_isCreate) {
            if (!OKCreate())return;
        }
        else {
            if (!OKEdit())return;
        }

        if (_oper) {
            _oper->setArgs(objID_String, _currentDataID);
            _oper->execProfession();
        }

        this->close();
    }

    void SliceDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void SliceDialog::on_comboBox_SliceType_currentIndexChanged(int index)
    {
        Q_UNUSED(index);

        //清除全部子界面
        QLayoutItem* item;
        while ((item = _ui->verticalLayout_SubWidget->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater(); // 推荐使用 deleteLater，以确保小部件在适当时机被删除
            }
            delete item; // 删除布局项
        }

        if (_clipWidget) {
            delete _clipWidget;
            _clipWidget = nullptr;
        }

        Interface::FITKPostDataType clipType = _ui->comboBox_SliceType->currentData().value<Interface::FITKPostDataType>();
        switch (clipType) {
        case Interface::FITKPostDataType::Post_SlicePlane: {
            _clipWidget = new ClipSliceWidgetPlane(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceSphere: {
            _clipWidget = new ClipSliceWidgetSphere(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceCylinder: {
            _clipWidget = new ClipSliceWidgetCylinder(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceBox: {
            _clipWidget = new ClipSliceWidgetBox(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        }

        Interface::FITKAbstractCFDPostData* clipData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (_clipWidget) {
            _ui->verticalLayout_SubWidget->addWidget(_clipWidget);
            if (_isCreate) _clipWidget->setDataToWidget(parentData, nullptr);
            else _clipWidget->setDataToWidget(parentData, clipData);
        }
    }

    void SliceDialog::InitCreate()
    {
        //设置名称
        QString name = _postDataManager->checkName("Slice-0");
        _ui->lineEdit_Name->setText(name);

        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        Interface::FITKPostDataType clipType = _ui->comboBox_SliceType->currentData().value<Interface::FITKPostDataType>();
        switch (clipType) {
        case Interface::FITKPostDataType::Post_SlicePlane: {
            _clipWidget = new ClipSliceWidgetPlane(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceSphere: {
            _clipWidget = new ClipSliceWidgetSphere(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceCylinder: {
            _clipWidget = new ClipSliceWidgetCylinder(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceBox: {
            _clipWidget = new ClipSliceWidgetBox(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        }

        if (_clipWidget) {
            _ui->verticalLayout_SubWidget->addWidget(_clipWidget);
            _clipWidget->setDataToWidget(parentData, nullptr);
        }
    }

    void SliceDialog::InitEdit()
    {
        Interface::FITKAbstractCFDPostData* clipData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        if (clipData == nullptr)return;
        //设置名称
        QString name = clipData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        //类型
        Interface::FITKPostDataType clipType = clipData->getPostDataType();
        _ui->comboBox_SliceType->setCurrentIndex(_ui->comboBox_SliceType->findData(QVariant::fromValue(clipType)));
        _ui->comboBox_SliceType->setEnabled(false);

        switch (clipType) {
        case Interface::FITKPostDataType::Post_SlicePlane: {
            _clipWidget = new ClipSliceWidgetPlane(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceSphere: {
            _clipWidget = new ClipSliceWidgetSphere(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceCylinder: {
            _clipWidget = new ClipSliceWidgetCylinder(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        case Interface::FITKPostDataType::Post_SliceBox: {
            _clipWidget = new ClipSliceWidgetBox(ClipSlice::Slice, _currentDataID, _parentDataID, this);
            break;
        }
        }

        if (_clipWidget) {
            _ui->verticalLayout_SubWidget->addWidget(_clipWidget);
            _clipWidget->setDataToWidget(parentData, clipData);
        }
    }

    bool SliceDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKAbstractCFDPostData* clipData = nullptr;
        Interface::FITKPostDataType clipType = _ui->comboBox_SliceType->currentData().value<Interface::FITKPostDataType>();
        switch (clipType) {
        case Interface::FITKPostDataType::Post_SlicePlane: {
            Interface::FITKCFDPostSlicePlane* data = new Interface::FITKCFDPostSlicePlane(_parentDataID);
            clipData = data;
            break;
        }
        case Interface::FITKPostDataType::Post_SliceSphere: {
            Interface::FITKCFDPostSliceSphere* data = new Interface::FITKCFDPostSliceSphere(_parentDataID);
            clipData = data;
            break;
        }
        case Interface::FITKPostDataType::Post_SliceCylinder: {
            Interface::FITKCFDPostSliceCylinder* data = new Interface::FITKCFDPostSliceCylinder(_parentDataID);
            clipData = data;
            break;
        }
        case Interface::FITKPostDataType::Post_SliceBox: {
            Interface::FITKCFDPostSliceBox* data = new Interface::FITKCFDPostSliceBox(_parentDataID);
            clipData = data;
            break;
        }
        }
        if (clipData == nullptr)return false;
        _currentDataID = clipData->getDataObjectID();
        getDataFromWidget(clipData);
        _postDataManager->appendDataObj(clipData);
        return true;
    }

    bool SliceDialog::OKEdit()
    {
        Interface::FITKAbstractCFDPostData* clipData = _postDataManager->getDataByID(_currentDataID);
        if (clipData == nullptr)return false;
        getDataFromWidget(clipData);
        return true;
    }

    void SliceDialog::getDataFromWidget(Interface::FITKAbstractCFDPostData * glyphData)
    {
        if (glyphData == nullptr)return;
        QString name = _ui->lineEdit_Name->text();
        glyphData->setDataObjectName(name);

        _clipWidget->getDataFromWidget(glyphData);
    }

    void SliceDialog::setDataToWidget(Interface::FITKAbstractCFDPostData * glyphData)
    {
        if (glyphData == nullptr)return;
    }
}
