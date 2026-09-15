/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractGlobalDataFactory.h
 * @brief 声明全局数据创建器抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef  __ABSTRACT_GOLBALDATAFACTORY_H__
#define  __ABSTRACT_GOLBALDATAFACTORY_H__

#include "FITKAppFrameworkAPI.h"
#include "FITKGlobalData.h"
#include <QHash>

namespace Core
{
    class FITKAbstractDataObject;
}

namespace AppFrame
{
    class FITKGlobalData;
    /**
     * @brief 创建全局数据
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKAppFrameworkAPI FITKAbstractGlobalDataFactory
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGlobalDataFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        explicit FITKAbstractGlobalDataFactory() = default;
        /**
         * @brief Destroy the FITKAbstractGlobalDataFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual ~FITKAbstractGlobalDataFactory() = 0;
        /**
         * @brief 执行创建操作，将数据存入全局数据对象
         * @param[i]  globalData    全局数据指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        void createData(FITKGlobalData* globalData);

    private:
        /**
         * @brief 创建全局网格，可以为空
         * @return Core::FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual Core::FITKAbstractDataObject* createMeshData();
        /**
         * @brief 创建全局几何，可以为空
         * @return Core::FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual Core::FITKAbstractDataObject* createGeoData();
        /**
         * @brief 创建全局物理数据，一般不为空
         * @return Core::FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual Core::FITKAbstractDataObject* createPhysicsData();
        /**
         * @brief 创建后处理数据
         * @return Core::FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual Core::FITKAbstractDataObject* createPostData();
//         /**
//          * @brief 创建软件运行时数据存储
//          * @return Core::FITKAbstractDataObject*
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-02-29
//          */
//         virtual Core::FITKAbstractDataObject* createRunTimeSetting();
        /**
         * @brief 创建其他数据，返回值哈希中的int至少为FITKGolbalData.h/FITKGlobalDT::GDTOther
         * @return QHash< int,Core::FITKAbstractDataObject*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual QHash< int,Core::FITKAbstractDataObject*> createOtherData();


    };
}



#endif
