/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorPropShellSandwich.h
 * @brief  夹层壳属性读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-28
 *********************************************************************/
#ifndef _FITKRadiossAdaptorPropShellSandwich_H_
#define _FITKRadiossAdaptorPropShellSandwich_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKPropSandwichShell;
    /**
     * @brief  夹层壳属性读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-10-28
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorPropShellSandwich : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorPropShellSandwich() = default;
        ~FITKRadiossAdaptorPropShellSandwich() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        bool adaptW() override;

    private:
        /**
        * @brief    读取属性数据
        * @param[i] prop
        * @return   bool
        * @author wangning (2185896382@qq.com)
        * @date   2025-10-24
        */
        bool readProp(FITKPropSandwichShell* prop);
        /**
         * @brief    读取参数
         * @param[i] text
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void readParam(const QStringList & lines);
        /**
         * @brief    分割参数
         * @param[i] text
         * @param[i] par
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void splitParam(const QString & line, const QList<QPair<QString, int>>& par);
        /**
         * @brief
         * @param[i] prop
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void setPropData(FITKPropSandwichShell* prop);

    private:
        /**
         * @brief  属性参数名称及数值
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        QHash<QString, double> _propParam{};
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKPropSandwichShell, FITKRadiossAdaptorPropShellSandwich);
    RegRadiossRadAdaptor(/PROP/TYPE11, FITKRadiossAdaptorPropShellSandwich);
    RegRadiossRadAdaptor(/PROP/SH_SANDW, FITKRadiossAdaptorPropShellSandwich, 1);
}
#endif // !_FITKRADIOSSADAPTORMATERIALELASTIC_H_