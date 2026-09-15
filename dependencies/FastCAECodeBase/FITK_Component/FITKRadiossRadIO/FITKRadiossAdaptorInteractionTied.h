/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorInteractionTied.h
 * @brief  绑定读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-15
 *********************************************************************/
#ifndef _FITKRadiossAdaptorInteractionTied_H_
#define _FITKRadiossAdaptorInteractionTied_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKInteractionKinematicTied;
    /**
     * @brief  绑定读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorInteractionTied : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorInteractionTied() = default;
        ~FITKRadiossAdaptorInteractionTied() = default;
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
        bool readInteraction(FITKInteractionKinematicTied* tie);
        /**
         * @brief    读取参数
         * @param[i] lines
         * @param[i] tie
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void readParam(const QStringList & lines, FITKInteractionKinematicTied* tie);
    };
    /**
     * @brief 注册适配器
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKInteractionKinematicTied, FITKRadiossAdaptorInteractionTied);
    RegRadiossRadAdaptor(/INTER/TYPE2, FITKRadiossAdaptorInteractionTied);
}
#endif // !_FITKRADIOSSADAPTORMATERIALVOID_H_
