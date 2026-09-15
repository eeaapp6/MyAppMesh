/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorRWallCYL.h
 * @brief  圆柱刚性壁读写适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-22
 *********************************************************************/
#ifndef __FITKRadiossAdaptorRWallCYL_H__
#define __FITKRadiossAdaptorRWallCYL_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRWallCylinder;
    /**
     * @brief  圆柱刚性壁读写适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-22
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorRWallCYL : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorRWallCYL() = default;
        ~FITKRadiossAdaptorRWallCYL() = default;
        /**
         * @brief
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取
         * @param[i] cyl
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        bool readRWallCYL(FITKRWallCylinder* cyl);
        /**
         * @brief    读取参数
         * @param[i] lines
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        void readParam(const QStringList& lines);
        /**
         * @brief    对象赋值
         * @param[i] plane
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        void setData(FITKRWallCylinder* plane);

    private:
        /**
         * @brief  属性参数
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-22
         */
        QHash<int, double> _param{};
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-22
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRWallCylinder, FITKRadiossAdaptorRWallCYL);
    RegRadiossRadAdaptor(/RWALL/CYL, FITKRadiossAdaptorRWallCYL);
}
#endif //!__FITKRADIOSSADAPTORWALLPLANE_H__
