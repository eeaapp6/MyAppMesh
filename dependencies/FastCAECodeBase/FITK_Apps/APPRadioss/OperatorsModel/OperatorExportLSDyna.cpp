/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorExportLSDyna.h"
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
#include "FITK_Component/FITKRadiossLSDynaTransfer/FITKRadiossLSDynaIOInterface.h"

namespace ModelOper
{
    bool FITKOperatorExportLSDyna::execGUI()
    {
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        // QString fileName = QFileDialog::getOpenFileName(mw, QObject::tr("Open INP File"),"" , "INP(*.inp)");

        QFileDialog dlg(mw, QObject::tr("Export K/KEY File"), wk, "LS-DYNA Files(*.k);;LS-DYNA Files(*.key)");
        dlg.setAcceptMode(QFileDialog::AcceptSave);  //打开模式
        dlg.setDefaultSuffix("k");
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
        //添加后缀
        const QString selectedFilter = dlg.selectedNameFilter();
        if (!fileName.contains('.')) {
            if (selectedFilter.contains("*.k"))
                fileName += ".k";
            else if (selectedFilter.contains("*.key"))
                fileName += ".key";
            else
                fileName += ".k"; // 默认后缀
        }
        //存储信息
        if (_emitter == nullptr) return false;

        this->setArgs("FileName", fileName);
        return true;
    }

    bool FITKOperatorExportLSDyna::execProfession()
    {
        QString fileName;
        bool ok = this->argValue<QString>("FileName", fileName);
        if (!ok) return false;

        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Reading project file %1 ...")).arg(fileName));

        //读写组件获取
        Radioss::FITKRadiossLSDynaIOInterface* IO = FITKAPP->getComponents()->
            getComponentTByName<Radioss::FITKRadiossLSDynaIOInterface>("FITKRadiossLSDynaIO");
        if (IO == nullptr) return false;
        IO->setDataObject("RadiossCaseData",FITKAPP->getGlobalData()->getPhysicsData<Core::FITKAbstractDataObject>());
        //设置文件名称
        IO->setFileName(fileName);
        //设置是否在线程运行
        IO->runInThread(true);
        //等待线程结束
        Core::FITKThreadPool::getInstance()->wait();

        //关联结束读取槽函数
        connect(IO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        //执行写出
        IO->exec(2);
 
        return true;
    }

    void FITKOperatorExportLSDyna::ioThreadFinishedSlot()
    {
        // TODO: Implement thread finished slot logic
    }
}
