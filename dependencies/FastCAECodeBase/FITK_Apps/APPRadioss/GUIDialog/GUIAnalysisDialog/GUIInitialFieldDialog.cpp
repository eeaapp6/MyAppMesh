/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIInitialFieldDialog.h"
#include "ui_GUIInitialFieldDialog.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QAction>

namespace GUI
{
    GUIInitialFieldDialog::GUIInitialFieldDialog(QWidget* parent)
        : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIInitialFieldDialog();
        _ui->setupUi(this);
        
        // 初始化界面
        // 设置标题
        this->setWindowTitle(tr("Initial Field"));
        // 去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        // 初始化控件
        _ui->lineEditName->setReadOnly(true);
        _ui->lineEditType->setReadOnly(true);
        _ui->lineEditKeyWord->setReadOnly(true);
        _ui->lineEditCoordinateSystem->setEnabled(false);
        _ui->widgetSet->setFirstNodeGroup(tr("None"), 0);

        // 连接信号槽
        connect(this, &QDialog::finished, this, &GUIInitialFieldDialog::on_pushButtonCancel_clicked);
    }

    GUIInitialFieldDialog::~GUIInitialFieldDialog()
    {
        // 重置模式
        _model = -1;

        // 清除界面
        if (_ui != nullptr) delete _ui;
        _ui = nullptr;
    }

    void GUIInitialFieldDialog::setModel(int model)
    {
        _model = model;
    }

    void GUIInitialFieldDialog::setInitialFieldName(const QString& text)
    {
        _ui->lineEditName->setText(text);
    }

    QString GUIInitialFieldDialog::getInitialFieldName()
    {
        return _ui->lineEditName->text();
    }

    void GUIInitialFieldDialog::setInitialFieldType(const QString& text)
    {
        _ui->lineEditType->setText(text);
    }

    void GUIInitialFieldDialog::setInitialFieldKeyWord(const QString& text)
    {
        _ui->lineEditKeyWord->setText(text);
    }

    void GUIInitialFieldDialog::setInitialFieldSetID(int id)
    {
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();

        // 高亮
        operGraph->hightObject(id);
        // 设置当前项
        _ui->widgetSet->findNodeGroup(id);
    }

    int GUIInitialFieldDialog::getInitialFieldSetID()
    {
        return _ui->widgetSet->getNodeGroupId();
    }

    QString GUIInitialFieldDialog::getInitialFieldSetName()
    {
        return _ui->widgetSet->getCurrentName();
    }

    void GUIInitialFieldDialog::setInitialFieldX(const QString& text)
    {
        _ui->lineEditVX->setText(text);
    }

    QString GUIInitialFieldDialog::getInitialFieldX()
    {
        return _ui->lineEditVX->text();
    }

    void GUIInitialFieldDialog::setInitialFieldY(const QString& text)
    {
        _ui->lineEditVY->setText(text);
    }

    QString GUIInitialFieldDialog::getInitialFieldY()
    {
        return _ui->lineEditVY->text();
    }

    void GUIInitialFieldDialog::setInitialFieldZ(const QString& text)
    {
        _ui->lineEditVZ->setText(text);
    }

    QString GUIInitialFieldDialog::getInitialFieldZ()
    {
        return _ui->lineEditVZ->text();
    }

    void GUIInitialFieldDialog::on_pushButtonOK_clicked()
    {
        // 初始化执行状态
        bool ok = false;

        // 根据模式创建或修改初始化场数据
        if (_model == 0) ok = createInitialField();
        else if (_model == 1) ok = modifyInitialField();
        else return;

        // 仅当执行成功时关闭对话框
        if (ok) this->accept();
    }

    void GUIInitialFieldDialog::on_pushButtonCancel_clicked()
    {
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();
        // 关闭对话框
        close();
    }
}