/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractInitialField.h"

namespace Radioss
{
     FITKAbstractInitialField::~FITKAbstractInitialField()
    {}
 

    FITKAbstractInitialField::InitialFieldType FITKAbstractInitialField::getType() const
    {
        return UNKNOWN;
    }

    void FITKAbstractInitialField::setValue(const double value[3])
    {
        m_value[0] = value[0];
        m_value[1] = value[1];
        m_value[2] = value[2];
    }

    double FITKAbstractInitialField::getValue(int index) const
    {
        return (index >= 0 && index < 3) ? m_value[index] : 0.0;
    }

    void FITKAbstractInitialField::setSetID(int setID)
    {
        m_setID = setID;
    }

    int FITKAbstractInitialField::getSetID() const
    {
        return m_setID;
    }

    void FITKAbstractInitialField::setSystemID(int systemID)
    {
        m_systemID = systemID;
    }

    int FITKAbstractInitialField::getSystemID() const
    {
        return m_systemID;
    }

}