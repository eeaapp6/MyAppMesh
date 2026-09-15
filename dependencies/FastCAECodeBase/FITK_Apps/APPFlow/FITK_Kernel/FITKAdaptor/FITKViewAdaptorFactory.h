/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKViewAdaptorFactory.h
 * @brief 数据适配器工厂
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 *
 */
#ifndef __FITK_VIEW_FACTORY_H___
#define __FITK_VIEW_FACTORY_H___

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
    class FITKAbstractViewAdaptor;

/**
 * @brief  类对象名称-适配器创建函数
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 */
#ifndef VIEWTypeFuns
#define VIEWTypeFuns   QHash<QString,std::function<FITKAbstractViewAdaptor*()> >
#endif

    class FITKViewAdaptorFactoryDeleator;
    /**
     * @brief       可视化数据适配器工厂。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-03-25
     */
    class FITKAdaptorAPI FITKViewAdaptorFactory : public Core::FITKAbstractObject
    {
        friend FITKViewAdaptorFactoryDeleator;
        DeclSingleton(FITKViewAdaptorFactory);

    public:
        /**
         * @brief 创建适配器
         * @param[i]  fileType       数据类型
         * @param[i]  className      数据类名称
         * @return FITKAbstractViewAdaptor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        FITKAbstractViewAdaptor* create(const QString& graphObjType, const QString & className );
        /**
         * @brief 创建适配器
         * @param[i]  fileType       数据类型
         * @param[i]  obj            数据类对象
         * @return FITKAbstractViewAdaptor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        FITKAbstractViewAdaptor* create(const QString& graphObjType, Core::FITKAbstractObject* obj);

        template<class T>
        /**
         * @brief 创建适配器并转换类型
         * @param[i]  graphObjType       数据类型
         * @param[i]  className      数据类名称
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        T* createT(const QString& graphObjType, const QString & className)
        {
            return dynamic_cast<T*>(create(graphObjType, className));
        }

        template<class T>
        /**
         * @brief 创建适配器并转换类型
         * @param[i]  fileType     数据类型
         * @param[i]  obj          数据类对象
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        T* createT(const QString& graphObjType, Core::FITKAbstractObject* obj)
        {
            return dynamic_cast<T*>(create(graphObjType, obj));
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
        QHash<QString, VIEWTypeFuns> _createFuns{};


    public:
        template < typename T >
        /**
         * @brief 注册器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        class FITKViewAdaFacRegHelper
        {
        public:
            FITKViewAdaFacRegHelper(QString file, QString className)
            {
                auto ins = FITKViewAdaptorFactory::getInstance();
                // 注册构造函数，T类型必须有默认的无参构造函数
                ins->_createFuns[file][className] = ([&] { return new T; });
            }
        };
    };


    class FITKViewAdaptorFactoryDeleator
    {
    public:
        explicit FITKViewAdaptorFactoryDeleator() = default;
        virtual ~FITKViewAdaptorFactoryDeleator();
    };
}   // namespace Adaptor


/**
 * @brief 声明注册宏，数据类型参数，数据类名称（带命名空间），Adaptor类名
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 */
#ifndef Register2FITKViewAdaptorFactory
#define Register2FITKViewAdaptorFactory( File,DataClass, T, ...)          \
 static Adaptor::FITKViewAdaptorFactory::FITKViewAdaFacRegHelper< T >   \
                _FITKDataViewRegHelper_##T##_##File##_##__VA_ARGS__( #File,#DataClass );
#endif


#ifndef FITKVIEWADAPTORFACTORY
#define FITKVIEWADAPTORFACTORY ( Adaptor::FITKViewAdaptorFactory::getInstance() )
#endif

#endif
