/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectPostModelBase.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkDataSetAttributes.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
namespace Exchange
{
    GraphVTKObjectPostModelBase::GraphVTKObjectPostModelBase(Core::FITKAbstractDataObject* dataObj) :
        GraphVTKObject3DPostBase(dataObj)
    {

    }

    GraphVTKObjectPostModelBase::~GraphVTKObjectPostModelBase()
    {

    }
}