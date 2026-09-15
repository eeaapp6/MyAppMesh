/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInitialField.h"

namespace Radioss
{
    FITKAbstractInitialField::InitialFieldType FITKInitialFieldTra::getType() const
    {
        return FITKAbstractInitialField::InitialFieldType::TRA;
    }

    FITKAbstractInitialField::InitialFieldType FITKInitialFieldAxis::getType() const
    {
        //获取初始化场类型
        return FITKAbstractInitialField::InitialFieldType::AXIS;
    }

    void FITKInitialFieldAxis::setDir(QString text)
    {
        //Set Direction X, Y, and Z
        m_dir = text;
    }

    QString FITKInitialFieldAxis::getDir()
    {
        //Get Direction X, Y, and Z
        return m_dir;
    }

    void FITKInitialFieldAxis::setFrameID(int id)
    {
        //Set Frame id
        m_frame_ID = id;
    }

    int FITKInitialFieldAxis::getFrameID()
    {
        //Get Frame id
        return m_frame_ID;
    }

    void FITKInitialFieldAxis::setVr(double value)
    {
        //Set Rotational velocity about the rotational axis
        m_rotationalVelocity = value;
    }

    double FITKInitialFieldAxis::getVr()
    {
        //Get Rotational velocity about the rotational axis
        return m_rotationalVelocity;
    }

}
