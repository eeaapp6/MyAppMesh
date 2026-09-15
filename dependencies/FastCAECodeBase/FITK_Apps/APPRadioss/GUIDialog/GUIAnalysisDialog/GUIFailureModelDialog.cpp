/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIFailureModelDialog.h"
#include "ui_GUIFailureModelDialog.h"
#include "FITK_Component/FITKWidget/FITKSciNotationLineEdit.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include <QComboBox>

namespace GUI
{
    GUIFailureModelDialog::GUIFailureModelDialog(Core::FITKActionOperator * oper, Radioss::FITKAbstractFailureModel * obj, QWidget * parent)
        : _oper(oper), _ui(new Ui::GUIFailureModelDialog), Core::FITKDialog(parent), _obj(obj)
    {
        _ui->setupUi(this);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        // 设置对话框标题
        setWindowTitle(tr("Failure Model"));
        // 允许窗口任意缩小
        //setMinimumSize(0, 0); 
        // 根据内容调整尺寸
        //adjustSize(); 
        //换行
        _ui->tableWidget->setWordWrap(true); 
    }

    GUIFailureModelDialog::~GUIFailureModelDialog()
    {
        // 释放UI
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIFailureModelDialog::init()
    {
        //虚函数
    }

    bool GUIFailureModelDialog::ckeckData()
    {
        //虚函数
        return false;
    }

    void GUIFailureModelDialog::writePythonScript()
    {
        //虚函数
    }

    QLineEdit* GUIFailureModelDialog::addLineEdit(const QString & label, const QString & objname)
    {
        //计算字符数和单词数,单词数达到5且总字符数大于34时，插入回车，设置单元格高度为两行
        QStringList words = label.split(" ", Qt::SkipEmptyParts);
        int totalChars = label.length();
        QString newLabel= label;
        int maxWords = 4;
        if (words.size() > 9)
            maxWords = words.size() / 2 + 1; 
        if (words.size() > maxWords && totalChars >= 39) {
            // 在第maxWords个单词后插入换行符
            int insertPos = 0;
            for (int i = 0; i < maxWords; ++i) {
                insertPos += words[i].length() + 1; // +1 是空格
            }
            newLabel = label.left(insertPos) + "\n" + label.mid(insertPos); // 插入\n
        }

        int row = _ui->tableWidget->rowCount();
        _ui->tableWidget->insertRow(row);
        //行标题不可编辑
        QTableWidgetItem* item = new QTableWidgetItem(newLabel);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidget->setItem(row, 0, item);

        Comp::FITKSciNotationLineEdit* lineEdit = new Comp::FITKSciNotationLineEdit(this);
        lineEdit->setObjectName(objname);
        //lineEdit->setStyleSheet("border: none;");

        _ui->tableWidget->setCellWidget(row, 1, lineEdit);
        return lineEdit;
    }

    QComboBox * GUIFailureModelDialog::addComboBox(const QString & label, const QString & objname)
    {
        //计算字符数和单词数,单词数达到4且总字符数大于34时，插入回车，设置单元格高度为两行
        QStringList words = label.split(" ", Qt::SkipEmptyParts);
        int totalChars = label.length();
        QString newLabel = label;
        int maxWords = 4;
        if (words.size() > 9)
            maxWords = words.size() / 2 + 1;
        if (words.size() > maxWords && totalChars >= 39) {
            // 在第maxWords个单词后插入换行符
            int insertPos = 0;
            for (int i = 0; i < maxWords; ++i) {
                insertPos += words[i].length() + 1; // +1 是空格
            }
            newLabel = label.left(insertPos) + "\n" + label.mid(insertPos); // 插入\n
        }

        int row = _ui->tableWidget->rowCount();
        _ui->tableWidget->insertRow(row);
        //行标题不可编辑
        QTableWidgetItem* item = new QTableWidgetItem(newLabel);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidget->setItem(row, 0, item);

        QComboBox* comBobox = new QComboBox(this);
        comBobox->setObjectName(objname);
        //comBobox->setStyleSheet("border: none;");

        _ui->tableWidget->setCellWidget(row, 1, comBobox);
        return comBobox;
    }

    Radioss::FITKRadiossFailureModelManager * GUIFailureModelDialog::getFailureManager()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        // 获取管理器
        return dataCase->getFailureModelManager();
    }

    void GUIFailureModelDialog::on_pushButtonOK_clicked()
    {
        auto mgr = this->getFailureManager();
        if (!_oper || !_obj || !mgr) return;
        if (!this->ckeckData()) return;

        _oldNameForScript = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEditName->text());
        //设置界面数据
        this->getDataFormWidget();

        //创建动作
        if (!_isEditMode)
        {
            mgr->appendDataObj(_obj);
        }
        _oper->execProfession();
        this->writePythonScript();

        this->accept();
    }

    void GUIFailureModelDialog::on_pushButtonCancel_clicked()
    {
        if (!_isEditMode) {
            //删除对象
            if (_obj) delete _obj;
            _obj = nullptr;
        }
        QDialog::reject();
    }
}