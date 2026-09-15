/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTransformWidget.h"

#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkEvent.h"
#include "vtkEventData.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkWidgetCallbackMapper.h"
#include "vtkWidgetEvent.h"
#include "vtkWidgetEventTranslator.h"

#include "FITKTransformRepresentation.h"

vtkStandardNewMacro(FITKTransformWidget);

//----------------------------------------------------------------------------
FITKTransformWidget::FITKTransformWidget()
{
    this->WidgetState = FITKTransformWidget::Start;
    this->ManagesCursor = 1;

    this->TranslationEnabled = true;

    // Define widget events
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent, vtkEvent::NoModifier, 0,
        0, nullptr, vtkWidgetEvent::Select, this, FITKTransformWidget::SelectAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent, vtkEvent::NoModifier,
        0, 0, nullptr, vtkWidgetEvent::EndSelect, this, FITKTransformWidget::EndSelectAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonPressEvent,
        vtkWidgetEvent::Translate, this, FITKTransformWidget::TranslateAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonReleaseEvent,
        vtkWidgetEvent::EndTranslate, this, FITKTransformWidget::EndSelectAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent,
        vtkEvent::ControlModifier, 0, 0, nullptr, vtkWidgetEvent::Translate, this,
        FITKTransformWidget::TranslateAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
        vtkEvent::ControlModifier, 0, 0, nullptr, vtkWidgetEvent::EndTranslate, this,
        FITKTransformWidget::EndSelectAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent, vtkEvent::ShiftModifier,
        0, 0, nullptr, vtkWidgetEvent::Translate, this, FITKTransformWidget::TranslateAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
        vtkEvent::ShiftModifier, 0, 0, nullptr, vtkWidgetEvent::EndTranslate, this,
        FITKTransformWidget::EndSelectAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonReleaseEvent,
        vtkWidgetEvent::EndScale, this, FITKTransformWidget::EndSelectAction);
    this->CallbackMapper->SetCallbackMethod(
        vtkCommand::MouseMoveEvent, vtkWidgetEvent::Move, this, FITKTransformWidget::MoveAction);

    {
        vtkSmartPointer<vtkEventDataButton3D> ed = vtkSmartPointer<vtkEventDataButton3D>::New();
        ed->SetDevice(vtkEventDataDevice::RightController);
        ed->SetInput(vtkEventDataDeviceInput::Trigger);
        ed->SetAction(vtkEventDataAction::Press);
        this->CallbackMapper->SetCallbackMethod(vtkCommand::Button3DEvent, ed.Get(),
            vtkWidgetEvent::Select3D, this, FITKTransformWidget::SelectAction3D);
    }

    {
        vtkSmartPointer<vtkEventDataButton3D> ed = vtkSmartPointer<vtkEventDataButton3D>::New();
        ed->SetDevice(vtkEventDataDevice::RightController);
        ed->SetInput(vtkEventDataDeviceInput::Trigger);
        ed->SetAction(vtkEventDataAction::Release);
        this->CallbackMapper->SetCallbackMethod(vtkCommand::Button3DEvent, ed.Get(),
            vtkWidgetEvent::EndSelect3D, this, FITKTransformWidget::EndSelectAction3D);
    }

    {
        vtkSmartPointer<vtkEventDataMove3D> ed = vtkSmartPointer<vtkEventDataMove3D>::New();
        ed->SetDevice(vtkEventDataDevice::RightController);
        this->CallbackMapper->SetCallbackMethod(
            vtkCommand::Move3DEvent, ed.Get(), vtkWidgetEvent::Move3D, this, FITKTransformWidget::MoveAction3D);
    }
}

//----------------------------------------------------------------------------
FITKTransformWidget::~FITKTransformWidget()
{

}

//----------------------------------------------------------------------------
void FITKTransformWidget::SetEnabled(int enabling)
{
    // int enabled = this->Enabled;

    // We do this step first because it sets the CurrentRenderer
    this->Superclass::SetEnabled(enabling);
}

