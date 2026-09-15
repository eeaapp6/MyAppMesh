/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaIOInterface.h
 * @brief  Interface for the FITKRadiossLSDynaIO component
 * @author guqingtao (15598887859@163.com)
 * @date   2025-07-01
 *********************************************************************/
#ifndef _FITKRadiossLSDynaIOInterface_H_
#define _FITKRadiossLSDynaIOInterface_H_

#include <QObject>
#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"
namespace Interface
{
    class FITKAbstractIO;
}
namespace Radioss
{
    /**
     * @brief  Interface for the FITKRadiossLSDynaIO component
     * @author guqingtao (15598887859@163.com)
     * @date   2025-07-01
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaIOInterface : public QObject, public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        explicit FITKRadiossLSDynaIOInterface() = default;
        virtual ~FITKRadiossLSDynaIOInterface()  =default;
        /**
         * @brief Execute the I/O operation
         * @param[i]  indexPort      1 for read, 2 for write
         * @return true  成功,多线程执行时候返回值无效
         * @return false 失败，多线程执行时候返回值无效
         * @author guqingtao (15598887859@163.com)
         * @date 2025-07-01
         */
        bool exec(const int indexPort) override;
        /**
         * @brief    设置文件名称
         * @param[i] fileName    文件名称，要求包含绝对路径
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-01
         */
        void setFileName(const QString& fileName);
        /**
         * @brief 是否在独立线程执行读写
         * @param[i]  inThread       是否在独立线程执行读写操作
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-01
         */
        void runInThread(const bool & inThread = true);
        /**
         * @brief 是否正在执行
         * @return true
         * @return false
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-01
         */
        bool isRunning();
        /**
         * @brief 执行是否成功
         * @return true
         * @return false
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-01
         */
        bool isSuccess();
        /**
         * @brief    获取部件名称，不能重复  返回 FITKRadiossLsDynaIO
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-01
         */
        QString getComponentName() override;

    signals:
        /**
         * @brief  读写线程结束信号
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-01
         */
        void ioThreadFinishedSig();

    private:
        /**
         * @brief  文件名称
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-01
         */
        QString _fileName{};
        /**
         * @brief  是否在线程运行
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-01
         */
        bool _runInThread{ true };
        /**
         * @brief  是否执行成功标记
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-01
         */
        bool _success{ false };
        /**
         * @brief  io 对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-01
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> _io{ nullptr };

    };
}
#endif
