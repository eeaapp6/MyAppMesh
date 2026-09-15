/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractMeshProcessor.h
 * @brief 网格生成结束处理器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-18
 *
 */
#ifndef __FITK_ABSTRACTMESH_PROCESSOR_H___
#define __FITK_ABSTRACTMESH_PROCESSOR_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include <QStringList>
#include <QHash>

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Interface
{
    /**
     * @brief 网格生成后处理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-18
     */
    class FITKInterfaceMeshGenAPI FITKAbstractMeshProcessor :
        public Core::FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKAbstractMeshProcessor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        explicit FITKAbstractMeshProcessor() = default;
        /**
         * @brief Destroy the FITKAbstractMeshProcessor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual ~FITKAbstractMeshProcessor() = 0;
        /**
         * @brief 设置参数
         * @param args 参数列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void setArgs(const QStringList & args);
        /**
         * @brief 开始执行业务操作
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void start(QStringList info = QStringList()) = 0;
        /**
         * @brief 插入数据对象，用于提取网格数据
         * @param[i]  key
         * @param[i]  value
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-14
         */
        virtual void insertDataObject(QString key, Core::FITKAbstractDataObject* value);

        /**
         * @brief       获取数据对象。
         * @param[in]   key：键
         * @return      数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-02
         */
        Core::FITKAbstractDataObject* getDataObject(QString key);

        /**
         * @brief       获取数据对象。
         * @param[in]   key：键
         * @return      数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-02
         */
        template <class T>
        T* getDataObjectAs(QString key)
        {
            return dynamic_cast<T*>(getDataObject(key));
        }

    protected:
        /**
         * @brief 参数列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        QStringList _args{};
        /**
         * @brief 数据对象，用于提取网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-14
         */
        QHash<QString, Core::FITKAbstractDataObject*> _dataObject{};
    };
}

#endif
