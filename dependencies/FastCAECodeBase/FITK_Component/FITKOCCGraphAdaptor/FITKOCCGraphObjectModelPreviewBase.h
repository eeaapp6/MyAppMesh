/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectModelPreviewBase.h
 * @brief       Abstract graph object for previewing OCC model.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMODELPREVIEWBASE_H__
#define __FITKOCCGRAPHOBJECTMODELPREVIEWBASE_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class TopoDS_Shape;

class FITKOCCAIS_ColoredShape;

namespace Exchange
{
    /**
     * @brief       Abstract graph object for previewing OCC model.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectModelPreviewBase : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectModelPreviewBase, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectModelPreviewBase);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   dataObj: The model data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelPreviewBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        virtual ~FITKOCCGraphObjectModelPreviewBase();

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Set the color to all vertice, edges and faces.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Set the shape of point mark.[virtual]
         * @param[in]   type: The type of the point mark
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setPointMark(FITKOCCCommons::PointMarkShape type) override;

        /**
         * @brief       Set the point size of vertice' marks.[override]
         * @param[in]   factor: The size factor( default 1.0 )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setPointSizeScale(double factor) override;

        /**
         * @brief       Set the line width of the edges( wire ).[override]
         * @param[in]   size: The line width
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setLineWidth(double size) override;

        /**
         * @brief       Hide or show the graph object's interactive objects.[override]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setVisible(bool visibility) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelPreviewBase() = default;

        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void generateShape();

    protected:
        /**
         * @brief       The shape data of the model.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        TopoDS_Shape* m_tpShape{ nullptr };

        /**
         * @brief       The interactive object for colored shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCAIS_ColoredShape* m_fAisShape{ nullptr };

        /**
         * @brief       The hash code of the shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        int m_hashCode = -1;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMODELPREVIEWBASE_H__
