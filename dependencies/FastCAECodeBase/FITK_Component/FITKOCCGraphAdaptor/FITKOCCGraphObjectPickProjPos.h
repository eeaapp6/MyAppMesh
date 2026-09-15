/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectPickProjPos.h
 * @brief       Graph object for projecting the mouse position to the closet
 *              pre-selected edge or face.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-13
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTPICKPROJPOS_H__
#define __FITKOCCGRAPHOBJECTPICKPROJPOS_H__

#include "FITKOCCGraphObjectPickToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class AIS_ColoredShape;

namespace Exchange
{
    /**
     * @brief       Graph object for projecting the mouse position to the closet pre-selected edge or face.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-13
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectPickProjPos : public FITKOCCGraphObjectPickToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectPickProjPos, FITKOCCGraphObjectPickToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectPickProjPos);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        FITKOCCGraphObjectPickProjPos();

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        ~FITKOCCGraphObjectPickProjPos();

        /**
         * @brief       Set the color to the arrow shape.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void setColor(QColor color) override;

        /**
         * @brief       Update the graph objects by the mouse moved position.[override]
         * @param[in]   mx: The mouse x
         * @param[in]   my: The mouse y
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void updateByMouseMove(int mx, int my) override;

        /**
         * @brief       Update the graph objects by the mouse picked position.[override]
         * @param[in]   mx: The mouse x
         * @param[in]   my: The mouse y
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void updateByMousePick(int mx, int my) override;

        /**
         * @brief       Clear the cache data.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void clearCache() override;

    private:
        /**
         * @brief       Generate the AIS_Shape and initialize the drawer properties.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void generateShape();

        /**
         * @brief       Set the preview shape style to mid point highlight style.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void setStyleToMidPoint();

        /**
         * @brief       Set the preview shape style to normal point highlight style.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void setStyleToNormalPoint();

        /**
         * @brief       Get the input role data for the projection target.
         * @param[out]  targetCmdId: The projection target command Id[quote]
         * @param[out]  targetVTopoId: The projection target virtual topo Id[quote]
         * @return      Has the input data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-18
         */
        bool getInputTargetIds(int & targetCmdId, int & targetVTopoId);

        /**
         * @brief       Get the shape by the input target ids.
         * @param[out]  shape: The target shape[quote]
         * @return      Has the input target shape
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-18
         */
        bool getTargetShape(TopoDS_Shape & shape);

    private:
        /**
         * @brief       The interactive object for previrewing the hovered point.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        AIS_ColoredShape* m_aisPreviewShape{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTPICKPROJPOS_H__
