/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaPartAdaptor.h
 * @brief  LS-DYNA Part数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaPartAdaptor_H_
#define _FITKRadiossLSDynaPartAdaptor_H_

#include "FITKRadiossLSDynaAbstractAdaptor.h"
#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossPart;
    /**
     * @brief  LS-DYNA Part数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaPartAdaptor : public FITKRadiossLSDynaAbstractAdaptor
    {
    public:
        explicit FITKRadiossLSDynaPartAdaptor();
        virtual ~FITKRadiossLSDynaPartAdaptor() = default;

        QString getAdaptorClass() override;
        /**
         * @brief    读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        bool adaptR() override;
        /**
         * @brief    写出
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        bool adaptW() override;

    private:
        bool readPartBlock();
        bool writePartBlock();

    private:
        FITKRadiossPart* m_partData = nullptr;
    };
    Register2FITKIOAdaptorFactory(k, Radioss::FITKRadiossPart, FITKRadiossLSDynaPartAdaptor)
    RegLSDynaAdaptor(*PART, FITKRadiossLSDynaPartAdaptor)
}

#endif