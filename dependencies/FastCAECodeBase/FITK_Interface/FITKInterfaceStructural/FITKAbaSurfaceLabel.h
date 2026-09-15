/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaSurfaceLabel.h
 * @brief Abaqus表面标记
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef __FITKABASURFACELABEL_H___
#define __FITKABASURFACELABEL_H___

#include "FITKInterfaceStructuralAPI.h"
#include <QObject>

namespace Interface
{
    /**
     * @brief Abaqus表面标记
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfaceStructuralAPI FITKAbaSurfaceLabel : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Abaqus的表面标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        enum AbaSurfLabel
        {
            Empty = 0,
            S1 = 1000,
            S2,
            S3,
            S4,
            S5,
            S6,
            E1 = 2000,
            E2,
            E3,
            E4,
            SPOS = 3000,
            SNEG,

            // 梁类型表面 4000

            // 几何壳面边
            Geom_SE = 5000
        };
        Q_ENUM(AbaSurfLabel);
        /**
         * @brief 获取面的vtk单元类型
         * @param[i]  abaEleType    Abaqus定义的单元类型
         * @param[i]  label         表面标记
         * @param[out]  index       表面索引,顺序为FITKAbstractElement的子类分别定义
         * @return int  vtk定义的cellType,参考vtkcelltype.h
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        static int GetFaceVtkType(int abaEleType, FITKAbaSurfaceLabel::AbaSurfLabel label, int &index);

        /**
         * @brief 获取abaqus的面标记
         * @param[i]  abaEleType     单元类型
         * @param[i]  index          单元面索引
         * @param[o]  textLabel      字符串标记
         * @return AbaSurfLabel
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        static AbaSurfLabel GetFaceLabel(int abaEleType, int index, QString &textLabel);
    private:
        /**
         * @brief Construct a new FITKAbaSurfaceLabel object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKAbaSurfaceLabel() = default;
        FITKAbaSurfaceLabel(const FITKAbaSurfaceLabel&) = default;
        /**
         * @brief Destroy the FITKAbaSurfaceLabel object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        ~FITKAbaSurfaceLabel() = default;

    };



}



#endif
