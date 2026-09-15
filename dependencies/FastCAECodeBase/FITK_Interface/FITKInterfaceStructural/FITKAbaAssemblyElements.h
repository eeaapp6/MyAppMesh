/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaAssemblyElements.h
 * @brief 装配单元类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-24
 *
 */
#ifndef _FITKABAASSEMBLY_ELEMENTS_H__
#define _FITKABAASSEMBLY_ELEMENTS_H__

#include "FITKInterfaceStructuralAPI.h"
#include <QObject>
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAssElement.h"

namespace Interface
{
    class FITKAbaAssembly;
     /**
      * @brief Abaqus定义的单元抽象类
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2024-06-24
      */
    class FITKInterfaceStructuralAPI FITKAbstractAbaAssemblyElement :
        public Core::FITKAbstractDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 单元类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        enum FITKAssElementType
        {
            None,
            CONN3D2,
            SpringDashpotA,
        };
        Q_ENUM(FITKAssElementType);
    public:
        /**
         * @brief Construct a new FITKAbstractAbaAssemblyElement object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAbstractAbaAssemblyElement()= default;
        /**
         * @brief Destroy the FITKAbstractAbaAssemblyElement object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAbstractAbaAssemblyElement() = default;
        /**
         * @brief 获取单元类型
         * @return FITKAssElementType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual FITKAssElementType getAssElementType() = 0;
        /**
         * @brief 获取单元的拷贝
         * @return FITKAbstractAbaAssemblyElement*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual FITKAbstractAbaAssemblyElement* getACopy() = 0;
    };
   

    /**
     * @brief 单元检索器，从assembkey中获取单元
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class FITKInterfaceStructuralAPI FITKAssemblyElementSearcher:
        public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKAssemblyElementSearcher object
         * @param[i]  ass            装配体对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAssemblyElementSearcher(FITKAbaAssembly* ass);
        /**
         * @brief Destroy the FITKAssemblyElementSearcher object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAssemblyElementSearcher() = default;

    private:
        /**
         * @brief 装配对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        FITKAbaAssembly* _assembly{};
    };




}



#endif
