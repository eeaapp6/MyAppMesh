/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectSketchCommand.h
 * @brief       Graph object for sketch data object and OCC graph widget.
 *              ( Legacy )
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-04
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTSKETCHCOMMAND_H__
#define __FITKOCCGRAPHOBJECTSKETCHCOMMAND_H__

#include "FITKOCCGraphObjectModelCommand.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

namespace Interface
{
    class FITKAbsGeoSketch2D;
}

namespace Exchange
{
    /**
     * @brief       Graph object for sketch data object and OCC graph widget.( Legacy )
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectSketchCommand : public FITKOCCGraphObjectModelCommand
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectSketchCommand, FITKOCCGraphObjectModelCommand);
        FITKCLASS(Exchange, FITKOCCGraphObjectSketchCommand);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   sketchData: The sketch data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKOCCGraphObjectSketchCommand(Interface::FITKAbsGeoSketch2D* sketchData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        ~FITKOCCGraphObjectSketchCommand() = default;

        /**
         * @brief       Set the select mode for this graph object's interactive objects.[override]
         * @param[in]   mode: The select mode
         * @param[in]   force: Force add the select mode[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void addSelectMode(CommonOCC::SelectMode mode, bool force = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKOCCGraphObjectSketchCommand() = default;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTSKETCHCOMMAND_H__
