/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshGenOFInterface.h"
#include "FITKOFMeshSizeGenerator.h"
#include "FITKOFGeometryMeshSizeGenerator.h"
#include "FITKOFMesherDriver.h"
#include "FITKOFMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"

namespace OF
{
    FITKMeshGenOFInterface::FITKMeshGenOFInterface()
    {
        //获取工厂
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (nullptr == mf) return;
        //注册相关类
        mf->regMeshSizeGenerator(new FITKOFMeshSizeGenerator);
        mf->regGeometryMeshSizeGenerator(new FITKOFGeometryMeshSizeGenerator);
        mf->regMesherDriver(new FITKOFMesherDriver);
        mf->regMeshProcessor(new FITKOFMeshProcessor);
    }


    QString FITKMeshGenOFInterface::getComponentName()
    {
        //名称
        return QString("FITKMeshGenOF");
    }

    bool FITKMeshGenOFInterface::exec(const int indexPort)
    {
        return false;
    }


}



