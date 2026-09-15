/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPolyRenderStrategy.h
 * @brief       Polydata render strategy.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-04-11
 *********************************************************************/

#ifndef FITKPOLYRENDERSTRATEGY_H
#define FITKPOLYRENDERSTRATEGY_H

#include "vtkObject.h"

#include "FITKVTKAlgorithmAPI.h"

class vtkActor2D;

class vtkWindow;
class vtkRenderer;
class vtkPolyDataMapper2D;

class FITKPolygonSource;

enum SourceType
{
    UserDefined = 0,
    Round,
    X_Shape,
    Triangle,
    Rect
};

class FITKVTKALGORITHMAPI FITKPolyRenderStrategy : public vtkObject
{
public:
    void PrintSelf(ostream& os, vtkIndent indent) override;
    vtkTypeMacro(FITKPolyRenderStrategy, vtkObject);
    static FITKPolyRenderStrategy* New();

    //@{
    /**
     * Set the renderer associated with this strategy.
     */
    virtual void SetRenderer(vtkRenderer* ren);
    vtkGetObjectMacro(Renderer, vtkRenderer);
    //@}

    // Added by ChengHaotian
    //@{
    /**
     * Set the input source data type.
     * 1: Round
     * 2: X shape
     * 3: Triangle
     * 4: Rect
     */
    void SetSourceType(SourceType type, bool drawPolygon = false);
    /**
      * 0: UserDefined( Unused )
     */
    SourceType GetSourceType();

    /**
     * Set the polygon color.
     */
    void SetColor(double rf, double gf, double bf);
    void SetColor(double* rgbf);

    /**
     * The size of the point's mark need to be drawn.
     */
    void SetPixelSize(int size);
    vtkGetMacro(PixelSize, int);

    /**
     * Render the poly data in the given position.
     */
    void RenderPoly(int x[2]);
    //@}

    /**
     * Release any graphics resources that are being consumed by this strategy.
     * The parameter window could be used to determine which graphic
     * resources to release.
     */
    virtual void ReleaseGraphicsResources(vtkWindow* window);

protected:
    FITKPolyRenderStrategy();
    ~FITKPolyRenderStrategy() override;

    vtkRenderer* Renderer;

    vtkPolyDataMapper2D* Mapper;
    vtkActor2D* Actor;

    bool DrawPolygon = false;
    int PixelSize = 20;
    SourceType SourceTypeValue = SourceType::Round;
    FITKPolygonSource* InputSource{ nullptr };

private:
    FITKPolyRenderStrategy(const FITKPolyRenderStrategy&) = delete;
    void operator=(const FITKPolyRenderStrategy&) = delete;
};

#endif // FITKPOLYRENDERSTRATEGY_H
