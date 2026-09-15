/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFFVSchemesWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFDiscretization.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
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
    FITKOFFVSchemesWriter::FITKOFFVSchemesWriter()
    {
    }
    FITKOFFVSchemesWriter::~FITKOFFVSchemesWriter()
    {
    }

    void FITKOFFVSchemesWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/fvSchemes").arg(filePath);
    }

    bool FITKOFFVSchemesWriter::run()
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
                m_fileName = QString("%1/system/fvSchemes").arg(m_filePath);
            else
                m_fileName = QString("%1/system/%2/fvSchemes").arg(m_filePath).arg(dirName);
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
                Ok = this->writeFluidRegionSchemes();
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                Ok = this->writeSolidRegionSchemes();
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::None)
                Ok = this->writeEmptySchemes();

            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
        }
        return true;
    }

    QString FITKOFFVSchemesWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFFVSchemesWriter::dictObjectFoamFile(QString location)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "fvSchemes");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFFVSchemesWriter::writeFluidRegionSchemes()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;

        //根据不同求解器设置求解通量
        Interface::FITKOFSolverTypeEnum::FITKOFSolverType solverType = solver->getSolverType();
        //获取离散数据
        Interface::FITKOFDiscretization* discretization = physicsData->getDiscretization();
        if (!discretization)return this->writeEmptySchemes();
        bool ok = this->writeDDTSchemes(discretization->getTimeOption());
        ok &= this->writeGradSchemes(discretization->getGradients());
        if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE)
            ok &= this->writeDivSchemesSimple(discretization);
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
            ok &= this->writeDivSchemesInter(discretization);
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION)
            ok &= this->writeDivSchemesSHTMultiRegion(discretization);
        ok &= this->writeLaplacianSchemes(discretization);
        ok &= this->writeInterpolationSchemes(discretization->getInterpolation());
        ok &= this->writesnGradSchemes(discretization->getGradients());
        ok &= this->writeFluxWall();
        return ok;
    }

    bool FITKOFFVSchemesWriter::writeSolidRegionSchemes()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;

        //根据不同求解器设置求解通量
        Interface::FITKOFSolverTypeEnum::FITKOFSolverType solverType = solver->getSolverType();
        //获取离散数据
        Interface::FITKOFDiscretization* discretization = physicsData->getDiscretization();
        if (!discretization)return this->writeEmptySchemes();
        bool ok = this->writeDDTSchemes(discretization->getTimeOption());
        ok &= this->writeGradSchemes(discretization->getGradients());
        if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE)
            ok &= this->writeDivSchemesSimple(discretization);
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
            ok &= this->writeDivSchemesInter(discretization);
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION)
            ok &= this->writeDivSchemesSHTMultiRegion(discretization);
        ok &= this->writeLaplacianSchemes(discretization);
        ok &= this->writeInterpolationSchemes(discretization->getInterpolation());
        ok &= this->writesnGradSchemes(discretization->getGradients());
        return ok;
    }

    bool FITKOFFVSchemesWriter::writeEmptySchemes()
    {
        if (!m_objectDict) return false;
        DICT::FITKDictGroup* timeOp = new DICT::FITKDictGroup;
        m_objectDict->append("ddtSchemes", timeOp);
        timeOp->append("default", "none");

        DICT::FITKDictGroup* gradSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("gradSchemes", gradSchemes);
        gradSchemes->append("default", "none");

        DICT::FITKDictGroup* divSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("divSchemes", divSchemes);
        divSchemes->append("default", "none");

        DICT::FITKDictGroup* laplacianSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("laplacianSchemes", laplacianSchemes);
        laplacianSchemes->append("default", "none");

        DICT::FITKDictGroup* interpolationSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("interpolationSchemes", interpolationSchemes);
        interpolationSchemes->append("default", "none");

        DICT::FITKDictGroup* snGradSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("snGradSchemes", snGradSchemes);
        snGradSchemes->append("default", "none");

        DICT::FITKDictGroup* fluxRequired = new DICT::FITKDictGroup;
        m_objectDict->append("fluxRequired", fluxRequired);
        fluxRequired->append("default", "no");
        return true;
    }

    bool FITKOFFVSchemesWriter::fvSchemesW()
    {
        if (!m_objectDict) return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;

        //根据不同求解器设置求解通量
        Interface::FITKOFSolverTypeEnum::FITKOFSolverType solverType = solver->getSolverType();
        //获取离散数据
        Interface::FITKOFDiscretization* discretization = physicsData->getDiscretization();
        if (!discretization)return this->writeEmptySchemes();
        bool ok = this->writeDDTSchemes(discretization->getTimeOption());
        ok &= this->writeGradSchemes(discretization->getGradients());
        if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE)
            ok &= this->writeDivSchemesSimple(discretization);
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
            ok &= this->writeDivSchemesInter(discretization);
        ok &= this->writeLaplacianSchemes(discretization);
        ok &= this->writeInterpolationSchemes(discretization->getInterpolation());
        ok &= this->writesnGradSchemes(discretization->getGradients());
        ok &= this->writeFluxWall();
        return ok;
    }

    bool FITKOFFVSchemesWriter::writeDDTSchemes(Core::FITKParameter* timeOption)
    {
        if (!timeOption)return false;
        DICT::FITKDictGroup* timeOp = new DICT::FITKDictGroup;
        m_objectDict->append("ddtSchemes", timeOp);
        Core::FITKParamRadioGroup* timePara = dynamic_cast<Core::FITKParamRadioGroup*>(timeOption->getDataByName("Time Type"));
        Core::FITKRadioGroupValue value = timePara->getCurrentRadioGroupValue();
        QString paraValue;
        if (value._name == "Steady State")
            paraValue = "steadyState";
        else if (value._name == "Implicit Euler")
            paraValue = "Euler";
        else if (value._name == "Backward")
            paraValue = "backward";
        else if (value._name == "Crank-Nicolson")
        {
            if (!value._value)return false;
            bool ok = false;
            double v = value._value->getDoubleParaByName("Crank-Nicolson", ok);
            paraValue = QString("CrankNicolson %1").arg(QString::number(v));
        }
        else if (value._name == "Local Euler")
            paraValue = "localEuler";
        //获取boundary
        QList<Core::FITKAbstractEasyParam*> baselist = timePara->getSubValues();
        bool isBounded = false;
        for (Core::FITKAbstractEasyParam* data : baselist)
        {
            if (data->getDataObjectName() == "Bounded")
            {
                Core::FITKParamBool* d = dynamic_cast<Core::FITKParamBool*>(data);
                if (!d)return false;
                isBounded = d->getValue();
            }
        }
        if (isBounded)
            timeOp->append("default", QString("bounded %1").arg(paraValue));
        else
            timeOp->append("default", paraValue);
        return true;
    }

    bool FITKOFFVSchemesWriter::writeGradSchemes(Core::FITKParameter* gradient)
    {
        if (!gradient)return false;
        DICT::FITKDictGroup* gradSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("gradSchemes", gradSchemes);
        Core::FITKParamRadioGroup* gradientData = dynamic_cast<Core::FITKParamRadioGroup*>(gradient->getDataByName("Gradient"));
        if (!gradientData)return false;
        Core::FITKRadioGroupValue value = gradientData->getCurrentRadioGroupValue();
        if (value._name.isEmpty()) return false;
        QString gradType;
        if (value._name == "Gauss")
        {
            if (!value._value)return false;
            Core::FITKParamCombox* gauss = dynamic_cast<Core::FITKParamCombox*>(value._value->getDataByName("Gauss"));
            if (!gauss)return false;
            QString gaussType = gauss->getCurrentValue().toLower().remove(' ');
            gradType = QString("Gauss %1").arg(gaussType);
        }
        else if (value._name == "Least Squares")
            gradType = "leastSquares";
        else if (value._name == "Fourth")
            gradType = "fourth";
        //获取boundary
        Core::FITKParameter* subValue = gradientData->getSubValuePara();
        if (!subValue) return false;
        Core::FITKParamRadioGroup* limitScheme = dynamic_cast<Core::FITKParamRadioGroup*>(subValue->getDataByName("Limiting Scheme"));
        if (!limitScheme)return false;
        Core::FITKRadioGroupValue valueLimit = limitScheme->getCurrentRadioGroupValue();
        if (valueLimit._name == "Cell Limited")
        {
            if (!valueLimit._value)return false;
            Core::FITKParamDouble* cell = dynamic_cast<Core::FITKParamDouble*>(valueLimit._value->getDataByName("Cell Limited"));
            if (!cell)return false;
            gradType = QString("cellLimited %1 %2").arg(gradType).arg(QString::number(cell->getValue()));
        }
        else if (valueLimit._name == "Face Limited")
        {
            if (!valueLimit._value)return false;
            Core::FITKParamDouble* face = dynamic_cast<Core::FITKParamDouble*>(valueLimit._value->getDataByName("Face Limited"));
            if (!face)return false;
            gradType = QString("faceLimited %1 %2").arg(gradType).arg(QString::number(face->getValue()));
        }
        gradSchemes->append("default", gradType);
        return true;
    }

    bool FITKOFFVSchemesWriter::writeDivSchemesSimple(Interface::FITKOFDiscretization* discretization)
    {
        if (!discretization)return false;
        DICT::FITKDictGroup* divSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("divSchemes", divSchemes);
        divSchemes->append("div((nuEff*dev2(T(grad(U)))))", "Gauss linear");
        divSchemes->append("div(R)", "Gauss linear");
        divSchemes->append("default", "bounded Gauss upwind");
        int count = discretization->getConvectionCount();
        for (int i = 0; i < count; ++i)
        {
            QString vName = discretization->getConvectionVName(i);
            Core::FITKParameter* vPara = discretization->getConvectionVPara(i);
            if (!vPara)continue;
            QString boundaryType = getConvectionValue(vPara);
            divSchemes->append(QString("div(phi,%1)").arg(vName), boundaryType);
        }
        return true;
    }

    bool FITKOFFVSchemesWriter::writeDivSchemesInter(Interface::FITKOFDiscretization* discretization)
    {
        if (!discretization)return false;
        DICT::FITKDictGroup* divSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("divSchemes", divSchemes);
        divSchemes->append("div(((rho*nuEff)*dev2(T(grad(U)))))", "Gauss linear");
        divSchemes->append("div(R)", "Gauss linear");
        divSchemes->append("default", "Gauss upwind");
        int count = discretization->getConvectionCount();
        for (int i = 0; i < count; ++i)
        {
            QString vName = discretization->getConvectionVName(i);
            Core::FITKParameter* vPara = discretization->getConvectionVPara(i);
            if (!vPara)continue;
            QString boundaryType = getConvectionValue(vPara);

            if (vName == "U")
                divSchemes->append(QString("div(rhoPhi,%1)").arg(vName), boundaryType);
            else
                divSchemes->append(QString("div(phi,%1)").arg(vName), boundaryType);
        }

        divSchemes->append("div(phi,alpha)", "Gauss vanLeer");
        divSchemes->append("div(phirb,alpha)", "Gauss linear");
        return true;
    }

    bool FITKOFFVSchemesWriter::writeDivSchemesSHTMultiRegion(Interface::FITKOFDiscretization * discretization)
    {
        if (!discretization)return false;
        DICT::FITKDictGroup* divSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("divSchemes", divSchemes);
        divSchemes->append("div(((rho*nuEff)*dev2(T(grad(U)))))", "Gauss linear");
        divSchemes->append("div(R)", "Gauss linear");
        divSchemes->append("default", "Gauss upwind");
        int count = discretization->getConvectionCount();
        for (int i = 0; i < count; ++i)
        {
            QString vName = discretization->getConvectionVName(i);
            Core::FITKParameter* vPara = discretization->getConvectionVPara(i);
            if (!vPara)continue;
            QString boundaryType = getConvectionValue(vPara);

            divSchemes->append(QString("div(phi,%1)").arg(vName), boundaryType);
        }
        return true;
    }

    QString FITKOFFVSchemesWriter::getConvectionValue(Core::FITKParameter* vPara)
    {
        if (!vPara)return QString();
        Core::FITKParamRadioGroup* convectionType = dynamic_cast<Core::FITKParamRadioGroup*>(vPara->getDataByName("Convection Type"));
        if (!convectionType)QString();
        Core::FITKRadioGroupValue conT = convectionType->getCurrentRadioGroupValue();
        QString boundaryType = conT._name.toLower().remove(' ');
        if (boundaryType == "upwind")
            boundaryType = "Gauss upwind";
        else if (boundaryType == "linearupwind")
            boundaryType = "Gauss linearUpwind";

        Core::FITKParameter* sub = convectionType->getSubValuePara();
        if (!sub)return QString();
        int count = sub->getDataCount();
        if (conT._labelValue.size() != count)return QString();
        bool selectRes[3]{ false };
        for (int i = 0; i < count; ++i)
        {
            if (!conT._labelValue.at(i)) continue;
            Core::FITKParamBool* selectData = dynamic_cast<Core::FITKParamBool*>(sub->getDataByIndex(i));
            if (!selectData)continue;
            QString name = selectData->getDataObjectName();
            if (name == "VectorSpecific")
                selectRes[0] = selectData->getValue();
            else if (name == "Bounded")
                selectRes[1] = selectData->getValue();
            else if (name == "SkewCorrected")
                selectRes[2] = selectData->getValue();
        }
        if (selectRes[0]) boundaryType += "V";
        if (selectRes[2]) boundaryType = "skewCorrected " + boundaryType;
        if (selectRes[1]) boundaryType = "bounded " + boundaryType;
        return boundaryType;
    }

    bool FITKOFFVSchemesWriter::writeLaplacianSchemes(Interface::FITKOFDiscretization* discretization)
    {
        if (!discretization)return false;
        DICT::FITKDictGroup* laplacianSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("laplacianSchemes", laplacianSchemes);
        
        Core::FITKParameter* interpolation = discretization->getInterpolation();
        if (!interpolation)return false;
        Core::FITKParamCombox* scheme = dynamic_cast<Core::FITKParamCombox*>(interpolation->getDataByName("Scheme"));
        if (!scheme)return false;
        QString schemeType = scheme->getCurrentValue().toLower().remove(' ');
        
        Core::FITKParameter* gradient = discretization->getGradients();
        if (!gradient) return false;
        Core::FITKParamRadioGroup* surGrad = dynamic_cast<Core::FITKParamRadioGroup*>(gradient->getDataByName("Surface Normal Gradient"));
        if (!surGrad) return false;
        Core::FITKRadioGroupValue surGradType = surGrad->getCurrentRadioGroupValue();

        QString defaultValue;
        if (surGradType._name == "Corrected")
            defaultValue = QString("Gauss %1 corrected").arg(schemeType);
        else if (surGradType._name == "Uncorrected")
            defaultValue = QString("Gauss %1 uncorrected").arg(schemeType);

        laplacianSchemes->append("default", defaultValue);
        return true;
    }

    bool FITKOFFVSchemesWriter::writeInterpolationSchemes(Core::FITKParameter* interpolation)
    {
        if (!interpolation)return false;
        DICT::FITKDictGroup* interpolationSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("interpolationSchemes", interpolationSchemes);
        
        Core::FITKParamCombox* scheme = dynamic_cast<Core::FITKParamCombox*>(interpolation->getDataByName("Scheme"));
        if (!scheme)return false;
        QString schemeType = scheme->getCurrentValue().toLower().remove(' ');
        interpolationSchemes->append("default", schemeType);
        return true;
    }

    bool FITKOFFVSchemesWriter::writesnGradSchemes(Core::FITKParameter* gradient)
    {
        if (!gradient)return false;
        DICT::FITKDictGroup* snGradSchemes = new DICT::FITKDictGroup;
        m_objectDict->append("snGradSchemes", snGradSchemes);
        
        Core::FITKParamRadioGroup* surGrad = dynamic_cast<Core::FITKParamRadioGroup*>(gradient->getDataByName("Surface Normal Gradient"));
        if (!surGrad) return false;
        Core::FITKRadioGroupValue surGradType = surGrad->getCurrentRadioGroupValue();

        QString defaultValue;
        if (surGradType._name == "Corrected")
            snGradSchemes->append("default", "corrected");
        else if (surGradType._name == "Uncorrected")
            snGradSchemes->append("default", "uncorrected");
        return true;
    }

    bool FITKOFFVSchemesWriter::writeFluxWall()
    {
        //不同求解器计算的通量是不一样的
        if (!m_objectDict) return false;
        DICT::FITKDictGroup* fluxRequired = new DICT::FITKDictGroup;
        m_objectDict->append("fluxRequired", fluxRequired);
        fluxRequired->append("default", "no");
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;

        //根据不同求解器设置求解通量
        Interface::FITKOFSolverTypeEnum::FITKOFSolverType solverType = solver->getSolverType();
        if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE)
        {
            fluxRequired->append("p", "");
        }
        else if (solverType == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            fluxRequired->append("p_rgh", "");
            fluxRequired->append("pcorr", "");
            fluxRequired->append("alpha", "");
        }

        //wallDist
        DICT::FITKDictGroup* wallDist = new DICT::FITKDictGroup;
        m_objectDict->append("wallDist", wallDist);
        wallDist->append("method", "meshWave");
        wallDist->append("nRequired", false);
        return true;
    }
}



