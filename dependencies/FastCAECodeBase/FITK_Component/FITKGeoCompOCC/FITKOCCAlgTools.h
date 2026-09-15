/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCAlgTools.h
 * @brief       The algorithm tools using OCC.

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-28
 *********************************************************************/

#ifndef __FITKOCCALGTOOLS_H__
#define __FITKOCCALGTOOLS_H__

#include "FITK_Interface/FITKInterfaceModel/FITKAbsAlgorithmTools.h"

class gp_Trsf;

namespace OCC
{
    /**
     * @brief       The algorithm tool creator.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-28
     */
    class FITKOCCAlgToolCreator : public Interface::FITKAbsAlgorithmToolsCreator
    {
    public:
        /**
         * @brief       Construction.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        explicit FITKOCCAlgToolCreator() = default;

        /**
         * @brief       Destruction.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        virtual ~FITKOCCAlgToolCreator() = default;

        /**
         * @brief       Create the transform tool.[override]
         * @return      The transform tool
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        Interface::FITKAbstractTransformTool* createTransformTool() override;

    };

    /**
     * @brief       Transformation calculate tool.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-28
     */
    class FITKOCCTransformTool : public Interface::FITKAbstractTransformTool
    {
    public:
        /**
         * @brief       Construction.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        explicit FITKOCCTransformTool();

        /**
         * @brief       Destruction.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        virtual ~FITKOCCTransformTool();

        /**
         * @brief       获取OCC变换。
         * @return      变换数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        const gp_Trsf* getInternalTransform();

        /**
         * @brief       Convert the degree to the radian.
         * @param[in]   degree: The degree
         * @return      The radian
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        double degToRad(double degree);

        /**
         * @brief       Convert the radian to the degree.
         * @param[in]   degree: The degree
         * @return      The angle
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        double radToDeg(double rad);

        /**
         * @brief       Perform a rotate and tranform.[override]
         * @param[in]   iTrans: The translate
         * @param[in]   iAxisPt1: The axis position 1
         * @param[in]   iAxisPt2: The axis position 2
         * @param[in]   iAngleDeg: The rotate angle
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-28
         */
        void setTransform(double* iTrans, double* iAxisPt1, double* iAxisPt2, double iAngleDeg) override;

        /**
         * @brief       Perform a translate with current transformation.[override]
         * @param[in]   iX: The translate of x
         * @param[in]   iY: The translate of y
         * @param[in]   iZ: The translate of z
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void transform(double iX, double iY, double iZ) override;

        /**
         * @brief       Perform a rotation with current transformation.[override]
         * @param[in]   iPt1: The axis position 1
         * @param[in]   iPt2: The axis position 2
         * @param[in]   iAngleDeg: The rotate angle
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void rotate(double* iPt1, double* iPt2, double iAngleDeg) override;

        /**
         * @brief       Get the rotation of current transformation.[override]
         * @param[out]  oPt1: The returned axis position 1
         * @param[out]  oPt2: The returned axis position 2
         * @param[out]  oAngleDeg: The returned rotate angle[quote]
         * @return      Is the transformation is a valid rotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        bool isRotation(double* oPt1, double* oPt2, double & oAngleDeg) override;

        /**
         * @brief       Transform the position.[override]
         * @param[in]   iPt: The input position
         * @param[out]  oPt: The output position
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void transformPoint(double* iPt, double* oPt) override;

        /**
         * @brief       Transform the direction.[override]
         * @param[in]   iDir: The input direction
         * @param[out]  oDir: The output direction
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void transformDirection(double* iDir, double* oDir) override;

        /**
         * @brief       Get the matrix and vector of the transformation.[override]
         * @param[out]  oMat: The returned matrix
         * @param[out]  oVec: The returned vector
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void data(double oMat[3][3], double* oVec) override;

        /**
         * @brief       Set the matrix and vector of the transformation.[override]
         * @param[in]   iMat: The matrix
         * @param[in]   iVec: The vector
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        void setData(double iMat[3][3], double* iVec) override;

    private:
        /**
         * @brief       The transformation of this tool.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-28
         */
        gp_Trsf* m_Transformation{ nullptr };

    };
}


#endif //!__FITKOCCALGTOOLS_H__
