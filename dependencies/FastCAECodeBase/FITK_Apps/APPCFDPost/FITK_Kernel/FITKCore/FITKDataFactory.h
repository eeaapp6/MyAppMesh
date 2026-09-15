/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


/**
 *
 * @file FITKDataFactory.h
 * @brief 数据创建工程
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKDATAFACTORY_H__
#define _FITKDATAFACTORY_H__

#include "FITKAbstractFactory.h"
#include "FITKCoreMacros.h"
#include <QMutex>
#include <QHash>
#include <functional>

namespace Core
{
    class FITKAbstractDataObject;
    class FITKDataFactoryDeleator;
    /**
     * @brief 数据创建工厂
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKCoreAPI FITKDataFactory : public FITKAbstractFactory
    {
        friend FITKDataFactoryDeleator;
            
        //声明为单例
        DeclSingleton(FITKDataFactory);

    public:
        /**
         * @brief 创建数据
         * @param[i]  key            数据注册的键值
         * @return FITKAbstractObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        FITKAbstractObject* create(const QString& key) override;

    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void finalize();
    private:
        /**
         * @brief 数据类型以及创建该类型数据的函数指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-07-24
         */
        QHash< QString, std::function< FITKAbstractDataObject*() > > _itemDataFuns{ };


    public:
        template < typename T >
        /**
         * @brief 注册辅助类
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        class FITKDataFacRegHelper
        {
        public:
            FITKDataFacRegHelper(QString type)
            {
                auto ins = FITKDataFactory::getInstance();
                // 注册构造函数，T类型必须有默认的无参构造函数
                if (!((ins->_itemDataFuns).contains(type)))
                    (ins->_itemDataFuns).insert(type, [&] { return new T; });
            }
        };


    };

    class FITKDataFactoryDeleator
    {
    public:
        explicit FITKDataFactoryDeleator() = default;
        virtual ~FITKDataFactoryDeleator();
    };

}


/**
 * @brief 声明注册宏
 */

#ifndef Register2FITKDataFactory
#define Register2FITKDataFactory( key, T, ... )  \
 static Core::FITKDataFactory::FITKDataFacRegHelper< T > \
       _FITKDataRegHelper_##T##_##__VA_ARGS__( #key );
#endif

#endif
