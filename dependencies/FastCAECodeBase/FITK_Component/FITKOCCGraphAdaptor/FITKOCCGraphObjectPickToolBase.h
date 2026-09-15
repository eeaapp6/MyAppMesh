/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectPickToolBase.h
 * @brief       Base graph object for pick tools.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-13
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTPICKTOOLBASE_H__
#define __FITKOCCGRAPHOBJECTPICKTOOLBASE_H__

#include "FITKOCCGraphObjectToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class AIS_ColoredShape;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Base graph object for pick tools.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-13
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectPickToolBase : public FITKOCCGraphObjectToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectPickToolBase, FITKOCCGraphObjectToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectPickToolBase);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        FITKOCCGraphObjectPickToolBase();

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        virtual ~FITKOCCGraphObjectPickToolBase();

        /**
         * @brief       Set the select mode for this graph object's interactive objects.[override]
         * @param[in]   mode: The select mode
         * @param[in]   force: Force add the select mode[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void addSelectMode(CommonOCC::SelectMode mode, bool force = false) override;

        /**
         * @brief       Clear all interactive objects select modes.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void clearSelectMode() override;

    protected:
        /**
         * @brief       The interactive object for previrewing the picked shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        AIS_ColoredShape* m_aisPickShape{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTPICKTOOLBASE_H__
