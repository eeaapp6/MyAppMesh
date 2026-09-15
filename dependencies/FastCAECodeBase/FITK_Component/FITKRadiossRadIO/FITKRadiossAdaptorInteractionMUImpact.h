/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorInteractionMUImpact.h
 * @brief  多用途冲击读写适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-25
 *********************************************************************/
#ifndef _FITKRadiossAdaptorInteractionMCImpact_H_
#define _FITKRadiossAdaptorInteractionMCImpact_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKInteractionMultiUsageImpact;
    /**
     * @brief  多用途冲击读写适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-23
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorInteractionMUImpact : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorInteractionMUImpact() = default;
        ~FITKRadiossAdaptorInteractionMUImpact() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取Interaction
         * @param[i] Interaction
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readInteraction(FITKInteractionMultiUsageImpact* MUImpact);
        /**
         * @brief    读取参数
         * @param[i] text
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void readParam(const QStringList & lines);
        /**
         * @brief    分割参数
         * @param[i] text
         * @param[i] par
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void splitParam(const QString & line, const QList<QPair<QString, double>>& par);
        /**
         * @brief
         * @param[i] mat
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void setInteractionData(FITKInteractionMultiUsageImpact* MUImpact);

    private:
        /**
         * @brief  参数名称及数值
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QHash<QString, double> _MUIParam{};
    };
    /**
     * @brief 注册适配器
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKInteractionMultiUsageImpact, FITKRadiossAdaptorInteractionMUImpact);
    RegRadiossRadAdaptor(/INTER/TYPE7, FITKRadiossAdaptorInteractionMUImpact);
}
#endif // !_FITKRADIOSSADAPTORMATERIALVOID_H_
