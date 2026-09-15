/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialPropDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRadMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"
#include "FITK_Component/FITKWidget/FITKSciNotationLineEdit.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include <QComboBox>
#define  FailureType Radioss::FITKAbstractFailureModel::FailureModelType

namespace GUI
{
    GUIMaterialPropDialog::GUIMaterialPropDialog(Interface::FITKAbstractMaterial* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper),_ui(new Ui::GUIMaterialPropDialog)
    {
        _ui->setupUi(this);
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取材料管理器
        _matMgr = caseObj->getMaterialManager();
        if (!_obj) 
            _isCreate = true;
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        //初始化失效参数
        _ui->checkBoxFail->setChecked(false);
        _ui->comboBoxFail->setEnabled(false);
        connect(_ui->checkBoxFail, &QCheckBox::toggled, [=](bool checked) {
            _ui->comboBoxFail->setEnabled(checked);
        });
    }

    GUIMaterialPropDialog::~GUIMaterialPropDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIMaterialPropDialog::init()
    {
        //初始化失效模型
        this->initFailComboBox();
    }

    bool GUIMaterialPropDialog::createObjData()
    {
        if (!_matMgr) return false;
        if(!editObjData())return false;
        _matMgr->appendDataObj(_obj);
        return true;
    }

    bool GUIMaterialPropDialog::editObjData()
    {
        Radioss::FITKAbstractRadMaterial* mat = dynamic_cast<Radioss::FITKAbstractRadMaterial*>(_obj);
        if (!_oper || !mat || !_matMgr) return false;
        if(!this->ckeckData()) return false;
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return false;
        //获取失效管理器
        Radioss::FITKRadiossFailureModelManager* failMgr = caseObj->getFailureModelManager();
        _oldNameForScript = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEdit_name->text());
        //设置界面数据
        this->getDataFormWidget();
        //设置失效模型ID
        mat->setFailState(false);
        mat->setFailID(-1);
        if (_ui->checkBoxFail->isChecked()) {
            bool ok = false;
            int failID = _ui->comboBoxFail->currentData().toInt(&ok);
            if (ok) {
                Radioss::FITKAbstractFailureModel* fail = failMgr->getDataByID(failID);
                if (!fail)return false;
                fail->addMaterialID(mat->getDataObjectID(),failMgr);
                mat->setFailState(true);
                mat->setFailID(failID);
            }
        }
        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    bool GUIMaterialPropDialog::ckeckData()
    {
        //虚函数重写
        return true;
    }

    void GUIMaterialPropDialog::writePythonScript()
    {
        //虚函数重写
    }

    void GUIMaterialPropDialog::initFailComboBox()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取失效管理器
        Radioss::FITKRadiossFailureModelManager* mgr = caseObj->getFailureModelManager();
        if (!mgr)return;
        Radioss::FITKAbstractRadMaterial* mat = dynamic_cast<Radioss::FITKAbstractRadMaterial*>(_obj);
        if (!mat)return;
        Radioss::FITKAbstractRadMaterial::RadMatType type = mat->getRadMatType();

