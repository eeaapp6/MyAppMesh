/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermoPhyPropDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiation.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermo.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalEquationOfState.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalThermodynamics.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalTransport.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFThermoPhyPropDictWriter::FITKOFThermoPhyPropDictWriter()
    {
    }
    FITKOFThermoPhyPropDictWriter::~FITKOFThermoPhyPropDictWriter()
    {
    }

    void FITKOFThermoPhyPropDictWriter::setFilePath(const QString& filePath)
    {
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        FITKOFAbstractDictWriter::setFilePath(filePath);
    }

    bool FITKOFThermoPhyPropDictWriter::run()
    {
        //获取物理数据和网格数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!physicsData || !meshData) return false;
        Interface::FITKOFThermo* thermo = physicsData->getThermo();
        if (!thermo) return true;
        //获取网格区域数据
        int count = meshData->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByIndex(i);
            if (!regionMesh) continue;
            int regionMeshID = regionMesh->getDataObjectID();
            Interface::FITKUnstructuredMeshVTK* mesh = regionMesh->getFieldMesh();
            if (!mesh) continue;
            Interface::FITKOFThermoPhysicalProp* thermoProp = thermo->getThermoPropRegionByRegionMeshID(regionMeshID);
            //获取路径和区域类型
            QString path = mesh->getUserData(Core::FITKUserData::FITKUserRole + 2).toString();
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = physicsData->getRegionMeshType(regionMesh->getDataObjectID());
            if (path.isEmpty() || typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::None) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/thermophysicalProperties").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/thermophysicalProperties").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile(QString("constant"));
            else
                this->dictObjectFoamFile(QString("constant/%1").arg(dirName));
            //设置写出的网格区域数据
            bool Ok = false;
            if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
                Ok = this->thermoFluidPhyPropDictW(thermoProp);
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                Ok = this->thermoSolidPhyPropDictW(thermoProp);
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

    QString FITKOFThermoPhyPropDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFThermoPhyPropDictWriter::dictObjectFoamFile(QString location)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "thermophysicalProperties");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFThermoPhyPropDictWriter::thermoFluidPhyPropDictW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp)
    {
        if (!thermoPhyProp) return false;
        //获取热物理数据
        Interface::FITKOFAbsThermoPhysicalEquationOfState* equation = thermoPhyProp->getEquationOfStatePara();
        Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics = thermoPhyProp->getThermodynamicsPara();
        Interface::FITKOFAbsThermoPhysicalTransport* transport = thermoPhyProp->getTransportPara();
        if (!equation || !thermodynamics || !transport) return false;
        //写出thermoType
        DICT::FITKDictGroup* thermoTypeGroup = new DICT::FITKDictGroup;
        thermoTypeGroup->append("type", "heRhoThermo");
        thermoTypeGroup->append("mixture", "pureMixture");
        //写出transport类型
        QString transportName = transport->getDataObjectName();
        QString transportType;
        if (transportName == "Polynomial")
            transportType = "polynomial";
        else if (transportName == "Sutherland")
            transportType = "sutherland";
        else if (transportName == "Constant")
            transportType = "const";
        thermoTypeGroup->append("transport", transportType);
        //写出thermo类型
        QString thermoName = thermodynamics->getDataObjectName();
        QString thermoType;
        if (thermoName == "Polynomial")
            thermoType = "hPolynomial";
        else if (thermoName == "Constant")
            thermoType = "hConst";
        thermoTypeGroup->append("thermo", thermoType);
        //写出equationOfState类型
        QString name = equation->getDataObjectName();
        QString equationType;
        if (name == "Perfect Gas")
            equationType = "perfectGas";
        else if (name == "Boussinesq")
            equationType = "Boussinesq";
        else if (name == "Incompressible Perfect Gas")
            equationType = "incompressiblePerfectGas";
        else if (name == "Constant Density")
            equationType = "rhoConst";
        else if (name == "Perfect Fluid")
            equationType = "perfectFluid";
        else if (name == "Adiabatic Perfect Fluid")
            equationType = "adiabaticPerfectFluid";
        else if (name == "Polynomial")
            equationType = "icoPolynomial";
        else if (name == "Peng-Robinson")
            equationType = "PengRobinsonGas";
        thermoTypeGroup->append("equationOfState", equationType);

        thermoTypeGroup->append("specie", "specie");
        thermoTypeGroup->append("energy", "sensibleEnthalpy");
        m_objectDict->append("thermoType", thermoTypeGroup);

        //写出mixture
        DICT::FITKDictGroup* mixtureParaGroup = new DICT::FITKDictGroup;
        this->speciePropW(thermoPhyProp->getSpeciePara(), mixtureParaGroup);
        this->thermodynamicsPropW(thermodynamics, mixtureParaGroup);
        this->transportPropW(transport, mixtureParaGroup);
        this->equationOfStatePropW(equation, mixtureParaGroup);

        m_objectDict->append("mixture", mixtureParaGroup);
        return true;
    }

    bool FITKOFThermoPhyPropDictWriter::thermoSolidPhyPropDictW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp)
    {
        if (!thermoPhyProp) return false;
        //获取热物理数据
        Interface::FITKOFAbsThermoPhysicalEquationOfState* equation = thermoPhyProp->getEquationOfStatePara();
        Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics = thermoPhyProp->getThermodynamicsPara();
        Interface::FITKOFAbsThermoPhysicalTransport* transport = thermoPhyProp->getTransportPara();
        if (!equation || !thermodynamics || !transport) return false;
        //写出thermoType
        DICT::FITKDictGroup* thermoTypeGroup = new DICT::FITKDictGroup;
        thermoTypeGroup->append("type", "heSolidThermo");
        thermoTypeGroup->append("mixture", "pureMixture");
        //写出transport类型
        QString transportName = transport->getDataObjectName();
        QString transportType;
        if (transportName == "Constant Isotropic")
            transportType = "constIsoSolid";
        else if (transportName == "Constant Anisotropic")
            transportType = "constAnIso";
        thermoTypeGroup->append("transport", transportType);
        //写出thermo类型
        QString thermoName = thermodynamics->getDataObjectName();
        QString thermoType;
        if (thermoName == "Polynomial")
            thermoType = "ePolynomial";
        else if (thermoName == "eConstant")
            thermoType = "eConst";
        thermoTypeGroup->append("thermo", thermoType);
        //写出equationOfState类型
        QString name = equation->getDataObjectName();
        QString equationType;
        if (name == "Constant Density")
            equationType = "rhoConst";
        thermoTypeGroup->append("equationOfState", equationType);

        thermoTypeGroup->append("specie", "specie");
        thermoTypeGroup->append("energy", "sensibleInternalEnergy");
        m_objectDict->append("thermoType", thermoTypeGroup);

        //写出mixture
        DICT::FITKDictGroup* mixtureParaGroup = new DICT::FITKDictGroup;
        this->speciePropW(thermoPhyProp->getSpeciePara(), mixtureParaGroup);
        this->thermodynamicsPropW(thermodynamics, mixtureParaGroup);
        this->transportPropW(transport, mixtureParaGroup);
        this->equationOfStatePropW(equation, mixtureParaGroup);

        m_objectDict->append("mixture", mixtureParaGroup);
        return true;
    }

    bool FITKOFThermoPhyPropDictWriter::speciePropW(Core::FITKParameter* specie, DICT::FITKDictGroup* group)
    {
        if (!specie) return false;
        //写出specie参数属性
        bool ok = false;
        double v = specie->getDoubleParaByName("molWeight", ok);
        DICT::FITKDictGroup* specieGroup = new DICT::FITKDictGroup;
        if (ok)
        {
            specieGroup->append("nMoles", 1.0);
            specieGroup->append("molWeight", v);
        }
        group->append("specie", specieGroup);
        return false;
    }

    bool FITKOFThermoPhyPropDictWriter::thermodynamicsPropW(Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics, DICT::FITKDictGroup* group)
    {
        if (!thermodynamics) return false;
        //写出thermodynamics参数属性
        DICT::FITKDictGroup* thermodynamicsGroup = new DICT::FITKDictGroup;
        this->parametersW(thermodynamics->getThermodynamicsPara(), thermodynamicsGroup);
        group->append("thermodynamics", thermodynamicsGroup);
        return false;
    }

    bool FITKOFThermoPhyPropDictWriter::transportPropW(Interface::FITKOFAbsThermoPhysicalTransport* transport, DICT::FITKDictGroup* group)
    {
        if (!transport) return false;
        //写出transport参数属性
        DICT::FITKDictGroup* transportGroup = new DICT::FITKDictGroup;
        this->parametersW(transport->getTransportPara(), transportGroup);
        group->append("transport", transportGroup);
        return false;
    }

    bool FITKOFThermoPhyPropDictWriter::equationOfStatePropW(Interface::FITKOFAbsThermoPhysicalEquationOfState* equation, DICT::FITKDictGroup* group)
    {
        if (!equation) return false;
        //写出equationOfState参数属性
        DICT::FITKDictGroup* equationOfStateGroup = new DICT::FITKDictGroup;
        this->parametersW(equation->getEquationOfStatePara(), equationOfStateGroup);
        group->append("equationOfState", equationOfStateGroup);
        return false;
    }

    bool FITKOFThermoPhyPropDictWriter::parametersW(Core::FITKParameter * para, DICT::FITKDictGroup * group)
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



