/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperOpenHDF5.h"
#include <QFile>
#include <QFileDialog>
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h" 
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataHDF5Interface.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

namespace OperModel
{
    bool OperOpenHDF5::execGUI()
    {
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open INP File"),"" , "INP(*.inp)");

        QFileDialog dlg(mw, QObject::tr("Open Project File"), wk, "HDF5(*.hdf5)");
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

    bool OperOpenHDF5::execProfession()
    {
        //文件存在
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        if (!ok) return false;
        QFileInfo finfo(fileName);
        if (!finfo.exists()) return false;

        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Reading project file %1 ...")).arg(fileName));

        ////获取数据对象
        //Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
        //    getPhysicsData<Radioss::FITKRadiossCase>();
        //if (caseObj == nullptr) return false;

        ////获取读取组件
        //auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKRadiossDataHDF5Interface>("FITKRadiossDataHDF5IO");
        //if (fitkAbaIO == nullptr) return false;

        ////关联信号，后续处理结束的事件
        //connect(fitkAbaIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));
        ////参数设置
        //fitkAbaIO->setFileName(fileName);
        //fitkAbaIO->setDataObject(caseObj);
        ////执行读取，1-读取 2- 写出
        //fitkAbaIO->exec(1);

        ReadRadiossIOHDF5* readThread = new ReadRadiossIOHDF5();
        readThread->setFileName(fileName);
        QString error{};
        if (!readThread->createContext(error))
        {
            AppFrame::FITKMessageError(error);
            delete readThread;
            return false;
        }

        connect(readThread, SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        readThread->push2ThreadPool();
        return true;
    }
    void OperOpenHDF5::ioThreadFinishedSlot()
    {
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        this->clearArgs();

        auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKRadiossDataHDF5Interface>("FITKRadiossDataHDF5IO");
        if (fitkAbaIO == nullptr) return;
        //执行结束打断信号，避免下次执行重复关联，以及其他读写线程触发该槽函数
        disconnect(fitkAbaIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));
        //是否读取成功
        const bool success = fitkAbaIO->isSuccess();
        if (!success)
        {
            //读取失败
            AppFrame::FITKMessageError(QString(tr("Failed reading inp file from %1")).arg(fileName));
            //             if (_emitter->objectName() == "actionImportAsModel" && _caseData)
            //                 delete _caseData;
            return;
        }
        //读取成功
        AppFrame::FITKMessageNormal(QString(tr("Read project file from %1")).arg(fileName));
        //打开的文件
        AppFrame::FITKRunTimeSetting* rs = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (rs)
            rs->setValue("ProjectFile", fileName);

        //刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }

        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper)
        {
            graphOper->updateAll();
        }
    }


    void ReadRadiossIOHDF5::consoleMessage(int level, const QString& str)
    {

    }

    void ReadRadiossIOHDF5::run()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList) return;

        //读入几何
        IO::FITKOCCIOHDF5Interface* geoIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        if (geoIO == nullptr) return;
        geoIO->setHDF5Root(_h5File);
        geoIO->setRunInThread(false);
        geoIO->setDataObject(cmdList);
        geoIO->exec(1);

        //Radioss数据读入 
        auto fitkAbaIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKRadiossDataHDF5Interface>("FITKRadiossDataHDF5IO");
        if (fitkAbaIO == nullptr) return;
        fitkAbaIO->setHDF5Root(_h5File);
        fitkAbaIO->setRunInThread(false);
        fitkAbaIO->setDataObject(caseObj);
        fitkAbaIO->exec(1);

        this->closeFile();
    }
}