        QList<FailureType> typeList;
        switch (type)
        {
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW0:{
            typeList = { {FailureType::ORTHSTRAIN},{FailureType::USERi } };
            break;
        }
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW1:{
            typeList = {{FailureType::ALTER},{FailureType::COCKCROFT },{FailureType::FLD },
            {FailureType::ENERGY },{FailureType::TENSSTRAIN },{FailureType::TBUTCHER },
            {FailureType::ORTHSTRAIN},{FailureType::LAD_DAMA},{FailureType::USERi},{FailureType::VISUAL}};
            break;
        }
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW2:{
            typeList = { {FailureType::ALTER},{FailureType::BIQUAD },{FailureType::COCKCROFT },
            {FailureType::FLD },{FailureType::NXT },{FailureType::ENERGY },{FailureType::ECM },
            {FailureType::TENSSTRAIN },{FailureType::JOHNSON },{FailureType::SPALLING },
            {FailureType::TAB1 },{FailureType::TBUTCHER }, {FailureType::WIERZBICKI },
            {FailureType::WILKINS },{FailureType::ORTHOBIQUAD },{FailureType::ORTHSTRAIN},
            {FailureType::USERi},{FailureType::VISUAL} };
            break;
        }
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW6:{
            typeList = { {FailureType::ENERGY},{FailureType::TENSSTRAIN}, {FailureType::TBUTCHER },
             {FailureType::USERi},{FailureType::VISUAL} };
            break;
        }
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW27:{
            typeList = { {FailureType::ALTER},{FailureType::BIQUAD },{FailureType::COCKCROFT },
            {FailureType::FLD },{FailureType::JOHNSON },{FailureType::SPALLING },
            {FailureType::TAB1 },{FailureType::TBUTCHER }, {FailureType::WIERZBICKI },
            {FailureType::WILKINS },{FailureType::ORTHSTRAIN},{FailureType::USERi},{FailureType::VISUAL}};
            break;
        }
        case Radioss::FITKAbstractRadMaterial::RadMatType::LAW36:{
            typeList = { {FailureType::ALTER},{FailureType::BIQUAD },{FailureType::COCKCROFT },
            {FailureType::FLD },{FailureType::NXT },{FailureType::ENERGY },{FailureType::HC_DSSE },
            {FailureType::ECM },{FailureType::TENSSTRAIN },{FailureType::JOHNSON },{FailureType::SPALLING },
            {FailureType::TAB1 },{FailureType::TBUTCHER },{FailureType::WILKINS },{FailureType::ORTHSTRAIN},
            {FailureType::USERi},{FailureType::VISUAL} };
            break;
        }
        default:
            break;
        }
        for (int i = 0; i < mgr->getDataCount(); ++i)
        {
            Radioss::FITKAbstractFailureModel* fail = mgr->getDataByIndex(i);
            if (!fail)continue;
            FailureType failType = fail->getFailureModelType();
            Core::FITKEnumTransfer<FailureType> ft;
            bool ok = false;
            QString str = ft.toString((FailureType)failType, ok);
            if (!ok)continue;
            if (typeList.contains(failType))
            {
                _ui->comboBoxFail->addItem(QString("%1(%2)").arg(fail->getDataObjectName()).arg(str),
                    fail->getDataObjectID());
            }
        }
        bool state = mat->isFailState();
        _ui->checkBoxFail->setChecked(state);
        if (!state)return;
        int index = _ui->comboBoxFail->findData(mat->getFailID());
        _ui->comboBoxFail->setCurrentIndex(index);
    }
    QTreeWidgetItem* GUIMaterialPropDialog::addChiledItem(QTreeWidgetItem* p, const QString& objName,const QString& label,const QString& unit)
    {
        if (!p)return nullptr;
        QWidget* widget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout();
        Comp::FITKSciNotationLineEdit* lineEdit = new Comp::FITKSciNotationLineEdit(this);
        lineEdit->setObjectName(objName);
        //// 设置 lineEdit 只能输入数字
        //QDoubleValidator *doubleValidator = new QDoubleValidator(0, 1e30, 9, this);
        //doubleValidator->setNotation(QDoubleValidator::StandardNotation);
        //lineEdit->setValidator(doubleValidator);

        QLabel* qlabel = new QLabel(this);
        qlabel->setText(unit);
        layout->addWidget(lineEdit);
        layout->addWidget(qlabel);
        layout->setStretch(0, 1);
        layout->setStretch(1, 1);
        widget->setLayout(layout);
        QTreeWidgetItem* item = new QTreeWidgetItem(p);
        item->setText(0, label);
        _ui->treeWidget_parameter->setItemWidget(item, 1, widget);
        return item;
    }

    void GUIMaterialPropDialog::on_pushButtonOK_clicked()
    {
        //创建动作
        if (_isCreate)
        {
            if (!this->createObjData())
                return;
            _oper->execProfession();
        }
        //修改动作
        else 
        {
            if (!this->editObjData())
                return;
            _oper->execProfession();
        }
        this->writePythonScript();
        this->accept();
    }

    void GUIMaterialPropDialog::on_pushButtonCancel_clicked()
    {
        //点击取消按键动作
        this->reject();
    }

    void GUIMaterialPropDialog::reject()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        if (_isCreate) {
            //删除对象
            if (_obj) delete _obj;
            _obj = nullptr;
        }
        QDialog::reject();
    }
}