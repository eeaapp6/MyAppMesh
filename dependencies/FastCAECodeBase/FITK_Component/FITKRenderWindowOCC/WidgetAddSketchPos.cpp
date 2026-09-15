/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "WidgetAddSketchPos.h"
#include "ui_WidgetAddSketchPos.h"

// Window
#include "FITKGraph3DWindowOCC.h"

// Qt
#include <QDoubleValidator>

namespace GUI {

    WidgetAddSketchPos::WidgetAddSketchPos(QWidget *parent) :
        QDialog(parent),
        m_Ui(new Ui::WidgetAddSketchPos)
    {
        m_Ui->setupUi(this);

        // 初始化界面。
        connect(m_Ui->btnOK, &QPushButton::clicked, this, &WidgetAddSketchPos::slot_confirm);

        m_Ui->editX->setValidator(new QDoubleValidator);
        m_Ui->editY->setValidator(new QDoubleValidator);
    }

    WidgetAddSketchPos::~WidgetAddSketchPos()
    {
        delete m_Ui;
    }

    void WidgetAddSketchPos::setRenderWindow(Render::FITKGraph3DWindowOCC* rw)
    {
        m_rw = rw;
    }

    void WidgetAddSketchPos::slot_confirm()
    {
        if (!m_rw)
        {
            return;
        }

        double x = m_Ui->editX->text().toDouble();
        double y = m_Ui->editY->text().toDouble();
        m_rw->addSketchPosition(x, y);

        if (!m_rw->getSketchStatus())
        {
            accept();
        }
    }
}

