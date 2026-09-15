/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSaveHDF5.h"

#include <QFileDialog>
#include <QDialog>
#include <QList>

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h" 

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"

// Geometry and IO
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"

#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace ModelOper
{
    bool OperSaveHDF5::execGUI()
    {
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* iDlg : dlgs)
        {
            if (iDlg)
            {
                iDlg->close();
            }
        }

        //从运行设置中获取保存的文件
        AppFrame::FITKRunTimeSetting* rs = FITKGLODATA->getRunTimeSetting();
        if (rs)
        {
            QString file = rs->getValueT<QString>("ProjectFile");
            if (QFile::exists(file))
            {
                this->setArgs("FileName", file);
                return true;
            }
        }

        //选择文件
        this->clearArgs();
        //主界面
        QWidget* mw = FITKGLODATA->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
//        _fileName = QFileDialog::getSaveFileName(mw, QObject::tr("Write INP File"), wk, "INP(*.inp)");
        QString fileName = QFileDialog::getSaveFileName(mw, QObject::tr("Save Project File"), wk, "HDF5(*.hdf5)", nullptr,
            QFileDialog::DontUseNativeDialog);
        if (fileName.isEmpty())
            return false;

        if (!fileName.toLower().endsWith(".hdf5"))
            fileName += ".hdf5";
        this->setArgs("FileName", fileName);
        return true;
    }

    bool OperSaveHDF5::execProfession()
    {
        //获取数据对象
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList) return false;

        auto fitkIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        if (fitkIO == nullptr) return false;

        //关联信号，后续处理结束的事件
        connect(fitkIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()));

        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        //参数设置
        fitkIO->setFileName(fileName); 
        fitkIO->setDataObject(cmdList);
        //执行读取，1-读取 2- 写出
        fitkIO->exec(2);
        return  true;
    }
    void OperSaveHDF5::ioThreadFinishedSlot()
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
            AppFrame::FITKMessageError(QString(tr("Failed writing project file to %1")).arg(fileName));

        }
        else
        {
            AppFrame::FITKMessageNormal(QString(tr("Write project file to %1")).arg(fileName));
            //保存的文件
            AppFrame::FITKRunTimeSetting* rs = FITKGLODATA->getRunTimeSetting();
            if (rs) 
                rs->setValue("ProjectFile", fileName);
        }
    }

}
