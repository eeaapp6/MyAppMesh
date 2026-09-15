/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractRadMaterial.h
 * @brief  RAD材料基类
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef __FITKAbstractRADMaterial_H__
#define __FITKAbstractRADMaterial_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"

namespace Radioss
{
    /**
     * @brief   RAD材料基类
     * @author wangning (2185896382@qq.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataAPI FITKAbstractRadMaterial : public Interface::FITKAbstractMaterial,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum RadMatType
        {
            None = -1,
            LAW0,           //VOID
            LAW1,           //ELAST
            LAW2,           //PLAS_JOHNS
            LAW6,           //HYDRO or HYD_VISC    
            LAW27,          //PLAS_BRIT
            LAW36,          //PLAS_TAB
        };  Q_ENUM(RadMatType)
    public:
        explicit FITKAbstractRadMaterial() = default;
        virtual ~FITKAbstractRadMaterial() = default;

        virtual RadMatType getRadMatType();
        /**
         * @brief   设置失效模型ID
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void setFailID(int id);
        int getFailID();
        /**
        * @brief   设置失效模型启用状态
        * @return   bool
        * @author wangning (2185896382@qq.com)
        * @date   2025-09-15
        */
        void setFailState(bool state);
        bool isFailState();

    private:
        //材料类型
        RadMatType _radMatType = RadMatType::None;

        //失效模型ID
        int _failID = -1;

        //失效模型启用状态
        bool _isFail = false;
    };
}

#endif
