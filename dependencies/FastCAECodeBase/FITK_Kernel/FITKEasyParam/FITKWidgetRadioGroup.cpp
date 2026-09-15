/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetRadioGroup.h"
#include "ui_FITKWidgetRadioGroup.h"
#include "FITKAbstractEasyParam.h"
#include "FITKParamRadioGroup.h"
#include "FITKParameter.h"
#include "FITKEasyParamWidgetFactory.h"
#include "FITKWidgetHBox.h"
#include "FITKWidgetComLine.h"

#include <QButtonGroup>
#include <QRadioButton>
#include <QPair>
#include <QFrame>
#include <QPushButton>

namespace Core
{
    FITKWidgetRadioGroup::FITKWidgetRadioGroup(FITKAbstractEasyParam * dataBase, QWidget * parent) :
        QWidget(parent)
    {
        _data = dynamic_cast<FITKParamRadioGroup*>(dataBase);
        if (_data == nullptr)return;
        _ui = new Ui::FITKWidgetRadioGroup();
        _ui->setupUi(this);
        init();
    }

    FITKWidgetRadioGroup::~FITKWidgetRadioGroup()
    {

    }

    void FITKWidgetRadioGroup::init()
    {
        if (_data == nullptr)return;
        initRadioData();
        initSubData();

        //设置默认选项
        int currentindex = _data->getCurrentValueIndex();
        auto radioButton = _group->button(currentindex);
        if (radioButton == nullptr)return;
        radioButton->setChecked(true);
        slotRadioClicked(currentindex);
    }

    void FITKWidgetRadioGroup::initRadioData()
    {
        if (_data == nullptr)return;
        _group = new QButtonGroup(this);

        QList<FITKRadioGroupValue> radioValues = _data->getRadioValues();
        if (radioValues.size() == 0)return;

        for (int i = 0; i < radioValues.size(); i++) {            
            auto radioValue = radioValues[i];
            //单选选项数据添加
            FITKParameter* values = radioValue._value;
            QList<QWidget*> widgetList = {};

            //radio单选按钮添加
            QRadioButton* radioButton = new QRadioButton(this);
            radioButton->setText(radioValue._name);
            widgetList.append(radioButton);
            _group->addButton(radioButton, i);

            //子数据添加
            if (values) {
                if (values->getDataCount() > 0) {
                    int width = this->width();
                    radioButton->setMinimumWidth(width*0.4);
                    radioButton->setMaximumWidth(width*0.4);
                }
                for (auto v : values->getParameter()) {
                    if (v == nullptr)continue;
                    QWidget* widget = FITKEasyParamWidgetFactory::createWidget(v, this);
                    if (widget == nullptr)continue;
                    widgetList.append(widget);
                }
            }
            FITKWidgetHBox* subWidget = new FITKWidgetHBox(widgetList, this);
            _radioWidgets.insert(i, subWidget);
            _ui->verticalLayout->addWidget(subWidget);
        }

        //禁用
        disableAllRadioWidget();
        connect(_group, SIGNAL(buttonClicked(int)), this, SLOT(slotRadioClicked(int)));
    }

    void FITKWidgetRadioGroup::initSubData()
    {
        if (_data == nullptr)return;

        auto subDataList = _data->getSubValues();
        if (subDataList.size() == 0)return;

        QFrame* line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        _ui->verticalLayout->addWidget(line);

        for (auto subData : subDataList) {
            if (subData == nullptr)continue;
            auto subDataType = subData->getParamType();
            QWidget* widget = nullptr;
            switch (subDataType){
            case FITKAbstractEasyParam::FITKEasyParamType::FEPInt:
            case FITKAbstractEasyParam::FITKEasyParamType::FEPDouble:
            case FITKAbstractEasyParam::FITKEasyParamType::FEPString:
            case FITKAbstractEasyParam::FITKEasyParamType::FEPCombox:
            case FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList: {
                widget = new FITKWidgetComLine(subData, this);
                break;
            }
            case FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup:
            case FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup: {
                widget = FITKEasyParamWidgetFactory::createWidget(subData, this);
                break;
            }
            case FITKAbstractEasyParam::FITKEasyParamType::FEPBool: {
                widget = FITKEasyParamWidgetFactory::createWidget(subData, this, subData->getDataObjectName());
                break;
            }
            }
            if (widget == nullptr)continue;
            _ui->verticalLayout->addWidget(widget);
            _subWidget.append(widget);
        }
    }

    void FITKWidgetRadioGroup::disableAllRadioWidget()
    {
        for (auto w : _radioWidgets.values()) {
            if (w == nullptr)continue;
            w->setSubWidgetEnable(false);
        }
    }

    void FITKWidgetRadioGroup::slotRadioClicked(int index)
    {
        if (index == -1)return;
        if (_data == nullptr)return;

        //Radio禁用
        disableAllRadioWidget();
        auto w = _radioWidgets.value(index);
        if (w) {
            w->setSubWidgetEnable(true);
        }

        //sub禁用
        _data->setCurrentIndex(index);
        QList<bool> subEnable = _data->getCurrentValueSubState();
        if (subEnable.size() != _subWidget.size())return;

        for (int i = 0; i < subEnable.size(); i++) {
            if (_subWidget[i] == nullptr)continue;
            if (subEnable[i] == true) {
                _subWidget[i]->setEnabled(true);
            }
            else {
                _subWidget[i]->setEnabled(false);
            }
        }
    }
}

