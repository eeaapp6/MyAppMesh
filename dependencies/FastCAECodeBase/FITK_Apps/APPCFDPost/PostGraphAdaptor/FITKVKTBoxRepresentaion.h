/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKVKTBoxRepresentaion.h
 * @brief  VKTBoxRepresentaion重写
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-12
 *********************************************************************/
#ifndef _FITKVKTBoxRepresentaion_H
#define _FITKVKTBoxRepresentaion_H

#include <vtkBoxRepresentation.h>

namespace Interface
{
    /**
     * @brief  VKTBoxRepresentaion重写
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-12
     */
    class FITKVKTBoxRepresentaion :public vtkBoxRepresentation
    {
    public:
        /**
         * @brief    创建接口
         * @return   FITKVKTBoxRepresentaion *
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        static FITKVKTBoxRepresentaion* New();
        /**
         * @brief    设置边界
         * @param[i] bounds     边界[6](XMin、XMax、YMin、YMax、ZMin、ZMax)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void setBounds(double* bounds);
    protected:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        FITKVKTBoxRepresentaion();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        ~FITKVKTBoxRepresentaion();
    };
}
#endif