/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorRWall.h
 * @brief  刚性壁读写适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-08-27
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORWALLPLANE_H__
#define __FITKRADIOSSADAPTORWALLPLANE_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRWallPlane;
    /**
     * @brief  刚性壁读写适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-08-27
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorRWallPlane : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorRWallPlane() = default;
        ~FITKRadiossAdaptorRWallPlane() = default;
        /**
         * @brief
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptW() override;
    private:
        /**
         * @brief    读取无限平面
         * @param[i] plane 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-27
         */
        bool readRWallPlane(FITKRWallPlane* plane);
        /**
         * @brief    读取参数
         * @param[i] lines 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-27
         */
        void readParam(const QStringList& lines);
        /**
         * @brief    对象赋值
         * @param[i] plane 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-27
         */
        void setData(FITKRWallPlane* plane);

    private:
        /**
         * @brief  属性参数
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-15
         */
        QHash<int, double> _param{};
    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRWallPlane, FITKRadiossAdaptorRWallPlane);
    RegRadiossRadAdaptor(/RWALL/PLANE, FITKRadiossAdaptorRWallPlane);
}
#endif //!__FITKRADIOSSADAPTORWALLPLANE_H__
