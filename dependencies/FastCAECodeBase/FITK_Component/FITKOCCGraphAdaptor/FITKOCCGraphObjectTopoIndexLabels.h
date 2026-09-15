/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectTopoIndexLabels.h
 * @brief       Graph object for previewing the labels of shape elements.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-02
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTTOPOINDEXLABELS_H__
#define __FITKOCCGRAPHOBJECTTOPOINDEXLABELS_H__

#include "FITKOCCGraphObjectTopoBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

// Forward declaration
class AIS_TextLabel;
class AIS_Shape;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing the labels of shape elements.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-02
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectTopoIndexLabels : public FITKOCCGraphObjectTopoBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectTopoIndexLabels, FITKOCCGraphObjectTopoBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectTopoIndexLabels);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   vTopoData: The virtual topo data object of the shape( edge, face or solid )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        FITKOCCGraphObjectTopoIndexLabels(Interface::FITKAbsVirtualTopo* vTopoData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        ~FITKOCCGraphObjectTopoIndexLabels();

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        void update(bool forceUpdate = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        FITKOCCGraphObjectTopoIndexLabels() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        bool initShapeData();

        /**
         * @brief       Generate the text labels with the shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        void generateShape();

    private:
        /**
         * @brief       The interactive text labels of all face edges.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-03
         */
        QList<AIS_TextLabel*> m_aisLabels;

        /**
         * @brief       The interactive shape object of the selected shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-03
         */
        AIS_Shape* m_aisShape{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTTOPOINDEXLABELS_H__
