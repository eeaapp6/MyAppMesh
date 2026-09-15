/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKIOAdaptorFactory.h
 * @brief 数据适配器工厂
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 *
 */
#ifndef __FITK_IO_FACTORY_H___
#define __FITK_IO_FACTORY_H___

#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITKAdaptorAPI.h"
#include <QMutex>
#include <QHash>
#include <functional>

namespace Core
{
    class FITKAbstractObject;
}

namespace Adaptor
{
    class FITKAbstractIOAdaptor;

/**
 * @brief 相同文件类型适配器创建函数
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 */
#ifndef FILETypeFuns
#define FILETypeFuns   QHash<QString,std::function<FITKAbstractIOAdaptor*()> >
#endif

    class FITKIOAdaptorFactoryDeleator;

    class FITKAdaptorAPI FITKIOAdaptorFactory : public Core::FITKAbstractObject
    {
        friend FITKIOAdaptorFactoryDeleator;
        DeclSingleton(FITKIOAdaptorFactory);

    public:
        /**
         * @brief 创建适配器
         * @param[i]  fileType       文件类型
         * @param[i]  className      数据类名称
         * @return FITKAbstractIOAdaptor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        FITKAbstractIOAdaptor* create(const QString& fileType, const QString & className );
        /**
         * @brief 创建适配器
         * @param[i]  fileType       文件类型
         * @param[i]  obj            数据类对象
         * @return FITKAbstractIOAdaptor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        FITKAbstractIOAdaptor* create(const QString& fileType, Core::FITKAbstractObject* obj);

        template<class T>
        /**
         * @brief 创建适配器并转换类型
         * @param[i]  fileType       文件类型
         * @param[i]  className      数据类名称
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        T* createT(const QString& fileType, const QString & className)
        {
            return dynamic_cast<T*>(create(fileType, className));
        }

        template<class T>
        /**
         * @brief 创建适配器并转换类型
         * @param[i]  fileType     文件类型
         * @param[i]  obj          数据了类对象
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        T* createT(const QString& fileType, Core::FITKAbstractObject* obj)
        {
            return dynamic_cast<T*>(create(fileType, obj));
        }

    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
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
         * @brief 创建Adaptor的函数指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        QHash<QString, FILETypeFuns> _createFuns{};


    public:
        template < typename T >
        /**
         * @brief 注册器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        class FITKIOAdaFacRegHelper
        {
        public:
            FITKIOAdaFacRegHelper(QString file, QString className)
            {
                auto ins = FITKIOAdaptorFactory::getInstance();
                // 注册构造函数，T类型必须有默认的无参构造函数
                ins->_createFuns[file][className] = ([&] { return new T; });
            }
        };
    };

    class FITKIOAdaptorFactoryDeleator
    {
    public:
        explicit FITKIOAdaptorFactoryDeleator() = default;
        virtual ~FITKIOAdaptorFactoryDeleator();
    };
}


/**
 * @brief 声明注册宏，文件参数，数据类名称（带命名空间），Adaptor类名
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 */
#ifndef Register2FITKIOAdaptorFactory
#define Register2FITKIOAdaptorFactory( File,DataClass, T, ...)          \
 static Adaptor::FITKIOAdaptorFactory::FITKIOAdaFacRegHelper< T >   \
                _FITKDataIORegHelper_##T##_##File_##__VA_ARGS__(#File, #DataClass);
#endif


#ifndef FITKIOADAPTORFACTORY
#define FITKIOADAPTORFACTORY ( Adaptor::FITKIOAdaptorFactory::getInstance() )
#endif

#endif
