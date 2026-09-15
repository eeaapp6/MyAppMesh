/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ColorMapperEditDialog.h"
#include "ui_ColorMapperEditDialog.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphProperty.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace GUI
{
    ColorMapperEditDialog::ColorMapperEditDialog(Interface::FITKAbstractCFDPostData* postData, EventOper::ParaWidgetInterfaceOperator* oper) :
        GUIDialogBase(oper, "ColorMapperEditDialog", FITKAPP->getGlobalData()->getMainWindow()), _postData(postData)
    {
        _ui = new Ui::ColorMapperEditDialog();
        _ui->setupUi(this);

        setWindowTitle(tr("Color mapper edit"));
        init();
    }

    ColorMapperEditDialog::~ColorMapperEditDialog()
    {
        if (_ui)delete _ui;
    }

    void ColorMapperEditDialog::init()
    {
        setDataToWidget();
    }

    void ColorMapperEditDialog::setDataToWidget()
    {
        if (_postData == nullptr)return;
        QString name = _postData->getDataObjectName();
        _ui->label_ReferenceName->setText(name);

        int colorNum = _postData->getScalaraBarColorNum();
        _ui->spinBox_Number->setValue(colorNum);

        Interface::PostGraphObjectBase* postGraph = _postGraphManager->getGraphObjectBase(_postData);
        if (postGraph == nullptr)return;
        Interface::PostGraphProperty* postProperty = postGraph->getProperty();
        if (postProperty == nullptr)return;
        postProperty->getCurrentField(_type, _filedName, _componentIndex);

        //获取参考范围
        double referenceRange[2] = { 0,0 };
        _postData->getRange(referenceRange, _type, _filedName, _componentIndex);
        _ui->label_ReferenceType->setText(_filedName);
        _ui->label_ReferenceMax->setText(QString::number(referenceRange[1]));
        _ui->label_ReferenceMin->setText(QString::number(referenceRange[0]));

        //获取实际范围
        double range[2] = { 0,0 };
        _postData->getScalarColorRange(range[0], range[1]);
        _ui->lineEdit_Max->setText(QString::number(range[1]));
        _ui->lineEdit_Min->setText(QString::number(range[0]));
    }

    void ColorMapperEditDialog::on_pushButton_OK_clicked()
    {
        int colorNum = _ui->spinBox_Number->value();
        double colorMin = _ui->lineEdit_Min->text().toDouble();
        double colorMax = _ui->lineEdit_Max->text().toDouble();

        _postData->setScalarBarColorNum(colorNum);
        _postData->setScalarColorRange(colorMin, colorMax);
        _postData->update();

        if (_oper) {
            _oper->execProfession();
        }
        this->accept();
    }

    void ColorMapperEditDialog::on_pushButton_Cencel_clicked()
    {
        this->reject();
    }
}