/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFCreatePatchDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeGeom.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFCreatePatchDictWriter::FITKOFCreatePatchDictWriter()
    {
    }
    FITKOFCreatePatchDictWriter::~FITKOFCreatePatchDictWriter()
    {
    }

    void FITKOFCreatePatchDictWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/createPatchDict").arg(filePath);
    }

    bool FITKOFCreatePatchDictWriter::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->createPatchDictW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();

        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFCreatePatchDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFCreatePatchDictWriter::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "createPatchDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFCreatePatchDictWriter::createPatchDictW()
    {
        if (!m_objectDict)return false;
        m_objectDict->append("pointSync", false);
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKRegionMeshSizeManager* meshRegionManager = meshGen->getRegionMeshSizeMgr();
        if (!meshRegionManager)return false;
        //获取集合大小
        QList<Interface::FITKAbstractRegionMeshSize*> geomList = meshRegionManager->getRigonByType(Interface::FITKAbstractRegionMeshSize::RegionType::RigonGeom);
        //创建字典组件
        DICT::FITKDictArray* patchArray = new DICT::FITKDictArray;
        m_objectDict->append("patches", patchArray);
        //获取几何数据
        for (Interface::FITKAbstractRegionMeshSize* geom : geomList)
        {
            Interface::FITKRegionMeshSizeGeom* geomData = dynamic_cast<Interface::FITKRegionMeshSizeGeom*>(geom);
            if (!geomData)continue;
            Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(geomData->getGeomID());
            if (!geoCommand)continue;
            //获取名称
            QString name = geoCommand->getDataObjectName();
            //获取模型
            Interface::FITKAbsGeoShapeAgent* geoModel = geoCommand->getShapeAgent();
            if (!geoModel)continue;
            if (!geoModel->getVirtualTopoManager())return false;
            Interface::FITKShapeVirtualTopoManager* faceMgr = geoModel->getVirtualTopoManager()->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            if (!faceMgr)return false;
            Interface::FITKGeoComponentManager* geoComponentManager = geoModel->getGeoComponentManager();
            if (!geoComponentManager)continue;
            //已设置的面
            QList<int> index_list;
            //获取面组链表
            QList<Interface::FITKGeoComponent*> componentList = geoComponentManager->getGeoComponent(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
            for (Interface::FITKGeoComponent* componentData : componentList)
            {
                if (!componentData)continue;
                //面id
                QList<int> id_list = componentData->getMember();
                if (id_list.isEmpty())continue;
                //获取名称
                QString groupName = componentData->getDataObjectName();
                DICT::FITKDictGroup* patch = new DICT::FITKDictGroup;
                patchArray->append(patch);
                patch->append("name", QString("%1_%2").arg(name).arg(groupName));
                // PatchInfo
                DICT::FITKDictGroup* patchInfo = new DICT::FITKDictGroup;
                patch->append("patchInfo", patchInfo);
                patchInfo->append("type", "wall");
                patch->append("constructFrom", "patches");
                DICT::FITKDictArray* patches = new DICT::FITKDictArray;
                patch->append("patches", patches);
                //添加面
                for (int i = 0; i < id_list.size(); ++i)
                {
                    Interface::FITKAbsVirtualTopo* topo = faceMgr->getDataByID(id_list.at(i));
                    if (!topo)continue;
                    int index = topo->getIndexLabel();
                    patches->append(QString("%1_face_%2").arg(name).arg(index));
                    index_list.append(index);
                }
            }
            int allFaceNum = geoModel->getShapeCount(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
            if (allFaceNum == index_list.size())continue;
            //设置剩余的面
            DICT::FITKDictGroup* patch = new DICT::FITKDictGroup;
            patchArray->append(patch);
            patch->append("name", name);
            //PatchInfo
            DICT::FITKDictGroup* patchInfo = new DICT::FITKDictGroup;
            patch->append("patchInfo", patchInfo);
            patchInfo->append("type", "wall");
            patch->append("constructFrom", "patches");
            DICT::FITKDictArray* patches = new DICT::FITKDictArray;
            patch->append("patches", patches);
            for (int j = 0; j < allFaceNum; ++j)
            {
                if (index_list.contains(j))continue;
                patches->append(QString("%1_face_%2").arg(name).arg(j + 1));
            }
        }
        return true;
    }
}



