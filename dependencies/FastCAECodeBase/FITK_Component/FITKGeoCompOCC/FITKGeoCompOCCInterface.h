/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGeoCompOCCInterface.h
 * @brief       OCC几何组件接口类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-10-16
 *********************************************************************/

#ifndef  _FITKGEOCOMPOCCINTERAFACE_H___
#define  _FITKGEOCOMPOCCINTERAFACE_H___

#include "FITKGeoCompOCCAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace OCC
{
    /**
     * @brief       OCC几何组件接口类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-16
     */
    class FITKGeoCompOCCAPI FITKGeoCompOCCInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief       设置三角化角度阈值。[静态]
         * @param[in]   angle：角度阈值，值越小三角化越精细
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static void SetDefaultAngle(double angle);

        /**
         * @brief       获取三角化角度阈值。[静态]
         * @return      当前角度阈值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static double GetDefaultAngle();

        /**
         * @brief       设置绝对最小偏差（Deflection下限）。[静态]
         * @param[in]   deflection：绝对最小偏差值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static void SetMinDeflectionAbs(double deflection);

        /**
         * @brief       获取绝对最小偏差（Deflection下限）。[静态]
         * @return      当前绝对最小偏差值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static double GetMinDeflectionAbs();

        /**
         * @brief       设置绝对最大偏差（Deflection上限）。[静态]
         * @param[in]   deflection：绝对最大偏差值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static void SetMaxDeflectionAbs(double deflection);

        /**
         * @brief       获取绝对最大偏差（Deflection上限）。[静态]
         * @return      当前绝对最大偏差值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static double GetMaxDeflectionAbs();

        /**
         * @brief       设置相对最小偏差比例（按模型尺寸）。[静态]
         * @param[in]   ratio：相对最小偏差比例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static void SetMinDeflectionRatio(double ratio);

        /**
         * @brief       获取相对最小偏差比例（按模型尺寸）。[静态]
         * @return      当前相对最小偏差比例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static double GetMinDeflectionRatio();

        /**
         * @brief       设置相对最大偏差比例（按模型尺寸）。[静态]
         * @param[in]   ratio：相对最大偏差比例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static void SetMaxDeflectionRatio(double ratio);

        /**
         * @brief       获取相对最大偏差比例（按模型尺寸）。[静态]
         * @return      当前相对最大偏差比例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-17
         */
        static double GetMaxDeflectionRatio();

        explicit FITKGeoCompOCCInterface();
        virtual ~FITKGeoCompOCCInterface();

        /**
        * @brief 获取部件名称，不能重复  return "FITKGeoCompOCC"
        * @return QString
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-04
        */
        virtual QString getComponentName() override;
 
        /**
         * @brief 执行读取或写出
         * @param indexPort    
         * @return
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual bool exec(const int indexPort) override;
         
        
    };
}


#endif // 