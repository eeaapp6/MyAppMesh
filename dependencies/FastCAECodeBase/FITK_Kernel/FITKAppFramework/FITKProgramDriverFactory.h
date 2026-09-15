/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKProgramDriverFactory.h
 * @brief 第三方程序驱动工厂
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-06-09
 *
 */
#ifndef _FITK_PROGRAMDRIVER_FACTORY_H___
#define _FITK_PROGRAMDRIVER_FACTORY_H___

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include <QMutex>
#include <QStringList>

namespace AppFrame
{
    class FITKAbstractProgramerDriver;

    /**
 * @brief 驱动器创建函数
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-10
 */
#ifndef DRIVERTypeFuns
#define DRIVERTypeFuns   QHash<QString,std::function<FITKAbstractProgramerDriver*()> >
#endif

    class FITKProgramDriverFactoryDeleator;

    /**
     * @brief 第三方程序驱动工厂声明
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-06-09
     */
    class FITKAppFrameworkAPI FITKProgramDriverFactory
        : public Core::FITKAbstractObject
    {
        friend FITKProgramDriverFactoryDeleator;
        //声明为单例
        DeclSingleton(FITKProgramDriverFactory);

    public:
        /**
         * @brief 获取注册到工厂的驱动器类型
         * @return QList<int>
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        QList<int> getProgramTypes();
         /**
          * @brief 根据类型获取驱动器名称列表
          * @param[i]  driverType     驱动器类型
          * @return QStringList
          * @author libaojun (libaojunqd@Foxmail.com)
          * @date 2024-06-15
          */
        QStringList getProgramDrivers(const int driverType);
        /**
         * @brief 获取驱动器创建函数指针
         * @param[i]  t              类型
         * @param[i]  driver         名称
         * @return std::function<FITKAbstractProgramerDriver*()>
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        std::function<FITKAbstractProgramerDriver*()> getDriverCreateFun(const int t, const QString driver);

    private:
        /**
         * @brief 初始化
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
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
         * @brief 创建驱动器的函数指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-10
         */
        QHash<int, DRIVERTypeFuns> _createFuns{};

    public:
        template < typename T >
        /**
         * @brief 注册器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-10
         */
        class FITKDriverRegHelper
        {
        public:
            FITKDriverRegHelper(int type, QString driverName)
            {
                auto ins = FITKProgramDriverFactory::getInstance();
                // 注册构造函数，T类型必须有默认的无参构造函数
                ins->_createFuns[type][driverName] = ([&] { return new T; });
            }
        };
    };

    class FITKProgramDriverFactoryDeleator
    {
    public:
        explicit FITKProgramDriverFactoryDeleator() = default;
        virtual ~FITKProgramDriverFactoryDeleator();
    };
}


/**
 * @brief 声明注册宏，类型(int,抽象定义)，驱动器名称
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-10
 */
#ifndef Register2FITKProgramDriverFactory
#define Register2FITKProgramDriverFactory( Type,DriverName,T, ...)          \
 static AppFrame::FITKProgramDriverFactory::FITKDriverRegHelper< T >   \
                _FITKDriverRegHelper_##T##_##DriverName##__VA_ARGS__(Type, #DriverName);
#endif

#ifndef FITKPROGRAMDRIVERFACTORY
#define FITKPROGRAMDRIVERFACTORY ( AppFrame::FITKProgramDriverFactory::getInstance() )
#endif

#endif

