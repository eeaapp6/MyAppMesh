/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphActor.h"

#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkRenderer.h>
#include <vtkCellData.h>
#include <vtkTriangleFilter.h>
#include <vtkPassThrough.h>
#include <vtkIdFilter.h>

vtkObjectFactoryNewMacro(FITKGraphActor);

bool FITKGraphActor::s_useTriangleFilter = false;

FITKGraphActor::FITKGraphActor() : FITKActorClipTool(this)
{
    // Create a mapper and set to the actor.
    this->Device = vtkActor::New();
    this->SetMapper(vtkSmartPointer<vtkDataSetMapper>::New());

    // Create a triangle filter for the actor.
    if (s_useTriangleFilter)
    {
        vtkSmartPointer<vtkIdFilter> idFilter = vtkSmartPointer<vtkIdFilter>::New();
        idFilter->SetInputConnection(m_passThrough->GetOutputPort());
        idFilter->SetCellIdsArrayName("_InternalMapperCellIds_");
        idFilter->SetCellIds(true);
        idFilter->SetPointIds(false);

        m_triangleFilter = vtkTriangleFilter::New();
        m_triangleFilter->SetInputConnection(idFilter->GetOutputPort());
    }
}

FITKGraphActor::~FITKGraphActor()
{
    // Delete the actor device.
    if (this->Device)
    {
        this->Device->Delete();
        this->Device = nullptr;
    }

    if (m_triangleFilter)
    {
        m_triangleFilter->Delete();
        m_triangleFilter = nullptr;
    }

    m_graphObj = nullptr;
}

void FITKGraphActor::SetUseTriangleFilter(bool flag)
{
    s_useTriangleFilter = flag;
}

bool FITKGraphActor::GetUseTriangleFilter()
{
    return s_useTriangleFilter;
}

void FITKGraphActor::SetMapper(vtkMapper* mapper)
{
    this->Superclass::SetMapper(mapper);
    setNewMapper(mapper);
}

int FITKGraphActor::getTriCellId(int triangleCellId)
{
    if (!s_useTriangleFilter)
    {
        return -1;
    }

    if (!m_triangleFilter)
    {
        vtkSmartPointer<vtkIdFilter> idFilter = vtkSmartPointer<vtkIdFilter>::New();
        idFilter->SetInputConnection(m_passThrough->GetOutputPort());
        idFilter->SetCellIdsArrayName("_InternalMapperCellIds_");
        idFilter->SetCellIds(true);
        idFilter->SetPointIds(false);

        m_triangleFilter = vtkTriangleFilter::New();
        m_triangleFilter->SetInputConnection(idFilter->GetOutputPort());
    }

    m_triangleFilter->Update();

    vtkDataSet* ds = m_triangleFilter->GetOutput();
    if (!ds)
    {
        return -1;
    }

    vtkCellData* cellData = ds->GetCellData();
    if (!cellData)
    {
        return -1;
    }

    vtkDataArray* cellIds = cellData->GetArray("_InternalMapperCellIds_");
    if (!cellIds)
    {
        return -1;
    }

    if (triangleCellId < 0 || triangleCellId >= cellIds->GetNumberOfTuples())
    {
        return -1;
    }

    int cellId = static_cast<int>(cellIds->GetTuple1(triangleCellId));
    return cellId;
}

void FITKGraphActor::setRelativeCoincidentTopologyPolygonOffsetParameters(double val)
{
    if (this->Mapper)
    {
        this->Mapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(val, val);
        this->Mapper->SetRelativeCoincidentTopologyLineOffsetParameters(val, val);
        this->Mapper->SetRelativeCoincidentTopologyPointOffsetParameter(val);
    }
}

void FITKGraphActor::setAutoRemoveNormals(bool flag)
{
    m_autoRemoveNormals = flag;

    // Try to remove the normals.
    if (m_autoRemoveNormals)
    {
        removeCellNormals();
    }
}

void FITKGraphActor::removeCellNormals()
{
    // Check the mapper.
    if (!this->Mapper)
    {
        return;
    }

    // Check the input data.
    vtkDataSet* dataset = this->Mapper->GetInput();
    if (!dataset)
    {
        return;
    }

    // Check the cell data.
    vtkCellData* cellData = dataset->GetCellData();
    if (!cellData)
    {
        return;
    }

    // Get the normals array.
    vtkDataArray* normals = cellData->GetNormals();
    if (!normals)
    {
        return;
    }

    cellData->SetNormals(nullptr);
    normals->Delete();

    update();
}

void FITKGraphActor::setColor(QColor color, bool doubleFace)
{
    if (!color.isValid())
    {
        return;
    }

    setFrontFaceColor(color);

    if (doubleFace)
    {
        setBackFaceColor(color);
    }
}

void FITKGraphActor::setFrontFaceColor(QColor color)
{
    if (!color.isValid())
    {
        return;
    }

    m_frontFaceColor = color;

    // Initialize front face property.
    vtkProperty* prop = this->GetProperty();
    if (!prop)
    {
        prop = vtkProperty::New();
        this->SetProperty(prop);
    }

    prop->SetColor(color.redF(), color.greenF(), color.blueF());
}

