/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKOFMeshIO/FITKOFMeshReader.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

namespace OF
{
    void FITKOFMeshProcessor::start(QStringList info /*= QStringList()*/)
    {
        QString path = getValue("WorkDir").toString();
        // 读取网格
        Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        //数据全部清空
        mesh->clear();
        auto ofMeshReader = FITKAPP->getComponents()->getComponentTByName<IO::FITKOFMeshReader>("IO::FITKOFMeshReader");
        if (ofMeshReader == nullptr) return;
        ofMeshReader->setMeshFolder(path + "/constant/");
        ofMeshReader->setMeshObj(mesh);
        ofMeshReader->exec(0);
    }
}



