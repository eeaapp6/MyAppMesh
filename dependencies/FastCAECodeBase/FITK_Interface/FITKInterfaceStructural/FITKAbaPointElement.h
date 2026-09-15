/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_ABA_POINT_ELEMENT_H___
#define __FITK_ABA_POINT_ELEMENT_H___

#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAssemblyElements.h"

namespace Interface
{
    class FITKInterfaceStructuralAPI FITKAbaPointElement : public FITKAbstractAbaAssemblyElement
        , public FITKAssemblyElementVertex
    {
    public:
        explicit FITKAbaPointElement() = default;
        virtual ~FITKAbaPointElement() = default;

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
        virtual FITKAbstractAbaAssemblyElement* getACopy() override;
    };
}


#endif
