/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSaveAs.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"  
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppHistoryFiles.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/SARibbonBarHelper.h"
#include <QFile>
#include <QFileDialog>

namespace ModelOper
{
    bool OperatorsSaveAs::execGUI()
    {
        this->clearArgs();
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        QFileDialog dlg(mw, QObject::tr("Save Project File"), wk, "HDF5(*.hdf5)");
        dlg.setAcceptMode(QFileDialog::AcceptSave);  //保存模式
        dlg.show();
        //阻塞，否则键盘按键事件不能处理
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;

        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.isEmpty()) return false;
        QString fileName = files.at(0);
        if (fileName.isEmpty())
            return false;
        if (!fileName.toLower().endsWith(".hdf5"))
            fileName += ".hdf5";
        this->setArgs("FileName", fileName);
        return true;
    }
    bool OperatorsSaveAs::execProfession()
    {
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;
        //写出工程文件
        Core::FITKActionOperator* oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionSave");
        if (!oper) return false;
        oper->setArgs("FileName", fileName);
        return oper->execProfession();
    }

}

