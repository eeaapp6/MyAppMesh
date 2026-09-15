/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIColorBarDialog.h"
#include "ui_GUIColorBarDialog.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include <QColorDialog>

namespace GUI
{
    GUIColorBarDialog::GUIColorBarDialog(EventOper::GraphEventOperator* oper, QWidget * parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUIColorBarDialog)
        , _oper(oper)
    {
        _ui->setupUi(this);

        _ui->groupBox->setEnabled(false);
        connect(_ui->checkBoxEdit, &QCheckBox::toggled, this, [&](bool state) {
            _ui->groupBox->setEnabled(state);
        });
        init();
    }

    GUIColorBarDialog::~GUIColorBarDialog()
    {
        if (_ui) delete _ui;
    }

    void GUIColorBarDialog::init()
    {
        if (!_oper)return;
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (!radiossPostData) return;
        _currentPostID = radiossPostData->getCurrentPostDataID();
        bool Flag = _oper->getRangerColorSetFlag(_currentPostID);
        //初始化颜色
        QColor min, max;
        _oper->getRangeColor(false, _currentPostID, min, max);
        QString styleMin = QString("background-color: rgb(%1, %2, %3);").arg(min.red()).arg(min.green()).arg(min.blue());
        _ui->pushButtonMin->setStyleSheet(styleMin);
        QString styleMax = QString("background-color: rgb(%1, %2, %3);").arg(max.red()).arg(max.green()).arg(max.blue());
        _ui->pushButtonMax->setStyleSheet(styleMax);

        _oper->getRangeColor(true, _currentPostID, min, max);
        if(Flag)
            styleMin = QString("background-color: rgb(%1, %2, %3);").arg(min.red()).arg(min.green()).arg(min.blue());
        _ui->pushButtonOutMin->setStyleSheet(styleMin);
        if(Flag)
            styleMax = QString("background-color: rgb(%1, %2, %3);").arg(max.red()).arg(max.green()).arg(max.blue());
        _ui->pushButtonOutMax->setStyleSheet(styleMax);

        _ui->checkBoxEdit->setChecked(Flag);
        //初始化最值
        _ui->lineEditInterval->setCurrentValidValue((double)radiossPostData->getAnimTimeInterval());
        double * range = _oper->getRange(_currentPostID);
        int num = _oper->getRangerColorNum(_currentPostID);
        if (!range)return;
        _ui->lineEditMin->setCurrentValidValue(range[0]);
        _ui->lineEditMax->setCurrentValidValue(range[1]);
        _ui->lineEditNum->setCurrentValidValue(num);
    }

    void GUIColorBarDialog::on_pushButton_OK_clicked()
    {
        if (!_oper)return;
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (!radiossPostData) return;
        double interval = 1000;
        _ui->lineEditInterval->getCurrentValidValue(interval);
        radiossPostData->setAnimTimeInterval(interval);
        if (!_ui->checkBoxEdit->isChecked()) {
            _oper->setRangerColorSetFlag(false);
            _oper->updateGraph(_currentPostID, true);
            _oper->render();
            accept();
            Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionUpdatePostGraph");
            if (oper)oper->execProfession();
            return;
        }
        _oper->setRangerColorSetFlag(true);
        double range[2]{ 0,1 }, num = 10;

        _ui->lineEditMin->getCurrentValidValue(range[0]);
        _ui->lineEditMax->getCurrentValidValue(range[1]);
        _ui->lineEditNum->getCurrentValidValue(num);
        _oper->setRange(_currentPostID, range);

        QColor colorMax = _ui->pushButtonMax->palette().color(QPalette::Button);
        QColor colorMin = _ui->pushButtonMin->palette().color(QPalette::Button);
        _oper->setRangeColor(true, colorMin, colorMax, 1);

        QColor colorOutMax = _ui->pushButtonOutMax->palette().color(QPalette::Button);
        QColor colorOutMin = _ui->pushButtonOutMin->palette().color(QPalette::Button);
        double min[4] = { colorOutMin.redF(),colorOutMin.greenF(),colorOutMin.blueF(),colorOutMin.alphaF() };
        double max[4] = { colorOutMax.redF(),colorOutMax.greenF(),colorOutMax.blueF(),colorOutMax.alphaF() };
        _oper->setOutRangerColor(true, max, min, (int)num);
        _oper->setRangerColorNum(true, (int)num, _currentPostID);

        _oper->updateGraph(_currentPostID,true);
        _oper->render();

        accept();
    }

    void GUIColorBarDialog::on_pushButton_Cancel_clicked()
    {
        reject();
    }

    void GUIColorBarDialog::on_pushButtonMax_clicked()
    {
        QColor oriC = _ui->pushButtonMax->palette().color(QPalette::Button);

        //颜色选择
        QColor color = QColorDialog::getColor(oriC, this, tr("Select Color"));
        if (!color.isValid()) return;

        const QString style = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red()).arg(color.green()).arg(color.blue());
        _ui->pushButtonMax->setStyleSheet(style);
    }

    void GUIColorBarDialog::on_pushButtonMin_clicked()
    {
        QColor oriC = _ui->pushButtonMin->palette().color(QPalette::Button);

        //颜色选择
        QColor color = QColorDialog::getColor(oriC, this, tr("Select Color"));
        if (!color.isValid()) return;

        const QString style = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red()).arg(color.green()).arg(color.blue());
        _ui->pushButtonMin->setStyleSheet(style);
    }
    void GUIColorBarDialog::on_pushButtonOutMax_clicked()
    {
        QColor oriC = _ui->pushButtonOutMax->palette().color(QPalette::Button);

        //颜色选择
        QColor color = QColorDialog::getColor(oriC, this, tr("Select Color"));
        if (!color.isValid()) return;

        const QString style = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red()).arg(color.green()).arg(color.blue());
        _ui->pushButtonOutMax->setStyleSheet(style);
    }
    void GUIColorBarDialog::on_pushButtonOutMin_clicked()
    {
        QColor oriC = _ui->pushButtonOutMin->palette().color(QPalette::Button);

        //颜色选择
        QColor color = QColorDialog::getColor(oriC, this, tr("Select Color"));
        if (!color.isValid()) return;

        const QString style = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red()).arg(color.green()).arg(color.blue());
        _ui->pushButtonOutMin->setStyleSheet(style);
    }
}