/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "LightSubWidget.h"
#include "ui_LightSubWidget.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphWidgetLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLightManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostEnum.h"

#include <QAction>
#include <QColor>
#include <QColorDialog>

Q_DECLARE_METATYPE(Interface::FITKPostLightType)

namespace GUI
{
    LightSubWidget::LightSubWidget(int currentDataID, QWidget* parent) :
        GUIWidgetBase(parent)
    {
        _ui = new Ui::LightSubWidget();
        _ui->setupUi(this);
        _currentDataID = currentDataID;

        init();
    }

    LightSubWidget::~LightSubWidget()
    {
        //if (_lineWidget) {
        //    _lineWidget->setIsShow(false);
        //    delete _lineWidget;
        //    _lineWidget = nullptr;
        //}

        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void LightSubWidget::init()
    {
        QSignalBlocker blocker(_ui->comboBox_Type);

        int index = _postLightManager->getDataIndex(_postLightManager->getDataByID(_currentDataID));
        _ui->label_Title->setText(QString("Light %1").arg(index));

        _ui->comboBox_Type->addItem(tr("Directional"), QVariant::fromValue(Interface::FITKPostLightType::Post_Direction));
        _ui->comboBox_Type->addItem(tr("Positional"), QVariant::fromValue(Interface::FITKPostLightType::Post_Positional));
        double range[2] = { 0,90 };
        _ui->lineEdit_ConeAngle->setRange(range);
        setDataToWidget(_postLightManager->getDataByID(_currentDataID));

        connect(_ui->lineEdit_FocX, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_FocY, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_FocZ, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_PosX, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_PosY, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_PosZ, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));
        connect(_ui->lineEdit_ConeAngle, SIGNAL(dataChanged()), this, SLOT(slotValueChange()));

        MainWindow* mainWin = FITKAPP->getGlobalData()->getMainWindowT<MainWindow>();
        if (mainWin == nullptr)return;
        QWidget* currentWidget = mainWin->getRenderWidget()->getRenderWidget();
        //_lineWidget = new Interface::PostGraphWidgetLine(currentWidget);
        //connect(_lineWidget, SIGNAL(sigValueChange(double*, double*)), this, SLOT(slotVTKLineWidgetValueChange(double*, double*)));

        slotValueChange();
    }

    void LightSubWidget::setWidgetTitle(QString title)
    {
        _ui->label_Title->setText(title);
    }

    void LightSubWidget::showEvent(QShowEvent * event)
    {
        //if (_lineWidget) {
        //    _lineWidget->setIsShow(true);
            reRender();
        //}
        GUIWidgetBase::showEvent(event);
    }

    void LightSubWidget::closeEvent(QCloseEvent * event)
    {
        //if (_lineWidget) {
        //    _lineWidget->setIsShow(false);
        //    delete _lineWidget;
        //    _lineWidget = nullptr;
            reRender();
        //}
        GUIWidgetBase::closeEvent(event);
    }

    void LightSubWidget::on_pushButton_Remove_clicked()
    {
        QAction lightDeleteAction;
        lightDeleteAction.setObjectName("actionLightDelete");
        auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionLightDelete");
        if (acOper == nullptr)return;
        acOper->setEmitter(&lightDeleteAction);
        acOper->setArgs(objID_String, _currentDataID);
        acOper->actionTriggered();

        reRender();

        this->close();
    }

    void LightSubWidget::on_comboBox_Type_currentIndexChanged(int index)
    {
        Q_UNUSED(index);

        Interface::FITKPostLightType type = _ui->comboBox_Type->currentData().value<Interface::FITKPostLightType>();
        switch (type){
        case Interface::FITKPostLightType::Post_Positional: {
            _ui->label_ConeAngle->show();
            _ui->lineEdit_ConeAngle->show();
            break;
        }
        case Interface::FITKPostLightType::Post_Direction: {
            _ui->label_ConeAngle->hide();
            _ui->lineEdit_ConeAngle->hide();
            break;
        }
        }

        slotValueChange();
    }

