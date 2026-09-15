/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectModelBase.h"
#include "GraphVTKObjectHighlighting.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
// VTK
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkThreshold.h>
#include <vtkDataObject.h>
namespace Exchange {

    GraphVTKObjectModelBase::GraphVTKObjectModelBase(Core::FITKAbstractDataObject* dataObj)
        : GraphVTKObject3D(dataObj)
    {
        // 设置是否有固定边界
        m_hasFixedBounds = true;

        init();
    }
    vtkThreshold* GraphVTKObjectModelBase::createOrphanMeshThreshold(int type, vtkAlgorithmOutput* inputConn, bool saved)
    {
        // Initialize the threshold filter for orphan mesh.
        vtkThreshold* threshold = vtkThreshold::New();
        if (inputConn)
        {
            threshold->SetInputConnection(inputConn);
        }

        if (saved)
        {
            m_orphanThresholds.push_back(threshold);
        }

        // Extract only orphan mesh by default.
#if VTK_MAJOR_VERSION > 9 || (VTK_MAJOR_VERSION == 9 && VTK_MINOR_VERSION >= 1)
        threshold->SetLowerThreshold(m_nativeMeshVisibility ? 0. : 1.);
        threshold->SetUpperThreshold(1.);
#else
        threshold->ThresholdBetween(m_nativeMeshVisibility ? 0. : 1., 1.);
#endif

        threshold->SetSelectedComponent(0);
        threshold->SetAllScalars(false);

        QString tagName = Interface::FITKUnstructuredMeshVTK::GetOrphanMeshTagName();

        if (type == 0)
        {
            threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
                QStringToCharA(tagName));
        }
        else
        {
            threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS,
                QStringToCharA(tagName));
        }

        return threshold;
    }

    void GraphVTKObjectModelBase::init()
    {
        // 创建高亮对象
        m_graphObjHighlight = new GraphVTKObjectHighlighting(_dataObj);

        if (m_graphObjHighlight)
        {
            m_graphObjHighlight->setRepresentation(1);
            m_graphObjHighlight->setColor(GraphVTKCommons::s_highlightColor);
            m_graphObjHighlight->setVisible(false);
            addAddinGraphObject(m_graphObjHighlight);
        }
    }

    void GraphVTKObjectModelBase::highlight(QColor color)
    {
        if (!m_graphObjHighlight || !getDataVisibility())
            return;

        m_graphObjHighlight->highlight(color);
        this->Superclass::highlight(color);
    }

    void GraphVTKObjectModelBase::disHighlight()
    {
        if (m_graphObjHighlight && _isHighlighting)
            m_graphObjHighlight->disHighlight();
        this->Superclass::disHighlight();
    }

}