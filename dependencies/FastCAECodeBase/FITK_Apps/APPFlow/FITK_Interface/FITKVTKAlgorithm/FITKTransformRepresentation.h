/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKTransformRepresentation.h
 * @brief       The representation for transform widget.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-10-31
 *********************************************************************/

#ifndef __FITKTransformRepresentation_H__
#define __FITKTransformRepresentation_H__

#include "vtkWidgetRepresentation.h"

#include "FITKVTKAlgorithmAPI.h"

// Forward declaration
class vtkActor;
class vtkPolyDataMapper;
class vtkLineSource;
class vtkSphereSource;
class vtkCellPicker;
class vtkProperty;
class vtkPolyData;
class vtkPoints;
class vtkPolyDataAlgorithm;
class vtkTransform;
class vtkBox;
class vtkDoubleArray;
class vtkGlyph3D;
class vtkArrowSource;
class vtkTransformFilter;
class vtkCubeSource;

/**
 * @brief       The representation for transform widget.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-10-31
 */
class FITKVTKALGORITHMAPI FITKTransformRepresentation : public vtkWidgetRepresentation
{
public:
    // The widget actor type enum.
    enum HandleActorType
    {
        HandleXPos = 0,
        HandleXNeg,
        HandleYPos,
        HandleYNeg,
        HandleZPos,
        HandleZNeg,
        HandleO,
        BoundBox
    };

    /**
     * Instantiate the class.
     */
    static FITKTransformRepresentation* New();

    void SetPosition(double* pos3);
    double* GetPosition();

    /**
     * Translate to the given position by current handle direction.     
     */
    void SetTranslateTo(double* pos);

    void GetTransform(vtkTransform* trans);

    //@{
    /**
     * Set or get the flag of reverse translation.
     */
    vtkSetMacro(EnableReverseTranslate, bool);
    vtkGetMacro(EnableReverseTranslate, bool);
    vtkBooleanMacro(EnableReverseTranslate, bool);
    //@}

    //@{
    /**
     * Set or get the control flag of handle position.
     */
    vtkSetMacro(HandleAtCenter, bool);
    vtkGetMacro(HandleAtCenter, bool);
    vtkBooleanMacro(HandleAtCenter, bool);
    //@}

    //@{
    /**
     * Set or get the handle size factor ( default is 1.0 ).
     */
    vtkSetMacro(HandleFactor, double);
    vtkGetMacro(HandleFactor, double);
    //@}

    //@{
    /**
     * Standard methods for the class.
     */
    vtkTypeMacro(FITKTransformRepresentation, vtkWidgetRepresentation);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    //@}

    //@{
    /**
     * Get the handle properties (the little balls are the handles). The
     * properties of the handles, when selected or normal, can be
     * specified.
     */
    vtkProperty* GetActorProperty(HandleActorType type);
    vtkGetObjectMacro(SelectedHandleProperty, vtkProperty);
    //@}

    //@{
    /**
     * Switches handles (the arrows and sphere) on or off by manipulating the underlying
     * actor visibility.
     */
    virtual void HandlesOn();
    virtual void HandlesOff();
    //@}

    //@{
    /**
     * Switches boundary box on or off by manipulating the underlying
     * actor visibility.
     */
    virtual void BoundBoxOn();
    virtual void BoundBoxOff();
    //@}

    //@{
    /**
     * These are methods that satisfy vtkWidgetRepresentation's API.
     */
    void PlaceWidget(double bounds[6]) override;
    void BuildRepresentation() override;
    int ComputeInteractionState(int X, int Y, int modify = 0) override;
    void StartWidgetInteraction(double e[2]) override;
    void WidgetInteraction(double e[2]) override;
    double* GetBounds() VTK_SIZEHINT(6) override;
    void ComplexInteraction(vtkRenderWindowInteractor* iren, vtkAbstractWidget* widget,
        unsigned long event, void* calldata) override;
    int ComputeComplexInteractionState(vtkRenderWindowInteractor* iren, vtkAbstractWidget* widget,
        unsigned long event, void* calldata, int modify = 0) override;
    void EndComplexInteraction(vtkRenderWindowInteractor* iren, vtkAbstractWidget* widget,
        unsigned long event, void* calldata) override;
    //@}

    //@{
    /**
     * Methods supporting, and required by, the rendering process.
     */
    void ReleaseGraphicsResources(vtkWindow*) override;
    int RenderOpaqueGeometry(vtkViewport*) override;
    int RenderTranslucentPolygonalGeometry(vtkViewport*) override;
    vtkTypeBool HasTranslucentPolygonalGeometry() override;
    //@}

    // Used to manage the state of the widget
    enum
    {
        Outside = 0,
        TranslatingXPos,
        TranslatingXNeg,
        TranslatingYPos,
        TranslatingYNeg,
        TranslatingZPos,
        TranslatingZNeg,
        TranslatingO
    };

    /**
     * The interaction state may be set from a widget or
     * other object. This controls how the interaction with the widget
     * proceeds. Normally this method is used as part of a handshaking
     * process with the widget: First ComputeInteractionState() is invoked that
     * returns a state based on geometric considerations (i.e., cursor near a
     * widget feature), then based on events, the widget may modify this
     * further.
     */
    void SetInteractionState(int state);

    /*
     * Register internal Pickers within PickingManager
     */
    void RegisterPickers() override;

protected:
    FITKTransformRepresentation();
    ~FITKTransformRepresentation() override;

    bool EnableReverseTranslate;
    bool HandleAtCenter;
    double HandleFactor;

    void SetPosition(double* pos3, double size);

    double Position[3]{ 0., 0., 0. };

    double EventPos[4]{ 0., 0., 0., 1. };

    // Manage how the representation appears
    double LastEventPosition[3];
    double LastEventOrientation[4];
    double StartEventOrientation[4];

    vtkCubeSource* BdBox;
    vtkPolyDataMapper* BdBoxMapper;
    vtkActor* BdBoxActor;

    vtkPoints** Points;
    vtkPolyData** PolyData;
    vtkDoubleArray** Normals;
    vtkGlyph3D** HandleGlyph3D;

    vtkArrowSource* HandleSource;
    vtkSphereSource* CenterSource;

    // glyphs representing hot spots (e.g., handles)
    vtkActor** Handle;
    vtkPolyDataMapper** HandleMapper;
    int HighlightHandle(vtkProp* prop); // returns cell id
    virtual void SizeHandles();

    // Do the picking
    vtkCellPicker* HandlePicker;
    vtkActor* CurrentHandle;

    // The internal transform
    vtkTransform* Transform;

    // Support GetBounds() method
    vtkBox* BoundingBox;

    // Properties used to control the appearance of selected objects and
    // the manipulator in general.
    vtkProperty** HandleProperty;
    vtkProperty* SelectedHandleProperty;
    virtual void CreateDefaultProperties();

    // Helper methods
    virtual void Translate(const double* p1, const double* p2);

private:
    FITKTransformRepresentation(const FITKTransformRepresentation&) = delete;
    void operator=(const FITKTransformRepresentation&) = delete;
};

#endif // !__FITKTransformRepresentation_H__
