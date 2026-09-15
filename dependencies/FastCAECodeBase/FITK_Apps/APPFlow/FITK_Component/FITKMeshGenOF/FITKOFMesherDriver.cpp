/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFMesherDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Component/FITKOFDictWriter/FITKOFDictWriterIO.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFGeometryData.h"
#include "FITK_Component/FITKOFDriver/FITKOFInputInfo.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include <QApplication>

namespace OF
{
    void FITKOFMesherDriver::startMesher(QStringList info /*= QStringList()*/)
    {
        // 工作路径
        QString path = getValue("WorkDir").toString();
        if (path.isEmpty() || !Core::CreateDir(path)) return;

        // 清理算例文件夹
        Core::ClearDir(path);
        // 写出STL文件
        QString stlFolder = path + "/constant/geometry";
        Core::CreateDir(stlFolder);
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return;
        factoryData->recordBoundaryMeshName();

        Interface::FITKOFGeometryData* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKOFGeometryData>();
        int nModel = geometryData->getDataCount();

        for (int i = 0; i < nModel; i++)
        {
            Interface::FITKAbsGeoCommand* model = geometryData->getDataByIndex(i);
            if (model != nullptr)
            {
                model->getShapeAgent()->writeSTLFile(stlFolder + "/" + model->getDataObjectName() + ".stl");
            }
        }

        // 写出字典文件
        auto dictIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOFDictWriterIO>("IO::FITKOFDictWriterIO");
        if (dictIO == nullptr) return;
        dictIO->setFilePath(path);
        *dictIO << IO::DictWriteType::BlockMesh << IO::DictWriteType::Control 
                << IO::DictWriteType::SnappyHexMesh << IO::DictWriteType::CreatePatch;
        bool ok = dictIO->exec();
        if (!ok)return;

        // 调用blockMesh
        auto proGramManager = FITKAPP->getProgramTaskManager();
        if (proGramManager == nullptr) return;
        AppFrame::FITKProgramInputInfo* inputInfo = new FoamDriver::FITKOFInputInfo();
        inputInfo->setArgs({ "-case", path });
        auto driver = proGramManager->createProgram(1, "FITKOFBlockMeshDriver", inputInfo);
        QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(blockMeshSlot()));
        driver->start();
    }

    void FITKOFMesherDriver::stopMesher(QStringList info /*= QStringList()*/)
    {
        //todo
    }

    void FITKOFMesherDriver::blockMeshSlot() {
        if (getValue("HasGeoMeshSize").toBool()) {
            // 调用blockMesh
            auto proGramManager = FITKAPP->getProgramTaskManager();
            if (proGramManager == nullptr) return;
            AppFrame::FITKProgramInputInfo* inputInfo = new FoamDriver::FITKOFInputInfo();
            QString path = getValue("WorkDir").toString();
            inputInfo->setArgs({ "-overwrite -case", path });
            auto driver = proGramManager->createProgram(1, "FITKOFSnappyHexMeshDriver", inputInfo);
            QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(snappyHexMeshSlot()));
            driver->start();
        }
        else {
            emit mesherFinished();
        }
    }

    void FITKOFMesherDriver::snappyHexMeshSlot()
    {
        // 调用createPatch
        auto proGramManager = FITKAPP->getProgramTaskManager();
        if (proGramManager == nullptr) return;
        AppFrame::FITKProgramInputInfo* inputInfo = new FoamDriver::FITKOFInputInfo();
        QString path = getValue("WorkDir").toString();
        inputInfo->setArgs({ "-overwrite -case", path });
        auto driver = proGramManager->createProgram(1, "FITKOFCreatePatchDriver", inputInfo);
        QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(createPatchSlot()));
        driver->start();
    }

    void FITKOFMesherDriver::createPatchSlot()
    {
        //驱动读取网格生成
        emit this->mesherFinished();
    }
}



