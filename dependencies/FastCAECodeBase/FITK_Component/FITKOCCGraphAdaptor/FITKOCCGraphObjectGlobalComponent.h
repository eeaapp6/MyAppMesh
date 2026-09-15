/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectGlobalComponent.h
 * @brief       Graph object for previewing the global geometry component.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-10-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTGLOBALCOMPONENT_H__
#define __FITKOCCGRAPHOBJECTGLOBALCOMPONENT_H__

#include "FITKOCCGraphObjectTopoBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

namespace Interface
{
    class FITKGlobalGeoComponent;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing the global geometry component.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectGlobalComponent : public FITKOCCGraphObjectTopoBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectGlobalComponent, FITKOCCGraphObjectTopoBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectGlobalComponent);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   globalComp: The global geometry component data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        FITKOCCGraphObjectGlobalComponent(Interface::FITKGlobalGeoComponent* globalComp);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        ~FITKOCCGraphObjectGlobalComponent();

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        void update(bool forceUpdate = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        FITKOCCGraphObjectGlobalComponent() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        bool initShapeData();

        /**
         * @brief       Generate the topo shape.
         * @param[in]   resultShape: The result shape[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        void generateTopoShape(TopoDS_Shape & resultShape);

        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        void generateShape();

    private:
        /**
         * @brief       The interactive object of the arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        AIS_ColoredShape* m_aisShape{ nullptr };
    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTGLOBALCOMPONENT_H__
