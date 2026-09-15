/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamCombox.h
 * @brief  ComBox类型参数
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef FITKParamCombox_H
#define FITKParamCombox_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    /**
     * @brief  ComBox类型参数
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamCombox : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamCombox();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamCombox();
        /**
         * @brief    获取数据类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType  数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝函数 
         * @param[i] data      原数据对象
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
        void setValue(QStringList value);
        /**
         * @brief    获取全部数据
         * @return   QStringList     全部数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        QStringList getValue();
        /**
         * @brief    获取当前数据
         * @return   QString        当前数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        QString getCurrentValue();
        /**
         * @brief    设置当前索引
         * @param[i] index        当前索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setIndex(int index);
        /**
         * @brief    获取当前索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        int getIndex();
    private:
        /**
         * @brief  数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        QStringList _value = {};
        /**
         * @brief  索引
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        int _index = -1;
    };
}

#endif