//----------------------------------------------------------------------
void FITKTransformWidget::SelectAction(vtkAbstractWidget* w)
{
    // We are in a static method, cast to ourself
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    // Get the event position
    int X = self->Interactor->GetEventPosition()[0];
    int Y = self->Interactor->GetEventPosition()[1];

    // Okay, make sure that the pick is in the current renderer
    if (!self->CurrentRenderer || !self->CurrentRenderer->IsInViewport(X, Y))
    {
        self->WidgetState = FITKTransformWidget::Start;
        return;
    }

    // Begin the widget interaction which has the side effect of setting the
    // interaction state.
    double e[2];
    e[0] = static_cast<double>(X);
    e[1] = static_cast<double>(Y);
    self->WidgetRep->StartWidgetInteraction(e);
    int interactionState = self->WidgetRep->GetInteractionState();
    if (interactionState == FITKTransformRepresentation::Outside)
    {
        return;
    }

    // Test for states that involve face or handle picking here so
    // selection highlighting doesn't happen if that interaction is disabled.
    // Non-handle-grabbing transformations are tested in the "Action" methods.

    // Translation
    if ((interactionState == FITKTransformRepresentation::TranslatingO ||
        interactionState == FITKTransformRepresentation::TranslatingXPos ||
        interactionState == FITKTransformRepresentation::TranslatingXNeg ||
        interactionState == FITKTransformRepresentation::TranslatingYPos ||
        interactionState == FITKTransformRepresentation::TranslatingYNeg ||
        interactionState == FITKTransformRepresentation::TranslatingZPos ||
        interactionState == FITKTransformRepresentation::TranslatingZNeg) &&
        self->TranslationEnabled == 0)
    {
        return;
    }

    // We are definitely selected
    self->WidgetState = FITKTransformWidget::Active;
    self->GrabFocus(self->EventCallbackCommand);

    // The SetInteractionState has the side effect of highlighting the widget
    reinterpret_cast<FITKTransformRepresentation*>(self->WidgetRep)->SetInteractionState(interactionState);

    // start the interaction
    self->EventCallbackCommand->SetAbortFlag(1);
    self->StartInteraction();
    self->InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
    self->Render();
}

//-------------------------------------------------------------------------
void FITKTransformWidget::SelectAction3D(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    // We want to compute an orthogonal vector to the plane that has been selected
    int interactionState = self->WidgetRep->ComputeComplexInteractionState(
        self->Interactor, self, vtkWidgetEvent::Select3D, self->CallData);

    if (interactionState == FITKTransformRepresentation::Outside)
    {
        return;
    }

    // Test for states that involve face or handle picking here so
    // selection highlighting doesn't happen if that interaction is disabled.
    // Non-handle-grabbing transformations are tested in the "Action" methods.

    // Translation
    if ((interactionState == FITKTransformRepresentation::TranslatingO ||
        interactionState == FITKTransformRepresentation::TranslatingXPos ||
        interactionState == FITKTransformRepresentation::TranslatingXNeg ||
        interactionState == FITKTransformRepresentation::TranslatingYPos ||
        interactionState == FITKTransformRepresentation::TranslatingYNeg ||
        interactionState == FITKTransformRepresentation::TranslatingZPos ||
        interactionState == FITKTransformRepresentation::TranslatingZNeg) &&
        self->TranslationEnabled == 0)
    {
        return;
    }

    // We are definitely selected
    if (!self->Parent)
    {
        self->GrabFocus(self->EventCallbackCommand);
    }

    self->WidgetState = FITKTransformWidget::Active;
    self->WidgetRep->StartComplexInteraction(
        self->Interactor, self, vtkWidgetEvent::Select3D, self->CallData);

    self->EventCallbackCommand->SetAbortFlag(1);
    self->StartInteraction();
    self->InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
}

//----------------------------------------------------------------------
void FITKTransformWidget::TranslateAction(vtkAbstractWidget* w)
{
    // We are in a static method, cast to ourself
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    if (self->TranslationEnabled == 0)
    {
        return;
    }

    // Get the event position
    int X = self->Interactor->GetEventPosition()[0];
    int Y = self->Interactor->GetEventPosition()[1];

    // Okay, make sure that the pick is in the current renderer
    if (!self->CurrentRenderer || !self->CurrentRenderer->IsInViewport(X, Y))
    {
        self->WidgetState = FITKTransformWidget::Start;
        return;
    }

    // Begin the widget interaction which has the side effect of setting the
    // interaction state.
    double e[2];
    e[0] = static_cast<double>(X);
    e[1] = static_cast<double>(Y);
    self->WidgetRep->StartWidgetInteraction(e);
    int interactionState = self->WidgetRep->GetInteractionState();
    if (interactionState == FITKTransformRepresentation::Outside)
    {
        return;
    }

    // We are definitely selected
    self->WidgetState = FITKTransformWidget::Active;
    self->GrabFocus(self->EventCallbackCommand);
    reinterpret_cast<FITKTransformRepresentation*>(self->WidgetRep)
        ->SetInteractionState(FITKTransformRepresentation::TranslatingO);

    // start the interaction
    self->EventCallbackCommand->SetAbortFlag(1);
    self->StartInteraction();
    self->InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
    self->Render();
}

