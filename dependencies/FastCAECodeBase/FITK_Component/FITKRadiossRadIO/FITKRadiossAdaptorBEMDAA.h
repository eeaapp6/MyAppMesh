/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorBEMDAA.h
 * @brief  BEM双重渐近近似（DAA）读取适配器
 * @author liuzhonghua
 * @date   2025-04-27
 *********************************************************************/
#ifndef _FITKRADIOSSADAPTORBEMDAA_H_
#define _FITKRADIOSSADAPTORBEMDAA_H_

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKBEMDoubleAsymptoticApproximation;

    class FITKRadiossRadIOAPI FITKRadiossAdaptorBEMDAA : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorBEMDAA() = default;
        ~FITKRadiossAdaptorBEMDAA() = default;

        QString getAdaptorClass() override;
        bool adaptR() override;
        bool adaptW() override;

    private:
        bool readParam(const QStringList& lines, FITKBEMDoubleAsymptoticApproximation* daa);
    };

    Register2FITKIOAdaptorFactory(rad, Radioss::FITKBEMDoubleAsymptoticApproximation, FITKRadiossAdaptorBEMDAA);
    RegRadiossRadAdaptor(/BEM/DAA, FITKRadiossAdaptorBEMDAA);
}

#endif // !_FITKRADIOSSADAPTORBEMDAA_H_
