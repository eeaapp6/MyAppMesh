/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelOffsetFace.h
 * @brief   OCC偏移面类。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2026-07-06
 *********************************************************************/

#ifndef  FITKOCCMODELOFFSETFACE_H
#define  FITKOCCMODELOFFSETFACE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOffsetFace.h"

#include <QList>
#include <gp_Pnt.hxx>

class gp_Dir;
class TopoDS_Face;

namespace OCC
{
    /**
     * @brief       OCC偏移面类。
     *              支持两种偏移模式：基于目标面的相对偏移、直接距离偏移。
     *              支持多源面选择，源面和目标面可为任意可偏移面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-06
     */
    class FITKGeoCompOCCAPI FITKOCCModelOffsetFace : public Interface::FITKAbsGeoOffsetFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKOCCModelOffsetFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        ~FITKOCCModelOffsetFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        bool update() override;

    };
}

#endif // !FITKOCCMODELOFFSETFACE_H
