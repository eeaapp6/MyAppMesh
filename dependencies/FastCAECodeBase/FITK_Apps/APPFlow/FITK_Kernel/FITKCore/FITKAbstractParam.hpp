/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


/**
 *
 * @file FITKAbstractParam.hpp
 * @brief 抽象参数模板声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef __FITKABSTRACTPARAM_HPP__
#define __FITKABSTRACTPARAM_HPP__

#include "FITKAbstractNDataObject.h"
#include <QMetaEnum>
#include <cmath>

namespace Core
{
    /**
     * @brief 抽象参数对象
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKAbstractParamObj : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKAbstractParamObj() = default;
#ifdef Q_OS_WIN
        virtual ~FITKAbstractParamObj() = 0 {};
#else
        virtual ~FITKAbstractParamObj() {};
#endif
        /**
         * @brief 是否默认值
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual bool isDefault() { return false; }
        /**
         * @brief 变量转为字符串
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual QString value2String() { return QString(); }
    };

    template<typename T>
    /**
     * @brief 抽象参数模板
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKAbstractParamEmum : public Core::FITKAbstractParamObj
    {
    public:
        /**
         * @brief Destroy the FITKAbstractParam object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        explicit FITKAbstractParamEmum() { _metaEnum =QMetaEnum::fromType<T>(); }
        /**
         * @brief Destroy the FITKAbstractParam object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual ~FITKAbstractParamEmum() = default;
        /**
         * @brief 设置值
         * @param[i]  v              设置的当前值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        void setValue(const T v)
        {
            _value = v;
        }
        /**
         * @brief 获取当前值
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T getValue() const
        {
            return _value;
        }
        /**
         * @brief 设置默认值
         * @param[i]  dv        默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual void setDefault(const T dv)
        {
            _defaultValue = dv;
        }
        /**
         * @brief 获取默认值
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T getDefault() const
        {
            return _defaultValue;
        }
        /**
         * @brief 当前是否为默认值
         * @return true  当前是默认值
         * @return false 当前不是默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual bool  isDefault()  override
        {
            return _value == _defaultValue;
        }
        /**
         * @brief 转为字符串
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual QString value2String() override
        {
            if (_metaEnum.isValid())
                return _metaEnum.valueToKey(_value);
            else return QString();
        }


    protected:
        /**
         * @brief 当前值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T _value{};
        /**
         * @brief 默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T _defaultValue{};
        /**
         * @brief 元对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        QMetaEnum _metaEnum;
    };


    template<typename T>
    /**
     * @brief 抽象参数模板
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKAbstractParamValue : public Core::FITKAbstractParamObj
    {
    public:
        /**
         * @brief Destroy the FITKAbstractParam object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        explicit FITKAbstractParamValue() = default;
        /**
         * @brief Destroy the FITKAbstractParam object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual ~FITKAbstractParamValue() = default;
        /**
         * @brief 设置值
         * @param[i]  v              设置的当前值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        void setValue(const T v)
        {
            _value = v;
        }
        /**
         * @brief 获取当前值
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T getValue() const
        {
            return _value;
        }
        /**
         * @brief 设置默认值
         * @param[i]  dv        默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual void setDefault(const T dv)
        {
            _defaultValue = dv;
        }
        /**
         * @brief 获取默认值
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T getDefault() const
        {
            return _defaultValue;
        }

        /**
         * @brief 当前是否为默认值
         * @return true  当前是默认值
         * @return false 当前不是默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual bool  isDefault()  override
        {
            return  _defaultValue == _value;
        }
        /**
         * @brief 转为字符串
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual QString value2String() override
        {
            return QString("%1").arg(_value);
        }
        /**
         * @brief 设置有效值范围
         * @param[i] minValue, maxValue 最小值与最大值，数据范围是[minValue,maxValue]
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-28
         */
        virtual void setRange(const T& minValue, const T& maxValue)
        {
            _range[0] = minValue;
            _range[1] = maxValue;
        }
        /**
         * @brief 是否在设定的参数范围内
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-28
         */
        virtual bool isValid() const
        {
            return _value >= _range[0] && _value <= _range[1];
        }

    protected:
        /**
         * @brief 当前值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T _value{};
        /**
         * @brief 默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T _defaultValue{};
        /**
         * @brief 有效值范围[_range[0],_range[1]]
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T _range[2]{};
    };
    /**
     * @brief double类型参数
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKAbstractParamValueDouble : public FITKAbstractParamValue<double>
    {
    public:
        explicit FITKAbstractParamValueDouble() = default;
        virtual ~FITKAbstractParamValueDouble() = default;
        /**
         * @brief 当前是否为默认值
         * @return true  当前是默认值
         * @return false 当前不是默认值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        virtual bool  isDefault()  override
        {
            return fabs(_defaultValue - _value)< 1e-6;
        }
    protected:
    private:
    };
}

#endif
