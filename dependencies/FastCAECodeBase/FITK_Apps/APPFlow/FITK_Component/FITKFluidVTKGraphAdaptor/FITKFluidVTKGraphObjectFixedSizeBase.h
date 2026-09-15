/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectFixedSizeBase.h
 * @brief   Graph object base for fixed sized mark.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-26
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTFIXEDSIZEBASE_H__
#define __FITKFLUIDVTKGRAPHOBJECTFIXEDSIZEBASE_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class FITKGraphActor;

class vtkGlyph3D;
class vtkDistanceToCamera;

namespace Comp
{
    class FITKGraphRender;
}

namespace Exchange
{
    /**
     * @brief   Graph object base for fixed sized mark.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-26
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectFixedSizeBase : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectFixedSizeBase, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectFixedSizeBase);

    public:
        /**
         * @brief   Constructor.
         * @param   dataObj: The data object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKFluidVTKGraphObjectFixedSizeBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        virtual ~FITKFluidVTKGraphObjectFixedSizeBase();

        /**
         * @brief   Set the render, which will add this graph object to itself.( for calculating size )[virtual][override]
         * @param   render: The FITKGrapgRender
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        virtual void setFITKRender(Comp::FITKGraphRender* render) override;

        /**
         * @brief   Get the screen size of the actor in this index.
         * @param   index: The actor index
         * @return  The screen size of the actor
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        double getScreenSize(int index);

        /**
         * @brief   Set the fixed-size of the graph actor or actors.
         * @param   pixelSize: The pixel size of actors need to be limited to.
         * @param   index: The index of the graph actor( -1 means set all actors to this size )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void setScreenSize(double pixelSize = 100., int index = -1);

    protected:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKFluidVTKGraphObjectFixedSizeBase() = default;

    protected:
        /**
         * @brief   Data glyphs
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        QList<vtkGlyph3D*> m_glyphList{};

        /**
         * @brief   Fixed size filters
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        QList<vtkDistanceToCamera*> m_distToCameraList{};

        /**
         * @brief   The glyph source pixel size.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        double m_glyphSize = 60.;

        /**
         * @brief   The default glyph size of fixed sized actors in graph window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        double m_defaultSize = 60.;

        /**
         * @brief   The default large glyph size of fixed sized actors in graph window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        const double m_defaultLargeSize = 180.;

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTFIXEDSIZEBASE_H__
