/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectTopoShape.h
 * @brief       Graph object for previewing the shape of the virtual topo.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-06
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTTOPOSHAPE_H__
#define __FITKOCCGRAPHOBJECTTOPOSHAPE_H__

#include "FITKOCCGraphObjectTopoBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing the shape of the virtual topo.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-06
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectTopoShape : public FITKOCCGraphObjectTopoBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectTopoShape, FITKOCCGraphObjectTopoBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectTopoShape);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   vTopoData: The virtual topo data object of the shape( edge or face )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKOCCGraphObjectTopoShape(Interface::FITKAbsVirtualTopo* vTopoData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        ~FITKOCCGraphObjectTopoShape();

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKOCCGraphObjectTopoShape() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        bool initShapeData();

        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void generateShape();
 
    private:
        /**
         * @brief       The interactive object of the arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        AIS_ColoredShape* m_aisShape{ nullptr };
    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTTOPOSHAPE_H__
