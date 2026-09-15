/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperImportMesh.h"
#include "MeshImporterNastran.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include "FITK_Component/FITKAbaqusData/FITKAbaqusData.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusPart.h"
#include "FITK_Component/FITKAbaqusIOINP/FITKAbaqusIOINPInterface.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusAssembly.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"
#include "FITK_Component/FITKAbaqusRadiossDataMapper/FITKAbaqusRadiossDataMapperInterface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include <QFile>
#include <QFileDialog>

namespace ModelOper
{

    bool FITKOperatorImportMesh::execGUI()
    {
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open INP File"),"" , "INP(*.inp)");

        QFileDialog dlg(mw, QObject::tr("Import Mesh File"), wk, "Nastran Files(*.bdf *.nas);;INP(*.inp)");
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


    bool FITKOperatorImportMesh::execProfession()
    {
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        if (!ok) return false;
        QFileInfo finfo(fileName); 
        if (!finfo.exists()) return false;
        const QString suffix = finfo.suffix().toLower();

        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Reading mesh file %1 ...")).arg(fileName));
        
        Core::FITKThreadPool::getInstance()->wait(); //等待之前的任务完成

        if(suffix == "bdf" || suffix == "nas")
        {
            //读取Nastran网格文件
            ModelOper::FITKNastranMeshImporter* importor = new ModelOper::FITKNastranMeshImporter();
            
            importor->setFileName(fileName);
            connect(importor, &Core::FITKThreadTask::taskFinishedSig, this, &FITKOperatorImportMesh::ioThreadFinishedSlot);
            Core::FITKThreadPool::getInstance()->execTask(importor);
        }

        if(suffix == "inp")
        {
            //读取Abaqus网格文件
            _abaqusDataCase = new AbaqusData::FITKDataCase();
            IO::INPReadLevel level = IO::ReadAssembly;
            //获取读取组件
            auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKAbaqusINPIO>("AbaqusINPIO");
            if (fitkAbaIO == nullptr) return false;
            //关联信号，后续处理结束的事件
            connect(fitkAbaIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot_inp()));
            //参数设置
            fitkAbaIO->setFileName(fileName);
            fitkAbaIO->setReadLevel(level);
            fitkAbaIO->setDataObject(_abaqusDataCase);
            //执行读取，1-读取 2- 写出
            fitkAbaIO->exec(1);
        }

        return true;
        //读写组件获取
    }

    void FITKOperatorImportMesh::ioThreadFinishedSlot_inp()
    {
        if (_abaqusDataCase == nullptr) return;
        //获取abaqus装配
        auto assembly = _abaqusDataCase->getAssembly();
        if (assembly == nullptr) return;
        if (assembly->getDataCount() > 0) return;
        //获取part管理器
        auto partManager = _abaqusDataCase->getPartManager();
        if (!partManager || partManager->getDataCount() == 0) return;
        //将part添加到装配中
        for (int i = 0; i < partManager->getDataCount(); i++)
        {
            auto part = partManager->getDataByIndex(i);
            Interface::FITKAbstractAssInstance* instance = new Interface::FITKAbstractAssInstance;
            instance->setModel(part->getDataObjectID());

            QString partName = part->getDataObjectName();
            QString instanceName = assembly->checkName(partName + "-1");
            instance->setDataObjectName(instanceName);
            assembly->appendDataObj(instance);
        }
        Radioss::FITKRadiossCase* radcaseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface* mapper = FITKAPP->getComponents()->
            getComponentTByName<AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface>("AbaqusRadiossDataMapper");
        if (radcaseData != nullptr && mapper != nullptr)
        {
            mapper->setRadiossCase(radcaseData);
            mapper->setAbaqusCase(_abaqusDataCase);
            const bool mapOK = mapper->exec(1);
            if (!mapOK)
            {
                AppFrame::FITKMessageError(tr("Failed to map Abaqus data to Radioss data!"));
                return;
            }
            else
            {
                AppFrame::FITKMessageNormal(tr("Successfully mapped Abaqus data to Radioss data!"));
            }
        }
        //等待线程结束
        Core::FITKThreadPool::getInstance()->wait();
        
        delete _abaqusDataCase;
        _abaqusDataCase = nullptr;

        this->ioThreadFinishedSlot(); //调用读取完成后的处理函数，刷新界面等
    }

    void FITKOperatorImportMesh::ioThreadFinishedSlot()
    {
         //获取Radioss IO接口
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!graphOper) return;
        graphOper->updateAllModel(false);
         
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



    FITKAbstractMeshImporter::FITKAbstractMeshImporter(QObject *parent)
        : FITKThreadTask(parent)
    {
        Q_UNUSED(parent);
    }
    
    FITKAbstractMeshImporter::~FITKAbstractMeshImporter()
    {}

    void FITKAbstractMeshImporter::setFileName(const QString& fileName)
    {
        _fileName = fileName;
    }

}