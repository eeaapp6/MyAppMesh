/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorImportGeometry.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include <QFileDialog>
#include "OperatorsInterface/TreeEventOperator.h"

namespace ModelOper
{
    OperatorsImportGeometry::OperatorsImportGeometry()
    {

    }

    OperatorsImportGeometry::~OperatorsImportGeometry()
    {

    }

    bool OperatorsImportGeometry::execGUI()
    {

        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        QFileDialog dlg(mw, QObject::tr("Import Model File"), wk, "Geometry File(*.brep *.stp *.step *.igs *.iges)");
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
        //if (_emitter == nullptr) return false;
        //存储
        this->setArgs("FileName", fileName);
        return true;
    }

    bool OperatorsImportGeometry::execProfession()
    {
        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString fileName;
        this->argValue<QString>("FileName", fileName);
        if (fileName.isEmpty()) return false;

        ImportReadThread* importThread = new ImportReadThread();
        importThread->_fileName = fileName;
        connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotImportFinish(bool, int)));
        pool->execTask(importThread);
        //       this->clearArgs();
        return true;
    }

    void OperatorsImportGeometry::slotImportFinish(bool result, int objID)
    {
        if (result == false)return;
        if (objID < 0)return;


        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper) {
            graphOper->updateGraph(objID,true);
        }
        //刷新树
        // 通过树形菜单事件处理器刷新树
        QStringList listOperator = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOperator.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOperator[i]));
            if (oper != nullptr) oper->updateTree();
            // 通过模拟双击事件删除数据
            if (listOperator[i] == "PartTreeEvent") oper->on_itemDoubleClicked(nullptr, nullptr, 0);
        }

    }

    void ImportReadThread::run()
    {
        emit sendProcessSig(this, -1);
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return;
        Radioss::FITKRadiossGeomPartManager* partMgr = radiossCase->getGeomPartManager();
        if (partMgr == nullptr) return;
        Radioss::FITKRadiossGeomPart* part = new Radioss::FITKRadiossGeomPart();
        partMgr->appendDataObj(part);
        Interface::FITKAbsGeoPart* geoPartData = part->getGeoPart();
        if (!geoPartData) return;
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (geoFactory == nullptr)return;

        Interface::FITKAbsGeoImportBase* geoObj = geoFactory->createCommandT<Interface::FITKAbsGeoImportBase>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTImport);
        if (geoObj == nullptr)return;
        geoObj->setFileName(_fileName);
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return;
        if (geoObj->getDataObjectName().isEmpty())
        {
            QFileInfo fileInfo(_fileName);
            // 获取文件名称（不包含路径与文件类型）
            QString name = fileInfo.baseName();
            geoObj->setDataObjectName(geoPartData->checkName(name));
            geoPartData->setDataObjectName(cmdList->checkName(name));
        }
        geoPartData->addAndUpdate(geoObj);
        emit sigImportFinish(true, geoPartData->getDataObjectID());
    }
}

