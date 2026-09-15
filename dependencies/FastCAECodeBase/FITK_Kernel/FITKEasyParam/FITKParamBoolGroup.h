/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamBoolGroup.h
 * @brief  bool组参数类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef FITKParamBoolGroup_H
#define FITKParamBoolGroup_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKParameter;
    /**
     * @brief  bool组参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamBoolGroup : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamBoolGroup();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamBoolGroup();
        /**
         * @brief    获取参数类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType  参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝数据
         * @param[i] data     被拷贝数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual void copyParaData(FITKAbstractEasyParam* data) override;
        /**
         * @brief    设置数据
         * @param[i] value   数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValue(bool value);
        /**
         * @brief    获取数据
         * @return   bool    数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        bool getValue();
        /**
         * @brief    设置数据组（子数据）
         * 只有在bool为true时，数据组有效
         * @param[i] g           数据组
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValueGroup(FITKParameter* g);
        /**
         * @brief    获取数据组（子数据）
         * 当bool为false时，数据组为空
         * @return   FITKParameter*    数据组
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParameter* getValueGroup();
    private:
        /**
         * @brief  数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        bool _value = false;
        /**
         * @brief  数据组
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        FITKParameter* _valueGroup = nullptr;
    };
}

#endif
