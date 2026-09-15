/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectHighlighting.h"

// VTK
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include <vtkProperty.h>
#include <vtkAlgorithmOutput.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkAppendFilter.h>

namespace Exchange
{
    GraphVTKObjectHighlighting::GraphVTKObjectHighlighting(Core::FITKAbstractDataObject* dataObj)
        : GraphVTKObject3D(dataObj)
    {
        m_appendFilter = vtkAppendFilter::New();
        // 创建高亮Actor并初始化属性
        m_actorHighlight = FITKGraphActor::New();
        m_actorHighlight->setGraphObject(this);
        m_actorHighlight->SetPickable(false);
        m_actorHighlight->SetVisibility(false);
        m_actorHighlight->setScalarVisibility(false);
        m_actorHighlight->setInputConnection(m_appendFilter->GetOutputPort());
        setRenderLayer(2);
        addActor(m_actorHighlight);
        // 初始化Actor属性
        m_actorHighlight->GetProperty()->SetLineWidth(GraphVTKCommons::_highlightLineWidth);
        m_actorHighlight->GetProperty()->SetColor(1.0, 0.0, 0.0);
        m_actorHighlight->GetProperty()->SetAmbient(1.0);
        m_actorHighlight->GetProperty()->SetDiffuse(0.0);
        m_actorHighlight->GetProperty()->SetSpecular(0.0);
    }

    GraphVTKObjectHighlighting::~GraphVTKObjectHighlighting()
    {
        deleteVtkObj(m_appendFilter);
    }

    void GraphVTKObjectHighlighting::setVisible(bool isVisible)
    {
        if (m_actorHighlight)
        {
            m_actorHighlight->SetVisibility(isVisible);
        }
    }

    void GraphVTKObjectHighlighting::setModelInputDataObject(vtkDataSet* inputData)
    {
        if (!inputData)
            return;
        m_actorHighlight->setInputDataObject(inputData);
    }

    void GraphVTKObjectHighlighting::setModelInputConnection(vtkAlgorithmOutput* input, int port)
    {
        if (!input || !m_appendFilter)
        {
            return;
        }

        // Remove all inputs.
        m_appendFilter->RemoveAllInputs();
        m_appendFilter->RemoveAllInputConnections(0);

        m_appendFilter->AddInputConnection(port, input);
    }

    void GraphVTKObjectHighlighting::addModelInputDataObject(vtkDataSet* dataSet)
    {
        if (!dataSet || !m_appendFilter)
        {
            return;
        }

        m_appendFilter->AddInputData(dataSet);
    }

    void GraphVTKObjectHighlighting::addModelInputConnection(vtkAlgorithmOutput* input, int port)
    {
        if (!input || !m_appendFilter)
        {
            return;
        }

        m_appendFilter->AddInputConnection(port, input);
    }

    void GraphVTKObjectHighlighting::removeModelInputDataObject(vtkDataSet* dataSet)
    {
        if (!dataSet || !m_appendFilter)
        {
            return;
        }

        m_appendFilter->RemoveInputData(dataSet);
    }

    void GraphVTKObjectHighlighting::removeModelInputConnection(vtkAlgorithmOutput* input, int port)
    {
        if (!input || !m_appendFilter)
        {
            return;
        }

        m_appendFilter->RemoveInputConnection(port, input);
    }

    void GraphVTKObjectHighlighting::clearModelInputs()
    {
        if (!m_appendFilter)
        {
            return;
        }

        // Remove all inputs.
        m_appendFilter->RemoveAllInputs();
        m_appendFilter->RemoveAllInputConnections(0);
    }

    void GraphVTKObjectHighlighting::highlight(QColor color)
    {
        if (m_actorHighlight)
        {
            if (color.isValid())
            {
                m_actorHighlight->GetProperty()->SetColor(
                    color.redF(),
                    color.greenF(),
                    color.blueF()
                );
            }
            else
            {
                m_actorHighlight->GetProperty()->SetColor(1.0, 0.0, 0.0);
            }
            setVisible(true);
            m_actorHighlight->Modified();
        }
        this->Superclass::highlight(color);
    }

    void GraphVTKObjectHighlighting::disHighlight()
    {
        if (m_actorHighlight)
        {
            m_actorHighlight->SetVisibility(false);
        }
        this->Superclass::disHighlight();
        m_actorHighlight->Modified();
    }

    void GraphVTKObjectHighlighting::setVertexVisible(bool visibility)
    {
        if (m_actorHighlight)
        {
            m_actorHighlight->GetProperty()->SetVertexVisibility(visibility);
        }
    }

    void GraphVTKObjectHighlighting::setVertexSize(double size)
    {
        if (!m_actorHighlight)
        {
            return;
        }
        m_actorHighlight->GetProperty()->SetPointSize(size);
    }

    void GraphVTKObjectHighlighting::setLineWidth(double width)
    {
        if (!m_actorHighlight)
        {
            return;
        }

        m_actorHighlight->GetProperty()->SetLineWidth(width);
    }

    void GraphVTKObjectHighlighting::setRepresentation(int rep)
    {
        if (!m_actorHighlight)
        {
            return;
        }

        int repVtk = qMax(qMin(2, rep), 0);
        m_actorHighlight->GetProperty()->SetRepresentation(repVtk);
    }

    void GraphVTKObjectHighlighting::setTransparency(double trans)
    {
        if (!m_actorHighlight)
        {
            return;
        }

        double opa = qMax(qMin(1. - trans, 1.), 0.);
        m_actorHighlight->GetProperty()->SetOpacity(opa);
    }

    void GraphVTKObjectHighlighting::setColor(QColor color)
    {
        if (!m_actorHighlight)
        {
            return;
        }

        m_actorHighlight->setColor(color);
    }
    void GraphVTKObjectHighlighting::setActorType(int type)
    {
        if (!m_actorHighlight)
        {
            return;
        }
        switch (type)
        {
            case -1:
                m_actorHighlight->setActorType(OtherActor);
                break;
            case 0:
                m_actorHighlight->setActorType(SolidActor);
                break;
            case 1:
                m_actorHighlight->setActorType(SurfaceActor);
                break;
            case 2:
                m_actorHighlight->setActorType(EdgeActor);
                break;
            case 3:
                m_actorHighlight->setActorType(VertexActor);
                break;
        default:
            break;
        }
        
    }
}