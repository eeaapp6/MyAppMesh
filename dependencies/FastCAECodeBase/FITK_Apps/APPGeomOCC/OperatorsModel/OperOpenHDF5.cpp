/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperOpenHDF5.h"

#include <QFile>
#include <QFileDialog>
#include <QDialog>

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h" 
#include "GUIFrame/MainWindow.h"

#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"

// Geometry and IO
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"

namespace ModelOper
{
    bool OperOpenHDF5::execGUI()
    {
        // 先退出草图模式。
        exitSketchMode();

        // 关闭所有现有对话框。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* iDlg : dlgs)
        {
            if (iDlg)
            {
                iDlg->close();
            }
        }

        //主界面
        QWidget* mw = FITKGLODATA->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open INP File"),"" , "INP(*.inp)");

        QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open Project File"), wk, "HDF5(*.hdf5)", nullptr,
            QFileDialog::DontUseNativeDialog);
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

        //获取数据对象
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList) return false;


        //获取读取组件
        auto fitkIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        if (fitkIO == nullptr) return false;

        //关联信号，后续处理结束的事件
        connect(fitkIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));
        //参数设置
        fitkIO->setFileName(fileName);
        fitkIO->setDataObject(cmdList);
        //执行读取，1-读取 2- 写出
        fitkIO->exec(1);

        return true;
    }
    void OperOpenHDF5::ioThreadFinishedSlot()
    {
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        this->clearArgs();

        auto fitkIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        if (fitkIO == nullptr) return;
        //执行结束打断信号，避免下次执行重复关联，以及其他读写线程触发该槽函数
        disconnect(fitkIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));

        //是否读取成功
        const bool success = fitkIO->isSuccess();
        if (!success)
        {
            //读取失败
            AppFrame::FITKMessageError(QString(tr("Failed reading project file from %1")).arg(fileName));
            //             if (_emitter->objectName() == "actionImportAsModel" && _caseData)
            //                 delete _caseData;
            return;
        }
        //读取成功
        AppFrame::FITKMessageNormal(QString(tr("Read project file from %1")).arg(fileName));
        //打开的文件
        AppFrame::FITKRunTimeSetting* rs = FITKGLODATA->getRunTimeSetting();
        if (rs)
            rs->setValue("ProjectFile", fileName);

        // 刷新树。
        GUI::MainWindow * mainWindow = dynamic_cast<GUI::MainWindow *>(FITKGLODATA->getMainWindow());
        if (mainWindow)
        {
            mainWindow->updateGeometryTree();
        }  

        // 刷新可视化窗口。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraphs();
        }
    }
}



