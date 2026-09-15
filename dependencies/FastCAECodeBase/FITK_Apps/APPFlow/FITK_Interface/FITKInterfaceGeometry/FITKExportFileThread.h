/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKExportFileThread.h
 * @brief   File IO thread class for exoprting geometry.
 *
 * @author  fulipeng (flipengqd@yeah.net)
 * @date    2025-01-16
 *********************************************************************/

#ifndef __FITKEXPORTFILETHREAD_H__
#define __FITKEXPORTFILETHREAD_H__
#include "FITKInterfaceGeometryAPI.h"
#include "FITKGeoEnum.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include <QList>

namespace Interface
{
    /**
     * @brief   File IO thread class for exoprting geometry.
     * @author  fulipeng (flipengqd@yeah.net)
     * @date    2025-01-16
     */
    class FITKInterfaceGeometryAPI FITKExportFileThread : public Core::FITKThreadTask
    {
        Q_OBJECT
    public:
        /**
         * @brief   Constructor.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        explicit FITKExportFileThread() = default;

        /**
         * @brief   Destructor.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        virtual ~FITKExportFileThread() = default;

        /**
         * @brief   The task execute function.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        virtual void run() override;

        /**
         * @brief   Set the geometry file path.
         * @param   filePath: The geometry file path
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        void setFilePath(QString filePath);

        /**
         * @brief   Set the imported method.
         * @param   type: The import command type
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        void setExportedMethod(Interface::FITKGeoEnum::FITKGeometryComType type);

        /**
         * @brief       添加需要导出的命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        void addExportCmdId(int cmdId);

        /**
         * @brief   Get the running state of the task.
         * @return  Is the task running
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        bool isRunning();

    signals:
        /**
         * @brief   Signal - For IO thread finish.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        void sig_readFinished(bool status);

    protected:
        /**
         * @brief   The geometry file path.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        QString m_filePath;

        /**
         * @brief   The imported method.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        Interface::FITKGeoEnum::FITKGeometryComType m_ioType = 
            Interface::FITKGeoEnum::FITKGeometryComType::FGTExport;

        /**
         * @brief   The new command ids.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        QList<int> m_cmdIds;

        /**
         * @brief   The running state of the task.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        bool m_isRunning = false;

    };
}

#endif // !__FITKExportFileThread_H__
