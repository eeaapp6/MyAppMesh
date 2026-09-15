/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorFailJOHNSON.h
 * @brief  失效模型JOHNSON读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef _FITKRadiossAdaptorFailJOHNSON_H_
#define _FITKRadiossAdaptorFailJOHNSON_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

    namespace Radioss
{
    class FITKFailureModelJOHNSON;
    /**
     * @brief  失效模型JOHNSON读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-11-10
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorFailJOHNSON : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorFailJOHNSON() = default;
        ~FITKRadiossAdaptorFailJOHNSON() = default;
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
        * @brief    读取数据
        * @param[i] fail
        * @return   bool
        * @author wangning (2185896382@qq.com)
        * @date   2025-10-24
        */
        bool readFailure(FITKFailureModelJOHNSON* fail);
        /**
         * @brief    读取参数
         * @param[i] lines
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void readParam(const QStringList& lines);
        /**
         * @brief    分割参数
         * @param[i] line
         * @param[i] par
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void splitParam(const QString& line, const QList<QPair<QString, int>>& par);
        /**
         * @brief
         * @param[i] Failure
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void setFailureData(FITKFailureModelJOHNSON* fail);

    private:
        /**
         * @brief  失效参数名称及数值
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        QHash<QString, double> _failParam{};
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKFailureModelJOHNSON, FITKRadiossAdaptorFailJOHNSON);
    RegRadiossRadAdaptor(/FAIL/JOHNSON, FITKRadiossAdaptorFailJOHNSON);
}
#endif 
