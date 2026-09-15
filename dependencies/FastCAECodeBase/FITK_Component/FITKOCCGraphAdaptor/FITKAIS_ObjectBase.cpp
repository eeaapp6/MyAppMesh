/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAIS_ObjectBase.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

FITKAIS_ObjectBase::FITKAIS_ObjectBase()
{
    // Nothing to do here.
}

FITKAIS_ObjectBase::~FITKAIS_ObjectBase()
{
    // Clear the data.
    m_graphObj = nullptr;
    m_dataObjId = -1;
}

void FITKAIS_ObjectBase::setGraphObject(Exchange::FITKOCCGraphObject3D* gObj)
{
    // Save the graph object.
    m_graphObj = gObj;
}

Exchange::FITKOCCGraphObject3D* FITKAIS_ObjectBase::getGraphObject()
{
    return m_graphObj;
}

void FITKAIS_ObjectBase::setDataObjectId(int dataObjId)
{
    // Save the data object id.
    m_dataObjId = dataObjId;
}

int FITKAIS_ObjectBase::getDataObjectId()
{
    return m_dataObjId;
}

Core::FITKAbstractNDataObject* FITKAIS_ObjectBase::getDataObject()
{
    Core::FITKAbstractNDataObject* nObj = FITKDATAREPO->getTDataByID<Core::FITKAbstractNDataObject>(m_dataObjId);
    return nObj;
}
