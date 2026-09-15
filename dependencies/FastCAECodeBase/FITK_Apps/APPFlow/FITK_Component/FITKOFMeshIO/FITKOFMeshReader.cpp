/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFMeshReader.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKOpenFOAMMeshReader.h"
#include <qcoreapplication.h>

QString IO::FITKOFMeshReader::getComponentName()
{
    return "IO::FITKOFMeshReader";
}

bool IO::FITKOFMeshReader::exec(const int indexPort)
{
    if (m_Folder.isEmpty() || m_Mesh == nullptr) return false;
    // 读取网格
    IO::FITKOpenFOAMMeshReader openFOAMMeshReader;
    openFOAMMeshReader.setFileName(m_Folder);
    openFOAMMeshReader.setDataObject(m_Mesh);
    return openFOAMMeshReader.read();
}

void IO::FITKOFMeshReader::setMeshFolder(QString folder)
{
    m_Folder = folder;
}

void IO::FITKOFMeshReader::setMeshObj(Interface::FITKUnstructuredFluidMeshVTK * mesh)
{
    m_Mesh = mesh;
}
