/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIBatchSettingDialog.h"
#include "ui_GUIBatchSettingDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

namespace GUI
{
    GUIBatchSettingDialog::GUIBatchSettingDialog(Core::FITKActionOperator* oper, QWidget* parent)
        : _oper(oper), GUIDialogBase(nullptr, parent), _ui(new Ui::GUIBatchSettingDialog())
    {
        // 加载布局
        _ui->setupUi(this);
        // 去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        // 初始化界面
        init();
    }

    GUIBatchSettingDialog::~GUIBatchSettingDialog()
    {
        //销毁ui
        if (_ui) delete _ui;
    }

    void GUIBatchSettingDialog::setModel(int model)
    {
        // 获取模式
        _model = model;
        // 初始化界面
        init();
    }

    void GUIBatchSettingDialog::init()
    {
        // 清空下拉框
        _ui->comboBox->clear();
        // 添加空白选项
        _ui->comboBox->addItem(tr("None"), -1);
        // 清空标签
        _ui->label_name->clear();

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;

        // 根据模式加载下拉选项
        if (_model == 0)
        {
            // 设置标签
            _ui->label_name->setText(tr("Material"));

            // 获取材料管理器
            Interface::FITKMaterialManager* manager = dataCase->getMaterialManager();
            if (manager == nullptr) return;

            // 遍历材料数据
            for (int i = 0; i < manager->getDataCount(); i++)
            {
                // 获取材料数据
                Interface::FITKAbstractMaterial* dataMaterial = manager->getDataByIndex(i);
                if (dataMaterial == nullptr) continue;
                
                // 添加下拉选项
                _ui->comboBox->addItem(dataMaterial->getDataObjectName(), dataMaterial->getDataObjectID());
            }
        }
        else if (_model == 1)
        {
            // 设置标签
            _ui->label_name->setText(tr("Property"));

            // 获取属性管理器
            Interface::FITKAbstractSectionManager* manager = dataCase->getSectionManager();
            if (manager == nullptr) return;

            // 遍历属性数据
            for (int i = 0; i < manager->getDataCount(); i++)
            {
                // 获取属性数据
                Interface::FITKAbstractSection* dataProperty = manager->getDataByIndex(i);
                if (dataProperty == nullptr) continue;
                
                // 添加下拉选项
                _ui->comboBox->addItem(dataProperty->getDataObjectName(), dataProperty->getDataObjectID());
            }
        }
    }

    void GUIBatchSettingDialog::on_pushButton_OK_clicked()
    {
        // 设置参数
        _oper->setArgs("ObjectDataID", _ui->comboBox->currentData().toInt());
        // 返回
        accept();
    }

    void GUIBatchSettingDialog::on_pushButton_Cancel_clicked()
    {
        // 返回
        reject();
    }
}