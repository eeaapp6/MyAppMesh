/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKGraphOperator.h
 * @brief 绘图控制器接口声明 
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-14
 * 
 */
#ifndef _FITKGraphOperator_H_
#define _FITKGraphOperator_H_

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"
#include "FITKRenderWindowVTKAPI.h"

namespace Comp
{
    class FITKGraphObject;
    class FITKGraph3DWindowVTK;
    /**
     * @brief 绘图控制器
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-14
     */
    class FITKRenderWindowVTKAPI FITKGraphOperator :public Core::FITKAbstractOperator
    {
    public:
        /**
         * @brief Construct a new FITKGraphOperator object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-14
         */
        FITKGraphOperator();
        /**
         * @brief Destroy the FITKGraphOperator object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-14
         */
        virtual ~FITKGraphOperator() = 0;
        /**
         * @brief 设置绘图操作器对应的渲染窗口
         * @param[i]  w              渲染窗口对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-18
         */
        void setGraph3DWindow(FITKGraph3DWindowVTK* w);
        /**
         * @brief 获取对应的渲染窗口对象
         * @return FITKGraph3DWindow* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-18
         */
        FITKGraph3DWindowVTK* getGraph3DWindow();
    protected:
        /**
         * @brief 渲染窗口对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-18
         */
        FITKGraph3DWindowVTK* m_graph3DWindow = nullptr;
    };
}

#endif
