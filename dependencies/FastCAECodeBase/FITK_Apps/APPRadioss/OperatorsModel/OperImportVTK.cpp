/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperImportVTK.h"
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QAction>
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "GUIFrame/MainWindow.h"
//#include "GUIDialog/GUIToolDialog/GUIFileDirectoryDialog.h"
#include "FITK_Component/FITKWidget/FITKFileDirectoryDialog.h"
#include "GUIWidget/GraphArea.h"
#include "GUIFrame/CentralWidget.h"
#include "GUIWidget/GraphMdiArea.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"


namespace OperModel
{
    bool OperImportVTK::execGUI()
    {
        //清理全部参数避免干扰
        this->clearArgs();
        //获取主窗口
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
//        GUI::GUIFileDirectoryDialog* fileDialog = new GUI::GUIFileDirectoryDialog(this, mainWin);
        GUI::FITKFileDirectoryDialog* fileDialog = new GUI::FITKFileDirectoryDialog(this, mainWin);
        fileDialog->iniFileFilterType("Support Files(*.vtk;;*.dat;;*.plt;;*.cgns;;*.vtp)");

        //设置对话框路径
        AppFrame::FITKAppSettings* AppSetting = FITKAPP->getAppSettings();
        if (AppSetting) {
            QString workDir = AppSetting->getWorkingDir();
            fileDialog->initWin(workDir);
        }

        fileDialog->show();
        return false;
    }

    bool OperImportVTK::execProfession()
    {
        //文件存在
        QStringList fileNames;
        bool ok = this->argValue<QStringList>("FileNames", fileNames);
        if (!ok || fileNames.isEmpty()) {
            return false;
        }
        //获取算例ID
        int caseDataID = -1, solutionID = -1;
        this->argValue<int>("CaseDataID", caseDataID);
        this->argValue<int>("SolutionID", solutionID);
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return false;
        Interface::FITKStruPost3DManager* post3DManager = radiossPostData->getPost3DManager();
        if (post3DManager == nullptr) return false;
        
        // 根据文件后缀筛选数据类型。
        QFileInfo fileInfo(fileNames.at(0));
        QString type = fileInfo.suffix().toLower();
        Interface::StruPostFileType fileType = Interface::StruPostFileType::SPFTNone;
        if (type == "vtk")
            fileType = Interface::StruPostFileType::SPFTVTK;
        else if (type == "vtu")
            fileType = Interface::StruPostFileType::SPFTVTU;
        else if (type == "vtp")
            fileType = Interface::StruPostFileType::SPFTVTP;
        else
            return false;
        //判断该解决方案是否已存在结果
        if (solutionID > 0)
        {
            int count = post3DManager->getDataCount();
            for (int i = 0; i < count; ++i)
            {
                Interface::FITKAbstractStructuralPostVTK* postData = post3DManager->getDataByIndex(i);
                if (!postData || postData->getPreCaseID() != solutionID) continue;
                post3DManager->removeDataObj(postData);
                break;
            }
        }
        //创建数据
        Interface::FITKAbstractStructuralPostVTK* dataObj = nullptr;
        if (fileNames.size() > 1) {
            //创建数据
            Interface::FITKDynamicStruPostVTK* postData = new Interface::FITKDynamicStruPostVTK(fileType);
            QFileInfo fileInfo(fileNames.at(0));
            postData->setFiles(fileNames);
            QString name = post3DManager->checkName(fileInfo.absolutePath());
            postData->setDataObjectName(name);
            dataObj = postData;
        }
        else {
            //创建数据
            Interface::FITKStaticStruPostVTK* postData = new Interface::FITKStaticStruPostVTK(fileType);
            postData->setFile(fileNames.at(0));
            //输出信息
            AppFrame::FITKMessageNormal(QString(tr("Reading vtk file %1 ...")).arg(fileNames.at(0)));
            QString name = post3DManager->checkName(fileNames.at(0));
            postData->setDataObjectName(name);
            dataObj = postData;
        }
        if (dataObj == nullptr)return false;
        //设置参数
        dataObj->setPreCaseID(solutionID);
        dataObj->setIsCellDataToPointData(false);
        radiossPostData->getPost3DManager()->appendDataObj(dataObj);
        radiossPostData->setCurrentPostDataID(dataObj->getDataObjectID());
        //更新后处理渲染
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPostProcess");
        if (graphOper)
        {
            graphOper->updateAll();
        }

        // 获取part树操作器
        EventOper::TreeEventOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("ResultTreeEvent");
        if (treeOper == nullptr) return false;
        treeOper->updateTree();

        // 设置第一帧数据
        Core::FITKActionOperator* oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionUpdateToolBarField");
        if (oper == nullptr) return false;
        QAction* action = new QAction();
        action->setObjectName("actionUpdateToolBarField");
        oper->setEmitter(action);
        oper->actionTriggered();

        delete action;
        return true;
    }

}



