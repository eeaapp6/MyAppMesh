/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperImportManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDelete.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/GraphInteractionOperator.h"

#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QDialog>

namespace ModelOper
{
    /**
     * @brief 界面逻辑，生成UI交互
     * @return 执行成功返回true，否则返回false
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-15
     */
    bool OperImportManager::execGUI()
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

        // 尝试获取模型数据。
        Interface::FITKAbsGeoCommand* cmd = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getDataByID(m_Id);

        switch (m_OperType)
        {
        case Create:
        {
            QStringList fileNames = QFileDialog::getOpenFileNames(FITKGLODATA->getMainWindow(), tr("Import From File"), "",
                // tr("All Files(*.brep;*.stp *.step;*.igs *.iges;*.stl);;"
                    tr("BREP files (*.brep);;STEP files (*.stp *.step);;"
                    "IGES files (*.igs *.iges);;STL files (*.stl)"), nullptr,
                QFileDialog::DontUseNativeDialog);
            if (fileNames.isEmpty()) return false;
            this->setArgs("FileNames", fileNames);
            this->setArgs("Delete", false);
            break;
        }
        case Edit:
        {
            printLog(2, tr("This model is not editable !"));
            return false;
        }
        case Delete:
        {
            // 移除数据。
            this->setArgs("Delete", true);
            deleteCmd(m_Id);
            break;
        }
        case Show:
        {
            if (!cmd)
            {
                return false;
            }

            // 启用数据。
            cmd->enable(true);

            // 更新可视化对象。
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                graphOper->updateGraph(m_Id);
            }

            return false;
        }
        case Hide:
        {
            if (!cmd)
            {
                return false;
            }

            // 禁用数据。
            cmd->enable(false);

            // 更新可视化对象。
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                graphOper->updateGraph(m_Id);
            }

            return false;
        }
        default:
            return false;
        }

        return true;
    }

    /**
     * @brief 业务处理逻辑，在execGUI后执行
     * @return 执行成功返回true，否则返回falses
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-15
     */
    bool OperImportManager::execProfession()
    {
        bool isDelete = false;
        QStringList fileNames;
        this->argValue("FileNames", fileNames);
        this->argValue("Delete", isDelete);

        if (isDelete)
        {
            // 退出草图模式。
            exitSketchMode();

            if (m_MainWindow != nullptr)
                m_MainWindow->updateGeometryTree();
            return true;
        }

        if (fileNames.isEmpty()) return false;

        // 退出草图模式。
        exitSketchMode();

        this->clearArgs();

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

        for (const QString fileName : fileNames)
        {
            auto importCmd = fac->createCommandT<Interface::FITKAbsGeoModelImport>(Interface::FITKGeoEnum::FITKGeometryComType::FGTImport);
            importCmd->setFileName(fileName);
            bool flag = importCmd->update();
            if (!flag)
            {
                continue;
            }

            QFileInfo fileInfo(fileName);
            importCmd->setDataObjectName(geoCmdList->checkName(fileInfo.completeBaseName()));
            geoCmdList->appendDataObj(importCmd);

            if (graphPreprocessOper != nullptr) {
                graphPreprocessOper->updateGraph(importCmd->getDataObjectID());
            }
        }

        // 初始化默认基准元素状态。
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            operPick->setCustomPickMode();
        }

        if (m_MainWindow != nullptr)
            m_MainWindow->updateGeometryTree();

        return true;
    }

} // namespace ModelOper
