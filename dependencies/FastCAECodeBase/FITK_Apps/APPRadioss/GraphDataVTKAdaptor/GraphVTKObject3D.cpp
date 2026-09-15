/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObject3D.h"

// VTK
#include <vtkObject.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkIntArray.h>
#include <vtkScalarBarActor.h>

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
    GraphVTKObject3D::GraphVTKObject3D(Core::FITKAbstractDataObject* dataObj)
        : Comp::FITKGraphObjectVTK(dataObj)
    {
        if (dataObj){
            connect(dataObj, &Core::FITKAbstractNDataObject::dataObjectDestoried, this, &GraphVTKObject3D::sig_needToBeDeleted);
        }

        //在形状信息中存储数据对象id
        if (dataObj){
            _shapeInfo.DataObjId = dataObj->getDataObjectID();
        }
    }

    GraphVTKObject3D::~GraphVTKObject3D()
    {
        deleteObjs(m_addinGraphObjList);
    }

    void GraphVTKObject3D::setTransparent(bool isOn)
    {
        // Override in sub-class.
        Q_UNUSED(isOn);
    }

    void GraphVTKObject3D::setColor(QColor color)
    {
        // Override in sub-class.
        Q_UNUSED(color);
    }

    void GraphVTKObject3D::updatePointSize(int size)
    {
        Q_UNUSED(size);
    }

    void GraphVTKObject3D::updateLineWidth(int width)
    {
        Q_UNUSED(width);
    }

    void GraphVTKObject3D::updateSHPSize(int size)
    {
        Q_UNUSED(size);
    }

    void GraphVTKObject3D::updateSubColor(QColor color, QVariant otherPar)
    {
        Q_UNUSED(color);
        Q_UNUSED(otherPar);
    }

    void GraphVTKObject3D::updateModelColor(QColor color)
    {
        Q_UNUSED(color);
    }

    void GraphVTKObject3D::updateOpacity(int element, double opacity)
    {
        Q_UNUSED(element);
        Q_UNUSED(opacity);
    }

    void GraphVTKObject3D::updateRepresentation(int arg)
    {
        Q_UNUSED(arg);
    }

    void GraphVTKObject3D::setPickMode(GraphVTKCommons::ShapePickMode mode)
    {
        _pickMode = mode;
    }

    int GraphVTKObject3D::getShapeIdByVTKCellId(int vtkCellId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        Q_UNUSED(vtkCellId);
        Q_UNUSED(pickType);
        Q_UNUSED(otherInfo);
        return 0;
    }

    const QVector<int> GraphVTKObject3D::getVTKCellIdsByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        Q_UNUSED(shapeId);
        Q_UNUSED(pickType);
        Q_UNUSED(otherInfo);
        return QVector<int>();
    }

    int GraphVTKObject3D::getVirTopoIndexByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType)
    {
        Q_UNUSED(shapeId);
        Q_UNUSED(pickType);
        return 0;
    }

    vtkDataSet * GraphVTKObject3D::getMesh(GraphVTKCommons::ShapePickMode pickType)
    {
        Q_UNUSED(pickType);
        return nullptr;
    }

    bool GraphVTKObject3D::getVisibility()
    {
        if (_dataObj) {
            return _dataObj->isEnable();
        }
        return false;
    }

    void GraphVTKObject3D::updateVisibility()
    {

    }

    bool GraphVTKObject3D::isHighlighting()
    {
        return _isHighlighting;
    }

    int GraphVTKObject3D::getRenderLayer()
    {
        return _renderLayer;
    }

    void GraphVTKObject3D::setRenderLayer(int layer)
    {
        _renderLayer = layer;
    }

    vtkRenderer * GraphVTKObject3D::getRenderer()
    {
        return _render;
    }

    void GraphVTKObject3D::setRenderer(vtkRenderer * render)
    {
        _render = render;
    }

    int GraphVTKObject3D::getHightlightRenderLayer()
    {
        return 1;
    }

    void GraphVTKObject3D::preHighlight()
    {

    }

    void GraphVTKObject3D::highlight(QColor color)
    {
        Q_UNUSED(color);

        //判断对象是否可见
        if (!getDataVisibility()){
            return;
        }
        _isHighlighting = true;
    }

    void GraphVTKObject3D::disHighlight()
    {
        _isHighlighting = false;
    }

    void GraphVTKObject3D::setVisible(bool visibility)
    {
        for (vtkProp* prop : m_actorList)
        {
            if (prop)
            {
                prop->SetVisibility(visibility);
            }
        }
    }

    void GraphVTKObject3D::setCellVisible(bool visibility)
    {
        Q_UNUSED(visibility);
    }

    void GraphVTKObject3D::setPointVisibility(bool visibility)
    {
        Q_UNUSED(visibility);
    }

    void GraphVTKObject3D::setLineVisibility(bool visibility)
    {
        Q_UNUSED(visibility);
    }

    void GraphVTKObject3D::update(bool forceUpdate)
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

    int GraphVTKObject3D::getDataId()
    {
        if (!_dataObj)
        {
            return -1;
        }

        return _dataObj->getDataObjectID();
    }

    GraphVTKCommons::ShapeInfo GraphVTKObject3D::getShapeInfo()
    {
        return _shapeInfo;
    }

    void GraphVTKObject3D::addAddinGraphObject(GraphVTKObject3D* obj)
    {
        if (!obj)
        {
            return;
        }

        if (m_addinGraphObjList.contains(obj))
        {
            return;
        }

        m_addinGraphObjList.push_back(obj);
    }

    QList<GraphVTKObject3D*> GraphVTKObject3D::getAddinGraphObjs()
    {
        return m_addinGraphObjList;
    }


    void GraphVTKObject3D::removeFromGraphWidget()
    {
        // Remove self.
        this->Superclass::removeFromGraphWidget();

        // Remove addin graph objects.
        for (GraphVTKObject3D* objAddin : m_addinGraphObjList)
        {
            if (!objAddin)
            {
                continue;
            }

            objAddin->removeFromGraphWidget();
        }
    }

    bool GraphVTKObject3D::contains(vtkProp * actor)
    {
        if (actor == nullptr)return false;

        bool isSelf = m_actorList.contains(actor);
        if (isSelf){
            return true;
        }

        //// Check if the actor is one of the additional graph's object.
        ////@{
        //bool isAddin = false;
        //for (FITKFluidVTKGraphObject3D* obj : m_addinGraphObjList)
        //{
        //    if (!obj)
        //    {
        //        continue;
        //    }

        //    isAddin |= obj->contains(actor);
        //}

        //if (isAddin)
        //{
        //    return true;
        //}
        ////@}

        return false;
    }

    void GraphVTKObject3D::setDataCase(int dataCaseId)
    {
        _dataCaseId = dataCaseId;
    }

    void GraphVTKObject3D::setStepId(int stepID)
    {
        _stepID = stepID;
    }

    vtkScalarBarActor* GraphVTKObject3D::getScalarBarActor()
    {
        return nullptr;
    }

    void GraphVTKObject3D::changeField(int type, QString fieldName, int compIndex)
    {
        Q_UNUSED(type);
        Q_UNUSED(fieldName);
        Q_UNUSED(compIndex);
    }

    void GraphVTKObject3D::updateTextInfo(const QString &info)
    {
        Q_UNUSED(info);
    }

    void GraphVTKObject3D::setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency)
    {
        Q_UNUSED(isOpenOutRanger);
        Q_UNUSED(minRangeColor);
        Q_UNUSED(maxRangeColor);
        Q_UNUSED(transparency);
    }

    void GraphVTKObject3D::getRangeColor(bool isOutRanger, QColor & minRangeColor, QColor & maxRangeColor)
    {
        Q_UNUSED(isOutRanger);
        Q_UNUSED(minRangeColor);
        Q_UNUSED(maxRangeColor);
    }
    void GraphVTKObject3D::setRange(double * range)
    {
        Q_UNUSED(range);
    }
    double * GraphVTKObject3D::getRange()
    {
        return nullptr;
    }
    void GraphVTKObject3D::setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum)
    {
        Q_UNUSED(isOpenOutRanger);
        Q_UNUSED(aboveRGBA);
        Q_UNUSED(belowRGBA);
        Q_UNUSED(colorNum);
    }

    void GraphVTKObject3D::setRangerColorSetFlag(bool isSet)
    {
        Q_UNUSED(isSet);
    }

    bool GraphVTKObject3D::getRangerColorSetFlag()
    {
        return false;
    }

    GraphVTKObject3D::GraphVTKObject3D()
        :FITKGraphObjectVTK()
    {
        // Nothing to do here.
    }

    void GraphVTKObject3D::initActorProperties(vtkProp * actor, QVariant details)
    {
        Q_UNUSED(actor);
        Q_UNUSED(details);
    }

    bool GraphVTKObject3D::getDataVisibility()
    {
        /*if (_dataObj == nullptr){
            return false;
        }*/

        return _dataObj->isEnable();
    }

    void GraphVTKObject3D::setLightPropertiesForSurface(vtkActor* fActor)
    {
        if (fActor == nullptr){
            return;
        }

        fActor->GetProperty()->SetAmbient(0.2);
        fActor->GetProperty()->SetDiffuse(0.8);
        fActor->GetProperty()->SetSpecular(0.2);
        fActor->GetProperty()->SetSpecularPower(64.);
    }

}