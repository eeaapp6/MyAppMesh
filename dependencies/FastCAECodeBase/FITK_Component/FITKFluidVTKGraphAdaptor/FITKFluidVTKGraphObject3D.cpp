/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObject3D.h"

// VTK
#include <vtkObject.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkIntArray.h>

// Graph 
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

// Graph widget
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Exchange
{
    FITKFluidVTKGraphObject3D::FITKFluidVTKGraphObject3D()
        :FITKGraphObjectVTK()
    {
        // Nothing to do here.
    }

    FITKFluidVTKGraphObject3D::FITKFluidVTKGraphObject3D(Core::FITKAbstractDataObject* dataObj)
        : Comp::FITKGraphObjectVTK(dataObj)
    {
        // Abstract data object class,
        // sub class of the render window interface data object.

        // Initialize the visibility hash.
        m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Shade] = true;
        m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Wireframe] = true;
        m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Vertex] = true;

        // If the data has been deleted, then delete self.
        // Do not use for now.
        // The graph object will be deleted by the graph provider,
        // when the dataObj has been deleted.
        if (dataObj)
        {
            connect(dataObj, &Core::FITKAbstractNDataObject::dataObjectDestoried, this, &FITKFluidVTKGraphObject3D::sig_needToBeDeleted);
        }

        // Save the data object id.
        if (dataObj)
        {
            m_shapeInfo.DataObjId = dataObj->getDataObjectID();
        }
    }

    FITKFluidVTKGraphObject3D::~FITKFluidVTKGraphObject3D()
    {
        // Clear all highlight first.
        disHighlight();

        // Delete the additional graph objects.
        deleteObjs(m_addinGraphObjList);
    }

    Core::FITKAbstractGraphWidget* FITKFluidVTKGraphObject3D::setConstGraphWidget(Core::FITKAbstractGraphWidget* widget)
    {
        Core::FITKAbstractGraphWidget* constWidget = getGraphWidget();
        if (constWidget)
        {
            return constWidget;
        }

        if (!widget)
        {
            return nullptr;
        }

        // Set the widget pointer to NULL if the widget has been destoryed.
        connect(widget, &Core::FITKAbstractGraphWidget::destroyed, this, [=]
        {
            m_constGraphWidget = nullptr;
        }, Qt::UniqueConnection);

        // Save the graph widget for removing actors.
        setGraphWidget(widget);
        m_constGraphWidget = widget;

        return widget;
    }

    void FITKFluidVTKGraphObject3D::setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible)
    {
        // Override in sub-class.
        m_viewModes[type] = visible;
    }

    void FITKFluidVTKGraphObject3D::setTransparent(bool isOn)
    {
        // Override in sub-class.
        Q_UNUSED(isOn);
    }

    void FITKFluidVTKGraphObject3D::setColor(QColor color, FITKFluidVTKCommons::ShapeType type, int index)
    {
        // Override in sub-class.
        Q_UNUSED(color);
        Q_UNUSED(type);
        Q_UNUSED(index);
    }

    void FITKFluidVTKGraphObject3D::setColor(QColor color)
    {
        // Override in sub-class.
        Q_UNUSED(color);
    }

    int FITKFluidVTKGraphObject3D::getNumberOf(FITKFluidVTKCommons::ShapeType type)
    {
        // Override in sub-class.
        Q_UNUSED(type);
        return 0;
    }

    void FITKFluidVTKGraphObject3D::setPickMode(FITKFluidVTKCommons::ShapePickMode mode)
    {
        // Override in sub-class.
        m_pickMode = mode;
    }

    int FITKFluidVTKGraphObject3D::getShapeIdByVTKCellId(int vtkCellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        // Override in sub-class.
        Q_UNUSED(vtkCellId);
        Q_UNUSED(topAbsShapeType);
        return -1;
    }

    const QVector<int> FITKFluidVTKGraphObject3D::getVTKCellIdsByShapeId(int shapeId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        // Override in sub-class.
        Q_UNUSED(shapeId);
        Q_UNUSED(topAbsShapeType);
        return QVector<int>();
    }

    const QVector<int> FITKFluidVTKGraphObject3D::getVTKCellIdsByVTKCellId(int cellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        // Override in sub-class.
        Q_UNUSED(cellId);
        Q_UNUSED(topAbsShapeType);
        return QVector<int>();
    }

    vtkDataSet* FITKFluidVTKGraphObject3D::getMesh(FITKFluidVTKCommons::ShapeType type)
    {
        // Override in sub-class.
        Q_UNUSED(type);
        return nullptr;
    }

    bool FITKFluidVTKGraphObject3D::isHighlighting()
    {
        return m_isHighlighting;
    }

    bool FITKFluidVTKGraphObject3D::isPreHighlighting()
    {
        return m_isPreHighlighting;
    }

    bool FITKFluidVTKGraphObject3D::isAdvancedHighlighting()
    {
        return m_isAdvHighlighting;
    }

    void FITKFluidVTKGraphObject3D::setFITKRender(Comp::FITKGraphRender* render)
    {
        Q_UNUSED(render);
    }

    void FITKFluidVTKGraphObject3D::setRenderLayer(int layer, int layerHighlight)
    {
        // Default 0.
        m_layer = layer;

        m_layerHighlight = (layerHighlight == -1) ? m_layer : layerHighlight;
    }

    int FITKFluidVTKGraphObject3D::getRenderLayer()
    {
        // Default 0, override in sub-class.
        return m_layer;
    }

    void FITKFluidVTKGraphObject3D::setRenderLayerHighlight(int layer)
    {
        // Default 0.
        m_layerHighlight = layer;
    }

    int FITKFluidVTKGraphObject3D::getRenderLayerHighlight()
    {
        // Default 0, override in sub-class.
        return m_layerHighlight;
    }

    void FITKFluidVTKGraphObject3D::initActorProperties(vtkProp* actor, QVariant details)
    {
        // Override if necessary.
        // Set the actor properties.
        Q_UNUSED(actor);
        Q_UNUSED(details);
    }

    void FITKFluidVTKGraphObject3D::preHighlight()
    {
        // If the object is invisible, then return.
        if (!getDataVisibility())
        {
            return;
        }

        // Override if necessary.
        // Pre-highlight the graph actors.
        m_isPreHighlighting = true;
    }

    void FITKFluidVTKGraphObject3D::highlight(FITKFluidVTKCommons::ShapeType type, QColor color)
    {
        Q_UNUSED(type);
        Q_UNUSED(color);

        // If the object is invisible, then return.
        if (!getDataVisibility())
        {
            return;
        }

        // Override if necessary.
        // Highlight the graph actors.
        m_isHighlighting = true;
    }

    void FITKFluidVTKGraphObject3D::disHighlight()
    {
        // Override if necessary.
        // Dis-highlight the graph actors.
        m_isHighlighting = false;
        m_isPreHighlighting = false;

        // Dis-advance-highlight at the same time.
        disAdvanceHighlight();
    }

    void FITKFluidVTKGraphObject3D::advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color)
    {
        // Override if necessary.
        // Highlight part of the graph actors.
        Q_UNUSED(type);
        Q_UNUSED(indice);
        Q_UNUSED(color);

        m_isAdvHighlighting = true;
    }

    void FITKFluidVTKGraphObject3D::disAdvanceHighlight()
    {
        // Override if necessary.
        m_isAdvHighlighting = false;
    }

    void FITKFluidVTKGraphObject3D::updateVisibility()
    {
        // Override if necessary.
        // Show or hide the graph actors.
        bool visibility = getDataVisibility();
        setVisible(visibility);
    }

    void FITKFluidVTKGraphObject3D::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        // Override if necessary.
        for (vtkProp* prop : m_actorList)
        {
            FITKGraphActor* actor = FITKGraphActor::SafeDownCast(prop);
            if (actor)
            {
                actor->update();
            }
        }
    }

    int FITKFluidVTKGraphObject3D::getDataId()
    {
        if (!_dataObj)
        {
            return -1;
        }

        return _dataObj->getDataObjectID();
    }

    FITKFluidVTKCommons::ShapeInfo FITKFluidVTKGraphObject3D::getShapeInfo()
    {
        return m_shapeInfo;
    }

    void FITKFluidVTKGraphObject3D::updateCameraNormal()
    {
        // Override if necessary.
    }

    void FITKFluidVTKGraphObject3D::setGraphSimplified(bool flag)
    {
        m_graphSimplified = flag;
    }

    void FITKFluidVTKGraphObject3D::simplified(bool enabled)
    {
        Q_UNUSED(enabled);
    }

    QList<FITKFluidVTKGraphObject3D*> FITKFluidVTKGraphObject3D::getAddinGraphObjs()
    {
        return m_addinGraphObjList;
    }

    void FITKFluidVTKGraphObject3D::setAddinInfo(QVariant info)
    {
        // Override if neccessary.
        Q_UNUSED(info);
    }

    bool FITKFluidVTKGraphObject3D::isRelatedTo(int dataObjId)
    {
        // Override if neccessary.
        // Whether the 'dataObjId' is the id of graph object's data object,
        // this function will return true.
        if (!_dataObj)
        {
            return false;
        }

        return _dataObj->getDataObjectID() == dataObjId;
    }

    void FITKFluidVTKGraphObject3D::setInputVariantData(QVariant value, int inputRole)
    {
        if (inputRole < 0 || inputRole == m_defaultOutputRole)
        {
            return;
        }

        m_userInfo[inputRole] = value;
    }

    QVariant FITKFluidVTKGraphObject3D::getOutputVariantData(int outputRole)
    {
        if (!m_userInfo.contains(outputRole) || outputRole < 0)
        {
            return QVariant();
        }

        return m_userInfo[outputRole];
    }

    void FITKFluidVTKGraphObject3D::setUserData(int role, QVariant value)
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

    QVariant FITKFluidVTKGraphObject3D::getUserData(int role)
    {
        if (!m_userInfo.contains(role) && role >= 0)
        {
            return QVariant();
        }

        return m_userInfo[role];
    }

    bool FITKFluidVTKGraphObject3D::contains(vtkProp* actor)
    {
        // Override if neccessary.
        if (!actor)
        {
            return false;
        }

        // Chekc if the actor is one of the graph's actors.
        bool isSelf = m_actorList.contains(actor);
        if (isSelf)
        {
            return true;
        }

        // Check if the actor is one of the additional graph's object.
        //@{
        bool isAddin = false;
        for (FITKFluidVTKGraphObject3D* obj : m_addinGraphObjList)
        {
            if (!obj)
            {
                continue;
            }

            isAddin |= obj->contains(actor);
        }

        if (isAddin)
        {
            return true;
        }
        //@}

        return false;
    }

    void FITKFluidVTKGraphObject3D::clearCache()
    {
        // Override if neccessary.
    }

    void FITKFluidVTKGraphObject3D::updateActors()
    {
        // Modify all actors.
        for (vtkProp* prop : m_actorList)
        {
            prop->Modified();
        }
    }

    bool FITKFluidVTKGraphObject3D::getDataVisibility()
    {
        // Check the data object.
        if (!_dataObj)
        {
            return false;
        }

        // Get the visibility from the data object.
        return _dataObj->isEnable();
    }

    bool FITKFluidVTKGraphObject3D::getParentDataVisibility(int id)
    {
        // Check the model id.
        if (id == -1)
        {
            return false;
        }

        // Check the model data.
        Core::FITKAbstractDataObject* parent = Core::FITKDataRepo::getInstance()->getDataByID(id);
        if (!parent)
        {
            return false;
        }

        return parent->isEnable();
    }

    void FITKFluidVTKGraphObject3D::setVisible(bool visibility)
    {
        // Default set all props managed by this graph object visible or invisible.
        for (vtkProp* prop : m_actorList)
        {
            if (prop)
            {
                prop->SetVisibility(visibility);
            }
        }
    }

    void FITKFluidVTKGraphObject3D::setLightPropertiesForLine(FITKGraphActor* fActor)
    {
        if (!fActor)
        {
            return;
        }

        // Line's light properties.
        //@{
//#if VTK_MAJOR_VERSION >= 9
//        fActor->GetProperty()->SetRoughness(1.);
//#endif
        fActor->GetProperty()->SetAmbient(0.2);
        fActor->GetProperty()->SetDiffuse(1.);
        fActor->GetProperty()->SetSpecular(0.7);
        fActor->GetProperty()->SetSpecularPower(64.);
        //@}
    }

    void FITKFluidVTKGraphObject3D::setLightPropertiesForSurface(FITKGraphActor* fActor)
    {
        if (!fActor)
        {
            return;
        }

        // Surface's light properties.
        //@{
//#if VTK_MAJOR_VERSION >= 9
//        fActor->GetProperty()->SetRoughness(0.2);
//#endif
        fActor->GetProperty()->SetAmbient(0.2);
        fActor->GetProperty()->SetDiffuse(0.8);
        fActor->GetProperty()->SetSpecular(0.2);
        fActor->GetProperty()->SetSpecularPower(64.);
        //@}
    }

    bool FITKFluidVTKGraphObject3D::test(QVariant& details)
    {
        // Nothing to do here.
        Q_UNUSED(details);
        return true;
    }

    //char* FITKFluidVTKGraphObject3D::QStringToCharA(QString str)
    //{
    //    return str.toUtf8().data();
    //}
}   // namespace Exchange