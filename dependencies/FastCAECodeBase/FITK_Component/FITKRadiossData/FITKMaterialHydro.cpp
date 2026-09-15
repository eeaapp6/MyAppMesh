/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMaterialHydro.h"

namespace Radioss
{
   
    const FITKMaterialHydroData& FITKMaterialHydro::getMaterialDataReadOnly() const
    {
        return m_data;
    }

    FITKMaterialHydroData& FITKMaterialHydro::getMaterialData()
    {
        return m_data;
    }

    void FITKMaterialHydro::setEOSID(int id)
    {
        _eosID = id;
    }

    int FITKMaterialHydro::getEOSID()
    {
        return _eosID;
    }
}
