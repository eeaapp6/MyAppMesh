/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObject3D.h"

// OCC
#include <AIS_InteractiveObject.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>

// Data
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKOCCGraphAdaptor, FITKOCCGraphAdaptorVersion);

namespace Exchange
{
    FITKOCCGraphObject3D::FITKOCCGraphObject3D(Core::FITKAbstractDataObject* dataObj)
        : Render::FITKGraphObjectOCC(dataObj)
    {
        // Initialize the visibility hash.
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Shade] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Wireframe] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Vertex] = true;

        // Save the data object id.
        if (dataObj)
        {
            m_graphInfo.DataObjId = dataObj->getDataObjectID();
        }
    }      
    
    FITKOCCGraphObject3D::~FITKOCCGraphObject3D()
    {
        // Clear the information.
        m_userInfo.clear();
    }

    void FITKOCCGraphObject3D::enableTopMost(bool flag)
    {
        // Override if necessary.
        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj.IsNull())
            {
                obj->SetZLayer(flag ? Graphic3d_ZLayerId_Topmost : Graphic3d_ZLayerId_Default);
            }
        }
    }

    void FITKOCCGraphObject3D::enableTop(bool flag)
    {
        // Override if necessary.
        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj.IsNull())
            {
                obj->SetZLayer(flag ? Graphic3d_ZLayerId_Top : Graphic3d_ZLayerId_Default);
            }
        }
    }

    int FITKOCCGraphObject3D::getShapeIndex(FITKOCCCommons::ShapeMeshType type, TopoDS_Shape & shape)
    {
        // Override in sub-class.
        Q_UNUSED(type);
        Q_UNUSED(shape);
        return -1;
    }

    TopoDS_Shape FITKOCCGraphObject3D::getIndexedShape(FITKOCCCommons::ShapeMeshType type, int index)
    {
        // Override in sub-class.
        Q_UNUSED(type);
        Q_UNUSED(index);
        return TopoDS_Shape();
    }

    FITKOCCCommons::GraphInfo FITKOCCGraphObject3D::getGraphInfo()
    {
        return m_graphInfo;
    }

    int FITKOCCGraphObject3D::getDataId()
    {
        if (_dataObj)
        {
            return _dataObj->getDataObjectID();
        }

        return -1;
    }

    Core::FITKAbstractDataObject* FITKOCCGraphObject3D::getDataObj()
    {
        return _dataObj;
    }

    void FITKOCCGraphObject3D::setVisible(bool visibility)
    {
        if (!m_context)
        {
            return;
        }

        // Show or hide all objects.
        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj || obj.IsNull())
            {
                continue;
            }

            // Show or hide with the context.
            if (visibility)
            {
                m_context->Display(obj, Standard_True);
            }
            else
            {
                m_context->Erase(obj, Standard_True);
            }
        }

        // Reset select modes.
        updateSelectModes();
    }

    void FITKOCCGraphObject3D::setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible)
    {
        // Override in sub-class.
        m_viewModes[type] = visible;
    }

    void FITKOCCGraphObject3D::clearCache()
    {
        // Nothing to de here.
    }

    void FITKOCCGraphObject3D::update(bool forceUpdate)
    {
        if (!m_context)
        {
            return;
        }

        // Force update all interactive objects.
        if (forceUpdate)
        {
            for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
            {
                if (!obj)
                {
                    continue;
                }

                m_context->Redisplay(obj, Standard_True);
            }
        }

        // Update base.
        Render::FITKGraphObjectOCC::update(forceUpdate);
    }

    void FITKOCCGraphObject3D::updateVisibility()
    {
        // Override if necessary.
        // Show or hide the graph actors.
        bool visibility = getDataVisibility();
        setVisible(visibility);
    }

    void FITKOCCGraphObject3D::updateInteractiveObjs()
    {
        // Override if necessary.
    }

    void FITKOCCGraphObject3D::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index)
    {
        // Override in sub-class.
        Q_UNUSED(color);
        Q_UNUSED(type);
        Q_UNUSED(index);
    }

    void FITKOCCGraphObject3D::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice)
    {
        // Override in sub-class.
        Q_UNUSED(color);
        Q_UNUSED(type);
        Q_UNUSED(indice);
    }

    void FITKOCCGraphObject3D::setColor(QColor color)
    {
        // Override in sub-class.
        Q_UNUSED(color);
    }

    void FITKOCCGraphObject3D::setEnableTransparency(bool isOn)
    {
        // Override in sub-class.
        Q_UNUSED(isOn);
    }

    void FITKOCCGraphObject3D::setPointMark(FITKOCCCommons::PointMarkShape type)
    {
        // Override in sub-class.
        Q_UNUSED(type);
    }

    void FITKOCCGraphObject3D::setPointSizeScale(double factor)
    {
        // Override in sub-class.
        Q_UNUSED(factor);
    }

    void FITKOCCGraphObject3D::setLineWidth(double size)
    {
        // Override in sub-class.
        Q_UNUSED(size);
    }

    void FITKOCCGraphObject3D::highlight(bool update)
    {
        // Override in sub-class.
        Q_UNUSED(update);

        if (!m_context)
        {
            return;
        }

        // Save the highlight flag.
        m_isHighlighting = true;
    }

    void FITKOCCGraphObject3D::disHighlight(bool update)
    {
        // Override in sub-class.
        Q_UNUSED(update);

        if (!m_context)
        {
            return;
        }

        // Save the highlight flag.
        m_isHighlighting = false;
    }
    
    bool FITKOCCGraphObject3D::isHighlighting()
    {
        return m_isHighlighting;
    }

    void FITKOCCGraphObject3D::setInputVariantData(QVariant value, int inputRole)
    {
        // Deal with the value in sub-class.
        // The roles have been defined in FITKOCCCommons.
        if (inputRole == -1)
        {
            inputRole = m_defaultInputRole;
        }
        else
        {
            if (inputRole > 1999 || inputRole < 1000)
            {
                return;
            }
        }

        m_userInfo[inputRole] = value;
    }

    QVariant FITKOCCGraphObject3D::getOutputVariantData(int outputRole)
    {
        if (outputRole == -1)
        {
            outputRole = m_defaultOutputRole;
        }

        if (!m_userInfo.contains(outputRole))
        {
            return QVariant();
        }

        return m_userInfo[outputRole];
    }

    void FITKOCCGraphObject3D::setUserData(int role, QVariant value)
    {
        // User information cannot set to the input and output role.
        if (role == m_defaultInputRole || role == m_defaultOutputRole)
        {
            return;
        }

        if (role < 0)
        {
            return;
        }

        m_userInfo[role] = value;
    }

    QVariant FITKOCCGraphObject3D::getUserData(int role)
    {
        if (!m_userInfo.contains(role) && role >= 0)
        {
            return QVariant();
        }

        return m_userInfo[role];
    }

    bool FITKOCCGraphObject3D::test(QVariant& details)
    {
        // Nothing to do here.
        Q_UNUSED(details);
        return true;
    }

    void FITKOCCGraphObject3D::enableTopMost(AIS_InteractiveObject* obj, bool flag)
    {
        if (obj)
        {
            obj->SetZLayer(flag ? Graphic3d_ZLayerId_Topmost : Graphic3d_ZLayerId_Default);
        }
    }

    void FITKOCCGraphObject3D::setOutputVariantData(QVariant value, int outputRole)
    {
        if (outputRole < m_defaultInputRole || outputRole >= m_defaultTempRole)
        {
            return;
        }

        m_userInfo[outputRole] = value;
    }

    void FITKOCCGraphObject3D::setTempVariantData(QVariant value, int role)
    {
        if (role < m_defaultTempRole)
        {
            return;
        }

        m_userInfo[role] = value;
    }

    QVariant FITKOCCGraphObject3D::getTempVariantData(int role)
    {
        if (role < m_defaultTempRole)
        {
            return QVariant();
        }

        return m_userInfo[role];
    }
}   // namespace Exchange