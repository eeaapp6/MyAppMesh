/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamDoubleList.h
 * @brief  双精度链表参数类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef _FITKParamDoubleList_H
#define _FITKParamDoubleList_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKParamDouble;
    /**
     * @brief  双精度链表参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class FITKEasyParamAPI FITKParamDoubleList : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamDoubleList();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual ~FITKParamDoubleList();
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
         * @param[i] index       数据索引
         * @param[i] value       数据值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValue(int index, double value);
        /**
         * @brief    设置数据
         * @param[i] value       数据链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setValue(QList<double> value);
        /**
         * @brief    设置数据
         * @param[i] values       数据链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setDoubleDatas(QList<FITKParamDouble*> values);
        /**
         * @brief    追加数据
         * @param[i] value       数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void pushDoubleData(FITKParamDouble* value);
        /**
         * @brief    获取数据 
         * @param[i] index       数据索引
         * @return   double      数据值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        double getValue(int index);
        /**
         * @brief    获取数据
         * @param[i] index       数据索引
         * @return   FITKParamDouble*  数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParamDouble* getDoubleData(int index);
        /**
         * @brief    获取数据链表
         * @return   QList<FITKParamDouble*>   数据链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        QList<FITKParamDouble*> getDoubleDatas();
        /**
         * @brief    设置范围
         * @param[i] range       数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setRange(double* range);
        /**
         * @brief    获取数据范围
         * @param[i] range       数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void getRange(double* range);
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
        QList<FITKParamDouble*> _value;
    };
}

#endif