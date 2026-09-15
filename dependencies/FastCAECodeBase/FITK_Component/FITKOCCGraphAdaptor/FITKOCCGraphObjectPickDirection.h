/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectPickDirection.h
 * @brief       Graph object for fetching the direction by the picked 
 *              position and object picked.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-12
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTPICKDIRECTION_H__
#define __FITKOCCGRAPHOBJECTPICKDIRECTION_H__

#include "FITKOCCGraphObjectPickToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class TopoDS_Shape;
class AIS_ColoredShape;

class FITKOCCAIS_ColoredShape;

namespace Exchange
{
    /**
     * @brief       Graph object for fetching the direction by the picked position and object picked.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-12
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectPickDirection : public FITKOCCGraphObjectPickToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectPickDirection, FITKOCCGraphObjectPickToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectPickDirection);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        FITKOCCGraphObjectPickDirection();

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        ~FITKOCCGraphObjectPickDirection();

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void update(bool forceUpdate = false) override;

        /**
         * @brief       Set the color to the arrow shape.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void setColor(QColor color) override;

        /**
         * @brief       Update the graph objects by the mouse picked position.[override]
         * @param[in]   mx: The mouse x
         * @param[in]   my: The mouse y
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void updateByMousePick(int mx, int my) override;

        /**
         * @brief       Clear the cache data.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void clearCache() override;

        /**
         * @brief       Update the graph objects by the saved select modes.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-08
         */
        void updateSelectModes() override;

    private:
        /**
         * @brief       Generate the AIS_Shape and initialize the drawer properties.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void generateShape();

        /**
         * @brief       Internal function: Get the user defined direction.
         * @param[out]  dir: The direction value list( length: 3 )[quote]
         * @return      If the direction exists
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        bool getInputUserDirection(QList<double> & dir);

        /**
         * @brief       Internal function: Get the anchor position for the arrow.
         * @param[out]  pos: The position value list( length: 3 )[quote]
         * @param[out]  dir: The direction value list( length: 3 )[quote]
         * @param[out]  anchoredShape: The copy of anchored shape[quote]
         * @return      If the anchor exists
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        bool getInputAnchor(QList<double> & pos, QList<double> & dir, TopoDS_Shape & anchoredShape);

        /**
         * @brief       Internal function: Get the anchor topo id for the arrow.
         * @param[out]  mainId: The anchored main data id( such as: command or datum )[quote]
         * @param[out]  subId: The anchored sub data id( such as: virtual topo )[quote]
         * @return      If the anchor exists
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        bool getInputAnchorId(int & mainId, int & subId);

        /**
         * @brief       Internal function: Get the flag for whether to calculate the normal for
         *              the shape center for the face or edge.
         * @return      Enable or disbale
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        bool getInputFlagForCenterNormal();

        /**
         * @brief       Internal function: Get the flag for whether to reverse the normal for
         *              the shape.
         * @return      Enable or disbale
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        bool getInputFlagForReverseNormal();

    private:
        /**
         * @brief       The interactive object of the arrow for previewing direction.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        FITKOCCAIS_ColoredShape* m_aisArrowShape{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTPICKDIRECTION_H__
