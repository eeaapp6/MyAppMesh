/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParamRadioGroup.h
 * @brief  单选组数据类型
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef FITKParamRadioGroup_H
#define FITKParamRadioGroup_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKParameter;
    /**
     * @brief  RadioGroupValue 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKRadioGroupValue
    {
        QString _name;            //名称（RadioButton的名称）
        FITKParameter* _value;    //数据组（RadioButton后面的数据）
        QList<bool> _labelValue;  //标签（公有数据在当前RadioButton勾选时是否可用标签）
    };
    
    //数据对象对应界面示例
    //|   RadioButton 数据组    （公有数据1可不用，在此RadioButton被点击时，标签为 false,true,true,true）
    //|   RadioButton           （公有数据2可不用，在此RadioButton被点击时，标签为 true,false,true,true）
    //|   RadioButton 数据组    （                                          标签为 true,true,true,true）
    //|   RadioButton           （                                          标签为 true,true,true,true）
    //|   RadioButton 数据组    （公有数据3可不用，在此RadioButton被点击时，标签为 true,true,false,true）
    //|-------------------------------------------
    //|   公有数据1
    //|   公有数据2
    //|   公有数据3
    //|   公有数据4

    /**
     * @brief  单选组数据类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-08-21
     */
    class FITKEasyParamAPI FITKParamRadioGroup :public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        FITKParamRadioGroup();
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        ~FITKParamRadioGroup();
        /**
         * @brief    数据拷贝
         * @param[i] data  拷贝对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        void copyParaData(FITKAbstractEasyParam* data) override;
        /**
         * @brief    获取当前数据对象类型
         * @return   FITKEasyParamType 数据对象类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        /**
         * @brief    添加数据单选组 
         * @param[i] name 名称
         * @param[i] value 数据组（FITKParamRadioGroup不可作为数据组参数对象）
         * @param[i] label 启用标签 - 必须存在子对象才能设置
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        void appendValue(QString name, FITKParameter* value, QList<bool> label = QList<bool>());
        /**
         * @brief    设置公有数据（子数据）对象
         * @param[i] subValue 公有对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        void setSubValue(FITKParameter* subValue);
        /**
         * @brief    获取选组数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        QList<FITKRadioGroupValue> getRadioValues();
        /**
         * @brief    获取所有的公有数据（子数据）
         * @return   QList<FITKAbstractEasyParam*> 公有数据（子数据）对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        QList<FITKAbstractEasyParam*> getSubValues();
        /**
         * @brief    获取所有公有数据（子数据）
         * @return   FITKParameter*   公有数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKParameter* getSubValuePara();
        /**
         * @brief    获取当前Radio下的公有数据（子数据）状态
         * @return   QList<bool>    公有数据（子数据）可用状态
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QList<bool> getCurrentValueSubState();
        /**
         * @brief    设置当前的索引
         * @param[i] index 索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        void setCurrentIndex(int index);
        /**
         * @brief    设置当前数据对象
         * @param[i] name 数据对象名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        void setCurrentValue(QString name);
        /**
         * @brief    获取当前数据对象名称
         * @return   QString 数据对象名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        QString getCurrentValue();
        /**
         * @brief    获取当前Radio信息
         * @return   FITKRadioGroupValue    Radio信息结构体
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        FITKRadioGroupValue getCurrentRadioGroupValue();
        /**
         * @brief    获取当前的数据索引
         * @return   int  数据索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-21
         */
        int getCurrentValueIndex();
    private:
        /**
         * @brief  当前数据索引
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        int _currentIndex = 0;
        /**
         * @brief  数据组
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        QList<FITKRadioGroupValue> _value{};
        /**
         * @brief  公有数据（子数据）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        FITKParameter* _subValue = {};
    };
}

#endif
