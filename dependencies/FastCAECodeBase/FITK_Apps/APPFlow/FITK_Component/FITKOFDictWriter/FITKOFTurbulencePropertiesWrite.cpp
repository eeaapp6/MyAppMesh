/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbulencePropertiesWrite.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFTurbulence.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFTurbulencePropertiesWrite::FITKOFTurbulencePropertiesWrite()
    {
    }
    FITKOFTurbulencePropertiesWrite::~FITKOFTurbulencePropertiesWrite()
    {
    }

    void FITKOFTurbulencePropertiesWrite::setFilePath(const QString& filePath)
    {
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        FITKOFAbstractDictWriter::setFilePath(filePath);
    }

    bool FITKOFTurbulencePropertiesWrite::run()
    {
        //获取物理数据和网格数据
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
            if (path.isEmpty() || typeRegion != Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/momentumTransport").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/momentumTransport").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            this->dictObjectFoamFile();
            //设置写出的网格区域数据
            bool Ok = this->turbulencePropW();
            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
            if (!Ok)
                return false;
        }

        return true;
    }

    QString FITKOFTurbulencePropertiesWrite::getFileName()
    {
        return m_fileName;
    }

    void FITKOFTurbulencePropertiesWrite::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"constant\"");
        group->append("object", "turbulenceProperties");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFTurbulencePropertiesWrite::turbulencePropW()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取湍流模型数据
        Interface::FITKAbstractOFTurbulence* turbulence = physicsData->getTurbulence();
        //获取湍流模式类型
        Interface::FITKOFSolverTypeEnum::FITKOFTurbulenceType type = Interface::FITKOFSolverTypeEnum::FITKOFTurbulenceType::TurbNone;
        if (turbulence) type = turbulence->getTurbulenceType();
        //设置模式参数数据
        DICT::FITKDictGroup* coeffs = nullptr;
        if (type == Interface::FITKOFSolverTypeEnum::FITKOFTurbulenceType::TurbNone)
        {
            //使用层流
            m_objectDict->append("simulationType", "laminar");
            return true;
        }
        else if (type == Interface::FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS)
        {
            m_objectDict->append("simulationType", "RAS");
            //设置模式数据
            DICT::FITKDictGroup* modelData = new DICT::FITKDictGroup;
            m_objectDict->append("RAS", modelData);
            modelData->append("turbulence", true);
            modelData->append("printCoeffs", true);
            //获取湍流名称
            QString turbName = this->turbModelName(turbulence->getDataObjectName());
            modelData->append("RASModel", turbName);
            coeffs = new DICT::FITKDictGroup;
            modelData->append(QString("%1Coeffs").arg(turbName), coeffs);
        }
        else if (type == Interface::FITKOFSolverTypeEnum::FITKOFTurbulenceType::LES)
        {
            m_objectDict->append("simulationType", "LES");
            //设置模式数据
            DICT::FITKDictGroup* modelData = new DICT::FITKDictGroup;
            m_objectDict->append("LES", modelData);
            modelData->append("turbulence", true);
            modelData->append("printCoeffs", true);
            //获取湍流名称
            QString turbName = this->turbModelName(turbulence->getDataObjectName());
            modelData->append("RASModel", turbName);
            coeffs = new DICT::FITKDictGroup;
            modelData->append(QString("%1Coeffs").arg(turbName), coeffs);
        }
        if (!coeffs)return false;
        Core::FITKParameter* turbPara = turbulence->getTurbulenceParams();
        //写出模型参数
        this->parametersW(turbPara, coeffs);
        return true;
    }

    QString FITKOFTurbulencePropertiesWrite::turbModelName(QString name)
    {
        //模板名称转换成写出文件的名称
        if (name == "k-Epsilon")
            return "kEpsilon";
        else if (name == "k-kL-Omega")
            return "kkLOmega";
        else if (name == "k-Omega")
            return "kOmega";
        else if (name == "k-Omega SST")
            return "kOmegaSST";
        else if (name == "k-Omega SST DDES")
            return "kOmegaSSTDDES";
        else if (name == "k-Omega SST DES")
            return "kOmegaSSTDES";
        else if (name == "k-Omega SST IDDES")
            return "kOmegaSSTIDDES";
        else if (name == "k-Omega SST SAS")
            return "kOmegaSSTSAS";
        else if (name == "Realizable k-Epsilon")
            return "realizableKE";
        else if (name == "RNG k-Epsilon")
            return "RNGkEpsilon";
        else if (name == "Smagorinsky")
            return "Smagorinsky";
        else if (name == "Spalart-Allmaras")
            return "SpalartAllmaras";
        else if (name == "Spalart-Allmaras DDES")
            return "SpalartAllmarasDDES";
        else if (name == "Spalart-Allmaras DES")
            return "SpalartAllmarasDES";
        else if (name == "Spalart-Allmaras IDDES")
            return "SpalartAllmarasIDDES";
        else if (name == "WALE")
            return "WALE";
        return QString();
    }

    bool FITKOFTurbulencePropertiesWrite::parametersW(Core::FITKParameter* para, DICT::FITKDictGroup* group)
    {
        if (!para || !group)return false;
        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Core::FITKAbstractEasyParam* paraData = para->getDataByIndex(i);
            if (!paraData)continue;
            if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBool)
            {
                Core::FITKParamBool* p = dynamic_cast<Core::FITKParamBool*>(paraData);
                if (!p)continue;
                group->append(p->getDataObjectName(), p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* p = dynamic_cast<Core::FITKParamDouble*>(paraData);
                if (!p)continue;
                group->append(p->getDataObjectName(), p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            {
                Core::FITKParamInt* p = dynamic_cast<Core::FITKParamInt*>(paraData);
                if (!p)continue;
                group->append(p->getDataObjectName(), p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPString)
            {
                Core::FITKParamString* p = dynamic_cast<Core::FITKParamString*>(paraData);
                if (!p)continue;
                group->append(p->getDataObjectName(), p->getValue());
            }
        }
        return true;
    }
}



