/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamBool.h
 * @brief  Bool类型类型参数
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef FITKParamBool_H
#define FITKParamBool_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    /**
     * @brief  Bool类型参数
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamBool : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamBool();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamBool();
        /**
         * @brief    获取参数类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType   参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝函数
         * @param[i] data    被拷贝数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual void copyParaData(FITKAbstractEasyParam* data) override;
        /**
         * @brief    设置数据
         * @param[i] value    数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValue(bool value);
        /**
         * @brief    获取数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        bool getValue();
    private:
        /**
         * @brief  数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        bool _value = false;
    };
}

#endif
