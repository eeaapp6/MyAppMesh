/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ParamWidget.h"
#include "ui_ParamWidget.h"
#include "FITK_Component/FITKWidget/FITKTreeTableWidget.h"

namespace GUI
{
    ParamWidget::ParamWidget(QWidget* parent /*= nullptr*/)
        :Core::FITKWidget(parent)
    {
        //初始化
        _ui = new Ui::ParamWidget;
        _ui->setupUi(this);
        this->clear();
    }

    

    void ParamWidget::setWidget(QWidget* widget)
    {
        //清空
        if (_widget != nullptr)
        {
            _ui->gridLayout->removeWidget(_widget);
            delete _widget;
            _widget = nullptr;
        }
        _widget = widget;
        //追加新的窗口
        if (widget)
            _ui->gridLayout->addWidget(widget);
        else
            this->clear();
    }

    void ParamWidget::clear()
    {
        //清空
        if (_widget != nullptr)
        {
            delete _widget;
            _widget = nullptr;
        }
        //默认显示一个空的树形表格
        Comp::FITKTreeTableWidget* t = new Comp::FITKTreeTableWidget(this);
        t->setColumnCount(2);
        QStringList header;
        header << tr("Name") << tr("Property");
        t->setHorizontalHeaderLabels(header);
        this->setWidget(t);
    }

    QWidget* ParamWidget::getCurrentWidget()
    {
        return _widget;
    }

}

