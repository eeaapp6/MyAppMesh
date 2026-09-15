/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperExportManager.h"

// Qt
#include <QFileDialog>
#include <QDialog>
#include <QDebug>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Abstract geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

namespace ModelOper
{
    bool OperExportManager::execGUI()
    {
        // 检查是否存在几何。
        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        QList<Interface::FITKAbsGeoCommand*> rootCmds = geoData->getRootCommandList();
        if (rootCmds.count() == 0)
        {
            printLog(2, tr("No model exsits !"));
            return false;
        }

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

        // 选择保存文件路径。
        QFileDialog dlg(FITKGLODATA->getMainWindow(), tr("Export To File"), "", 
            tr("BREP files (*.brep);;STP files (*.stp);;STEP files (*.step);;IGS files (*.igs);;IGES files (*.iges);;STL files (*.stl)"));
        dlg.setOption(QFileDialog::DontUseNativeDialog);
        dlg.setAcceptMode(QFileDialog::AcceptSave);

        QString fileName;
        if (QDialog::Accepted == dlg.exec())
        {
            QString suffixFilter = dlg.selectedNameFilter();
            QStringList files = dlg.selectedFiles();
            if (suffixFilter.isEmpty() || files.isEmpty())
            {
                return false;
            }

            QString suffixWithHalfBrak = suffixFilter.split("*").last().trimmed();
            QString realSuffix = suffixWithHalfBrak.mid(0, suffixWithHalfBrak.length() - 1);

            fileName = files.first();
            if (!fileName.endsWith(realSuffix))
            {
                fileName += realSuffix;
            }
        }

        if (fileName.isEmpty())
        {
            return false;
        }

        // 保存参数。
        setArgs("FileName", fileName);

        return true;
    }

    bool OperExportManager::execProfession()
    {
        // 获取并清除参数。
        QString fileName;
        argValue("FileName", fileName);
        clearArgs();
        if (fileName.isEmpty())
        {
            return false;
        }

        // 创建导出命令。
        Interface::FITKInterfaceGeometryFactory* fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        Interface::FITKAbsGeoModelExport* cmd = fac->createCommandT<Interface::FITKAbsGeoModelExport>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExport);
        if (!cmd)
        {
            return false;
        }

        // 退出草图模式。
        exitSketchMode();

        cmd->setFileName(fileName);
        bool flag = cmd->update();

        // 析构。
        delete cmd;

        // 输出日志。
        if (flag)
        {    
            printLog(1, tr("The models have been saved to file \"%1\".").arg(fileName));
        }
        else
        {
            printLog(3, tr("Failed to save the models !"));
        }

        return flag;
    }

} // namespace ModelOper
