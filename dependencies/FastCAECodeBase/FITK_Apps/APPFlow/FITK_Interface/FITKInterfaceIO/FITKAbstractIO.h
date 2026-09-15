/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractIO.h
 * @brief 文件读写类抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-08
 *
 */
#ifndef _FITKABSTRACTIO_H____
#define _FITKABSTRACTIO_H____

#include "FITKInterfaceIOAPI.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Interface
{
    /**
     * @brief 文件读写抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-08
     */
    class FITKInterfaceIOAPI FITKAbstractIO : public Core::FITKThreadTask
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractIO object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        explicit FITKAbstractIO() = default;
        /**
         * @brief Destroy the FITKAbstractIO object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        virtual ~FITKAbstractIO() = 0;
        /**
         * @brief 设置读取文件的名称
         * @param[i]  fileName       文件名称，带绝对路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        virtual void setFileName(const QString& fileName);
        /**
         * @brief 获取文件名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        QString getFileName() const;
        /**
         * @brief 设置数据
         * @param[i] data 数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data);
        /**
         * @brief 推送到线程池执行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        void push2ThreadPool();

        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        virtual void consoleMessage(int level, const QString& str) = 0;

    protected:
        /**
         * @brief 文件名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        QString _fileName{};
        /**
         * @brief 操作的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        Core::FITKAbstractDataObject* _data{};

    };


}


#endif
