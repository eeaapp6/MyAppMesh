/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorSurfaceSeg.h
 * @brief  Seg面集合 (/SURF/SEG) 读写适配器声明
 * @author  wangning
 * @date   2025-09-12
 * @details
 *********************************************************************/
#ifndef _FITKRADIOSS_ADAPTOR_SURFACE_SEG_H__
#define _FITKRADIOSS_ADAPTOR_SURFACE_SEG_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"


namespace Radioss
{
    class FITKRadiossSurfaceSeg;
    /**
     * @class  FITKRadiossAdaptorSurfaceSeg
     * @brief  节点集合适配器
     * @author  wangning
     * @date   2025-09-12
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorSurfaceSeg : public FITKRadiossAbstractAdaptor
    {
    public:
        /** @brief 默认构造 (无额外初始化) */
        explicit FITKRadiossAdaptorSurfaceSeg() = default;
        /** @brief 默认析构 (无资源需释放) */
        virtual ~FITKRadiossAdaptorSurfaceSeg() = default;

        /**
         * @brief  获取适配器类别标识
         * @return 适配器类别名称
         * @author  libaojun
         * @date   2025-08-20
         */
        QString getAdaptorClass() override;

        /**
         * @brief  从 Radioss 输入读取 /GRNOD/NODE 数据
         * @return true 读取并解析成功; false 读取/语义校验失败
         * @author  libaojun
         * @date   2025-08-20
         */
        bool adaptR() override;

        /**
         * @brief  将内部节点集合写出为 /GRNOD/NODE 关键字块
         * @return true 写出成功; false 数据不完整或输出失败
         * @author  libaojun
         * @date   2025-08-20
         */
        bool adaptW() override;

    };


    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossSurfaceSeg, FITKRadiossAdaptorSurfaceSeg);
    /**
     * @brief  注册 Radioss 关键字与适配器映射 (/SURF/SEG)
     */
    RegRadiossRadAdaptor(/SURF/SEG, FITKRadiossAdaptorSurfaceSeg);
}


#endif ///< _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__