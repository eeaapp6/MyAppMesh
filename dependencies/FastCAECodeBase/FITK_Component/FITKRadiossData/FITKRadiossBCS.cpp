/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossBCS.h"

namespace Radioss
{
    FITKAbstractBCS::BCSType FITKRadiossBCS::getBCSType() const
    {
        // 在派生类中重写以提供具体类型
        return FITKAbstractBCS::BCSType::BSSCyclic;
    }
    void FITKRadiossBCS::setFreedomFixed(int index, bool fixed)
    {
        if (index < 1 || index > 6) return; // 索引范围检查
        _freedomFixed[index - 1] = fixed; // 设置自由度固定状态
    }

    bool FITKRadiossBCS::isFreedomFixed(int index) const
    {
        if (index < 1 || index > 6) return false; // 索引范围检查
        return _freedomFixed[index - 1]; // 获取自由度固定状态
    }


    
    FITKAbstractBCS::BCSType FITKRadiossBCSCyclic::getBCSType() const
    {
        // 在派生类中重写以提供具体类型
        return FITKAbstractBCS::BCSType::BSSCyclic;
    }



    FITKAbstractBCS::BCSType FITKRadiossBCSLagMul::getBCSType() const
    {
        // 在派生类中重写以提供具体类型
        return FITKAbstractBCS::BCSType::BCSLAGMUL;
    }


} // namespace Radioss
