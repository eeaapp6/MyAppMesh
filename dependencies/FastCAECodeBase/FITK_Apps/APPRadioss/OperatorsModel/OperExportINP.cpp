/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperExportINP.h"
#include <QFileDialog>
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKExportFileThread.h"
#include "FITK_Component/FITKAbaqusIOINP/FITKAbaqusIOINPInterface.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCaseManager.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusData.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusPart.h"


#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusPart.h"
#include "FITK_Component/FITKAbaqusRadiossDataMapper/FITKAbaqusRadiossDataMapperInterface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"



namespace OperModel
{
    bool OperExportINP::execGUI()
    {

        this->clearArgs();
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();

        // 获取导出文件方式。
        if (_emitter == nullptr) return false;
        auto name = _emitter->objectName();



        QString title, suffixes;
        if (name == "actionExportAsParts")
        {

            title = QObject::tr("Export Part");
            suffixes = "ACIS SAT(*.sat);;STEP(*.stp *.step)";
        }
        else if (name == "actionExportAsModel")
        {
            title = QObject::tr("Write INP File");
            suffixes = "INP(*.inp)";
        }

        QFileDialog dlg(mw, title, wk, suffixes);
        dlg.setAcceptMode(QFileDialog::AcceptSave);  //保存模式
        dlg.setLabelText(QFileDialog::DialogLabel::Reject, tr("Cancel"));
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
        if (fileName.isEmpty()) return false;
        QString nameFilter = dlg.selectedNameFilter();
        QFileInfo fileInfo(fileName);
        QString suffix = fileInfo.suffix();
        if (suffix.isEmpty())
        {
            if (nameFilter == "ACIS SAT(*.sat)")       fileName += ".sat";
            else if (nameFilter == "STEP(*.stp *.step)")    fileName += ".stp";
            else if (nameFilter == "INP(*.inp)")            fileName += ".inp";
        }

        //存储信息
        if (name == "actionExportAsParts")
        {
            //this->setArgs("PartGeoID", part->getGeoCommandObjID());
        }

        this->setArgs("Emitter", name);
        this->setArgs("FileName", fileName);
        return true;

    }

    bool OperExportINP::execProfession()
    {
        //文件存在
        QString fileName;
        if (!argValue("FileName", fileName)) return false;

        //触发者
        QString name;
        if (!argValue("Emitter", name)) return false;
        if (name == "actionExportAsParts")
        {
            int cmdId = -1;
            if (!argValue("PartGeoID", cmdId)) return false;

            // 创建读取线程。
            if (!m_partsIO)
            {
                m_partsIO = new Interface::FITKExportFileThread;
                connect(m_partsIO, &Interface::FITKExportFileThread::sig_readFinished, this, &OperExportINP::slot_geometryPartsIOFinished);
                m_partsIO->setAutoDelete(true);

                // 保存状态。
                setArgs("IsReading", true);
                connect(m_partsIO, &QObject::destroyed, this, [&]
                {
                    m_partsIO = nullptr;
                    setArgs("IsReading", false);
                });
            }
            else
            {
                if (m_partsIO->isRunning())
                {
                    AppFrame::FITKMessageWarning(QString(tr("Please wait for the completion of the previous task!")).arg(fileName));
                    return false;
                }
            }

            m_partsIO->addExportCmdId(cmdId);
            m_partsIO->setFilePath(fileName);
            m_partsIO->setExportedMethod(Interface::FITKGeoEnum::FGTExport);

            Core::FITKThreadPool::getInstance()->execTask(m_partsIO);
        }
        else if (name == "actionExportAsModel")
        {
            Radioss::FITKRadiossCase* radcaseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface* mapper = FITKAPP->getComponents()->
                getComponentTByName<AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface>("AbaqusRadiossDataMapper");
            AbaqusData::FITKDataCase* abaCase = new AbaqusData::FITKDataCase();
            if (radcaseData != nullptr && mapper != nullptr)
            {
                mapper->setRadiossCase(radcaseData);
                mapper->setAbaqusCase(abaCase);
                const bool mapOK = mapper->exec(2);
                if (!mapOK)
                {
                    AppFrame::FITKMessageError(tr("Failed to map Radioss data to Abaqus data!"));
                    return false;
                }
                else
                {
                    AppFrame::FITKMessageNormal(tr("Successfully mapped Radioss data to Abaqus data!"));
				}
            }



            //获取数据对象
            auto caseData = abaCase;
            if (caseData == nullptr) return false;

            //获取需要创建绑定约束的几何信息
            QHash<int, QList<int>> coplanesOfPart = {};
            AbaqusData::FITKAbaqusPartManager* partManager = caseData->getPartManager();
            for (int partIndex = 0; partIndex < partManager->getDataCount(); ++partIndex) {
                AbaqusData::FITKAbaqusPart* part = partManager->getDataByIndex(partIndex);
                if (part == nullptr) {
                    continue;
                }
                QList<int> coplanes;
                coplanesOfPart.insert(part->getDataObjectID(), coplanes);
            }

            auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKAbaqusINPIO>("AbaqusINPIO");
            if (fitkAbaIO == nullptr) return false;

            //关联信号，后续处理结束的事件
            connect(fitkAbaIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));
            QString fileName;
            this->argValue<QString>("FileName", fileName);
            //参数设置
            fitkAbaIO->setCoplanesOfPart(coplanesOfPart);
            fitkAbaIO->setFileName(fileName);
            fitkAbaIO->setDataObject(caseData);
            //执行读取，1-读取 2- 写出
            fitkAbaIO->exec(2);
        }
        return true;
    }
    void OperExportINP::ioThreadFinishedSlot()
    {
        auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKAbaqusINPIO>("AbaqusINPIO");
        if (fitkAbaIO == nullptr) return;
        QString fileName;
        this->argValue<QString>("FileName", fileName);
        this->clearArgs();
        //执行结束打断信号，避免下次执行重复关联，以及其他读写线程触发该槽函数
        disconnect(fitkAbaIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));
        //是否读取成功
        const bool success = fitkAbaIO->isSuccess();

        if (!success)
        {
            //读取失败
            AppFrame::FITKMessageError(QString(tr("Failed writing inp file to %1")).arg(fileName));
        }
        else
        {
            AppFrame::FITKMessageNormal(QString(tr("Write inp file to %1")).arg(fileName));
        }

       


    }

    void OperExportINP::slot_geometryPartsIOFinished(bool status)
    {
        QString fileName;
        argValue("FileName", fileName);
        this->clearArgs();
        //disconnect(m_partsIO, &Interface::FITKExportFileThread::sig_readFinished, this, &OperExportINP::slot_geometryPartsIOFinished);
        if (status) AppFrame::FITKMessageNormal(QString(tr("Write file to %1")).arg(fileName));
        else        AppFrame::FITKMessageError(QString(tr("Failed writing file to %1")).arg(fileName));
    }
}
