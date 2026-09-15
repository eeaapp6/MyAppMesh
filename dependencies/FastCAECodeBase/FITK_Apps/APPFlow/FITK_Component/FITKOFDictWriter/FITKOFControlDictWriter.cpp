/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFControlDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFControlDictWriter::FITKOFControlDictWriter()
    {
    }
    FITKOFControlDictWriter::~FITKOFControlDictWriter()
    {
    }

    void FITKOFControlDictWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/controlDict").arg(filePath);
    }

    bool FITKOFControlDictWriter::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->controlDictW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();

        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFControlDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFControlDictWriter::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "controlDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFControlDictWriter::controlDictW()
    {
        if (!m_objectDict)return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据和网格数据
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData || !meshData)return false;
        //获取求解器
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver) return this->writeDefaultControl();
        Interface::FITKOFSolverTypeEnum::FITKOFSolverType type = solver->getSolverType();
        //获取运行求解器配置
        Interface::FITKOFRunControl* runControl = physicsData->getRunControl();
        if (!runControl)return false;
        Core::FITKParameter* timeControl = runControl->getTimeControl();
        Core::FITKParameter* outputControl = runControl->getOutputControl();
        if (!timeControl || !outputControl)return false;
        //获取区域求解类型
        int count = meshData->getDataCount();
        QHash<QString, QString> regionType;
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByIndex(i);
            if (!regionMesh) continue;
            int regionMeshID = regionMesh->getDataObjectID();
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = physicsData->getRegionMeshType(regionMeshID);
            if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::None) continue;
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
                regionType.insert(regionMesh->getDataObjectName(), "fluid");
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                regionType.insert(regionMesh->getDataObjectName(), "solid");
        }
        //设置求解器
        if (type == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE)
            m_objectDict->append("application", "simpleFoam");
        else if (type == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
            m_objectDict->append("application", "interFoam");
        else if (type == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION)
            m_objectDict->append("application", "foamMultiRun");
        //写出区域求解类型
        if (regionType.size() > 1)
        {
            DICT::FITKDictGroup* regionSolversGroup = new DICT::FITKDictGroup;
            m_objectDict->append("regionSolvers", regionSolversGroup);
            for (QHash<QString, QString>::iterator iter = regionType.begin(); iter != regionType.end(); ++iter)
                regionSolversGroup->append(iter.key(), iter.value());
        }
        //获取是否调整时间步长
        Core::FITKParamBoolGroup* adjustTimeStep = dynamic_cast<Core::FITKParamBoolGroup*>(timeControl->getDataByName("Adjust Time Stepping"));
        bool isAdjustTS = false;
        if (adjustTimeStep)
        {
            isAdjustTS = adjustTimeStep->getValue();
            m_objectDict->append("adjustTimeStep", isAdjustTS);
        }
        //设置初始值
        bool ok = false;
        double initTime = timeControl->getDoubleParaByName("Intial Time [s]", ok);
        if (ok) m_objectDict->append("deltaT", initTime);
        else m_objectDict->append("deltaT", 1);
        //获取结束时间
        bool isSetEndT = false;
        int iterations = timeControl->getIntParaByName("Number of Iterations", ok);
        if (!isSetEndT && ok)
        {
            isSetEndT = true;
            m_objectDict->append("endTime", iterations);
        }
        double simuT = timeControl->getDoubleParaByName("Simulation Time [s]", ok);
        if (!isSetEndT && ok)
        {
            isSetEndT = true;
            m_objectDict->append("endTime", simuT);
        }
        if (!isSetEndT) m_objectDict->append("endTime", 1);
        //设置调整时间步长
        if (adjustTimeStep)
        {
            Core::FITKParameter* adjustTime = adjustTimeStep->getValueGroup();
            if (!adjustTime) return false;
            double value = adjustTime->getDoubleParaByName("max Alpha Co", ok);
            if (ok)
            {
                if (isAdjustTS) m_objectDict->append("maxAlphaCo", value);
                else m_objectDict->append("maxAlphaCo", 0.5);
            }
            value = adjustTime->getDoubleParaByName("max Co", ok);
            if (ok)
            {
                if (isAdjustTS) m_objectDict->append("maxCo", value);
                else m_objectDict->append("maxCo", 2);
            }
            value = adjustTime->getDoubleParaByName("max Delta Time", ok);
            if (ok)
            {
                if (isAdjustTS) m_objectDict->append("maxDeltaT", value);
                else m_objectDict->append("maxDeltaT", 1);
            }
        }
        //保留多少结果 等于0是不清理结果文件
        Core::FITKParamBoolGroup* resultFiles = dynamic_cast<Core::FITKParamBoolGroup*>(outputControl->getDataByName("Clean Old Result Files"));
        if (!resultFiles) return false;
        if (resultFiles->getValue())
        {
            Core::FITKParameter* resultFilesNum = resultFiles->getValueGroup();
            if (!resultFilesNum) return false;
            m_objectDict->append("purgeWrite", resultFilesNum->getIntParaByName("Max No.of Files", ok));
        }
        else m_objectDict->append("purgeWrite", 0);
        //参数配置
        m_objectDict->append("startFrom", "latestTime");
        m_objectDict->append("startTime", 0);
        m_objectDict->append("stopAt", "endTime");
        m_objectDict->append("timeFormat", "general");
        m_objectDict->append("timePrecision", 6);
        if (adjustTimeStep)m_objectDict->append("writeControl", "adjustableRunTime");
        else m_objectDict->append("writeControl", "timeStep");
        //增量
        int wInterInt = outputControl->getIntParaByName("Write Interval", ok);
        if (ok)
            m_objectDict->append("writeInterval", wInterInt);
        else
        {
            double wInterDouble = outputControl->getDoubleParaByName("Write Interval", ok);
            if (ok) m_objectDict->append("writeInterval", wInterDouble);
        }
        if (!ok) return false;
        m_objectDict->append("writePrecision", 7);
        m_objectDict->append("runTimeModifiable", true);
        m_objectDict->append("compression", "uncompressed");
        m_objectDict->append("timePrecision", 6);
        m_objectDict->append("writeFormat", "ascii");
        m_objectDict->append("writePrecision", 7);
        //functions


        //OptimisationSwitches
        DICT::FITKDictGroup* optim = new DICT::FITKDictGroup;
        m_objectDict->append("OptimisationSwitches", optim);
        optim->append("fileHandler", "uncollated");
        optim->append("fileModificationSkew", 0);
        optim->append("maxMasterFileBufferSize", "1.0E9");
        optim->append("maxThreadFileBufferSize", "1.0E9");
        optim->append("mpiBufferSize", 200000000);
        return true;
    }

    bool FITKOFControlDictWriter::writeDefaultControl()
    {
        if (!m_objectDict)return false;
        m_objectDict->append("deltaT", 1);
        m_objectDict->append("endTime", 1);
        m_objectDict->append("purgeWrite", 2);
        m_objectDict->append("startFrom", "latestTime");
        m_objectDict->append("startTime", 0);
        m_objectDict->append("stopAt", "endTime");
        m_objectDict->append("writeFormat", "ascii");
        m_objectDict->append("compression", "uncompressed");
        m_objectDict->append("timeFormat", "general");
        m_objectDict->append("writeControl", "timeStep");
        m_objectDict->append("writeInterval", 10);
        m_objectDict->append("writePrecision", 7);
        m_objectDict->append("timePrecision", 6);
        return true;
    }
}



