/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbstractRadiossData.h
 * @brief Radioss数据类基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-04-23
 */
#ifndef __FITKABSTRACT_RADIOS_DATA_H__
#define __FITKABSTRACT_RADIOS_DATA_H__

#include "FITKRadiossDataAPI.h"
#include <QString>

#ifndef RadiossKeyWords
#define RadiossKeyWord(aClass,KeyWord)   \
public: \
    static const QString Get##aClass##RadiossKeyWord() { return QString(#KeyWord) ; } 
#endif



namespace Radioss
{
    /**
     * @brief Radioss数据基类
     * @details 此抽象基类定义了所有Radioss数据类型的通用接口。
     *          包括材料、单元、约束等数据类型都需要继承此类。
     *          提供了统一的数据访问和处理方法。
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-04-23
     */
    class FITKRadiossDataAPI FITKAbstractRadiossData 
    {
    public:
        /**
         * @brief 构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-23
         */
        explicit FITKAbstractRadiossData() = default;
        /**
         * @brief 析构函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-23
         */
        virtual ~FITKAbstractRadiossData() = 0;

        /**
         * @brief 获取Radioss关键字标识
         * @return QString 返回数据对象对应的Radioss关键字
         * @details 此方法返回对象在Radioss输入文件中的关键字标识符。
         *          每种Radioss数据类型都有其特定的关键字，用于在输入文件中标识。
         *          派生类必须实现此方法以提供具体的关键字。
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-23
         */
//        virtual QString getRadiossFormatKeyWords() const;
        /**
         * @brief    设置可见性
         * @param[i] isVisible 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-22
         */
        void setVisible(bool isVisible);
        /**
         * @brief    获取可见性
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-22
         */
        bool getVisible();

    private:
        /**
         * @brief  可见性
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-22
         */
        bool _isVisible{ true };
    };
}

#endif