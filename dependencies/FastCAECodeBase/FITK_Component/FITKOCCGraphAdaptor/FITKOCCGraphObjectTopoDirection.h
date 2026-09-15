/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectTopoDirection.h
 * @brief       Graph object for previewing shape direction for edge and face.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-04
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTTOPODIRECTION_H__
#define __FITKOCCGRAPHOBJECTTOPODIRECTION_H__

#include "FITKOCCGraphObjectTopoBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class AIS_Shape;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing shape direction for edge and face.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectTopoDirection : public FITKOCCGraphObjectTopoBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectTopoDirection, FITKOCCGraphObjectTopoBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectTopoDirection);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   vTopoData: The virtual topo data object of the shape( edge or face )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKOCCGraphObjectTopoDirection(Interface::FITKAbsVirtualTopo* vTopoData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        ~FITKOCCGraphObjectTopoDirection();

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKOCCGraphObjectTopoDirection() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        bool initShapeData();

        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        void generateShape();
 
    private:
        /**
         * @brief       The interactive object of the arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        AIS_Shape* m_aisShape{ nullptr };
    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTTOPODIRECTION_H__
