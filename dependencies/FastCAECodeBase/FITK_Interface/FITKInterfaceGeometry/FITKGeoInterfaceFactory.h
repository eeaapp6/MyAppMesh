/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGeoInterfaceFactory.h
 * @brief  几何接口注册工厂
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-12
 *
 */
#ifndef _FITK_GEOMETRY_INTERFACE_FACTORY_H_
#define _FITK_GEOMETRY_INTERFACE_FACTORY_H_

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKAbsVirtualTopo.h"
#include <QMutex>
#include <QObject>
#include <QHash>
#include <functional>

namespace Interface
{
    class FITKAbsVirtualTopoCreator;
    class FITKAbstractGeomToolsCreator;
    /**
     * @brief 几何命令创建函数指针类型声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-12
     */
    typedef std::function<FITKAbsGeoCommand*(void)> GeoCommandFuns;

    /**
     * @brief       基准元素构造函数指针。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    typedef std::function<FITKAbsGeoDatum*(void)> GeoDatumFuns;

    class FITKInterfaceGeometryFactoryDeleator;
    /**
     * @brief 几何接口创建工厂
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-12
     */
    class FITKInterfaceGeometryAPI FITKInterfaceGeometryFactory : public QObject
    {
        friend FITKInterfaceGeometryFactoryDeleator;
        //声明为单例
        DeclSingleton(FITKInterfaceGeometryFactory);
    public:
        /**
         * @brief 插入几何命令创建函数，重复插入会覆盖
         * @param[i]  t              命令类型
         * @param[i]  f              函数指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void insertCommandCreateFun(FITKGeoEnum::FITKGeometryComType t, GeoCommandFuns f);

        /**
         * @brief       注册基准元素与构造函数指针。
         * @param[in]   t：基准元素类型
         * @param[in]   f：函数指针
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        void insertDatumCreateFun(FITKGeoEnum::FITKDatumType t, GeoDatumFuns f);

        /**
         * @brief 创建几何命令
         * @param[i]  t              命令类型
         * @return FITKAbsGeoCommand* 命令对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbsGeoCommand* createCommand(FITKGeoEnum::FITKGeometryComType t);
        /**
         * @brief 清空全部的接口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
        void clearAllInterface();

        /**
         * @brief       检查是否支持该类型建模功能。
         * @param[in]   type：命令类型
         * @return      是否支持
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        bool getCommandSupported(FITKGeoEnum::FITKGeometryComType type);

        /**
         * @brief       检查是否支持该类型基准元素创建功能。
         * @param[in]   type：基准元素类型
         * @return      是否支持
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        bool getDatumSupported(FITKGeoEnum::FITKDatumType type);

        template <class T>
        /**
         * @brief 创建几何命令
         * @param[i]  t            命令类型
         * @return T*  命令对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        T* createCommandT(FITKGeoEnum::FITKGeometryComType t)
        {
            FITKAbsGeoCommand* c = this->createCommand(t);
            T* ct = dynamic_cast<T*>(c);
            if (!ct && c)
            {
                delete c;
            }

            return ct;
        }

        /**
         * @brief       创建基准元素。
         * @param[in]   t：基准元素类型
         * @return      基准元素对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKAbsGeoDatum* createDatum(FITKGeoEnum::FITKDatumType t);

        /**
         * @brief       创建指定类型基准元素。
         * @param[in]   t：基准元素类型
         * @param[in]  editable：是否可编辑[缺省]
         * @return      基准元素对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        template <class T>
        T* createDatumT(FITKGeoEnum::FITKDatumType t, bool editable = true)
        {
            FITKAbsGeoDatum* datum = this->createDatum(t);
            if (!datum)
            {
                return nullptr;
            }

            T* datumT = dynamic_cast<T*>(datum);
            if (!datumT)
            {
                delete datum;
                return nullptr;
            }

            datumT->setEditable(editable);
            return datumT;
        }

        /**
         * @brief 设置工具生成器
         * @param[i]  c              生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        void setGeomToolsCreator(FITKAbstractGeomToolsCreator* c);
        /**
         * @brief 获取工具生成器
         * @return FITKAbstractGeomToolsCreator* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        FITKAbstractGeomToolsCreator* getGeomToolsCreator();


    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void initialize();
        /**
         * @brief 完成，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void finalize();
    private:
        /**
         * @brief 函数指针存储哈希表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        QHash<FITKGeoEnum::FITKGeometryComType, GeoCommandFuns> _interfaceFuns{};

        /**
         * @brief       基准元素构造函数指针字典。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        QHash<FITKGeoEnum::FITKDatumType, GeoDatumFuns> _interfaceDatumFuns{};
        
        /**
         * @brief 工具生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        FITKAbstractGeomToolsCreator* _geomToolsCreator{};

    };
    class FITKInterfaceGeometryFactoryDeleator
    {
    public:
        explicit FITKInterfaceGeometryFactoryDeleator() = default;
        virtual ~FITKInterfaceGeometryFactoryDeleator();
    };

}



#endif

