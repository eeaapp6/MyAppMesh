/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFOperatingConditionsW.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFOperatingConditions.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFOperatingConditionsW::FITKOFOperatingConditionsW()
    {
    }
    FITKOFOperatingConditionsW::~FITKOFOperatingConditionsW()
    {
    }

    void FITKOFOperatingConditionsW::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = filePath;
    }

    bool FITKOFOperatingConditionsW::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        m_physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!m_physicsData)return false;
        this->gravitationalFileW();
        this->accelerationFileW();
        return true;
    }

    QString FITKOFOperatingConditionsW::getFileName()
    {
        return m_fileName;
    }

    bool FITKOFOperatingConditionsW::gravitationalFoamFile(QString location)
    {
        //写出FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "uniformDimensionedVectorField");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "g");
        m_objectDict->append("FoamFile", group);
        return true;
    }

    bool FITKOFOperatingConditionsW::accelerationFoamFile(QString location)
    {
        //写出FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "uniformDimensionedScalarField");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "hRef");
        m_objectDict->append("FoamFile", group);
        return true;
    }

    bool FITKOFOperatingConditionsW::gravitationalFileW()
    {
        //在流体区域中写出
        //获取initial
        if (!m_physicsData) return false;
        Interface::FITKOFOperatingConditions* operatingCond = m_physicsData->getOperatingConditions();
        if (!operatingCond)return false;
        Core::FITKParameter* gaData = operatingCond->getGravitationalAcceleration();
        if (!gaData)return false;
        Core::FITKParamDoubleList* gPara = dynamic_cast<Core::FITKParamDoubleList*>(gaData->getDataByName("g [m/s2]"));
        if (!gPara)return false;
        //获取重力数据
        int count = gPara->getCount();
        QStringList valueList;
        for (int i = 0; i < count; ++i)
            valueList.append(QString::number(gPara->getValue(i)));
        if (valueList.size() != 3)return false;
        //获取物理数据和网格数据
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshData) return false;
        //获取网格区域数据
        count = meshData->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByIndex(i);
            if (!regionMesh) continue;
            Interface::FITKUnstructuredMeshVTK* mesh = regionMesh->getFieldMesh();
            if (!mesh) continue;
            //获取路径和区域类型
            QString path = mesh->getUserData(Core::FITKUserData::FITKUserRole + 2).toString();
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = m_physicsData->getRegionMeshType(regionMesh->getDataObjectID());
            if (path.isEmpty() || typeRegion != Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/g").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/g").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            if (dirName.toLower() == "constant")
                this->gravitationalFoamFile(QString("constant"));
            else
                this->gravitationalFoamFile(QString("constant/%1").arg(dirName));

            //设置重力数据
            m_objectDict->append("dimensions", "[0 1 -2 0 0 0 0]");
            m_objectDict->append("value", QString("(%1)").arg(valueList.join(' ')));
            //写出字典文件
            this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
        }

        return true;
    }

    bool FITKOFOperatingConditionsW::accelerationFileW()
    {
        //在流体区域中写出

        if (!m_physicsData)return false;
        //获取initial
        Interface::FITKOFOperatingConditions* operatingCond = m_physicsData->getOperatingConditions();
        if (!operatingCond)return false;
        Core::FITKParameter* gaData = operatingCond->getGravitationalAcceleration();
        if (!gaData)return false;
        Core::FITKParamDouble* hrefPara = dynamic_cast<Core::FITKParamDouble*>(gaData->getDataByName("href [m]"));
        if (!hrefPara)return false;
        m_fileName = QString("%1/constant/hRef").arg(m_filePath);
        //获取加速度数据
        double value = hrefPara->getValue();

        //获取物理数据和网格数据
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshData) return false;
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
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = m_physicsData->getRegionMeshType(regionMesh->getDataObjectID());
            if (path.isEmpty() || typeRegion != Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/hRef").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/hRef").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            if (dirName.toLower() == "constant")
                this->accelerationFoamFile(QString("constant"));
            else
                this->accelerationFoamFile(QString("constant/%1").arg(dirName));

            //设置加速度数据
            m_objectDict->append("dimensions", "[0 1 0 0 0 0 0]");
            m_objectDict->append("value", value);
            //写出字典文件
            this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
        }
        return true;
    }
}



