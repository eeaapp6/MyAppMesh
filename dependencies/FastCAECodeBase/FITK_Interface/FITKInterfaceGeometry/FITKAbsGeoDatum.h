/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoDatum.h
 * @brief       基准元素抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef  FITKABSGEODATUM_H
#define  FITKABSGEODATUM_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

#include "FITKGeoEnum.h"
#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       基准元素抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoDatum : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoDatum);
    public:
        /**
         * @brief       基准元素创建方式枚举。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-31
         */
        enum DatumCreatedType
        {
            DCT_System = 0,
            DCT_UserDefine
        };

        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKAbsGeoDatum() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual ~FITKAbsGeoDatum() = default;

        /**
         * @brief       设置是否可编辑。
         * @param[in]   flag：是否可编辑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-11
         */
        void setEditable(bool flag);

        /**
         * @brief       设置基准元素位置。
         * @param[in]   pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void setPosition(double* pos);

        /**
         * @brief       获取基准元素位置。
         * @param[out]  pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void getPosition(double* pos);

        /**
         * @brief       获取基准元素类型。[纯虚函数]
         * @return      基准元素类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual FITKGeoEnum::FITKDatumType getDatumType() = 0;

        /**
         * @brief       获取几何命令类型。[虚函数][重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-10
         */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       获取基准元素可编辑性。
         * @return      是否可编辑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-31
         */
        bool editable();

        /**
         * @brief       更新数据对象。[虚函数]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        virtual bool update() { return true; };

        /**
         * @brief  打印信息到控制台.
         * @param  type 信息类型 1-normal 2-warning 3-error 4-info
         * @param  msg 信息内容
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        void printLog(QString msg, int type = 1)
        {
            AppFrame::FITKSignalTransfer* sigTrans = FITKAPP->getSignalTransfer();
            if (sigTrans)
            {
                sigTrans->outputMessageSig(type, msg);
            }
        }

    protected:
        /**
         * @brief       位置坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        double m_pos[3]{ 0., 0., 0. };

        /**
         * @brief       基准元素创建方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-31
         */
        DatumCreatedType m_createdType = DCT_UserDefine;

        // 指定数据管理器为友元类。
        friend class FITKDatumList;

    };

    /**
     * @brief       基准点类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoDatumPoint : public FITKAbsGeoDatum
    {
        FITKCLASS(Interface, FITKAbsGeoDatumPoint);

        friend class FITKPrincipalDatumList;

        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKAbsGeoDatumPoint() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual ~FITKAbsGeoDatumPoint() = default;

        /**
         * @brief       获取基准元素类型。[重写]
         * @return      基准元素类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

    };

    /**
     * @brief       基准线类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoDatumLine : public FITKAbsGeoDatum
    {
        friend class FITKPrincipalDatumList;

        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKAbsGeoDatumLine() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual ~FITKAbsGeoDatumLine() = default;

        /**
         * @brief       获取基准元素类型。[虚函数][重写]
         * @return      基准元素类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        /**
         * @brief       设置有限基准线信息。
         * @param[in]   pos1：位置1
         * @param[in]   pos2：位置2
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
        void setFiniteLine(double* pos1, double* pos2);

        /**
         * @brief       获取无限基准线信息。
         * @param[out]  pos1：位置1
         * @param[out]  pos2：位置2
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
        void getFiniteLine(double* pos1, double* pos2);

        /**
         * @brief       设置有限基准线信息。
         * @param[in]   pos：中心
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
        void setInfiniteLine(double* pos, double* dir);

        /**
         * @brief       获取无限基准线信息。
         * @param[out]  pos：中心
         * @param[out]  dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
        void getInfiniteLine(double* pos, double* dir);

        /**
         * @brief       设置基准线结束坐标。
         * @param[in]   pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        void setPosition2(double* pos);

        /**
         * @brief       获取基准线结束坐标。
         * @param[out]   pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        void getPosition2(double* pos);

        /**
         * @brief       设置基准线方向。
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
         //void setDirection(double* dir);

         /**
          * @brief       获取基准线方向。
          * @param[out]  dir：方向
          * @author      ChengHaotian (yeguangbaozi@foxmail.com)
          * @date        2024-10-16
          */
        void getDirection(double* dir);

        /**
         * @brief       设置基准线是否为无限范围。
         * @param[in]   flag：是否无限
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-11
         */
        void setIsInfinite(bool flag);

        /**
         * @brief       获取基准线结束坐标。
         * @return      是否为无限基准线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-11
         */
        bool getIsInfinite();

        /**
         * @brief       获取基准线类型。
         * @return      基准线类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKGeoEnum::DatumAxisType getAxisType();

    private:
        /**
         * @brief       系统内部方法 - 设置基准线类型。
         * @param[in]   type：基准线类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        void setAxisType(FITKGeoEnum::DatumAxisType type);

    protected:
        /**
         * @brief       基准线结束坐标。（有限基准线使用）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        double m_pos2[3]{ 0., 0., 1. };

        /**
         * @brief       基准线方向。（无限基准线使用）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-16
         */
        double m_dir[3]{ 0., 0., 0. };

        /**
         * @brief       是否无限。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-11
         */
        bool m_isInfinite = false;

        /**
         * @brief       基准线类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKGeoEnum::DatumAxisType m_axisType = FITKGeoEnum::DatumAxisType::DAT_UserDefine;

    };

    /**
     * @brief       基准平面类。(默认XOY平面)
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoDatumPlane : public FITKAbsGeoDatum
    {
        friend class FITKPrincipalDatumList;

        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKAbsGeoDatumPlane() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual ~FITKAbsGeoDatumPlane();

        /**
         * @brief       获取基准元素类型。[重写]
         * @return      基准元素类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        /**
         * @brief       设置基准面信息。
         * @param[in]   pos：面中心点坐标
         * @param[in]   normal：面法向
         * @param[in]   up：面向上方向
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void setPlane(double* pos, double* normal, double* up);

        /**
         * @brief       获取基准面信息。
         * @param[out]  pos：面中心点坐标
         * @param[out]  normal：面法向
         * @param[out]  up：面向上方向
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void getPlane(double* pos, double* normal, double* up);

        /**
         * @brief       设置基准面法向。
         * @param[in]   nor：法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void setNormal(double* nor);

        /**
         * @brief       获取基准面法向。
         * @param[out]   nor：法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void getNormal(double* nor);

        /**
         * @brief       设置基准面向上方向。
         * @param[in]   up：向上方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void setUp(double* up);

        /**
         * @brief       获取基准面向上方向。
         * @param[out]  up：向上方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        void getUp(double* up);

        /**
         * @brief       获取基准面类型。
         * @return      基准面类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKGeoEnum::DatumPlnType getPlaneType();

    private:
        /**
         * @brief       系统内部方法 - 设置基准面类型。
         * @param[in]   type：基准面类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        void setPlaneType(FITKGeoEnum::DatumPlnType type);

    protected:
        /**
         * @brief       基准面法向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        double m_nor[3]{ 0., 0., 1. };

        /**
         * @brief       基准面向上方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        double m_up[3]{ 0., 1., 0. };

        /**
         * @brief       基准面类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKGeoEnum::DatumPlnType m_plnType = FITKGeoEnum::DatumPlnType::DPT_UserDefine;

    };

    /**
     * @brief       系统默认基准元素管理器类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-05-23
     */
    class FITKInterfaceGeometryAPI FITKPrincipalDatumList :
        public Core::FITKAbstractDataManager<Interface::FITKAbsGeoDatum>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        explicit FITKPrincipalDatumList() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        virtual ~FITKPrincipalDatumList() = default;

        /**
         * @brief       获取系统默认基准面。
         * @param[in]   type：基准面类型
         * @return      基准面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKAbsGeoDatumPlane* getPrincipalPlane(FITKGeoEnum::DatumPlnType type);

        /**
         * @brief       获取系统默认基准轴。
         * @param[in]   type：基准轴类型
         * @return      基准轴
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKAbsGeoDatumLine* getPrincipalAxis(FITKGeoEnum::DatumAxisType type);

    };

    /**
     * @brief       基准元素管理器类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKInterfaceGeometryAPI FITKDatumList :
        public Core::FITKAbstractDataManager<FITKAbsGeoDatum>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        explicit FITKDatumList();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        virtual ~FITKDatumList();

        /**
         * @brief       获取系统默认基准面。
         * @param[in]   type：基准面类型
         * @return      基准面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-17
         */
        FITKAbsGeoDatumPlane* getPrincipalPlane(FITKGeoEnum::DatumPlnType type);

        /**
         * @brief       获取系统默认基准轴。
         * @param[in]   type：基准轴类型
         * @return      基准轴
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-17
         */
        FITKAbsGeoDatumLine* getPrincipalAxis(FITKGeoEnum::DatumAxisType type);

        /**
         * @brief       重新根据ID查询数据逻辑，增加系统默认基准元素判断。[虚函数][重写]
         * @param[in]   id：数据ID
         * @return      基准元素
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-17
         */
        virtual Interface::FITKAbsGeoDatum* getDataByID(const int id) override;

    protected:
        /**
         * @brief       系统默认基准元素管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        FITKPrincipalDatumList* m_principalDatumMgr{ nullptr };

    };
}

#endif // !FITKABSGEODATUM_H
