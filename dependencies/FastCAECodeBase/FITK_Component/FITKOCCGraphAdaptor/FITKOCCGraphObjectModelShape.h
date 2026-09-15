/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectModelShape.h
 * @brief       Graph object for TopoDS_Shape and OCC graph widget.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMODELSHAPE_H__
#define __FITKOCCGRAPHOBJECTMODELSHAPE_H__

#include "FITKOCCGraphObjectModelBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
namespace Interface
{
    class FITKAbsGeoShapeAgent;
}

namespace Exchange
{
    /**
     * @brief       Graph object for TopoDS_Shape and OCC graph widget.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectModelShape : public FITKOCCGraphObjectModelBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectModelShape, FITKOCCGraphObjectModelBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectModelShape);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   modelData: The model data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelShape(Interface::FITKAbsGeoShapeAgent* modelData);

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        virtual ~FITKOCCGraphObjectModelShape();

        /**
         * @brief       Get the geometry command for the model.[virtual][override]
         * @return      The geometry command for the model
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual Interface::FITKAbsGeoCommand* getGeoCommand() override;

        /**
         * @brief       Get the visibility from the data object.[virtual][override]
         * @return      The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        virtual bool getDataVisibility() override;

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void update(bool forceUpdate = false) override;
   
    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelShape() = default;

    protected:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool initShapeData();

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMODELSHAPE_H__
