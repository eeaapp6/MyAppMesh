/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGraphLegendScaleActor.h
 * @brief  External vtkLegendScaleActor with FITKGraphAxisActor2D.
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-07-16
 *********************************************************************/

#ifndef FITKGRAPHLEGENDSCALEACTOR_H__
#define FITKGRAPHLEGENDSCALEACTOR_H__

#include "vtkCoordinate.h" // For vtkViewportCoordinateMacro
#include "vtkProp.h"
#include "FITKVTKAlgorithmAPI.h"
#include "FITKGraphAxisActor2D.h"

class vtkTextProperty;
class vtkPolyData;
class vtkPolyDataMapper2D;
class vtkActor2D;
class vtkTextMapper;
class vtkPoints;
class vtkCoordinate;

class FITKVTKALGORITHMAPI FITKGraphLegendScaleActor : public vtkProp
{
public:
    /**
     * Instantiate the class.
     */
    static FITKGraphLegendScaleActor* New();

    ///@{
    /**
     * Standard methods for the class.
     */
    vtkTypeMacro(FITKGraphLegendScaleActor, vtkProp);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    ///@}

    enum AttributeLocation
    {
        DISTANCE = 0,
        XY_COORDINATES = 1
    };

    ///@{
    /**
     * Specify the mode for labeling the scale axes. By default, the axes are
     * labeled with the distance between points (centered at a distance of
     * 0.0). Alternatively if you know that the view is down the z-axis; the
     * axes can be labeled with x-y coordinate values.
     */
    vtkSetClampMacro(LabelMode, int, DISTANCE, XY_COORDINATES);
    vtkGetMacro(LabelMode, int);
    void SetLabelModeToDistance() { this->SetLabelMode(DISTANCE); }
    void SetLabelModeToXYCoordinates() { this->SetLabelMode(XY_COORDINATES); }
    ///@}

    ///@{
    /**
     * Set/Get the flags that control which of the four axes to display (top,
     * bottom, left and right). By default, all the axes are displayed.
     */
    vtkSetMacro(RightAxisVisibility, vtkTypeBool);
    vtkGetMacro(RightAxisVisibility, vtkTypeBool);
    vtkBooleanMacro(RightAxisVisibility, vtkTypeBool);
    vtkSetMacro(TopAxisVisibility, vtkTypeBool);
    vtkGetMacro(TopAxisVisibility, vtkTypeBool);
    vtkBooleanMacro(TopAxisVisibility, vtkTypeBool);
    vtkSetMacro(LeftAxisVisibility, vtkTypeBool);
    vtkGetMacro(LeftAxisVisibility, vtkTypeBool);
    vtkBooleanMacro(LeftAxisVisibility, vtkTypeBool);
    vtkSetMacro(BottomAxisVisibility, vtkTypeBool);
    vtkGetMacro(BottomAxisVisibility, vtkTypeBool);
    vtkBooleanMacro(BottomAxisVisibility, vtkTypeBool);
    ///@}

    ///@{
    /**
     * Indicate whether the legend scale should be displayed or not.
     * The default is On.
     */
    vtkSetMacro(LegendVisibility, vtkTypeBool);
    vtkGetMacro(LegendVisibility, vtkTypeBool);
    vtkBooleanMacro(LegendVisibility, vtkTypeBool);
    ///@}

    ///@{
    /**
     * Convenience method that turns all the axes either on or off.
     */
    void AllAxesOn();
    void AllAxesOff();
    ///@}

    ///@{
    /**
     * Convenience method that turns all the axes and the legend scale.
     */
    void AllAnnotationsOn();
    void AllAnnotationsOff();
    ///@}

    ///@{
    /**
     * Set/Get the offset of the right axis from the border. This number is expressed in
     * pixels, and represents the approximate distance of the axes from the sides
     * of the renderer. The default is 50.
     */
    vtkSetClampMacro(RightBorderOffset, int, 5, VTK_INT_MAX);
    vtkGetMacro(RightBorderOffset, int);
    ///@}

