/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeoCommandProp.h"
#include "FITK_Kernel/FITKCore/FITKColorMap.hpp"

namespace Interface
{

    // 静态变量初始化
    unsigned int FITKGeoCommandProp::_InstanceCount = 0;
    uint64_t FITKGeoCommandProp::_Default = 0; // 默认值初始化为0

    FITKGeoCommandProp::FITKGeoCommandProp()
        : m_number(_Default) // 初始化64位数字为0
    {
        // 设置默认颜色属性
        m_color = Core::GetcAColorFromColorMap(_InstanceCount);
        _InstanceCount++;
    }

    FITKGeoCommandProp::~FITKGeoCommandProp()
    {
    }

    void FITKGeoCommandProp::setColor(const QColor& color)
    {
        m_color = color;
    }
    QColor FITKGeoCommandProp::getColor() const
    {
        return m_color;
    }

    /**
     * @brief 获取64位数字属性
     * @author libaojun
     * @date 2025-04-08
     */
    uint64_t FITKGeoCommandProp::getNumber() const
    {
        return m_number;
    }

    /**
     * @brief 设置64位数字属性
     * @author libaojun
     * @date 2025-04-08
     */
    void FITKGeoCommandProp::setNumber(uint64_t number)
    {
        m_number = number;
    }

    void FITKGeoCommandProp::SetDefault(uint64_t defaultValue)
    {
        _Default = defaultValue;
    }
    uint64_t FITKGeoCommandProp::GetDefault() 
    {
        return _Default;
    }

    /**
     * @brief 位与运算函数
     * @author libaojun
     * @date 2025-04-08
     */
    bool FITKGeoCommandProp::bitwiseAnd(uint64_t other) const
    {
        return (m_number & other) != 0;
    }

    /**
     * @brief 位或运算函数
     * @author libaojun
     * @date 2025-04-08
     */
    void FITKGeoCommandProp::bitwiseOr(uint64_t other)
    {
        m_number |= other;
    }
}