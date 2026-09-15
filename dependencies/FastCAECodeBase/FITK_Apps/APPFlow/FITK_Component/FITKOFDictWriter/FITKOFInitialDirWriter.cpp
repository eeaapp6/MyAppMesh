/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFInitialDirWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITKOFChangeDictionaryDictW.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundaryType.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFInitialConditions.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFInitialDirWriter::FITKOFInitialDirWriter()
    {
        m_boundaryTool = new FITKOFChangeDictionaryDictW;
    }
    FITKOFInitialDirWriter::~FITKOFInitialDirWriter()
    {
        if (!m_boundaryTool) delete m_boundaryTool;
    }

    void FITKOFInitialDirWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = m_filePath;
    }

    bool FITKOFInitialDirWriter::run()
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
            if (path.isEmpty() || typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::None) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            QString filePath;
            if (dirName.toLower() == "constant")
                filePath = QString("%1/0/").arg(m_filePath);
            else
                filePath = QString("%1/0/%2/").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (filePath.isEmpty())return false;
            basicDataBoundaryW(filePath, dirName, regionMesh->getDataObjectID());
            regionPropDataW(filePath, dirName, regionMesh->getDataObjectID());
            
        }
        return true;
    }

    QString FITKOFInitialDirWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFInitialDirWriter::dictObjectFoamFile(QString location, QString vName)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        if (vName == "U")
            group->append("class", "volVectorField");
        else
            group->append("class", "volScalarField");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", vName);
        group->append("arch", "\"LSB;label=32;scalar=64\"");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFInitialDirWriter::basicDataBoundaryW(QString filePath, QString dirName, int regionMeshID)
    {
        // 获取物理数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        //获取initial
        Interface::FITKOFInitialConditions* initialCond = physicsData->getInitialConditions();
        if (!initialCond)return false;
        Core::FITKParameter* vPara = initialCond->getBasicData();
        for (int i = 0; i < vPara->getDataCount(); ++i)
        {
            Core::FITKAbstractEasyParam* d = vPara->getDataByIndex(i);
            if (!d)continue;
            QString vName = d->getDataObjectName();
            m_fileName = QString("%1/%2").arg(filePath).arg(vName);
            //打开字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile("0", vName);
            else
                this->dictObjectFoamFile(QString("0/%1").arg(dirName), vName);
            //设置写出的网格区域数据
            bool Ok = this->initialFileWrite(vName, d, regionMeshID);
            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();
            //关闭字典工程
            this->closeDictObject();
        }
        
        return true;
    }

    bool FITKOFInitialDirWriter::regionPropDataW(QString filePath, QString dirName, int regionMeshID)
    {
        // 获取物理数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        //获取initial
        Interface::FITKOFInitialConditions* initialCond = physicsData->getInitialConditions();
        if (!initialCond)return false;
        Interface::FITKOFInitPropRegions * regionProp = initialCond->getInitPropRegionByRegionMeshID(regionMeshID);
        if (!regionProp) return false;
        Core::FITKParameter * vPara = regionProp->getRegionPara();
        for (int i = 0; i < vPara->getDataCount(); ++i)
        {
            Core::FITKAbstractEasyParam* d = vPara->getDataByIndex(i);
            if (!d)continue;
            QString vName = d->getDataObjectName();
            m_fileName = QString("%1/%2").arg(filePath).arg(vName);
            //打开字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile("0", vName);
            else
                this->dictObjectFoamFile(QString("0/%1").arg(dirName), vName);
            //设置写出的网格区域数据
            bool Ok = this->initialFileWrite(vName, d, regionMeshID, false);
            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();
            //关闭字典工程
            this->closeDictObject();
        }
        //如果是共轭传热需写出P文件
        if (physicsData->getSolver() && physicsData->getSolver()->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION)
        {
            m_fileName = QString("%1/%2").arg(filePath).arg("p");
            //打开字典工程
            this->openDictObject();
            //FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile("0", "p");
            else
                this->dictObjectFoamFile(QString("0/%1").arg(dirName), "p");
            //设置写出的网格区域数据
            bool Ok = this->initialCHTMultiRegionPFile(regionMeshID);
            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();
            //关闭字典工程
            this->closeDictObject();
        }
        return true;
    }

    bool FITKOFInitialDirWriter::initialFileWrite(QString vName, Core::FITKAbstractEasyParam* vPara, int regionMeshID, bool isBasic)
    {
        if (!m_objectDict) return false;
        bool ok = this->dimensionsW(vName);
        if (!ok)return false;
        QString value;
        Core::FITKAbstractEasyParam::FITKEasyParamType type = vPara->getParamType();
        if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
        {
            Core::FITKParamDouble * p = dynamic_cast<Core::FITKParamDouble*>(vPara);
            if (p) value = QString("uniform %1").arg(p->getValue());
        }
        else if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
        {
            Core::FITKParamDoubleList * p = dynamic_cast<Core::FITKParamDoubleList*>(vPara);
            if (p)
            {
                QStringList vlist;
                for (int i = 0; i < p->getCount(); ++i)
                    vlist.append(QString::number(p->getValue(i)));
                value = QString("uniform (%1)").arg(vlist.join(' '));
            }
        }
        if (value.isEmpty())return false;
        m_objectDict->append("internalField", value);
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!physicsData || !meshData)return false;
        Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByID(regionMeshID);
        if (!regionMesh) return false;
        Interface::FITKBoundaryMeshVTKManager * boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
        if (!boundaryMeshMgr) return false;
        //获取离散数据
        Interface::FITKOFBoundaryManager* boundaryMgr = physicsData->getBoundaryManager();
        if (!boundaryMgr)return false;
        int count = boundaryMeshMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshMgr->getDataByIndex(i);
            if (!boundaryMesh) continue;
            //获取boundaryField
            DICT::FITKDictGroup* boundaryField = nullptr;
            DICT::FITKDictValue* boundF = m_objectDict->value("boundaryField");
            if (!boundF)
            {
                boundaryField = new DICT::FITKDictGroup;
                m_objectDict->append("boundaryField", boundaryField);
            }
            else
                boundaryField = boundF->toGroup(ok);
            if (!boundaryField)continue;
            //创建边界数据
            DICT::FITKDictGroup* boundaryG = new DICT::FITKDictGroup;
            boundaryField->append(boundaryMesh->getDataObjectName(), boundaryG);
            //获取边界数据
            int regionBoundaryID = boundaryMesh->getDataObjectID();
            Interface::FITKOFBoundary * boundary = boundaryMgr->getBoundary(regionBoundaryID);
            if (!boundary)
            {
                boundaryG->append("type", "empty");
            }
            else
            {
                Interface::FITKOFAbsBoundaryType* para = boundary->getBoundary(vName);
                if (!para)
                {
                    boundaryG->append("type", "calculated");
                    boundaryG->append("value", value);
                }
                if (m_boundaryTool)
                    m_boundaryTool->paraBTyW(para, boundaryG, vName, regionMeshID, isBasic);
                if (vName.contains("alpha"))
                {
                    DICT::FITKDictValue* v = boundaryG->value("phi");
                    if (v)
                    {
                        boundaryG->removeKey("phi");
                        boundaryG->append("value", value);
                    }
                }
            }
        }
        //int count = boundaryMgr->getDataCount();
        //for (int i = 0; i < count; ++i)
        //{
        //    Interface::FITKOFBoundary* boundary = boundaryMgr->getDataByIndex(i);
        //    if (!boundary)continue;
        //    if (boundary->getMeshRegionID() != regionMeshID) continue;
        //    //获取boundaryField
        //    DICT::FITKDictGroup* boundaryField = nullptr;
        //    DICT::FITKDictValue* boundF = m_objectDict->value("boundaryField");
        //    if (!boundF)
        //    {
        //        boundaryField = new DICT::FITKDictGroup;
        //        m_objectDict->append("boundaryField", boundaryField);
        //    }
        //    else
        //        boundaryField = boundF->toGroup(ok);
        //    if (!boundaryField)continue;
        //    Interface::FITKBoundaryMeshVTK* boundaryMesh = boundary->getMeshBoundary();
        //    if (!boundaryMesh)return false;
        //    DICT::FITKDictGroup* boundaryG = new DICT::FITKDictGroup;
        //    boundaryField->append(boundaryMesh->getDataObjectName(), boundaryG);
        //    Interface::FITKOFAbsBoundaryType* para = boundary->getBoundary(vName);
        //    if (!para)
        //    {
        //        boundaryG->append("type", "calculated");
        //        boundaryG->append("value", value);
        //    }
        //    if (m_boundaryTool)
        //        m_boundaryTool->paraBTyW(para, boundaryG, vName);
        //    if (vName.contains("alpha"))
        //    {
        //        DICT::FITKDictValue* v = boundaryG->value("phi");
        //        if (v)
        //        {
        //            boundaryG->removeKey("phi");
        //            boundaryG->append("value", value);
        //        }
        //    }
        //}
        return true;
    }

    bool FITKOFInitialDirWriter::dimensionsW(QString vName)
    {
        if (!m_objectDict) return false;
        if (vName == "p")
            m_objectDict->append("dimensions", "[0 2 -2 0 0 0 0]");
        else if (vName == "U")
            m_objectDict->append("dimensions", "[0 1 -1 0 0 0 0]");
        else if (vName == "omega")
            m_objectDict->append("dimensions", "[0 0 -1 0 0 0 0]");
        else if (vName == "k")
            m_objectDict->append("dimensions", "[0 2 -2 0 0 0 0]");
        else if (vName == "T")
            m_objectDict->append("dimensions", "[0 0 0 1 0 0 0]");
        else if (vName == "nut")
            m_objectDict->append("dimensions", "[0 2 -1 0 0 0 0]");
        else if (vName.contains("alpha"))
            m_objectDict->append("dimensions", "[0 0 0 0 0 0 0]");
        else if (vName == "p_rgh")
            m_objectDict->append("dimensions", "[1 -1 -2 0 0 0 0]");
        return true;
    }

    bool FITKOFInitialDirWriter::initialCHTMultiRegionPFile(int regionMeshID)
    {
        // 获取物理数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!physicsData || !meshData) return false;
        Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType type = physicsData->getRegionMeshType(regionMeshID);
        //获取initial
        Interface::FITKOFInitialConditions* initialCond = physicsData->getInitialConditions();
        if (!initialCond)return false;
        //获取区域初始化属性
        Interface::FITKOFInitPropRegions * regionProp = initialCond->getInitPropRegionByRegionMeshID(regionMeshID);
        if (!regionProp) return false;
        Core::FITKParameter * vParaList = regionProp->getRegionPara();
        if (!vParaList) return false;
        Core::FITKAbstractEasyParam* vPara = vParaList->getDataByName("p_rgh");
        QString value;
        if (vPara)
        {
            Core::FITKParamDouble * p = dynamic_cast<Core::FITKParamDouble*>(vPara);
            if (p) value = QString("uniform %1").arg(p->getValue());
        }
        else
        {
            value = QString("uniform 0.0");
        }
        m_objectDict->append("dimensions", "[1 -1 -2 0 0 0 0]");
        m_objectDict->append("internalField", value);
        //获取边界数据
        Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByID(regionMeshID);
        if (!regionMesh) return false;
        Interface::FITKBoundaryMeshVTKManager * boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
        if (!boundaryMeshMgr) return false;
        //获取边界属性管理器数据
        Interface::FITKOFBoundaryManager* boundaryMgr = physicsData->getBoundaryManager();
        if (!boundaryMgr)return false;
        int count = boundaryMeshMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshMgr->getDataByIndex(i);
            if (!boundaryMesh) continue;
            //获取boundaryField
            bool ok = false;
            DICT::FITKDictGroup* boundaryField = nullptr;
            DICT::FITKDictValue* boundF = m_objectDict->value("boundaryField");
            if (!boundF)
            {
                boundaryField = new DICT::FITKDictGroup;
                m_objectDict->append("boundaryField", boundaryField);
            }
            else
                boundaryField = boundF->toGroup(ok);
            if (!boundaryField)continue;
            //创建边界数据
            DICT::FITKDictGroup* boundaryG = new DICT::FITKDictGroup;
            boundaryField->append(boundaryMesh->getDataObjectName(), boundaryG);
            //获取边界数据
            int regionBoundaryID = boundaryMesh->getDataObjectID();
            Interface::FITKOFBoundary * boundary = boundaryMgr->getBoundary(regionBoundaryID);
            if (!boundary)
            {
                boundaryG->append("type", "empty");
            }
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
            {
                boundaryG->append("type", "calculated");
                boundaryG->append("value", value);
            }
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
            {
                boundaryG->append("type", "zeroGradient");
            }
        }
        return true;
    }
}



