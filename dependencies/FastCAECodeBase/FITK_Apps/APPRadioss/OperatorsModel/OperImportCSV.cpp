/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperImportCSV.h"

#include "GUIFrame/MainWindow.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "GUIDialog/GUIPostDialog/GUICSVCurveDialog.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

namespace ModelOper
{
    bool OperImportCSV::execGUI()
    {
        this->clearArgs();
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        // 获取工作目录
        QString wk;
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // 打开文件窗口
        QFileDialog dlg(FITKAPP->getGlobalData()->getMainWindow(),QObject::tr("Import Port File"), wk,"Text Files(*.csv);;All Files(*.*)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);
        dlg.setFileMode(QFileDialog::ExistingFile); 
        dlg.show();
        // 阻塞等待用户选择
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;
        // 获取选择文件名称
        QStringList files = dlg.selectedFiles();
        if (files.isEmpty()) return false;
        this->setArgs("ImportFile", files.at(0));
        return true;
    }

    bool OperImportCSV::execProfession()
    {
        //获取后处理2维数据管理器
        Radioss::FITKRadiossPostData* postData = FITKAPP->getGlobalData()->getPostData<Radioss::FITKRadiossPostData>();
        if (!postData) return false;
        Radioss::FITKRadiossPost2DManager* post2DManager = postData->getPost2DManager();
        if (!post2DManager) return false;
        //设置参数
        int caseID = -1, solutionID = -1;
        this->argValue<int>("CaseID", caseID);
        this->argValue<int>("SolutionID", solutionID);

        QString fileName{};
        this->argValue<QString>("ImportFile", fileName);
        //创建后处理2维数据
        if (solutionID > 0)
        {
            int count = post2DManager->getDataCount();
            for (int i = 0; i < count; i++)
            {
                Radioss::FITKRadiossPost2DData* data = post2DManager->getDataByIndex(i);
                if (!data) return false;
                if (solutionID == data->getSolutionID())
                {
                    data->setFileName(fileName);
                    return true;
                }
            }
        }
        Radioss::FITKRadiossPost2DData* post2DData = new Radioss::FITKRadiossPost2DData();
        post2DData->setDataObjectName(fileName);
        post2DData->setCaseDataID(caseID);
        post2DData->setSolutionID(solutionID);
        post2DData->setFileName(fileName);
        post2DManager->appendDataObj(post2DData);

        //更新后处理树
        EventOper::TreeEventOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("ResultTreeEvent");
        if (treeOper == nullptr) return false;
        treeOper->updateTree();
        return true;
    }
}
  