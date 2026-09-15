/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsConstruction.h
 * @brief       构造线数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-12-31
 *********************************************************************/

#ifndef  __FITKABSCONSTRUCTION_H__
#define  __FITKABSCONSTRUCTION_H__

#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbsGeoCommand.h"

#include "FITKGeoEnum.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       构造线数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-31
     */
    class FITKInterfaceGeometryAPI FITKAbsConstruction : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsConstruction);

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        FITKAbsConstruction(Interface::FITKGeoEnum::FITKConstructionType type = Interface::FITKGeoEnum::FCTLine);

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        virtual ~FITKAbsConstruction() = default;

        /**
         * @brief       刷新。[虚函数][重写]
         * @return      是否刷新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-27
         */
        virtual bool update() override
        {
            return true;
        }

        /**
         * @brief       获取命令类型。[虚函数][重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-27
         */
        virtual Interface::FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
            
        /**
         * @brief       设置构造线类型。
         * @param[in]   type：构造线类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-27
         */
        void setConstructionType(Interface::FITKGeoEnum::FITKConstructionType type);

        /**
         * @brief       获取构造线类型。
         * @return      构造线类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        Interface::FITKGeoEnum::FITKConstructionType getConstructionType();

        /**
         * @brief       获取坐标。
         * @param[out]  point1：第一点坐标
         * @param[out]  point2：第二点坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        void getPositions(double* point1, double* point2);

        /**
         * @brief       设置坐标。
         * @param[in]   point1：第一点坐标
         * @param[in]   point2：第二点坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-02
         */
        void setPositions(double* point1, double* point2);

        /**
         * @brief       设置第一点坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        void setPosition1(double x, double y, double z);

        /**
         * @brief       设置第一点坐标。
         * @param[in]   point：第一点坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-02
         */
        void setPosition1(double* point);

        /**
         * @brief       设置第二点坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        void setPosition2(double x, double y, double z);

        /**
         * @brief       设置第二点坐标。
         * @param[in]   point：第二点坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-02
         */
        void setPosition2(double* point);

    protected:
        /**
         * @brief       构造线类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        Interface::FITKGeoEnum::FITKConstructionType m_type = Interface::FITKGeoEnum::FCTNone;

        /**
         * @brief       节点1坐标。（直线第一点坐标或圆心坐标）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        double m_point_1[3]{ 0., 0., 0. };

        /**
         * @brief       节点2坐标。（直线第二点坐标或圆上任意一点坐标）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        double m_point_2[3]{ 0., 0., 0. };

    };

    /**
     * @brief       构造线管理器。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-31
     */
    class FITKInterfaceGeometryAPI FITKConstructionManager : public Core::FITKAbstractDataManager<FITKAbsConstruction>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        explicit FITKConstructionManager() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        virtual ~FITKConstructionManager() = default;

        /**
         * @brief       尝试刷新数据并添加到子命令列表。（刷新失败不会添加到列表且不自动析构）[虚函数][重写]
         * @param[in]   item：构造线对象
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        virtual void appendDataObj(Core::FITKAbstractDataObject* item) override;

        /**
         * @brief       获取最后一个构造线。
         * @param[in]   type：构造线类型
         * @return      构造线对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        FITKAbsConstruction* getLastConstruction(Interface::FITKGeoEnum::FITKConstructionType type = 
            Interface::FITKGeoEnum::FITKConstructionType::FCTNone);

    };
}

#endif // !__FITKABSCONSTRUCTION_H__
