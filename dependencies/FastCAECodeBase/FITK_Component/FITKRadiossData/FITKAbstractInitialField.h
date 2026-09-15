/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITKABSTRACT_INI_FIELD_H__
#define __FITKABSTRACT_INI_FIELD_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
     /**
     * @class  FITKInitialValue
     * @brief  初始值数据对象
     * @author libaojun
     * @date   2025-08-20
     */
    class FITKRadiossDataAPI FITKAbstractInitialField : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum InitialFieldType
        {
            UNKNOWN = 0,
            TRA,         ///<! Translational material velocity
            ROT,         ///<! Rotational material velocity
            TG,          ///<! Translation and grid material velocity (only for ALE Materials)
            GRID,        ///<! Grid material velocity, only for ALE Materials
            AXIS,        ///<! Initialize both translational and rotational velocities on a group of nodes in a given coordinate system
        };
        Q_ENUM(InitialFieldType);

    public:
        explicit FITKAbstractInitialField() =default;
        virtual ~FITKAbstractInitialField() =0;
 
        virtual InitialFieldType getType() const =0;
        /**
         * @brief 设置初始值
         * @param[in] value 初始值数组，长度为3
         */
        void setValue(const double value[3]);
        /**
         * @brief 获取初始值
         * @return const double* 初始值数组，长度为3
         */
         double getValue(int index) const;
        /**
         * @brief 设置集合ID
         * @param[in] setID 集合ID
         */
        void setSetID(int setID);
        /**
         * @brief 获取集合ID
         * @return int 集合ID
         */
        int getSetID() const;
        /**
         * @brief 设置局部坐标系ID
         * @param[in] systemID 局部坐标系ID
         */
        void setSystemID(int systemID);
        /**
         * @brief 获取局部坐标系ID
         * @return int 局部坐标系ID
         */
        int getSystemID() const;

    private:
 
        /**
         * @brief 初始值数组 Vx Vy Vz
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-20
         */
        double m_value[3]{0.0,0.0,0.0}; 
        /**
         * @brief 集合ID
         * @author LiBaojun 
         */
        int m_setID{-1}; 
        /**
         * @brief 局部坐标系ID
         * @author LiBaojun 
         */
        int m_systemID{-1}; 
    };

    class FITKRadiossDataAPI FITKInitialFieldManager : 
        public Core::FITKAbstractDataManager<FITKAbstractInitialField>
    {
    public:
        explicit FITKInitialFieldManager() = default;
        virtual ~FITKInitialFieldManager() = default;
    };


}


#endif