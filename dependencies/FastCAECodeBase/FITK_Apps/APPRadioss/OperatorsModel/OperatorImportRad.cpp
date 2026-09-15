/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorImportRad.h"
#include <QFile>
#include <QFileDialog>
#include <QAction>
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Component/FITKRadiossRadIO/FITKRadiossRadIOInterface.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusPart.h"
#include "FITK_Component/FITKAbaqusRadiossDataMapper/FITKAbaqusRadiossDataMapperInterface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace ModelOper
{

    bool FITKOperatorImportRad::execGUI()
    {
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open INP File"),"" , "INP(*.inp)");

        QFileDialog dlg(mw, QObject::tr("Import Rad File"), wk, "Rad Files(*.rad)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);  //打开模式
        dlg.show();
        
        //阻塞，否则键盘事件处理会出问题
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
        //存储信息
        if (_emitter == nullptr) return false;

        this->setArgs("FileName", fileName);
        return true;
    }

    bool FITKOperatorImportRad::execProfession()
    {
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        if (!ok) return false;
        QFileInfo finfo(fileName); 
        if (!finfo.exists()) return false;

        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Reading project file %1 ...")).arg(fileName));

        //读写组件获取
        Radioss::FITKRadiossRadIoInterface* IO = FITKAPP->getComponents()->
            getComponentTByName<Radioss::FITKRadiossRadIoInterface>("FITKRadiossRadIO");
        if (IO == nullptr) return false;
        //设置文件名称
        IO->setFileName(fileName);

        //solution名称
        fileName = finfo.completeBaseName();
        QString solutionName = fileName;
        if (fileName.endsWith("_0000", Qt::CaseSensitive) ||
            fileName.endsWith("_0001", Qt::CaseSensitive))
        {
            solutionName = fileName.left(fileName.size()-5);
        }
        
        //新建求解方案
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        Radioss::FITKRadiossSolutionManager* soluMgr = caseData->getSolutionManager();
        if(soluMgr == nullptr) return false;
        Radioss::FITKRadiossSolution* solution = new Radioss::FITKRadiossSolution;
        solution->setDataObjectName(soluMgr->checkName(solutionName));
        soluMgr->appendDataObj(solution);
        soluMgr->setCurrentSolution(solution->getDataObjectID());

        IO->setDataObject("RadiossCaseData", caseData);
        //设置是否在线程运行
        IO->runInThread(true);
        //等待线程结束
        Core::FITKThreadPool::getInstance()->wait();

        //关联结束读取槽函数
        connect(IO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        //执行读取
        IO->exec(1);
 
        return true;
    }

    void FITKOperatorImportRad::ioThreadFinishedSlot()
    {
        //获取Radioss IO接口
        Radioss::FITKRadiossRadIoInterface* IO = FITKAPP->getComponents()->
            getComponentTByName<Radioss::FITKRadiossRadIoInterface>("FITKRadiossRadIO");
        if (IO == nullptr) return;

        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface* mapper = FITKAPP->getComponents()->
            getComponentTByName<AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface>("AbaqusRadiossDataMapper");
        if (caseData != nullptr && mapper != nullptr)
        {
            AbaqusData::FITKDataCase* abaCase = new AbaqusData::FITKDataCase();
            mapper->setRadiossCase(caseData);
            mapper->setAbaqusCase(abaCase);
            const bool mapOK = mapper->exec(2);
            const int partCount = abaCase->getPartManager() ? abaCase->getPartManager()->getDataCount() : 0;
            AppFrame::FITKMessageNormal(QString::fromLocal8Bit("Test Radioss2Abaqus mapMeshParts finished. ok=%1, abaqus part count=%2")
                .arg(mapOK ? QString("true") : QString("false"))
                .arg(partCount));
            delete abaCase;
        }

        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!graphOper) return;
        graphOper->updateAllModel(true);
         
        // 通过树形菜单事件处理器刷新树
        QStringList listOperator = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOperator.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOperator[i]));
            if (oper != nullptr)
            {
                oper->updateTree();
                // 通过模拟双击事件删除数据
                if (listOperator[i] == "PartTreeEvent") oper->on_itemDoubleClicked(nullptr, nullptr, 0);
            }
        }

        
    }
}
