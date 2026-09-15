/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMaterialPlasJohns.h"

namespace Radioss
{


    void FITKMaterialPlasJohns::setType( FITKMaterialPlasJohns::MaterialPlasJohnsType type)
    {
        // 设置Johnson-Cook材料模型的类型
        m_type = type;
    }

    FITKMaterialPlasJohns::MaterialPlasJohnsType FITKMaterialPlasJohns::getType() const
    {
        return m_type;
    }
    const FITKMaterialPlasJohnsData &FITKMaterialPlasJohns::getMaterialDataReadOnly() const
    {
        return m_data;
    }
    FITKMaterialPlasJohnsData &FITKMaterialPlasJohns::getMaterialData()
    {
        return m_data;
    }
}
