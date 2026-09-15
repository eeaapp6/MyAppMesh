/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKGeoCommandProp.h
 * @brief 几何命令属性类
 * @author libaojun
 * @date 2025-04-08
 */
#ifndef _FITK_GEO_COMMAND_PROP_H___
#define _FITK_GEO_COMMAND_PROP_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include <QColor>

namespace Interface
{
    /**
     * @brief 几何命令属性类
     * @author Assistant 
     * @date 2025-04-08
     */
    class FITKInterfaceGeometryAPI FITKGeoCommandProp //: public Core::FITKAbstractDataObject
    { 
    public:
        /**
         * @brief 构造函数
         */
        explicit FITKGeoCommandProp();

        /**
         * @brief 析构函数
         */
        ~FITKGeoCommandProp() ;

        /**
         * @brief 获取颜色属性
         * @return QColor 颜色属性
         * @author libaojun
         * @date 2025-04-08
         */
        QColor getColor() const ;

        /**
         * @brief 设置颜色属性
         * @param color 颜色属性
         * @author libaojun
         * @date 2025-04-08
         */
        void setColor(const QColor& color);

        /**
         * @brief 获取64位数字属性
         * @author libaojun
         * @date 2025-04-08
         * @return uint64_t 64位数字属性
         */
        uint64_t getNumber() const;

        /**
         * @brief 设置64位数字属性
         * @author libaojun
         * @date 2025-04-08
         * @param number 64位数字属性
         */
        void setNumber(uint64_t number);
        
        /**
         * @brief 获取默认值
         * @author libaojun
         * @date 2025-04-08
         * @return uint64_t 默认值
         */
        static uint64_t GetDefault() ;

        /**
         * @brief 设置默认值
         * @author libaojun
         * @date 2025-04-08
         * @param defaultValue 默认值
         */
        static void SetDefault(uint64_t defaultValue) ;

        /**
         * @brief 位与运算函数
         * @author libaojun
         * @date 2025-04-08
         * @param other 另一个64位数字
         * @return bool 位与运算结果
         */
        bool bitwiseAnd(uint64_t other) const;

        /**
         * @brief 位或运算函数
         * @author libaojun
         * @date 2025-04-08
         * @param other 另一个64位数字
         */
        void bitwiseOr(uint64_t other);

private:
        /**
         * @brief 静态计数变量
         */
        static unsigned int _InstanceCount;
        /**
         * @brief 默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-08
         */
        static uint64_t  _Default;
         /**
          * @brief 颜色属性
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2025-04-08
          */
        QColor m_color;  
        /**
         * @brief 64位数字属性
         * @details 该属性用于存储64位数字值，通常用于标识或状态表示。
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-08
         */
        uint64_t m_number{0};  
    };
}

#endif