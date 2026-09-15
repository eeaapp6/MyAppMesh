/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractInteraction.h"

namespace Radioss
{
    FITKInteractionDefaultAbstract::DefaultInteractionType
        FITKInteractionDefaultAbstract::getDefaultInteractionType()
    {
        return FITKInteractionDefaultAbstract::DefaultInteractionType::DIT_None;
    }

    FITKAbstractInteraction::~FITKAbstractInteraction()
    {
    }

    FITKAbstractInteraction::InteractionType 
        FITKAbstractInteraction::getInteractionType()
    {
        return FITKAbstractInteraction::InteractionType::IT_None;
    }
    void FITKAbstractInteraction::setMasterSurfaceID(int id)
    {
        m_masterSurfaceID = id;
    }

    int FITKAbstractInteraction::getMasterSurfaceID() const
    {
        return m_masterSurfaceID;
    }

    void FITKAbstractInteraction::setNodeGroupID(int id)
    {
        m_nodeGroupID = id;
    }

    int FITKAbstractInteraction::getNodeGroupID() const
    {
        return m_nodeGroupID;
    }

} // namespace Radioss

 