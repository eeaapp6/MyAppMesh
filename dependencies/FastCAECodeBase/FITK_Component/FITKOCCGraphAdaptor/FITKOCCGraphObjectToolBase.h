/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectToolBase.h
 * @brief       Base graph object for tools.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-14
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTTOOLBASE_H__
#define __FITKOCCGRAPHOBJECTTOOLBASE_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>
#include <QVector3D>

// Forward declaration
class QVariant;

class TopoDS_Shape;
class AIS_ColoredShape;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Base graph object for tools.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-14
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectToolBase : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectToolBase, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectToolBase);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        FITKOCCGraphObjectToolBase();

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        virtual ~FITKOCCGraphObjectToolBase() = default;

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Set the color.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Hide or show the graph object's interactive objects.[virtual][override]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        void setVisible(bool visibility) override;

        /**
         * @brief       Get the visibility.[virtual][override]
         * @return      The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        virtual bool getDataVisibility() override;

    protected:
        /**
         * @brief       Get the virtual topo data object by id.
         * @param[in]   vId: The virtual topo id
         * @return      The data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        Interface::FITKAbsVirtualTopo* getVirtualTopoById(int vId);

        /**
         * @brief       Get the TopoDS_Shape by virtual topo data object.
         * @param[in]   vTopo: The virtual topo data object
         * @return      The OCC shape data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        TopoDS_Shape getTopoShapeByVirtualTopo(Interface::FITKAbsVirtualTopo* vTopo);

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTTOOLBASE_H__
