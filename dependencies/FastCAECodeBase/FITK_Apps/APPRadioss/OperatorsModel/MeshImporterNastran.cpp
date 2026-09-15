/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MeshImporterNastran.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKNastranBDFIO/FITKNastranBDFIOInterface.h"
#include "FITK_Component/FITKAbaqusRadiossDataMapper/FITKAbaqusRadiossDataMapperInterface.h"

namespace ModelOper
{
    FITKNastranMeshImporter::FITKNastranMeshImporter(QObject *parent)
        : FITKAbstractMeshImporter(parent)
    {
    }

    void FITKNastranMeshImporter::run()
    {
        // 获取Radioss数据对象
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        // 获取NastranBDFIO组件
        Nastran::FITKNastranBDFIOInterface* nasIO = FITKAPP->getComponents()->
                 getComponentTByName<Nastran::FITKNastranBDFIOInterface>("NastranBDFIO");
        if (nasIO == nullptr) return;
        // 获取AbaqusRadiossDataMapper组件
        AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface* mapper = FITKAPP->getComponents()->
                 getComponentTByName<AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface>("AbaqusRadiossDataMapper");
        if (mapper == nullptr) return;
        // 创建Abaqus数据对象 暂存bdf数据
        AbaqusData::FITKDataCase* abaCase = new AbaqusData::FITKDataCase();
        // 读取Nastran文件到Abaqus数据对象
        nasIO->setFileName(_fileName);
        nasIO->setDataObject("Model", abaCase);
        nasIO->runInThread(false); // 不在线程运行
        nasIO->setParentThreadTask(this);
        bool ok = nasIO->exec(1); // 读取操作
        // 进行Abaqus到Radioss的映射
        if (ok)
        {
            mapper->setAbaqusCase(abaCase);
            mapper->setRadiossCase(caseData);
            ok = mapper->exec(1); // Abaqus到Radioss映射
        }

        delete abaCase;
    }




}
