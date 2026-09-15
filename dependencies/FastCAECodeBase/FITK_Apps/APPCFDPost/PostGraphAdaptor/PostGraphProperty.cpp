/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphProperty.h"
#include "PostGraphObjectBase.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

#include <vtkDataSetMapper.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkAssembly.h>
#include <vtkProp3DCollection.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkScalarBarWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>

namespace Interface
{
    PostGraphProperty::PostGraphProperty(PostGraphObjectBase* postGraphObj, FITKAbstractCFDPostData* postData) :
        _postGraphObj(postGraphObj), _postData(postData)
    {

    }

    PostGraphProperty::~PostGraphProperty()
    {

    }

    void PostGraphProperty::setCurrentField(FITKPostFieldType type, QString fieldName, int componentIndex /*= -1*/)
    {
        if (_postData == nullptr)return;
        QList<vtkMapper*> mappers = _postData->getMappers();
        if (mappers.isEmpty())return;

        _fieldType = type;
        _fieldName = fieldName;
        _componentIndex = componentIndex;

        //qstring转char*
        QByteArray name = fieldName.toLocal8Bit();
        char* va = name.data();

        for (vtkMapper* mapper : mappers) {
            switch (_fieldType) {
            case Interface::FITKPostFieldType::Post_None:mapper->SetScalarVisibility(false); break;
            case Interface::FITKPostFieldType::Post_Point: {
                mapper->SetScalarVisibility(true);
                mapper->SetScalarModeToUsePointFieldData();
                break;
            }
            case Interface::FITKPostFieldType::Post_Cell: {
                mapper->SetScalarVisibility(true);
                mapper->SetScalarModeToUseCellFieldData();
                break;
            }
            }

            mapper->SelectColorArray(va);
#if VTK_MAJOR_VERSION >= 8
            mapper->SetArrayComponent(componentIndex);
#else
            mapper->ColorByArrayComponent(va, componentIndex);
#endif
            double range[2] = { 0,0 };
            _postData->getRange(range, _fieldType, fieldName, componentIndex);
            if (fabs(range[0] - range[1]) < 1e-6)
            {
                if (fabs(range[0]) < 1e-6)
                {
                    range[0] = -1e-5;
                    range[1] = 1e-5;
                }
                else
                {
                    range[0] -= range[0] * 0.0001;
                    range[1] += range[1] * 0.0001;
                }

            }
            mapper->SetScalarRange(range);
            mapper->Update();

            //设置LookupTable
            vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
            if (lookupTable == nullptr) {
                continue;
            }
            if (componentIndex == -1)
            {
                lookupTable->SetVectorModeToMagnitude();
            }
            else
            {
                lookupTable->SetVectorModeToComponent();
            }

            lookupTable->SetVectorComponent(componentIndex);
            lookupTable->SetRange(range);
            lookupTable->Build();
        }

        //控制色带
        vtkScalarBarWidget* scalarBar = _postData->getScalarBarWidget();
        if (scalarBar == nullptr) {
            return;
        }
        scalarBar->GetScalarBarActor()->SetTitle(va);
        switch (_fieldType){
        case Interface::FITKPostFieldType::Post_None: scalarBar->Off();break;
        case Interface::FITKPostFieldType::Post_Point:
        case Interface::FITKPostFieldType::Post_Cell: {
            if (_isShowScalarBar)scalarBar->On();
            else scalarBar->Off();
        }
        }
    }

    void PostGraphProperty::getCurrentField(FITKPostFieldType& type, QString& filedName, int& componentIndex)
    {
        type = _fieldType;
        filedName = _fieldName;
        componentIndex = _componentIndex;
    }

    void PostGraphProperty::setCurrentDisplayType(FITKPostDisplayType type)
    {
        if (_postGraphObj == nullptr)return;
        for (vtkProp* a : _postGraphObj->getActors()) {
            if (a == nullptr)continue;
            vtkActor* actor = dynamic_cast<vtkActor*>(a);
            if (actor == nullptr)continue;
            vtkProperty* property = actor->GetProperty();
            switch (type) {
            case Interface::FITKPostDisplayType::Post_Point:property->SetRepresentationToPoints(); break;
            case Interface::FITKPostDisplayType::Post_Edge:property->SetRepresentationToWireframe(); break;
            case Interface::FITKPostDisplayType::Post_Face:property->SetRepresentationToSurface(); property->SetEdgeVisibility(false); break;
            case Interface::FITKPostDisplayType::Post_FaceWithEdge:property->SetRepresentationToSurface(); property->SetEdgeVisibility(true); break;
            }
        }
    }

    FITKPostDisplayType PostGraphProperty::getCurrentDisplayType()
    {
        return _displayType;
    }

    void PostGraphProperty::hideScalarBarWidget()
    {
        vtkScalarBarWidget* scalarBar = _postData->getScalarBarWidget();
        if (scalarBar == nullptr)return;
        scalarBar->Off();
    }

    void PostGraphProperty::showScalarBarWidget()
    {
        vtkScalarBarWidget* scalarBar = _postData->getScalarBarWidget();
        if (scalarBar == nullptr)return;
        scalarBar->On();
    }

    void PostGraphProperty::setScalarBarWidgetIsShow(bool isShow)
    {
        _isShowScalarBar = isShow;
        if (_postData == nullptr)return;
        vtkScalarBarWidget* scalarBar = _postData->getScalarBarWidget();
        if (scalarBar == nullptr)return;
        if (_isShowScalarBar) {
            scalarBar->On();
        }
        else {
            scalarBar->Off();
        }
    }
    bool PostGraphProperty::getScalarBarWidgetOnOrOff()
    {
        vtkScalarBarWidget* scalarBar = _postData->getScalarBarWidget();
        if (scalarBar == nullptr)return false;
        if (scalarBar->GetEnabled() == 1)return true;
        else return false;
    }
    bool PostGraphProperty::getScalarBarWidgetIsShow()
    {
        return _isShowScalarBar;
    }

    void PostGraphProperty::setOpacity(double opacity)
    {
        if (_postGraphObj == nullptr)return;
        for (vtkProp* a : _postGraphObj->getActors()) {
            vtkActor* actor = dynamic_cast<vtkActor*>(a);
            if (actor == nullptr)continue;
            vtkProperty* prop = actor->GetProperty();
            if (prop == nullptr)continue;
            prop->SetOpacity(opacity);
        }
    }

    double PostGraphProperty::getOpacity()
    {
        double opacity = 0.0;
        if (_postGraphObj == nullptr)return opacity;
        for (vtkProp* a : _postGraphObj->getActors()) {
            vtkActor* actor = dynamic_cast<vtkActor*>(a);
            if (actor == nullptr)continue;
            vtkProperty* prop = actor->GetProperty();
            if (prop == nullptr)continue;
            opacity = prop->GetOpacity();
            break;
        }
        return opacity;
    }
}