    void LightSubWidget::on_pushButton_SelectColor_clicked()
    {
        Interface::FITKCFDPostLight* lightData = _postLightManager->getDataByID(_currentDataID);
        if (lightData == nullptr)return;
        double color[3] = { 0,0,0 };
        lightData->getColor(color);

        QColor c;
        c.setRedF(color[0]);
        c.setGreenF(color[1]);
        c.setBlueF(color[2]);

        QColorDialog dialog(this);
        dialog.setCurrentColor(c);
        dialog.exec();

        c = dialog.currentColor();

        color[0] = c.redF();
        color[1] = c.greenF();
        color[2] = c.blueF();

        lightData->setColor(color);

        QPalette gradientPalette;
        gradientPalette.setColor(QPalette::Button, c);
        _ui->pushButton_SelectColor->setPalette(gradientPalette);

        slotValueChange();
    }

    void LightSubWidget::slotValueChange()
    {
        Interface::FITKCFDPostLight* lightData = _postLightManager->getDataByID(_currentDataID);
        if (lightData == nullptr)return;
        //if (_lineWidget == nullptr)return;
        double foc[3] = { 0,0,0 }, pos[3] = { 0,0,0 }, coneAngle = 0.0;
        foc[0] = _ui->lineEdit_FocX->value();
        foc[1] = _ui->lineEdit_FocY->value();
        foc[2] = _ui->lineEdit_FocZ->value();
        pos[0] = _ui->lineEdit_PosX->value();
        pos[1] = _ui->lineEdit_PosY->value();
        pos[2] = _ui->lineEdit_PosZ->value();
        coneAngle = _ui->lineEdit_ConeAngle->value();

        Interface::FITKPostLightType type = _ui->comboBox_Type->currentData().value<Interface::FITKPostLightType>();

        //_lineWidget->setLine(pos, foc);
        lightData->setFocalPoint(foc);
        lightData->setPosition(pos);
        lightData->setConeAngle(coneAngle);
        lightData->setLightType(type);

        //更新渲染窗口
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper) {
            graphOper->updateLight(_currentDataID);
        }
    }

    void LightSubWidget::slotVTKLineWidgetValueChange(double * startPoint, double * endPoint)
    {
        //if (_lineWidget == nullptr)return;
        //_ui->lineEdit_PosX->setValue(startPoint[0]);
        //_ui->lineEdit_PosY->setValue(startPoint[1]);
        //_ui->lineEdit_PosZ->setValue(startPoint[2]);

        //_ui->lineEdit_FocX->setValue(endPoint[0]);
        //_ui->lineEdit_FocY->setValue(endPoint[1]);
        //_ui->lineEdit_FocZ->setValue(endPoint[2]);

        //slotValueChange();
    }

    void LightSubWidget::setDataToWidget(Interface::FITKCFDPostLight * data)
    {
        QSignalBlocker blocker(_ui->comboBox_Type);
        if (data == nullptr)return;
        
        //设置类型
        Interface::FITKPostLightType type = data->getLightType();
        int typeIndex = _ui->comboBox_Type->findData(QVariant::fromValue(type));
        _ui->comboBox_Type->setCurrentIndex(typeIndex);
        switch (type) {
        case Interface::FITKPostLightType::Post_Positional: {
            _ui->label_ConeAngle->show();
            _ui->lineEdit_ConeAngle->show();
            break;
        }
        case Interface::FITKPostLightType::Post_Direction: {
            _ui->label_ConeAngle->hide();
            _ui->lineEdit_ConeAngle->hide();
            break;
        }
        }

        //灯光位置
        double position[3] = { 0,0,0 };
        data->getPosition(position);
        _ui->lineEdit_PosX->setValue(position[0]);
        _ui->lineEdit_PosY->setValue(position[1]);
        _ui->lineEdit_PosZ->setValue(position[2]);

        //灯光焦点
        double focalPoint[3] = { 0,0,0 };
        data->getFocalPoint(focalPoint);
        _ui->lineEdit_FocX->setValue(focalPoint[0]);
        _ui->lineEdit_FocY->setValue(focalPoint[1]);
        _ui->lineEdit_FocZ->setValue(focalPoint[2]);

        //灯光角度
        double coneAngle = 0;
        coneAngle = data->getConeAngle();
        _ui->lineEdit_ConeAngle->setValue(coneAngle);

        //颜色
        double color[3] = { 0,0,0 };
        data->getColor(color);
        QColor c;
        c.setRedF(color[0]);
        c.setGreenF(color[1]);
        c.setBlueF(color[2]);
        QPalette gradientPalette;
        gradientPalette.setColor(QPalette::Button, c);
        _ui->pushButton_SelectColor->setPalette(gradientPalette);
    }
}

