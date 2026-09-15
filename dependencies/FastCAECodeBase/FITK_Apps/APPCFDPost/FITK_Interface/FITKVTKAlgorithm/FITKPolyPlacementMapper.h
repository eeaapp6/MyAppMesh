/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPolyPlacementMapper.h
 * @brief       Polydata placement mapper.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-04-11
 *********************************************************************/

#ifndef FITKPOLYPLACEMENTMAPPER_H
#define FITKPOLYPLACEMENTMAPPER_H

#include "vtkMapper2D.h"

#include "FITKVTKAlgorithmAPI.h"

class vtkRenderer;
class vtkCoordinate;
class vtkSelectVisiblePoints;
class FITKPolyRenderStrategy;

class FITKVTKALGORITHMAPI FITKPolyPlacementMapper : public vtkMapper2D
{
public:
    static FITKPolyPlacementMapper* New();
    vtkTypeMacro(FITKPolyPlacementMapper, vtkMapper2D);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    /**
     * Draw non-overlapping labels to the screen.
     */
    void RenderOverlay(vtkViewport* viewport, vtkActor2D* actor) override;

    //@{
    /**
     * The size of the point's mark need to be drawn.
     */
    //vtkSetMacro(PixelSize, int);
    //vtkGetMacro(PixelSize, int);

    /**
     * Set the poly data label rendering strategy.
     */
    virtual void SetRenderStrategy(FITKPolyRenderStrategy* s);
    vtkGetObjectMacro(RenderStrategy, FITKPolyRenderStrategy);
    //@}

    //@{
    /**
     * The maximum fraction of the screen that the labels may cover.
     * Label placement stops when this fraction is reached.
     */
    vtkSetClampMacro(MaximumLabelFraction, double, 0., 1.);
    vtkGetMacro(MaximumLabelFraction, double);
    //@}

    //@{
    /**
     * The type of iterator used when traversing the labels.
     * May be vtkLabelHierarchy::FRUSTUM or vtkLabelHierarchy::FULL_SORT
     */
    vtkSetMacro(IteratorType, int);
    vtkGetMacro(IteratorType, int);
    //@}

    //@{
    /**
     * Use label anchor point coordinates as normal vectors and eliminate those
     * pointing away from the camera. Valid only when points are on a sphere
     * centered at the origin (such as a 3D geographic view). Off by default.
     */
    vtkGetMacro(PositionsAsNormals, bool);
    vtkSetMacro(PositionsAsNormals, bool);
    vtkBooleanMacro(PositionsAsNormals, bool);
    //@}

    //@{
    /**
     * Enable drawing spokes (lines) to anchor point coordinates that were perturbed
     * for being coincident with other anchor point coordinates.
     */
    vtkGetMacro(GeneratePerturbedLabelSpokes, bool);
    vtkSetMacro(GeneratePerturbedLabelSpokes, bool);
    vtkBooleanMacro(GeneratePerturbedLabelSpokes, bool);
    //@}

    //@{
    /**
     * Use the depth buffer to test each label to see if it should not be displayed if
     * it would be occluded by other objects in the scene. Off by default.
     */
    vtkGetMacro(UseDepthBuffer, bool);
    vtkSetMacro(UseDepthBuffer, bool);
    vtkBooleanMacro(UseDepthBuffer, bool);
    //@}

    //@{
    /**
     * Tells the placer to place every label regardless of overlap.
     * Off by default.
     */
    vtkSetMacro(PlaceAllLabels, bool);
    vtkGetMacro(PlaceAllLabels, bool);
    vtkBooleanMacro(PlaceAllLabels, bool);
    //@}

    //@{
    /**
     * Whether to render traversed bounds. Off by default.
     */
    vtkSetMacro(OutputTraversedBounds, bool);
    vtkGetMacro(OutputTraversedBounds, bool);
    vtkBooleanMacro(OutputTraversedBounds, bool);
    //@}

    //@{
    /**
     * Get the transform for the anchor points.
     */
    vtkGetObjectMacro(AnchorTransform, vtkCoordinate);
    //@}

    /**
     * Release any graphics resources that are being consumed by this mapper.
     * The parameter window could be used to determine which graphic
     * resources to release.
     */
    void ReleaseGraphicsResources(vtkWindow*) override;

protected:
    FITKPolyPlacementMapper();
    ~FITKPolyPlacementMapper() override;

#if VTK_MAJOR_VERSION < 9
    void GetAnchorFrustumPlanes(double frustumPlanes[24], vtkRenderer* ren, vtkCoordinate* anchorTransform);
#endif

    virtual void SetAnchorTransform(vtkCoordinate*);

    int FillInputPortInformation(int port, vtkInformation* info) override;

    class Internal;
    Internal* Buckets;

    // int PixelSize = 20;
    FITKPolyRenderStrategy* RenderStrategy{ nullptr };

    vtkCoordinate* AnchorTransform;
    vtkSelectVisiblePoints* VisiblePoints;
    double MaximumLabelFraction;
    bool PositionsAsNormals;
    bool GeneratePerturbedLabelSpokes;
    bool UseDepthBuffer;
    bool PlaceAllLabels;
    bool OutputTraversedBounds;

    int LastRendererSize[2];
    double LastCameraPosition[3];
    double LastCameraFocalPoint[3];
    double LastCameraViewUp[3];
    double LastCameraParallelScale;
    int IteratorType;

private:
    FITKPolyPlacementMapper(const FITKPolyPlacementMapper&) = delete;
    void operator=(const FITKPolyPlacementMapper&) = delete;
};

#endif // FITKPOLYPLACEMENTMAPPER_H