//----------------------------------------------------------------------
void FITKTransformWidget::MoveAction(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    // See whether we're active
    if (self->WidgetState == FITKTransformWidget::Start)
    {
        return;
    }

    // compute some info we need for all cases
    int X = self->Interactor->GetEventPosition()[0];
    int Y = self->Interactor->GetEventPosition()[1];

    // Okay, adjust the representation
    double e[2];
    e[0] = static_cast<double>(X);
    e[1] = static_cast<double>(Y);
    self->WidgetRep->WidgetInteraction(e);

    // moving something
    self->EventCallbackCommand->SetAbortFlag(1);
    self->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
    self->Render();
}

//----------------------------------------------------------------------
void FITKTransformWidget::MoveAction3D(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    // See whether we're active
    if (self->WidgetState == FITKTransformWidget::Start)
    {
        return;
    }

    // Okay, adjust the representation
    self->WidgetRep->ComplexInteraction(
        self->Interactor, self, vtkWidgetEvent::Move3D, self->CallData);

    // moving something
    self->EventCallbackCommand->SetAbortFlag(1);
    self->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
}

//----------------------------------------------------------------------
void FITKTransformWidget::EndSelectAction(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);
    if (self->WidgetState == FITKTransformWidget::Start)
    {
        return;
    }

    // Return state to not active
    self->WidgetState = FITKTransformWidget::Start;
    reinterpret_cast<FITKTransformRepresentation*>(self->WidgetRep)
        ->SetInteractionState(FITKTransformRepresentation::Outside);
    self->ReleaseFocus();

    self->EventCallbackCommand->SetAbortFlag(1);
    self->EndInteraction();
    self->InvokeEvent(vtkCommand::EndInteractionEvent, nullptr);
    self->Render();
}

//----------------------------------------------------------------------
void FITKTransformWidget::EndSelectAction3D(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    if (self->WidgetState != FITKTransformWidget::Active ||
        self->WidgetRep->GetInteractionState() == FITKTransformRepresentation::Outside)
    {
        return;
    }

    // Return state to not selected
    self->WidgetRep->EndComplexInteraction(
        self->Interactor, self, vtkWidgetEvent::Select3D, self->CallData);

    self->WidgetState = FITKTransformWidget::Start;
    if (!self->Parent)
    {
        self->ReleaseFocus();
    }

    self->EventCallbackCommand->SetAbortFlag(1);
    self->EndInteraction();
    self->InvokeEvent(vtkCommand::EndInteractionEvent, nullptr);
}

//----------------------------------------------------------------------
void FITKTransformWidget::StepAction3D(vtkAbstractWidget* w)
{
    FITKTransformWidget* self = reinterpret_cast<FITKTransformWidget*>(w);

    // We want to compute an orthogonal vector to the plane that has been selected
    int interactionState = self->WidgetRep->ComputeComplexInteractionState(
        self->Interactor, self, vtkWidgetEvent::Select3D, self->CallData);

    if (interactionState == FITKTransformRepresentation::Outside)
    {
        return;
    }

    // self->WidgetRep->SetInteractionState(FITKTransformRepresentation::Outside);

    // Okay, adjust the representation
    self->WidgetRep->ComplexInteraction(
        self->Interactor, self, vtkWidgetEvent::Move3D, self->CallData);

    // moving something
    self->EventCallbackCommand->SetAbortFlag(1);
    self->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
}

//----------------------------------------------------------------------
void FITKTransformWidget::CreateDefaultRepresentation()
{
    if (!this->WidgetRep)
    {
        this->WidgetRep = FITKTransformRepresentation::New();
    }
}

//----------------------------------------------------------------------------
void FITKTransformWidget::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Translation Enabled: " << (this->TranslationEnabled ? "On\n" : "Off\n");
}
