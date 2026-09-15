/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsAlgorithmTools.h
 * @brief       抽象算法工具。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-12-20
 *********************************************************************/

#ifndef __FITKABSALGORITHMTOOLS_H__
#define __FITKABSALGORITHMTOOLS_H__ 

#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"

#include "FITKInterfaceModelAPI.h"

namespace Interface
{
    /**
     * @brief       变换结构体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-20
     */
    struct FITKInterfaceModelAPI ToolTransform
    {
        double Angle = 0.;
        double Pt[3]{ 0., 0., 0. };
        double Dir[3]{ 0., 0., 1. };
        double Translate[3]{ 0., 0., 0. };
    };

    /**
     * @brief       变换计算工具类。（右手）
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-20
     */
    class FITKInterfaceModelAPI FITKAbstractTransformTool : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        explicit FITKAbstractTransformTool() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        virtual ~FITKAbstractTransformTool() = default;

        /**
         * @brief       创建变换计算工具类。[静态]
         * @return      变换计算工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        static FITKAbstractTransformTool* createTool();

        /**
         * @brief       平移+旋转。[虚函数]
         * @param[in]   iTrans：平移量
         * @param[in]   iAxisPt1：旋转轴坐标1
         * @param[in]   iAxisPt2：旋转轴坐标2
         * @param[in]   iAngleDeg：旋转角（角度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-28
         */
        virtual void setTransform(double* iTrans, double* iAxisPt1, double* iAxisPt2, double iAngleDeg)
        {
            Q_UNUSED(iTrans);
            Q_UNUSED(iAxisPt1);
            Q_UNUSED(iAxisPt2);
            Q_UNUSED(iAngleDeg);
        }

        /**
         * @brief       执行一次平移。[虚函数]
         * @param[in]   iX：X方向平移量
         * @param[in]   iY：Y方向平移量
         * @param[in]   iZ：Z方向平移量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void transform(double iX, double iY, double iZ)
        {
            Q_UNUSED(iX);
            Q_UNUSED(iY);
            Q_UNUSED(iZ);
        }

        /**
         * @brief       执行一次旋转。[虚函数]
         * @param[in]   iPt1：旋转轴坐标1
         * @param[in]   iPt2：旋转轴坐标2
         * @param[in]   iAngleDeg：旋转角（角度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void rotate(double* iPt1, double* iPt2, double iAngleDeg)
        {
            Q_UNUSED(iPt1);
            Q_UNUSED(iPt2);
            Q_UNUSED(iAngleDeg);
        }

        /**
         * @brief       获取当前变换旋转信息。[虚函数]
         * @param[out]  oPt1：旋转轴坐标1
         * @param[out]  oPt2：旋转轴坐标2
         * @param[out]  oAngleDeg：旋转角（角度）[引用]
         * @return      当前变换是否为有效旋转
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual bool isRotation(double* oPt1, double* oPt2, double & oAngleDeg)
        {
            Q_UNUSED(oPt1);
            Q_UNUSED(oPt2);
            Q_UNUSED(oAngleDeg);
            return false;
        }

        /**
         * @brief       坐标变换。
         * @param[out]  iPt：原始坐标
         * @param[out]  oPt：输出坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void transformPoint(double* iPt, double* oPt)
        {
            Q_UNUSED(iPt);
            Q_UNUSED(oPt);
        }

        /**
         * @brief       方向变换。
         * @param[in]   iDir：原始方向
         * @param[out]  oDir：输出方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void transformDirection(double* iDir, double* oDir)
        {
            Q_UNUSED(iDir);
            Q_UNUSED(oDir);
        }

        /**
         * @brief       获取当前变换矩阵与向量。[虚函数]
         * @param[out]  oMat：变换矩阵
         * @param[out]  oVec：平移向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void data(double oMat[3][3], double* oVec)
        {
            Q_UNUSED(oMat);
            Q_UNUSED(oVec);
        }

        /**
         * @brief       设置变换矩阵与向量。[虚函数]
         * @param[in]   iMat：变换矩阵
         * @param[in]   iVec：平移向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-23
         */
        virtual void setData(double iMat[3][3], double* iVec)
        {
            Q_UNUSED(iMat);
            Q_UNUSED(iVec);
        }

    };

    /**
     * @brief       抽象算法工具创建器。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-20
     */
    class FITKInterfaceModelAPI FITKAbsAlgorithmToolsCreator
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        explicit FITKAbsAlgorithmToolsCreator() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        virtual ~FITKAbsAlgorithmToolsCreator() = default;

        /**
         * @brief       创建变换工具。[虚函数]
         * @return      变换工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        virtual Interface::FITKAbstractTransformTool* createTransformTool();

    };
}

#endif //!__FITKABSALGORITHMTOOLS_H__
