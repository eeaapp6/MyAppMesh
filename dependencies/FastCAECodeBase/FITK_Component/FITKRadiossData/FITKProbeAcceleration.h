/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKProbeAcceleration.h
 * @brief  探针 加速度计
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-23
 *********************************************************************/
#ifndef __FITKPROBEACCELERATION_H__
#define __FITKPROBEACCELERATION_H__

#include "FITKAbstractProbe.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
     /**
     * @class  FITKProbeAcceleration
     * @brief  加速度计对象
     * @author liuzhonghua
     * @date   2025-08-20
     */
    class FITKRadiossDataAPI FITKProbeAcceleration : public FITKAbstractProbe
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKProbeAcceleration)
        RadiossKeyWord(FITKProbeAcceleration, /ACCEL)
    public:
        explicit FITKProbeAcceleration() =default;
        virtual ~FITKProbeAcceleration();
 
        virtual ProbeType getType() const override;
        /**
         * @brief    设置坐标系ID
         * @param[i] sysID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        void setSystemID(int sysID);
        /**
         * @brief    获取坐标系ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        int getSystemID();
        /**
         * @brief    设置加速度计单点集合ID(不是节点ID，需要取出集合中节点的ID)
         * @param[i] nodeID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        void setSetID(int nodeID);
        /**
         * @brief    获取加速度计节点ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        int getNodeID();
        /**
        * @brief    获取单点集合ID
        * @return   int
        * @author   liuzhonghua (liuzhonghuaszch@163.com)
        * @date     2025-09-23
        */
        int getSetID();
        /**
         * @brief    设置截止频率
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        void setCutOffFreq(double freq);
        /**
         * @brief    获取截止频率
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        double getCutOffFreq();

    private:
 
        /**
         * @brief 截止频率
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-20
         */
        double m_frequencyCut{ 1.65 };
        /**
         * @brief 集合ID
         * @author LiBaojun 
         */
        int m_SetID{-1}; 
        /**
         * @brief 局部坐标系ID
         * @author LiBaojun 
         */
        int m_systemID{-1}; 
    };

}


#endif