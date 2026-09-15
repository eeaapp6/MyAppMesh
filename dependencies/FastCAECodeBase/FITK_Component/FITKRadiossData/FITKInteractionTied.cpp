/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInteractionTied.h"

namespace Radioss
{
    FITKInteractionDefaultAbstract::DefaultInteractionType
        FITKInteractionKinematicTiedDefault::getDefaultInteractionType()
    {
        return FITKInteractionDefaultAbstract::DefaultInteractionType::DIT_KinematicTied;
    }

    void FITKInteractionKinematicTiedDefault::setIgnoreFlag(int flag)
    {
        m_ignoreFlag = flag;
    }

    int FITKInteractionKinematicTiedDefault::getIgnoreFlag() const
    {
        return m_ignoreFlag;
    }

    void FITKInteractionKinematicTiedDefault::setSpotWeldFormulationFlag(int flag)
    {
        m_spotWeldFormulationFlag = flag;
    }

    int FITKInteractionKinematicTiedDefault::getSpotWeldFormulationFlag() const
    {
        return m_spotWeldFormulationFlag;
    }

    void FITKInteractionKinematicTiedDefault::setSearchFormulationFlag(int flag)
    {
        m_searchFormulationFlag = flag;
    }

    int FITKInteractionKinematicTiedDefault::getSearchFormulationFlag() const
    {
        return m_searchFormulationFlag;
    }

    void FITKInteractionKinematicTiedDefault::setNodeDeletionFlag(int flag)
    {
        m_nodeDeletionFlag = flag;
    }

    int FITKInteractionKinematicTiedDefault::getNodeDeletionFlag() const
    {
        return m_nodeDeletionFlag;
    }

    void FITKInteractionKinematicTiedDefault::setIstfFlagDefault(int flag)
    {
        m_IstfFlag_Default = flag;
    }

    int FITKInteractionKinematicTiedDefault::getIstfFlagDefault() const
    {
        return m_IstfFlag_Default;
    }

/************************************************************/

    FITKInteractionKinematicTied::FITKInteractionKinematicTied()
    {
        _value = new FITKInteractionKinematicTiedDefault();
    }

    FITKInteractionKinematicTied::~FITKInteractionKinematicTied()
    {
        FITKInteractionKinematicTiedDefault* value = _value.getObjectPtr();
        if (value) delete value;
    }

    FITKAbstractInteraction::InteractionType FITKInteractionKinematicTied::getInteractionType()
    {
        return InteractionType::IT_KinematicTied;
    }

    FITKInteractionKinematicTiedDefault* FITKInteractionKinematicTied::getValue()
    {
        return _value.getObjectPtr();
    }

    TiedOptionalData & FITKInteractionKinematicTied::getOptionalData()
    {
        return _optionalData;
    }

    void FITKInteractionKinematicTied::setLevel(int level)
    {
        m_level = level;
    }

    int FITKInteractionKinematicTied::getLevel() const
    {
        return m_level;
    }

    void FITKInteractionKinematicTied::setSearchDistance(double dist)
    {
        m_searchDistance = dist;
    }

    double FITKInteractionKinematicTied::getSearchDistance() const
    {
        return m_searchDistance;
    }

} // namespace Radioss


