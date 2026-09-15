/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorProbeSection.h
 * @brief  截面探针读写适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-06
 *********************************************************************/
#ifndef __FITKRadiossAdaptorProbeSection_H__
#define __FITKRadiossAdaptorProbeSection_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKProbeSection;
    /**
     * @brief  截面探针读写适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-11-06
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorProbeSection : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorProbeSection() = default;
        ~FITKRadiossAdaptorProbeSection() = default;
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
        * @brief    读取数据
        * @param[i] mat
        * @return   bool
        * @author wangning (2185896382@qq.com)
        * @date   2025-10-24
        */
        bool readSection(FITKProbeSection* data);
        /**
         * @brief    读取参数
         * @param[i] text
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void readParam(const QStringList& lines, FITKProbeSection* data);
        /**
         * @brief    分割参数
         * @param[i] text
         * @param[i] par
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void splitParam(const QString& line, const QList<QPair<QString, int>>& par);
        /**
         * @brief
         * @param[i] mat
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void setSectionData(FITKProbeSection* data);

    private:
        /**
         * @brief  截面参数名称及数值
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        QHash<QString, double> _sectionParam{};
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKProbeSection, FITKRadiossAdaptorProbeSection);
    RegRadiossRadAdaptor(/SECT, FITKRadiossAdaptorProbeSection);
}
#endif //!__FITKRADIOSSADAPTORPROPSHELLVALUE_H__

