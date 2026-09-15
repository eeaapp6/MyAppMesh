/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorSurfaceOther.h
 * @brief  各种类型面集合 读写适配器声明
 * @author  wangning
 * @date   2025-09-25
 * @details
 *********************************************************************/
#ifndef _FITKRADIOSS_ADAPTOR_SURFACE_OTHER_H__
#define _FITKRADIOSS_ADAPTOR_SURFACE_OTHER_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"


namespace Radioss
{
    class FITKRadiossSurfaceSeg;
    /**
     * @class  FITKRadiossAdaptorSurfaceOther
     * @brief  Part面集合适配器
     * @author  wangning
     * @date   2025-09-25
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorSurfaceOther : public FITKRadiossAbstractAdaptor
    {
    public:
        /** @brief 默认构造 (无额外初始化) */
        explicit FITKRadiossAdaptorSurfaceOther() = default;
        /** @brief 默认析构 (无资源需释放) */
        virtual ~FITKRadiossAdaptorSurfaceOther() = default;

        QString getAdaptorClass() override;

        bool adaptR() override;

    };

    /**
     * @brief  注册 Radioss 关键字与适配器映射 
     */
    RegRadiossRadAdaptor(/SURF/PART, FITKRadiossAdaptorSurfaceOther);
    RegRadiossRadAdaptor(/SURF/GRSHEL, FITKRadiossAdaptorSurfaceOther,1);
    RegRadiossRadAdaptor(/SURF/GRSH3N, FITKRadiossAdaptorSurfaceOther,2);
    RegRadiossRadAdaptor(/SURF/SURF, FITKRadiossAdaptorSurfaceOther,3);
}


#endif ///< _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__