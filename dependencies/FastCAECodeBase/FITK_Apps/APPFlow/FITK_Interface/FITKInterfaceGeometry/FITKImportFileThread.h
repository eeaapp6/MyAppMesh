/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKImportFileThread.h
 * @brief       File IO thread class for importing geometry.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-12-18
 *********************************************************************/

#ifndef __FITKIMPORTFILETHREAD_H__
#define __FITKIMPORTFILETHREAD_H__

#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

#include <QList>

#include "FITKGeoEnum.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       File IO thread class for importing geometry.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-18
     */
    class FITKInterfaceGeometryAPI FITKImportFileThread : public Core::FITKThreadTask
    {
        Q_OBJECT
    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        explicit FITKImportFileThread() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        virtual ~FITKImportFileThread() = default;

        /**
         * @brief       The task execute function.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        virtual void run() override;

        /**
         * @brief       Set the geometry file path.
         * @param[in]   filePath: The geometry file path
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        void setFilePath(QString filePath);

        /**
         * @brief       Set the imported method.
         * @param[in]   type: The import command type
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        void setImportedMethod(Interface::FITKGeoEnum::FITKGeometryComType type);

        /**
         * @brief       Get the ids of the reader geometry command.
         * @return      The id list[const][quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        const QList<int> & newCmdIds();

        /**
         * @brief       Get the running state of the task.
         * @return      Is the task running
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        bool isRunning();

    signals:
        /**
         * @brief       Signal - For IO thread finish.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        void sig_readFinished(bool status);

    protected:
        /**
         * @brief       The geometry file path.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        QString m_filePath;

        /**
         * @brief       The imported method.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        Interface::FITKGeoEnum::FITKGeometryComType m_ioType = 
            Interface::FITKGeoEnum::FITKGeometryComType::FGTImport;

        /**
         * @brief       The new command ids.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        QList<int> m_newIds;

        /**
         * @brief       The running state of the task.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        bool m_isRunning = false;

    };
}

#endif // !__FITKIMPORTFILETHREAD_H__
