/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectDimensionToolBase.h
 * @brief       Base graph object for dimension tool.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-09
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTDIMENSIONTOOLBASE_H__
#define __FITKOCCGRAPHOBJECTDIMENSIONTOOLBASE_H__

#include "FITKOCCGraphObjectToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <Standard_Version.hxx>

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
     * @brief       Base graph object for dimension tool.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-09
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectDimensionToolBase : public FITKOCCGraphObjectToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectDimensionToolBase, FITKOCCGraphObjectToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectDimensionToolBase);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        FITKOCCGraphObjectDimensionToolBase();

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        virtual ~FITKOCCGraphObjectDimensionToolBase();

        /**
         * @brief       Clear the cache data.[virtual][override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        virtual void clearCache() override;

        /**
         * @brief       Update the graph object.[virtual][override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        virtual void update(bool forceUpdate = false) override;

        /**
         * @brief       Set the virtual topos need to be measured with.
         * @param[in]   ids: The virtual topo id list
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-10
         */
        void setVirtualTopoIds(QList<int> ids);

        /**
         * @brief       Clear the data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-10
         */
        void clearData();

    protected:
        /**
         * @brief       Update the geometry list.[pure virtual]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-10
         */
        virtual bool updateMeasureGeometrys() = 0;

        /**
         * @brief       Resize the measure tool.[pure virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        virtual void rescale() = 0;

        /**
         * @brief       Create the interactive shape for previewing all measured shapes.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void createPreviewShape();

        /**
         * @brief       Get the input ids from the variant input.
         * @return      The input virtual topo ids.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        QList<int> getInputItems();

    protected:
        /**
         * @brief       The interactive object for previewing the shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        AIS_ColoredShape* m_aisShape{ nullptr };

        /**
         * @brief       The default font size.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        double m_defaultFontHeight = 25.;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTDIMENSIONTOOLBASE_H__
