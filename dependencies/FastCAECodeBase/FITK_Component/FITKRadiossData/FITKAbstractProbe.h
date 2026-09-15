/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractProbe.h
 * @brief  探针数据基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-23
 *********************************************************************/
#ifndef __FITKABSTRACT_PROBE_H__
#define __FITKABSTRACT_PROBE_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
     /**
      * @brief  探针数据基类
      * @author liuzhonghua (liuzhonghuaszch@163.com)
      * @date   2025-09-23
      */
    class FITKRadiossDataAPI FITKAbstractProbe : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum ProbeType
        {
            Probe_Unknown = 0,
            Probe_Acceleration = 1,
            Probe_Section = 2,
        };
        Q_ENUM(ProbeType);

    public:
        explicit FITKAbstractProbe() =default;
        virtual ~FITKAbstractProbe() =0;
        /**
         * @brief    获取类型
         * @return   ProbeType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        virtual ProbeType getType() const =0;

    private:
    };
    /**
     * @brief  探针数据管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKProbeManager :
        public Core::FITKAbstractDataManager<FITKAbstractProbe>
    {
    public:
        explicit FITKProbeManager() = default;
        virtual ~FITKProbeManager() = default;
    };


}


#endif