/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKGravity.cpp
 * @brief 重力场数据对象实现。
 * @author libaojun
 * @date 2025-08-25
 */
#include "FITKGravity.h"

namespace Radioss
{
    int FITKGravity::getTimeFunctionID() const 
    { 
        return _timeFunctionID; 
    }
    
    void FITKGravity::setTimeFunctionID(int id) 
    { 
        _timeFunctionID = id; 
    }
    
    FITKGravity::GravityDirection FITKGravity::getGravityDirection() const 
    { 
        return _gravityDirection; 
    }
    
    void FITKGravity::setGravityDirection(GravityDirection direction) 
    { 
        _gravityDirection = direction; 
    }
    
    int FITKGravity::getSystemID() const 
    { 
        return _coordinateSystemID; 
    }
    
    void FITKGravity::setSystemID(int id) 
    { 
        _coordinateSystemID = id; 
    }
    
    int FITKGravity::getSensorID() const 
    { 
        return _sensorID; 
    }
    
    void FITKGravity::setSensorID(int id) 
    { 
        _sensorID = id; 
    }
    
    int FITKGravity::getNodeGroupID() const 
    { 
        return _nodeGroupID; 
    }
    
    void FITKGravity::setNodeGroupID(int id) 
    { 
        _nodeGroupID = id; 
    }
    
    double FITKGravity::getAScaleX() const 
    { 
        return _AScaleX; 
    }
    
    void FITKGravity::setAScaleX(double scale) 
    { 
        _AScaleX = scale; 
    }
    
    double FITKGravity::getFScaleY() const 
    { 
        return _FScaleY; 
    }
    
    void FITKGravity::setFScaleY(double scale) 
    { 
        _FScaleY = scale; 
    }
}
