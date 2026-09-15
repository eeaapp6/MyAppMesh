/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorMaterialPlasBrit.h
 * @brief  脆性失效材料读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-24
 *********************************************************************/
#ifndef _FITKRadiossAdaptorMaterialPlasBrit_H_
#define _FITKRadiossAdaptorMaterialPlasBrit_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialPlasBrit;
    /**
     * @brief  脆性失效材料读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-10-24
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorMaterialPlasBrit : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorMaterialPlasBrit() = default;
        ~FITKRadiossAdaptorMaterialPlasBrit() = default;
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
        * @brief    读取材料数据
        * @param[i] mat
        * @return   bool
        * @author wangning (2185896382@qq.com)
        * @date   2025-10-24
        */
        bool readMaterial(FITKMaterialPlasBrit* mat);
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
         * @param[i] mat
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        void setMatData(FITKMaterialPlasBrit* mat);

    private:
        /**
         * @brief  材料参数名称及数值
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-24
         */
        QHash<QString, double> _materialParam{};
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKMaterialPlasBrit, FITKRadiossAdaptorMaterialPlasBrit);
    RegRadiossRadAdaptor(/MAT/LAW27, FITKRadiossAdaptorMaterialPlasBrit);
    RegRadiossRadAdaptor(/MAT/PLAS_BRIT, FITKRadiossAdaptorMaterialPlasBrit, 1);
}
#endif // !_FITKRADIOSSADAPTORMATERIALELASTIC_H_