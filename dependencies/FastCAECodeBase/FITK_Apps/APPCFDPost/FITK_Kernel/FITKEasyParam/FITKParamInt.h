/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamInt.h
 * @brief  整型参数类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef FITKParamInt_H
#define FITKParamInt_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    /**
     * @brief  整型参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamInt : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    整型参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamInt();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamInt();
        /**
         * @brief    获取数据对象类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝数据对象
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
        void setValue(int value);
        /**
         * @brief    获取数据
         * @return   int     数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        int getValue();
        /**
         * @brief    设置范围
         * @param[i] range    数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setRange(int* range);
        /**
         * @brief    获取范围
         * @param[i] range     数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void getRange(int* range);
    private:
        /**
         * @brief  数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        int _value = 0;
        /**
         * @brief  数据范围
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        int _range[2] = { -99999999,99999999 };
    };
}

#endif
