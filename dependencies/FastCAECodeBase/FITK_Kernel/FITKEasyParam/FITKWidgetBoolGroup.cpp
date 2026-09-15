/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetBoolGroup.h"
#include "ui_FITKWidgetBoolGroup.h"
#include "FITKParamBool.h"
#include "FITKParamBoolGroup.h"
#include "FITKParameter.h"
#include "FITKWidgetComLine.h"

namespace Core 
{
    FITKWidgetBoolGroup::FITKWidgetBoolGroup(FITKParamBoolGroup * dataBase, QWidget * parent) :
        QWidget(parent), _dataBase(dataBase)
    {
        _ui = new Ui::FITKWidgetBoolGroup();
        _ui->setupUi(this);
        init();
    }

    FITKWidgetBoolGroup::~FITKWidgetBoolGroup()
    {
        if (_ui)delete _ui;
    }

    void FITKWidgetBoolGroup::init()
    {
        if (_dataBase == nullptr)return;

        _ui->widget_Sub->hide();

        updateWidget();
    }

    void FITKWidgetBoolGroup::on_checkBox_clicked()
    {
        if (_dataBase == nullptr)return;
        _dataBase->setValue(_ui->checkBox->isChecked());
        updateWidget();
    }

    void FITKWidgetBoolGroup::updateWidget()
    {
        if (!_dataBase)return;

        QString name = _dataBase->getDataObjectName();
        _ui->label_Name->setText(name);

        //控制子界面是否显示
        if (_dataBase->getValue()) {
            _ui->widget_Sub->show();
            _ui->checkBox->setChecked(true);
        }
        else {
            _ui->widget_Sub->hide();
            _ui->checkBox->setChecked(false);
        }

        //清除子参数
        QLayoutItem* item;
        while ((item = _ui->verticalLayout_Sub->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater(); // 推荐使用 deleteLater，以确保小部件在适当时机被删除
            }
            delete item; // 删除布局项
        }
        //子界面添加数据
        if (!_dataBase->getValueGroup())return;
        for (auto v : _dataBase->getValueGroup()->getParameter()) {
            if (!v)continue;
            QWidget* w = new FITKWidgetComLine(v, this);
            _ui->verticalLayout_Sub->addWidget(w);
        }
    }
}