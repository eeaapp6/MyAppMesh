/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorInitialField.h
 * @brief  初速度读写适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-08-28
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORINITIALFIELDTRA_H__
#define __FITKRADIOSSADAPTORINITIALFIELDTRA_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKInitialFieldTra;
    /**
     * @brief  初速度读写适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-08-27
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorInitialFieldTra : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorInitialFieldTra() = default;
        ~FITKRadiossAdaptorInitialFieldTra() = default;
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
         * @brief    读取数据
         * @param[i] gravity 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-27
         */
        bool readInivel(FITKInitialFieldTra* inivel);
        /**
         * @brief    读取参数
         * @param[i] lines 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-27
         */
        void readParam(const QStringList& lines, FITKInitialFieldTra* inivel);
    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKInitialFieldTra, FITKRadiossAdaptorInitialFieldTra);
    RegRadiossRadAdaptor(/INIVEL/TRA, FITKRadiossAdaptorInitialFieldTra);
}
#endif //!__FITKRADIOSSADAPTORINITIALFIELDTRA_H__
