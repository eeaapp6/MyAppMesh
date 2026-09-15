/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractAutoSaver.h
 * @brief 声明抽象自动保存器，用于自动保存工程文件、脚本等
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-09-13
 * 
 */
#ifndef _FITKABSTRACT_AUTO_SAVER_H___
#define _FITKABSTRACT_AUTO_SAVER_H___
 
#include "FITKAppFrameworkAPI.h"
#include <QObject>

class QTimer;

namespace AppFrame
{
    /**
     * @brief 抽象自动保存器，间隔一定时间执行一次
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-09-13
     */
    class FITKAppFrameworkAPI FITKAbstractAutoSaver : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractAutoSaver object
         * @param[i] msc 触发时间间隔 单位毫秒
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        explicit FITKAbstractAutoSaver(const int & msc = 120000);
        /**
         * @brief Destroy the FITKAbstractAutoSaver object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        virtual ~FITKAbstractAutoSaver();
        /**
         * @brief 启动自动保存器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        virtual void start();
        /**
         * @brief 停止计时
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        virtual void stop();
        /**
         * @brief 设置时间间隔，默认2分钟
         * @param[i]  msc            触发时间间隔 单位毫秒
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        void setTimeInterval(const int & msc =120000 );
        /**
         * @brief 自动保存逻辑
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        virtual void autoSave() = 0;
        /**
         * @brief 程序正常运行结束，执行操作
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        virtual void finalize() = 0;
    
    private slots:
        /**
         * @brief 超时信号
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        void timeOutSlot();

    private:
        /**
         * @brief 计时器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        QTimer* _timer{};
    };
    

}


#endif
