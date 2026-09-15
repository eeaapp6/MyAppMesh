/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamComboxVector.h
 * @brief  combox组参数类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef _FITKParamComboxVector_H
#define _FITKParamComboxVector_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKParamCombox;
    /**
     * @brief  combox组参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamComboxVector : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamComboxVector();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamComboxVector();
        /**
         * @brief    获取数据对象类型
         * @return   FITKAbstractEasyParam::FITKEasyParamType    数据对象类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    拷贝数据
         * @param[i] data        被拷贝数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual void copyParaData(FITKAbstractEasyParam* data) override;
        /**
         * @brief    获取数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        int getCount();
        /**
         * @brief    设置数据
         * @param[i] values       数据链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setDatas(QList<FITKParamCombox*> values);
        /**
         * @brief    追加数据
         * @param[i] value       数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void pushData(FITKParamCombox* value);
        /**
         * @brief    获取数据
         * @param[i] index       数据索引
         * @return   FITKParamDouble*  数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamCombox* getData(int index);
        /**
         * @brief    获取数据链表
         * @return   QList<FITKParamCombox*>   数据链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        QList<FITKParamCombox*> getDatas();
    private:
        /**
         * @brief    清空数据
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void clear();
    private:
        /**
         * @brief  数据链表
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        QList<FITKParamCombox*> _value;
    };
}

#endif