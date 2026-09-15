/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUISolverRunDialog.h"
#include "ui_GUISolverRunDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include <QCheckBox>
#include <QRadioButton>
#include <QFileDialog>
#include <QMessageBox>

namespace GUI
{
    GUISolverRunDialog::GUISolverRunDialog(Core::FITKAbstractOperator* oper, QWidget * parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUISolverRunDialog)
        , _oper(oper)
    {
        _ui->setupUi(this);
        init();
    }

    GUISolverRunDialog::~GUISolverRunDialog()
    {
        if (_ui) delete _ui;
    }

    void GUISolverRunDialog::init()
    {
        // 清空
        _ui->tableWidget_Model->clear();
        _ui->tableWidget_Model->setRowCount(0);
        // 设置列数
        _ui->tableWidget_Model->setColumnCount(1);
        _ui->tableWidget_Model->setHorizontalHeaderLabels(QStringList{ "Solution"});


        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
         //获取求解管理器
        Radioss::FITKRadiossSolutionManager* solutionModel = caseData->getSolutionManager();
        if (solutionModel == nullptr) return ;

        _setSoliutionID = solutionModel->getCurrentSolutionID();

        for (int i = 0; i < solutionModel->getDataCount(); ++i) {
            Radioss::FITKRadiossSolution* solution = solutionModel->getDataByIndex(i);
            if (!solution)continue;
            QRadioButton* butt = this->addLine(solution->getDataObjectName());
            if (solution->getDataObjectID() == _setSoliutionID)
                butt->setChecked(true);
            connect(butt, &QRadioButton::clicked, this, [=](bool checked) {
                if(checked)
                    _setSoliutionID = solution->getDataObjectID();
            });
        }

    }

    QRadioButton* GUISolverRunDialog::addLine(const QString & label)
    {
        int row = _ui->tableWidget_Model->rowCount();
        _ui->tableWidget_Model->insertRow(row);

        QRadioButton * butt = new QRadioButton(label, _ui->tableWidget_Model);
        _ui->tableWidget_Model->setCellWidget(row, 0, butt);

        return butt;
    }

    void GUISolverRunDialog::on_pushButton_OK_clicked()
    {
        if (!_oper) return;
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        Radioss::FITKRadiossSolutionManager* solutionModel = caseData->getSolutionManager();
        if (solutionModel == nullptr) return;
        //切换当前求解方案
        solutionModel->setCurrentSolution(_setSoliutionID);

        //获取工作目录
        QString wk;
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        if (!QDir(wk).exists())
        {
            QMessageBox::warning(this, QObject::tr("Warning"), QObject::tr("Work Dir not exist! Please set the working path to calculate."), QMessageBox::StandardButton::Ok);
            reject();
        }

        QString starterFile;
        QString engineFile;
        QString caseName;
        bool isImport = false;

        int modeType = _ui->comboBox_Model->currentIndex();
        switch (modeType)
        {
        case 0:
        {
            isImport = false;
            //获取数据
            QList<QRadioButton*> list = _ui->tableWidget_Model->findChildren<QRadioButton*>();
            caseName = solutionModel->getCurrentSolution()->getDataObjectName();
            starterFile = QString("%1/%2/%3%4").arg(wk).arg(caseName).arg(caseName).arg("_0000.rad");
            engineFile = QString("%1/%2/%3%4").arg(wk).arg(caseName).arg(caseName).arg("_0001.rad");
            break;
        }
        case 1:
        {
            isImport = true;
            //获取数据
            QString modelFile = _ui->lineEdit_ImportModel->text();
            caseName = QString(QFileInfo(modelFile).baseName()).replace("_0000", "").replace("_0001", "");
            starterFile = QString("%1/%2_0000.rad").arg(QFileInfo(modelFile).absolutePath()).arg(caseName);
            engineFile = QString("%1/%2_0001.rad").arg(QFileInfo(modelFile).absolutePath()).arg(caseName);
            break;
        }
        default:
            break;
        }
        if (caseName.isEmpty())return;
        //设置求解工作路径
        QString workDir = QString("%1/%2").arg(wk).arg(caseName);

        _oper->setArgs("ThreadNum", _ui->spinBox_ThreadNum->value());
        _oper->setArgs("CaseDataID", caseData->getDataObjectID());
        _oper->setArgs("SolutionID", _setSoliutionID);

        _oper->setArgs("StarterFileName", starterFile);
        _oper->setArgs("EngineFileName", engineFile);
        _oper->setArgs("CaseName", caseName);
        _oper->setArgs("WorkDir", workDir);
        _oper->setArgs("isImport", isImport);
        accept();
    }

    void GUISolverRunDialog::on_pushButton_Cancel_clicked()
    {
        reject();
    }

    void GUISolverRunDialog::on_comboBox_Model_currentIndexChanged(int index)
    {
        _ui->stackedWidget->setCurrentIndex(index);
    }

    void GUISolverRunDialog::on_pushButton_ImportModel_clicked()
    {
        //获取工作目录
        QString wk;
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();

        QFileDialog dlg(this, QObject::tr("Model File"), wk, "RAD(*.rad)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);  //保存模式
        dlg.show();
        //阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return;
        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.isEmpty()) return;
        QString fileName = files.at(0);

        _ui->lineEdit_ImportModel->setText(fileName);
    }
}