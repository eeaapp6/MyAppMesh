/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGraphSettingDialog.h"
#include "ui_GUIGraphSettingDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Component/FITKWidget/FITKSciNotationLineEdit.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include <QColorDialog>
#include <QValidator>
#include <QColor>
namespace GUI
{
    GUIGraphSettingDialog::GUIGraphSettingDialog(Core::FITKActionOperator* oper, QWidget* parent) :
        GUIDialogBase(nullptr, parent), _oper(oper)
    {
        _ui = new Ui::GUIGraphSettingDialog;
        _ui->setupUi(this);

        initUi();
    }
    GUIGraphSettingDialog::~GUIGraphSettingDialog()
    {

    }
    void GUIGraphSettingDialog::initUi()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings)
        {
            auto setButtonColor = [](QPushButton* button, const QColor& color) {
                if (button) {
                    button->setStyleSheet(QString("background-color: %1;").arg(color.name()));
                    button->setAutoDefault(false);
                }
                };
            //几何体设置
            QVariant geoLineWidth = appSettings->getVarient("GraphSetting/GeoLineWidth");
            if (!geoLineWidth.isValid())
            {
                geoLineWidth = 1.0;
            }
            QVariant geoPointSize = appSettings->getVarient("GraphSetting/GeoPointSize");
            if (!geoPointSize.isValid())
            {
                geoPointSize = 5.0;
            }
            QVariant geoPointColor = appSettings->getVarient("GraphSetting/GeoPointColor");
            if (!geoPointColor.isValid())
            {
                geoPointColor = QColor(255, 255, 255);
            }
            QVariant geoLineColor = appSettings->getVarient("GraphSetting/GeoLineColor");
            if (!geoLineColor.isValid())
            {
                geoLineColor = QColor(0, 0, 0);
            }
            _ui->lineEdit_GeoLineWidth->setCurrentValidValue(geoLineWidth.value<double>());
            _ui->lineEdit_GeoPointSize->setCurrentValidValue(geoPointSize.value<double>());
            setButtonColor(_ui->pushButton_GeoPointColor, geoPointColor.value<QColor>());
            setButtonColor(_ui->pushButton_GeoLineColor, geoLineColor.value<QColor>());

            //网格设置
            QVariant meshLineWidth = appSettings->getVarient("GraphSetting/MeshLineWidth");
            if (!meshLineWidth.isValid())
            {
                meshLineWidth = 1.0;
            }
            QVariant meshPointSize = appSettings->getVarient("GraphSetting/MeshPointSize");
            if (!meshPointSize.isValid())
            {
                meshPointSize = 5.0;
            }
            QVariant meshPointColor = appSettings->getVarient("GraphSetting/MeshPointColor");
            if (!meshPointColor.isValid())
            {
                meshPointColor = QColor(255, 255, 255);
            }
            QVariant meshLineColor = appSettings->getVarient("GraphSetting/MeshLineColor");
            if (!meshLineColor.isValid())
            {
                meshLineColor = QColor(0, 0, 0);
            }
            QVariant meshSHPSize = appSettings->getVarient("GraphSetting/MeshSHPSize");
            if(!meshSHPSize.isValid())
            {
                meshSHPSize = 5.0;
            }
            _ui->lineEdit_MeshLineWidth->setCurrentValidValue(meshLineWidth.value<double>());
            _ui->lineEdit_MeshPointSize->setCurrentValidValue(meshPointSize.value<double>());
            _ui->lineEdit_MeshSPHSize->setCurrentValidValue(meshSHPSize.value<double>());
            setButtonColor(_ui->pushButton_MeshPointColor, meshPointColor.value<QColor>());
            setButtonColor(_ui->pushButton_MeshLineColor, meshLineColor.value<QColor>());
            _ui->pushButton_OK->setAutoDefault(false);
            _ui->pushButton_Cancel->setAutoDefault(false);
            _ui->pushButton_Apply->setAutoDefault(false);
        }
    }

    void GUIGraphSettingDialog::on_pushButton_GeoPointColor_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        QColor initialColor = appSettings->getValue<QColor>("GraphSetting/GeoPointColor");
        QColor color = QColorDialog::getColor(initialColor, this, tr("Select Geometry Point Color"));
        if (color.isValid())
        {
            _ui->pushButton_GeoPointColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        }
    }

    void GUIGraphSettingDialog::on_pushButton_MeshPointColor_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        QColor initialColor = appSettings->getValue<QColor>("GraphSetting/MeshPointColor");
        QColor color = QColorDialog::getColor(initialColor, this, tr("Select Mesh Point Color"));
        if (color.isValid())
        {
            _ui->pushButton_MeshPointColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        }
    }

    void GUIGraphSettingDialog::on_pushButton_GeoLineColor_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        QColor initialColor = appSettings->getValue<QColor>("GraphSetting/GeoLineColor");
        QColor color = QColorDialog::getColor(initialColor, this, tr("Select Geometry Line Color"));
        if (color.isValid())
        {
            _ui->pushButton_GeoLineColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        }
    }

    void GUIGraphSettingDialog::on_pushButton_MeshLineColor_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        QColor initialColor = appSettings->getValue<QColor>("GraphSetting/MeshLineColor");
        QColor color = QColorDialog::getColor(initialColor, this, tr("Select Mesh Line Color"));
        if (color.isValid())
        {
            _ui->pushButton_MeshLineColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        }
    }

    void GUIGraphSettingDialog::on_pushButton_OK_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings)
        {
            //几何体设置
            double geoLineWidth = _ui->lineEdit_GeoLineWidth->getCurrentValidText().toDouble();
            double geoPointSize = _ui->lineEdit_GeoPointSize->getCurrentValidText().toDouble();
            QColor geoPointColor = _ui->pushButton_GeoPointColor->palette().color(QPalette::Button);
            QColor geoLineColor = _ui->pushButton_GeoLineColor->palette().color(QPalette::Button);
            appSettings->setValue("GraphSetting/GeoLineWidth", geoLineWidth);
            appSettings->setValue("GraphSetting/GeoPointSize", geoPointSize);
            appSettings->setValue("GraphSetting/GeoPointColor", geoPointColor);
            appSettings->setValue("GraphSetting/GeoLineColor", geoLineColor);
            //网格设置
            double meshLineWidth = _ui->lineEdit_MeshLineWidth->getCurrentValidText().toDouble();
            double meshPointSize = _ui->lineEdit_MeshPointSize->getCurrentValidText().toDouble();
            double meshSPHSize = _ui->lineEdit_MeshSPHSize->getCurrentValidText().toDouble();
            QColor meshPointColor = _ui->pushButton_MeshPointColor->palette().color(QPalette::Button);
            QColor meshLineColor = _ui->pushButton_MeshLineColor->palette().color(QPalette::Button);
            appSettings->setValue("GraphSetting/MeshLineWidth", meshLineWidth);
            appSettings->setValue("GraphSetting/MeshPointSize", meshPointSize);
            appSettings->setValue("GraphSetting/MeshSPHSize", meshSPHSize);
            appSettings->setValue("GraphSetting/MeshPointColor", meshPointColor);
            appSettings->setValue("GraphSetting/MeshLineColor", meshLineColor);

            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                // 应用几何体设置
                graphOper->updateLineWidth(-1, geoLineWidth);
                graphOper->updatePointSize(-1, geoPointSize);
                graphOper->updateColor(-1, geoPointColor, 0); // 0 for Point
                graphOper->updateColor(-1, geoLineColor, 1);  // 1 for Line

                // 应用网格设置
                graphOper->updateLineWidth(-2, meshLineWidth);
                graphOper->updatePointSize(-2, meshPointSize);
                graphOper->updateColor(-2, meshPointColor, 0); // 0 for Point
                graphOper->updateColor(-2, meshLineColor, 1);  // 1 for Line
                graphOper->updateSHPSize(-2, meshSPHSize);
            }
        }
        accept();
    }

    void GUIGraphSettingDialog::on_pushButton_Cancel_clicked()
    {
        reject();
    }

    void GUIGraphSettingDialog::on_pushButton_Apply_clicked()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings)
        {
            //几何体设置
            double geoLineWidth = _ui->lineEdit_GeoLineWidth->getCurrentValidText().toDouble();
            double geoPointSize = _ui->lineEdit_GeoPointSize->getCurrentValidText().toDouble();
            QColor geoPointColor = _ui->pushButton_GeoPointColor->palette().color(QPalette::Button);
            QColor geoLineColor = _ui->pushButton_GeoLineColor->palette().color(QPalette::Button);
            appSettings->setValue("GraphSetting/GeoLineWidth", geoLineWidth);
            appSettings->setValue("GraphSetting/GeoPointSize", geoPointSize);
            appSettings->setValue("GraphSetting/GeoPointColor", geoPointColor);
            appSettings->setValue("GraphSetting/GeoLineColor", geoLineColor);
            //网格设置
            double meshLineWidth = _ui->lineEdit_MeshLineWidth->getCurrentValidText().toDouble();
            double meshPointSize = _ui->lineEdit_MeshPointSize->getCurrentValidText().toDouble();
            double meshSPHSize = _ui->lineEdit_MeshSPHSize->getCurrentValidText().toDouble();
            QColor meshPointColor = _ui->pushButton_MeshPointColor->palette().color(QPalette::Button);
            QColor meshLineColor = _ui->pushButton_MeshLineColor->palette().color(QPalette::Button);
            appSettings->setValue("GraphSetting/MeshLineWidth", meshLineWidth);
            appSettings->setValue("GraphSetting/MeshPointSize", meshPointSize);
            appSettings->setValue("GraphSetting/MeshPointColor", meshPointColor);
            appSettings->setValue("GraphSetting/MeshLineColor", meshLineColor);
            appSettings->setValue("GraphSetting/MeshSPHSize", meshSPHSize);

            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                // 应用几何体设置
                graphOper->updateLineWidth(-1, geoLineWidth);
                graphOper->updatePointSize(-1, geoPointSize);
                graphOper->updateColor(-1, geoPointColor, 0); // 0 for Point
                graphOper->updateColor(-1, geoLineColor, 1);  // 1 for Line

                // 应用网格设置
                graphOper->updateLineWidth(-2, meshLineWidth);
                graphOper->updatePointSize(-2, meshPointSize);
                graphOper->updateColor(-2, meshPointColor, 0); // 0 for Point
                graphOper->updateColor(-2, meshLineColor, 1);  // 1 for Line
                graphOper->updateSHPSize(-2, meshSPHSize);
            }
        }
    }
}