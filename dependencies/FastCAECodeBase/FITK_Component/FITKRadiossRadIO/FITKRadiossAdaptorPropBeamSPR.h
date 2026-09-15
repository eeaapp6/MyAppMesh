/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorPropBeamSPR.h
 * @brief  梁弹簧属性读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-18
 *********************************************************************/
#ifndef __FITKRadiossAdaptorPropBeamSPR_H__
#define __FITKRadiossAdaptorPropBeamSPR_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKPropBeamSPR;
    /**
     * @brief  梁弹簧属性读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorPropBeamSPR : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorPropBeamSPR() = default;
        ~FITKRadiossAdaptorPropBeamSPR() = default;
        /**
         * @brief
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取属性信息
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readPropBeam(FITKPropBeamSPR* beam);
        /**
         * @brief    读取参数
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void readParam(const QStringList& lines);
        /**
         * @brief    属性对象赋值
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void setPropData(FITKPropBeamSPR* beam);
    private:
        /**
         * @brief  属性参数
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QHash<int, double> _beamPar{};
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKPropBeamSPR, FITKRadiossAdaptorPropBeamSPR);
    RegRadiossRadAdaptor(/PROP/TYPE13, FITKRadiossAdaptorPropBeamSPR);
    RegRadiossRadAdaptor(/PROP/SPR_BEAM, FITKRadiossAdaptorPropBeamSPR, 0);

}
#endif //!__FITKRADIOSSADAPTORPROPSHELL_H__
