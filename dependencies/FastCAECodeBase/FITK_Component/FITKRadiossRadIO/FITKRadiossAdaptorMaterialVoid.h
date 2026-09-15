/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorMaterialElastic.h
 * @brief  空材料读取适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-11
 *********************************************************************/
#ifndef _FITKRADIOSSADAPTORMATERIALVOID_H_
#define _FITKRADIOSSADAPTORMATERIALVOID_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialVoid;
    /**
     * @brief  空材料读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-11
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorMaterialVoid : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorMaterialVoid() = default;
        ~FITKRadiossAdaptorMaterialVoid() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        bool adaptW() override;
    private:
        /**
         * @brief    读取材料
         * @param[i] mat 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        bool readMaterial(FITKMaterialVoid* mat);
        /**
         * @brief    读取参数
         * @param[i] lines 
         * @param[i] mat 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        bool readParam(const QStringList & lines, FITKMaterialVoid* mat);
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKMaterialVoid, FITKRadiossAdaptorMaterialVoid);
    RegRadiossRadAdaptor(/MAT/LAW0, FITKRadiossAdaptorMaterialVoid);
    RegRadiossRadAdaptor(/MAT/VOID, FITKRadiossAdaptorMaterialVoid,1);
}
#endif // !_FITKRADIOSSADAPTORMATERIALVOID_H_