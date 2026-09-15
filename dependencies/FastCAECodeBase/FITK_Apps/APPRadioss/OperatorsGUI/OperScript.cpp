/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperScript.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppScriptHelper.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
//#include "FITK_Kernel/FITKAppFramework/FITKAppHistoryFiles.h"
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"
#include "GUIFrame/MainWindow.h"
#include <QWidget>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

bool GUIOper::OperScript::execGUI()
{
    //根据不同按钮执行不同操作
    if (_emitter == nullptr) return false;
    QString name = _emitter->objectName();
    //开始记录
    if (name == "actionRecordScript")
    {
        this->recordScript();
    }
    //结束记录
    else if (name == "actionEndScript")
    {
        this->endScript();
    }
    //执行脚本
    else if (name == "actionRunScript")
    {
        this->runScript();
    }
    //清理变量
    else if (name == "actionClearVariable")
    {
        this->clearVariable();
    }
    //打开脚本
    else if (name == "actionfileOpenScript")
    {
        this->openScript();
    }
    return false;
}

void GUIOper::OperScript::recordScript()
{
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return;
    //记录脚本名与结束记录脚本按钮状态设置
    QAction* act = mw->findChild<QAction*>("actionRecordScript");
    if (act) act->setEnabled(false);
    act = mw->findChild<QAction*>("actionEndScript");
    if (act) act->setEnabled(true);
    //打开记录状态
    AppFrame::FITKAppScriptHelper* h = FITKAPP->getScriptHelper();
    if (h)
        h->enableRecord();

}

bool GUIOper::OperScript::execProfession()
{
    this->runScript();
    return false;
}

void GUIOper::OperScript::endScript()
{
    GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
    if (!mw) return;
    //关闭记录状态
    AppFrame::FITKAppScriptHelper* h = FITKAPP->getScriptHelper();
    if (!h) return;

    //获取工作目录
    QString wk;
    AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
    if (settings)
        wk = settings->getWorkingDir();
    //对话框选择文件
    QString fileName = QFileDialog::getSaveFileName(mw, tr("Save Script"), wk, tr("Py File(*.py)"));
    if (fileName.isEmpty())
    {
        QString message = QString(tr("Whether to end script and clear script ?"));
        int ret = QMessageBox::warning(nullptr, tr("Warning"), message, tr("Yes"), tr("No"));
        if (ret == 0)
        {
            //记录脚本与结束记录脚本按钮状态设置
            QAction* act = mw->findChild<QAction*>("actionRecordScript");
            if (act) act->setEnabled(true);
            act = mw->findChild<QAction*>("actionEndScript");
            if (act) act->setEnabled(false);

            h->clearTextRepo();
            h->enableRecord(false);
        }
        return;
    }
    //写出文件
    h->exportScriptFile(fileName);

    //记录脚本名与结束记录脚本按钮状态设置
    QAction* act = mw->findChild<QAction*>("actionRecordScript");
    if (act) act->setEnabled(true);
    act = mw->findChild<QAction*>("actionEndScript");
    if (act) act->setEnabled(false);

    h->enableRecord(false);

    //将文件添加到近期文件里
    /*AppFrame::FITKAppHistoryFiles* historyFiles = FITKAPP->getGlobalData()->getHistoryFiles();
    if (historyFiles == nullptr) return;
    historyFiles->addFile(fileName, AppFrame::FITKAppHistoryFileInfo::HistoryFileType::HFTUserDef);
    mw->updateHistoryFiles();*/
}

void GUIOper::OperScript::runScript()
{
    //获取文件路径与文件名
    QString fileName;
    bool isOk = argValue("FileName", fileName);
    clearArgs();

    if (!isOk || fileName.isEmpty())
    {
        fileName = QFileDialog::getOpenFileName(nullptr, tr("Run Script"), "", tr("Py File(*.py)"));
    }
 
    if (fileName.isEmpty())return;

    //执行脚本文件
    AppFrame::FITKAppScriptHelper* h = FITKAPP->getScriptHelper();
    if (h) 
    {
        h->execScriptFile(fileName);
    }
}

void GUIOper::OperScript::openScript()
{
    QAction * obj = dynamic_cast<QAction *>(_emitter);
    if (obj == nullptr) return;
    GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
    if (!mw) return;
    QString fileObjName = obj->text();
    //执行脚本文件
    AppFrame::FITKAppScriptHelper* h = FITKAPP->getScriptHelper();
    if (h) {
        h->execScriptFile(fileObjName);
        //将文件添加到近期文件里
        /*AppFrame::FITKAppHistoryFiles* historyFiles = FITKAPP->getGlobalData()->getHistoryFiles();
        if (historyFiles == nullptr) return;
        historyFiles->addFile(fileObjName, AppFrame::FITKAppHistoryFileInfo::HistoryFileType::HFTUserDef);
        mw->updateHistoryFiles();*/
    }
}

void GUIOper::OperScript::clearVariable()
{
    AppFrame::FITKAppScriptHelper* h = FITKAPP->getScriptHelper();
    if (h) h->clearTextRepo();

    //Python::FITKPythonInterface* py = Python::FITKPythonInterface::getInstance();
    //if (py)py->claerPythonEnvoirment();

}