void FITKGraphActor::setBackFaceColor(QColor color)
{
    if (!color.isValid())
    {
        return;
    }

    m_backFaceColor = color;

    if (!m_enableBackFace)
    {
        return;
    }

    // Initialize back face property.
    vtkProperty* prop = this->GetBackfaceProperty();
    if (!prop)
    {
        prop = vtkProperty::New();
        this->SetBackfaceProperty(prop);
    }

    prop->SetColor(color.redF(), color.greenF(), color.blueF());
}

void FITKGraphActor::setEnableBackFaceColor(bool isOn)
{
    if (isOn)
    {
        setBackFaceColor(m_backFaceColor);
    }
    else
    {
        vtkProperty* prop = this->GetBackfaceProperty();
        if (!prop)
        {
            prop = vtkProperty::New();
            this->SetBackfaceProperty(prop);
        }

        prop->SetColor(m_frontFaceColor.redF(), m_frontFaceColor.greenF(), m_frontFaceColor.blueF());
    }

    m_enableBackFace = isOn;
}

void FITKGraphActor::setGraphObject(Comp::FITKGraphObjectVTK* obj)
{
    m_graphObj = obj;
}

Comp::FITKGraphObjectVTK* FITKGraphActor::getGraphObject()
{
    return m_graphObj;
}

void FITKGraphActor::setActorType(ActorType type)
{
    m_actorType = type;
}

ActorType FITKGraphActor::getActorType()
{
    return m_actorType;
}

void FITKGraphActor::setDataType(DataType type)
{
    m_dataType = type;
}

DataType FITKGraphActor::getDataType()
{
    return m_dataType;
}

vtkDataSet* FITKGraphActor::getInputAsDataSet()
{
    if (!this->Mapper)
    {
        return nullptr;
    }

    return this->Mapper->GetInputAsDataSet();
}

void FITKGraphActor::setScalarVisibility(bool isOn)
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarVisibility(isOn);
}

void FITKGraphActor::setScalarModeToDefault()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarModeToDefault();
}

void FITKGraphActor::setScalarModeToUsePointData()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarModeToUsePointData();
}

void FITKGraphActor::setScalarModeToUseCellData()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarModeToUseCellData();
}

void FITKGraphActor::setScalarModeToUsePointFieldData()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarModeToUsePointFieldData();
}

void FITKGraphActor::setScalarModeToUseCellFieldData()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SetScalarModeToUseCellFieldData();
}

void FITKGraphActor::selectScalarArray(QString name)
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    this->Mapper->SelectColorArray(name.toUtf8().data());
}

void FITKGraphActor::update()
{
    // If mapper is empty, return.
    if (!this->Mapper)
    {
        return;
    }

    // Update the actor's mapper.
    this->Mapper->Update();
}

void FITKGraphActor::setUserData(uint role, QVariant value)
{
    // User information cannot set to the input and output role.
    if (role < 0)
    {
        return;
    }

    m_userInfo[role] = value;
}

QVariant FITKGraphActor::getUserData(uint role)
{
    if (!m_userInfo.contains(role) && role >= 0)
    {
        return QVariant();
    }

    return m_userInfo[role];
}

// Override from vtkActor
//@{
void FITKGraphActor::ReleaseGraphicsResources(vtkWindow* window)
{
    // Override release resources function.
    this->Device->ReleaseGraphicsResources(window);
    this->Superclass::ReleaseGraphicsResources(window);
}

int FITKGraphActor::RenderOpaqueGeometry(vtkViewport* viewport)
{
    // Override render opaque function.
    if (!this->Mapper)
    {
        return 0;
    }
    if (!this->Property)
    {
        this->GetProperty();
    }

    if (this->GetIsOpaque())
    {
        vtkRenderer* ren = static_cast<vtkRenderer*>(viewport);
        this->Render(ren, this->Mapper);
        return 1;
    }

    return 0;
}

int FITKGraphActor::RenderTranslucentPolygonalGeometry(vtkViewport* viewport)
{
    // Override render opaque function.
    if (!this->Mapper)
    {
        return 0;
    }
    if (!this->Property)
    {
        this->GetProperty();
    }

    if (!this->GetIsOpaque())
    {
        vtkRenderer* ren = static_cast<vtkRenderer*>(viewport);
        this->Render(ren, this->Mapper);
        return 1;
    }

    return 0;
}

void FITKGraphActor::Render(vtkRenderer* ren, vtkMapper* vtkNotUsed(mapper))
{
    // Override render function.
    this->Property->Render(this, ren);

    this->Device->SetProperty(this->Property);
    this->Property->Render(this, ren);
    if (this->BackfaceProperty)
    {
        this->BackfaceProperty->BackfaceRender(this, ren);
        this->Device->SetBackfaceProperty(this->BackfaceProperty);
    }

    /* render the texture */
    if (this->Texture)
    {
        this->Texture->Render(ren);
    }
    this->Device->SetTexture(this->GetTexture());

    // make sure the device has the same matrix
    this->ComputeMatrix();
    this->Device->SetUserMatrix(this->Matrix);
    if (this->GetPropertyKeys())
    {
        this->Device->SetPropertyKeys(this->GetPropertyKeys());
    }
    this->Device->Render(ren, this->Mapper);
}

void FITKGraphActor::ShallowCopy(vtkProp* prop)
{
    // Override shallow copy.
    this->Superclass::ShallowCopy(prop);
}
//@}