/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_NON_LINEAR_CURVE_H__
#define __FITK_NON_LINEAR_CURVE_H__

#include "FITKRadiossDataAPI.h"
#include "FITKAbstractRadiossData.h"
#include <QList>
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{

    /**
     * @brief 非线性曲线类
     * @details 此类用于描述Radioss中的非线性曲线数据，通常用于材料模型的应力-应变关系。
     * @see FITKAbstractRadiossData
     * @author LiBaojun
     * @date 2025-04-28
     */
    class FITKRadiossDataAPI FITKNonLinerCurve : public Core::FITKAbstractNDataObject,  public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKNonLinerCurve); //!< FITK类标识
        RadiossKeyWord(FITKNonLinerCurve, /FUNCT) //!< Radioss关键字标识

    public:
        struct FITKNonLinerCurvePoint
        {
            double x{0}; //!< x坐标
            double y{0}; //!< y坐标
        };
        
    public:
        explicit FITKNonLinerCurve() = default;   
        virtual ~FITKNonLinerCurve() = default;
        
        /**
         * @brief 获取所有曲线点
         * @return 返回曲线点列表
         * @author libaojun
         * @date 2025-04-25
         */
        QList<FITKNonLinerCurvePoint> getPoints() const;

        /**
         * @brief 添加曲线点
         * @param point 要添加的点
         * @author libaojun
         * @date 2025-04-25
         */
        void addPoint(const FITKNonLinerCurvePoint& point);

        /**
         * @brief 删除指定索引的曲线点
         * @param index 点的索引
         * @return 是否删除成功
         * @author libaojun
         * @date 2025-04-25
         */
        bool removePoint(int index);

        /**
         * @brief 修改指定索引的曲线点
         * @param index 点的索引
         * @param point 新的点数据
         * @return 是否修改成功
         * @author libaojun
         * @date 2025-04-25
         */
        bool updatePoint(int index, const FITKNonLinerCurvePoint& point);

        /**
         * @brief 获取指定索引的曲线点
         * @param index 点的索引
         * @return 曲线点数据
         * @author libaojun
         * @date 2025-04-25
         */
        FITKNonLinerCurvePoint getPoint(int index) const;

        /**
         * @brief 获取曲线点数量
         * @return 点的数量
         * @author libaojun
         * @date 2025-04-25
         */
        int getPointCount() const;
        /**
         * @brief 移除全部曲线点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-21
         */
        void removeAllPoints();

    private:
        QList<FITKNonLinerCurvePoint> m_points{}; //!< 曲线点列表
    };

    /**
     * @brief 非线性曲线数据管理器类
     * @details 此类用于管理非线性曲线数据对象的生命周期和访问。
     * @see FITKAbstractDataManager
     * @author LiBaojun
     * @date 2025-04-28
     */
    class FITKNonLinerCurveManager : public Core::FITKAbstractDataManager<FITKNonLinerCurve>
    {
        //FITKSafeDownCastA(FITKNonLinerCurveManager, FITKAbstractDataManager<FITKNonLinerCurve>);
        public:
            explicit FITKNonLinerCurveManager() = default;
            virtual ~FITKNonLinerCurveManager() = default;
    };
    
}

#endif