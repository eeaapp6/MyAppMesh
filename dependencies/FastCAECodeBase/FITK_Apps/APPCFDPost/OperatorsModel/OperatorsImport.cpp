/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsImport.h"

#include "GUIFrame/MainWindow.h"
#include "GUIDialog/FileDialogDirectory.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSteady.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostUnSteady.h"

#include <iostream>
#include <QDebug>

namespace OperModel {
    OperatorsImport::OperatorsImport()
    {

    }

    bool OperatorsImport::execGUI()
    {
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
        GUI::FileDialogDirectory* fileDialog = new GUI::FileDialogDirectory(this, mainWin);
        fileDialog->appendFileFilterType("VTK File(*.vtk;;*.vtp;;*.vtu)");
        fileDialog->appendFileFilterType("Tecplot File(*.dat;;*.plt)");
        fileDialog->appendFileFilterType("*.cgns");

        //设置对话框路径
        AppFrame::FITKAppSettings* AppSetting = FITKAPP->getAppSettings();
        if (AppSetting) {
            QString workDir = AppSetting->getWorkingDir();
            QString path = AppSetting->getValue<QString>(CFDPostSelectFilePath);
            if (!path.isEmpty()) {
                fileDialog->initWin(path);
            }
            else if (!workDir.isEmpty()) {
                fileDialog->initWin(workDir);
            }
        }
        
        fileDialog->show();
        return false;
    }

    bool OperatorsImport::execProfession()
    {
        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString files = "";
        QString filePath = "";
        argValue("files", files);
        argValue("filePath", filePath);
        if (files.isEmpty())return false;

        //存储选择路径
        AppFrame::FITKAppSettings* AppSetting = FITKAPP->getAppSettings();
        if (AppSetting) {
            AppSetting->setValue(CFDPostSelectFilePath, filePath);
        }

        ImportReadThread* importThread = new ImportReadThread();
        importThread->_fileName = files;
        connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotImportFinish(bool, int)));
        pool->execTask(importThread);

        this->clearArgs();
        return true;
    }

    void OperatorsImport::slotImportFinish(bool isOk, int objID)
    {
        if (isOk == false)return;
        if (objID < 0)return;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        EventOper::ParaWidgetInterfaceOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("ModelTreeEvent");
        if (treeOper == nullptr)return;

        graphOper->updateSingle(objID);
        treeOper->updateTree();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void ImportReadThread::run()
    {
        Interface::FITKCFDPost3DManager* manager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (manager == nullptr)return;

        //文件信息获取
        QString f(_fileName);
        f = f.replace(QString("\\"), QString("/"));
        const int index = _fileName.lastIndexOf(".");
        const int nsize = _fileName.size();
        QString suffix = _fileName.right(nsize - index - 1).toLower();
        const int lIndex = f.lastIndexOf("/");
        QString name = _fileName.right(nsize - lIndex - 1);

        bool isGroup = false;
        if (_fileName.indexOf("...") > 0)isGroup = true;

        Interface::FITKAbstractCFDPostData* postData = nullptr;
        if (isGroup) {
            Interface::FITKCFDPostUnSteady* data = new Interface::FITKCFDPostUnSteady(suffix, _fileName);
            QStringList fileList = data->getFiles();
            if (fileList.size() <= 0) {
                delete data;
                return;
            }
            data->setFile(fileList[0]);
            data->setDataObjectName(name);
            postData = data;
        }
        else {
            Interface::FITKCFDPostSteady* data = new Interface::FITKCFDPostSteady(suffix);
            data->setFile(_fileName);
            data->setDataObjectName(name);
            postData = data;
        }

        manager->appendDataObj(postData);

        emit sigImportFinish(true, postData->getDataObjectID());
    }
}
