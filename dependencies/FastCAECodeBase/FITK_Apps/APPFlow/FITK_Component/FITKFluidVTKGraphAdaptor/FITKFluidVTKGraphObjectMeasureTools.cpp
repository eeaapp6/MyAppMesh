/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectMeasureTools.h"

// VTK
#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceWidget.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAngleWidget.h>

namespace Exchange
{
    FITKFluidVTKGraphObjectMeasureTools::FITKFluidVTKGraphObjectMeasureTools()
        : FITKFluidVTKGraphObject3D(nullptr)
    {
        // Initialize.
        init();

        // Set the layer need to be rendered.
        setRenderLayer(1);
    }

    FITKFluidVTKGraphObjectMeasureTools::~FITKFluidVTKGraphObjectMeasureTools()
    {
        // Set the pointer to NULL.
        m_currentWidget = nullptr;
    }

    void FITKFluidVTKGraphObjectMeasureTools::init()
    {
        // Initialize distance widget.
        //@{
        // Create the distance widget.
        m_distWidget = vtkDistanceWidget::New();

        // Initialize representation.
        vtkSmartPointer<vtkDistanceRepresentation2D> distRep2D = vtkSmartPointer<vtkDistanceRepresentation2D>::New();
        distRep2D->SetLabelFormat("L = %-#6.3g");
        m_distWidget->SetRepresentation(distRep2D);
        //@}

        // Initialize angle widget.
        //@{
        // Create the angle widget.
        m_angleWidget = vtkAngleWidget::New();

        // Initialize representation.
        vtkSmartPointer<vtkAngleRepresentation2D> angleRep2D = vtkSmartPointer<vtkAngleRepresentation2D>::New();
        angleRep2D->SetLabelFormat("Deg = %-#3.3g");
        m_angleWidget->SetRepresentation(angleRep2D);
        //@}

        // Disable the widget.
        m_distWidget->EnabledOff();
        m_angleWidget->EnabledOff();

        // Add to widget list.
        addWidget(m_distWidget);
        addWidget(m_angleWidget);
    }

    void FITKFluidVTKGraphObjectMeasureTools::setVisible(bool visibility)
    {
        if (!m_currentWidget)
        {
            return;
        }

        // Change the visibility( enable or disable ) for current measure widget.
        m_currentWidget->SetEnabled(visibility);
    }

    void FITKFluidVTKGraphObjectMeasureTools::setMeasureType(MeasureType type)
    {
        switch (type)
        {
        case MT_Distance:
            m_currentWidget = m_distWidget;
            break;
        case MT_Angle:
            m_currentWidget = m_angleWidget;
            break;
        case MT_None:
        default:
            m_currentWidget = nullptr;
            break;
        }

        // Show the widget.
        setVisible(true);
    }

    void FITKFluidVTKGraphObjectMeasureTools::setDisplayPosition(int index, double* pos)
    {
        if (index < 0 || index > 2 || !m_currentWidget)
        {
            return;
        }

        // The distance widget.
        if (m_currentWidget == m_distWidget)
        {
            if (index == 0)
            {
                m_distWidget->GetDistanceRepresentation()->SetPoint1DisplayPosition(pos);
            }
            else if (index == 1)
            {
                m_distWidget->GetDistanceRepresentation()->SetPoint2DisplayPosition(pos);
            }
        }
        // The angle widget.
        else if (m_currentWidget == m_angleWidget)
        {
            if (index == 0)
            {
                m_angleWidget->GetAngleRepresentation()->SetPoint1DisplayPosition(pos);
            }
            else if (index == 1)
            {
                m_angleWidget->GetAngleRepresentation()->SetCenterDisplayPosition(pos);
            }
            else if (index == 2)
            {
                m_angleWidget->GetAngleRepresentation()->SetPoint2DisplayPosition(pos);
            }
        }
    }
}   // namespace Exchange