    ///@{
    /**
     * Set/Get the offset of the top axis from the border. This number is expressed in
     * pixels, and represents the approximate distance of the axes from the sides
     * of the renderer. The default is 30.
     */
    vtkSetClampMacro(TopBorderOffset, int, 5, VTK_INT_MAX);
    vtkGetMacro(TopBorderOffset, int);
    ///@}

    ///@{
    /**
     * Set/Get the offset of the left axis from the border. This number is expressed in
     * pixels, and represents the approximate distance of the axes from the sides
     * of the renderer. The default is 50.
     */
    vtkSetClampMacro(LeftBorderOffset, int, 5, VTK_INT_MAX);
    vtkGetMacro(LeftBorderOffset, int);
    ///@}

    ///@{
    /**
     * Set/Get the offset of the bottom axis from the border. This number is expressed in
     * pixels, and represents the approximate distance of the axes from the sides
     * of the renderer. The default is 30.
     */
    vtkSetClampMacro(BottomBorderOffset, int, 5, VTK_INT_MAX);
    vtkGetMacro(BottomBorderOffset, int);
    ///@}

    ///@{
    /**
     * Get/Set the corner offset. This is the offset factor used to offset the
     * axes at the corners. Default value is 2.0.
     */
    vtkSetClampMacro(CornerOffsetFactor, double, 1.0, 10.0);
    vtkGetMacro(CornerOffsetFactor, double);
    ///@}

    ///@{
    /**
     * Set/Get the labels text properties for the legend title and labels.
     */
    vtkGetObjectMacro(LegendTitleProperty, vtkTextProperty);
    vtkGetObjectMacro(LegendLabelProperty, vtkTextProperty);
    ///@}

    ///@{
    /**
     * These are methods to retrieve the FITKGraphAxisActor2D used to represent
     * the four axes that form this representation. Users may retrieve and
     * then modify these axes to control their appearance.
     */
    vtkGetObjectMacro(RightAxis, FITKGraphAxisActor2D);
    vtkGetObjectMacro(TopAxis, FITKGraphAxisActor2D);
    vtkGetObjectMacro(LeftAxis, FITKGraphAxisActor2D);
    vtkGetObjectMacro(BottomAxis, FITKGraphAxisActor2D);
    ///@}

    ///@{
    /**
     * Standard methods supporting the rendering process.
     */
    virtual void BuildRepresentation(vtkViewport* viewport);
    void GetActors2D(vtkPropCollection*) override;
    void ReleaseGraphicsResources(vtkWindow*) override;
    int RenderOverlay(vtkViewport*) override;
    int RenderOpaqueGeometry(vtkViewport*) override;
    ///@}

protected:
    FITKGraphLegendScaleActor();
    ~FITKGraphLegendScaleActor() override;

    int LabelMode;
    int RightBorderOffset;
    int TopBorderOffset;
    int LeftBorderOffset;
    int BottomBorderOffset;
    double CornerOffsetFactor;

    // The four axes around the borders of the renderer using custom axis actors
    FITKGraphAxisActor2D* RightAxis;
    FITKGraphAxisActor2D* TopAxis;
    FITKGraphAxisActor2D* LeftAxis;
    FITKGraphAxisActor2D* BottomAxis;

    // Control the display of the axes
    vtkTypeBool RightAxisVisibility;
    vtkTypeBool TopAxisVisibility;
    vtkTypeBool LeftAxisVisibility;
    vtkTypeBool BottomAxisVisibility;

    // Support for the legend.
    vtkTypeBool LegendVisibility;
    vtkPolyData* Legend;
    vtkPoints* LegendPoints;
    vtkPolyDataMapper2D* LegendMapper;
    vtkActor2D* LegendActor;
    vtkTextMapper* LabelMappers[6];
    vtkActor2D* LabelActors[6];
    vtkTextProperty* LegendTitleProperty;
    vtkTextProperty* LegendLabelProperty;
    vtkCoordinate* Coordinate;

    vtkTimeStamp BuildTime;

private:
    FITKGraphLegendScaleActor(const FITKGraphLegendScaleActor&) = delete;
    void operator=(const FITKGraphLegendScaleActor&) = delete;
};

#endif // !FITKGRAPHLEGENDSCALEACTOR_H__