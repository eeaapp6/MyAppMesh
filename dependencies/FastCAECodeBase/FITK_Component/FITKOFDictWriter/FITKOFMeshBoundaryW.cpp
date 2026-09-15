/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFMeshBoundaryW.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementList.h"
#include <QTextStream>
#include <QFileInfo>
#include <QtMath>
#include <QFile>
#include <QDir>
//命名空间
namespace IO
{
    FITKOFMeshBoundaryW::FITKOFMeshBoundaryW()
    {
    }
    FITKOFMeshBoundaryW::~FITKOFMeshBoundaryW()
    {
    }

    void FITKOFMeshBoundaryW::setFilePath(const QString& filePath)
    {
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        FITKOFAbstractDictWriter::setFilePath(filePath);
    }

    bool FITKOFMeshBoundaryW::run()
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
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/polyMesh/boundary").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/polyMesh/boundary").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取路径
            QDir dir;
            path = m_fileName.mid(0, m_fileName.lastIndexOf('/'));
            //路径是否存在
            if (!dir.exists(path))return true;
            //打开文件
            QFile file(m_fileName);
            if (file.exists())file.remove();
            //设置写出
            if (!file.open(QIODevice::ReadWrite | QIODevice::NewOnly))
            {
                file.close();
                return false;
            }
            //打开文件流
            m_stream = new QTextStream(&file);

            //FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile("constant/polyMesh");
            else
                this->dictObjectFoamFile(QString("constant/%1/polyMesh").arg(dirName));
            //写出数据
            bool Ok = this->meshBoundaryW(regionMesh->getDataObjectID());
            //关闭文件
            file.close();
            delete m_stream;
            m_stream = nullptr;
        }

        return true;
    }

    QString FITKOFMeshBoundaryW::getFileName()
    {
        return m_fileName;
    }

    void FITKOFMeshBoundaryW::dictObjectFoamFile(QString location)
    {
        //FoamFile
        if (!m_stream)return;
        *m_stream << "FoamFile" << endl;
        *m_stream << "{" << endl;
        *m_stream << "    version    2.0;" << endl;
        *m_stream << "    format    ascii;" << endl;
        *m_stream << "    class    polyBoundaryMesh;" << endl;
        *m_stream << QString("    location    \"%1\";").arg(location) << endl;
        *m_stream << "    object    boundary;" << endl;
        *m_stream << "}" << endl;
    }

    bool FITKOFMeshBoundaryW::meshBoundaryW(int meshRegionID)
    {
        if (!m_stream)return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取网格数据
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshData)return false;
        Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByID(meshRegionID);
        if (!regionMesh) return false;
        Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
        if (!boundaryMeshMgr)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取BoundaryManager
        Interface::FITKOFBoundaryManager* boundaryPhyMgr = physicsData->getBoundaryManager();
        if (!boundaryPhyMgr)return false;
        //设置边界
        int count = boundaryMeshMgr->getDataCount();
        *m_stream << count << endl;
        *m_stream << "(" << endl;
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshMgr->getDataByIndex(i);
            if (!boundaryMesh)continue;
            QString meshName = boundaryMesh->getDataObjectName();
            *m_stream << QString("    %1").arg(meshName) << endl;
            *m_stream << "    {" << endl;
            //获取边界类型
            Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType type = Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BEmpty;
            Interface::FITKOFBoundary* boundaryData = boundaryPhyMgr->getBoundary(boundaryMesh->getDataObjectID());
            if (boundaryData) type = boundaryData->getBoundaryType();
            if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall)
                *m_stream << QString("        type            wall;") << endl;
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet || type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet
                || type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet || type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow)
                *m_stream << QString("        type            patch;") << endl;
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BSymmetry)
                *m_stream << QString("        type            symmetry;") << endl;
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BWedge)
                *m_stream << QString("        type            wedge;") << endl;
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BEmpty)
                *m_stream << QString("        type            empty;") << endl;
            else if (type == Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType::BMappedWall)
            {
                //需要写出绑定的边界数据-必要的
                *m_stream << QString("        type            mappedWall;") << endl;
                QString regionName, boundaryName;
                Interface::FITKOFBoundary* boundaryMap = boundaryPhyMgr->getMappendWallBoundary(boundaryData->getDataObjectID());
                if (boundaryMap)
                {
                    Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByID(boundaryMap->getMeshRegionID());
                    if (regionMesh)
                        regionName = regionMesh->getDataObjectName();
                    Interface::FITKBoundaryMeshVTK* boundary = boundaryMap->getMeshBoundary();
                    if (boundary)
                        boundaryName = boundary->getDataObjectName();
                }
                *m_stream << QString("        neighbourRegion    %1;").arg(regionName) << endl;
                *m_stream << QString("        neighbourPatch    %1;").arg(boundaryName) << endl;
                *m_stream << QString("        transformType    none;") << endl;
            }
            //设置面和单元
            int faceNum = boundaryMesh->getElementCount();
            *m_stream << QString("        nFaces          %1;").arg(faceNum) << endl;
            QVariant value = boundaryMesh->getUserData(Core::FITKUserData::FITKUserRole);
            *m_stream << QString("        startFace       %1;").arg(value.toInt()) << endl;
            *m_stream << "    }" << endl;
        }
        
        *m_stream << ")" << endl;
        return true;
    }
}



