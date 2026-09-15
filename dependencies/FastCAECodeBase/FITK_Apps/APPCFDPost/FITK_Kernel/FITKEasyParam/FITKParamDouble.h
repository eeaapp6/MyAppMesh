/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamDouble.h
 * @brief  双精度参数类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef FITKParamDouble_H
#define FITKParamDouble_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    /**
     * @brief  双精度参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamDouble : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamDouble();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamDouble();
        /**
         * @brief    获取参数类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝函数
         * @param[i] data      被拷贝数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual void copyParaData(FITKAbstractEasyParam* data) override;
        /**
         * @brief    设置数据
         * @param[i] value      数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValue(double value);
        /**
         * @brief    获取数据
         * @return   double    数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        double getValue();
        /**
         * @brief    设置数据范围
         * @param[i] range     数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setRange(double* range);
        /**
         * @brief    获取数据范围
         * @param[i] range    数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void getRange(double* range);
    private:
        /**
         * @brief  数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        double _value = 0.0;
        /**
         * @brief  数据范围
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        double _range[2] = { -99999999,99999999 };
    };
}

#endif
