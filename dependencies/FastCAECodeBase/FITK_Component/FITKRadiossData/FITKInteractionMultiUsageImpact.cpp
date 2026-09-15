/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInteractionMultiUsageImpact.h"

namespace Radioss
{
    FITKInteractionDefaultAbstract::DefaultInteractionType
        FITKInteractionMultiUsageImpactDefaultValue::getDefaultInteractionType()
    {
        return FITKInteractionDefaultAbstract::DefaultInteractionType::DIT_MultiUsageImpact;
    }

    FITKInteractionMultiUsageImpact::FITKInteractionMultiUsageImpact()
    {
        _defaultValue = new FITKInteractionMultiUsageImpactDefaultValue();
    }

    FITKInteractionMultiUsageImpact::~FITKInteractionMultiUsageImpact()
    {
        FITKInteractionMultiUsageImpactDefaultValue * obj = _defaultValue.getObjectPtr();
        if (obj)
        {
            delete obj;
            obj = nullptr;
        }
    }

    FITKAbstractInteraction::InteractionType FITKInteractionMultiUsageImpact::getInteractionType()
    {
        return IT_MultiUsageImpact;
    }
    FITKInteractionMultiUsageImpactDefaultValue *FITKInteractionMultiUsageImpact::getValues()
    {
        return _defaultValue.getObjectPtr();
    }
    MultiUsageImpactOptionalData& FITKInteractionMultiUsageImpact::getOptionalData()
    {
        return _optionalData;
    }      
} // namespace Radioss
