/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFFVOptionsWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolution.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFFVOptionsWriter::FITKOFFVOptionsWriter()
    {
    }
    FITKOFFVOptionsWriter::~FITKOFFVOptionsWriter()
    {
    }

    void FITKOFFVOptionsWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/fvOptions").arg(filePath);
    }

    bool FITKOFFVOptionsWriter::run()
    {
        // 获取物理数据和网格数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!physicsData || !meshData) return false;
        //获取网格区域数据
        int count = meshData->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByIndex(i);
            if (!regionMesh) continue;
            Interface::FITKUnstructuredMeshVTK* mesh = regionMesh->getFieldMesh();
            if (!mesh) continue;
            //获取路径和区域类型
            QString path = mesh->getUserData(Core::FITKUserData::FITKUserRole + 2).toString();
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = physicsData->getRegionMeshType(regionMesh->getDataObjectID());
            if (path.isEmpty()) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/system/fvOptions").arg(m_filePath);
            else
                m_fileName = QString("%1/system/%2/fvOptions").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile(QString("system"));
            else
                this->dictObjectFoamFile(QString("system/%1").arg(dirName));
            //设置写出的网格区域数据
            bool Ok = false;
            if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
                Ok = this->writeFluidRegionOptions();
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                Ok = this->writeSolidRegionOptions();
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                Ok = true;

            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
        }
        return true;
    }

    QString FITKOFFVOptionsWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFFVOptionsWriter::dictObjectFoamFile(QString location)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "fvOptions");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFFVOptionsWriter::writeFluidRegionOptions()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取离散数据
        Interface::FITKOFSolution* solutionData = physicsData->getSolution();
        if (!solutionData)return false;
        Core::FITKParameter* limits = solutionData->getLimits();
        if (!limits)return false;
        Core::FITKParamBoolGroup* velocityLimit = dynamic_cast<Core::FITKParamBoolGroup*>(limits->getDataByName("Velocity Damping"));
        if (velocityLimit && velocityLimit->getValue())
        {
            DICT::FITKDictGroup* vL = new DICT::FITKDictGroup;
            m_objectDict->append("velocityLimit", vL);
            vL->append("active", true);
            vL->append("type", "limitVelocity");
            DICT::FITKDictGroup* limitVelocityCoeffs = new DICT::FITKDictGroup;
            vL->append("limitVelocityCoeffs", limitVelocityCoeffs);
            Core::FITKParameter* vaGroup = velocityLimit->getValueGroup();
            if (!vaGroup)return false;
            bool ok = false;
            double v = vaGroup->getDoubleParaByName("Umax", ok);
            if (!ok)return false;
            limitVelocityCoeffs->append("max", v);
            limitVelocityCoeffs->append("selectionMode", "all");
            limitVelocityCoeffs->append("U", "U");
        }
        Core::FITKParamBoolGroup* temperatureLimit = dynamic_cast<Core::FITKParamBoolGroup*>(limits->getDataByName("Temperature Damping"));
        if (temperatureLimit && temperatureLimit->getValue())
        {
            DICT::FITKDictGroup* tL = new DICT::FITKDictGroup;
            m_objectDict->append("temperatureLimit", tL);
            tL->append("active", true);
            tL->append("type", "limitTemperature");
            DICT::FITKDictGroup* limitTemperatureCoeffs = new DICT::FITKDictGroup;
            tL->append("limitTemperatureCoeffs", limitTemperatureCoeffs);
            Core::FITKParameter* vaGroup = temperatureLimit->getValueGroup();
            if (!vaGroup)return false;
            bool ok = false;
            double max = vaGroup->getDoubleParaByName("Tmax [K]", ok);
            double min = vaGroup->getDoubleParaByName("Tmin [K]", ok);
            if (!ok)return false;
            limitTemperatureCoeffs->append("max", max);
            limitTemperatureCoeffs->append("max", min);
            limitTemperatureCoeffs->append("selectionMode", "all");
        }
        return true;
    }

    bool FITKOFFVOptionsWriter::writeSolidRegionOptions()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取离散数据
        Interface::FITKOFSolution* solutionData = physicsData->getSolution();
        if (!solutionData)return false;
        Core::FITKParameter* limits = solutionData->getLimits();
        if (!limits)return false;
        Core::FITKParamBoolGroup* temperatureLimit = dynamic_cast<Core::FITKParamBoolGroup*>(limits->getDataByName("Temperature Damping"));
        if (temperatureLimit && temperatureLimit->getValue())
        {
            DICT::FITKDictGroup* tL = new DICT::FITKDictGroup;
            m_objectDict->append("temperatureLimit", tL);
            tL->append("active", true);
            tL->append("type", "limitTemperature");
            DICT::FITKDictGroup* limitTemperatureCoeffs = new DICT::FITKDictGroup;
            tL->append("limitTemperatureCoeffs", limitTemperatureCoeffs);
            Core::FITKParameter* vaGroup = temperatureLimit->getValueGroup();
            if (!vaGroup)return false;
            bool ok = false;
            double max = vaGroup->getDoubleParaByName("Tmax [K]", ok);
            double min = vaGroup->getDoubleParaByName("Tmin [K]", ok);
            if (!ok)return false;
            limitTemperatureCoeffs->append("max", max);
            limitTemperatureCoeffs->append("max", min);
            limitTemperatureCoeffs->append("selectionMode", "all");
        }
        return true;
    }

}



