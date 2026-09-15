/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5Writer.h"
#include "FITKFlowOFHDF5AdaptorGeometryMeshSize.h"
#include "FITKFlowOFHDF5AdaptorMaterialPoint.h"
#include "FITKFlowOFHDF5AdaptorRegionMesh.h"
#include "FITKFlowOFHDF5AdaptorPhysicsData.h"
#include "FITKFlowOFHDF5AdaptorMesh.h"
#include "FITKFlowOFHDF5AdaptorGeometryComponent.h"
#include "FITKFlowOFHDF5AdaptorGeometryRefine.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//几何
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionGeometryRefine.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

//物理场
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"

#include <QDebug>

namespace IO
{
    void FITKFlowOFHDF5Writer::run()
    {
        *_resultMark = true;
        *_resultMark &= writeVersion();
        *_resultMark &= writeDataGeometry();
        *_resultMark &= writeGeoComponent();
        *_resultMark &= writeGeometryRefine();
        *_resultMark &= writeGeoMeshSize();
        *_resultMark &= writeMaterialPoint();
        *_resultMark &= writeRegionMesh();
        *_resultMark &= writeMesh();
        *_resultMark &= writePhysicsData();
       
        this->sendCalculateProgress(100);
    }

    bool FITKFlowOFHDF5Writer::writeVersion()
    {
        QString name = "Version";
        H5::Group versionGroup = _h5File->createGroup(name.toStdString());
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = versionGroup.createAttribute(name.toStdString(), H5::PredType::NATIVE_DOUBLE, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_DOUBLE, &_version);
        return true;
    }

    bool FITKFlowOFHDF5Writer::writeDataGeometry()
    {
        //获取几何数据链表
        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)return false;
        IO::FITKOCCIOHDF5Interface* h5IO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        bool isW = false;
        if (h5IO)
        {
            //设置数据并写出几何的工程数据
            h5IO->setHDF5Root(getHDF5File());
            h5IO->setRunInThread(false);
            h5IO->setDataObject(geoCmdList);
            h5IO->exec(2);
            isW = h5IO->isSuccess();
        }
        if (!isW)
            consoleMessage(3, QString("Write Geometry Errors"));

        return isW;
    }

    bool FITKFlowOFHDF5Writer::writeGeoComponent()
    {
        bool isW = true;
        //创建几何节点
        H5::Group geometryGroup = _h5File->createGroup("GeoComponent");
        //获取几何数据链表
        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)return false;
        //获取几何
        int count = geoCmdList->getDataCount();
        //写出长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = geometryGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKAbsGeoCommand* comm = geoCmdList->getDataByIndex(i);
            if (!comm) continue;
            std::string geoName = comm->getDataObjectName().toStdString();
            // 创建字符串类属性
            std::string name = std::to_string(i);
            H5::DataSpace dataspace;
            H5::StrType   datatypeStr(H5::PredType::C_S1, H5T_VARIABLE);
            H5::Attribute desAttr = geometryGroup.createAttribute(name, datatypeStr, dataspace);
            desAttr.write(datatypeStr, geoName);
            H5::Group geoComGroup = geometryGroup.createGroup(geoName);
            //写出几何面组
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryComponent>("HDF5", "Interface::FITKGeoComponentManager");
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&geoComGroup);
            adaptor->setDataObject(comm);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Command Errors, CaseName : %1").arg(comm->getDataObjectName()));
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKFlowOFHDF5Writer::writeGeometryRefine()
    {
        bool isW = true;
        //创建几何节点
        H5::Group geoRefineGroup = _h5File->createGroup("GeoRefine");
        //获取几何划分网格管理器
        Interface::FITKRegionGeometryRefineManager* geoRefineManager = Interface::FITKMeshGenInterface::getInstance()->getRegionGeometryRefineManager();
        if (!geoRefineManager)return false;
        //获取数据
        int count = geoRefineManager->getDataCount();
        //写出长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = geoRefineGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKRegionGeometryRefine* geoRefine = geoRefineManager->getDataByIndex(i);
            if (!geoRefine)continue;
            if (!geoRefine->getGeometryModel())continue;
            std::string name = std::to_string(i);
            H5::Group geoRefGroup = geoRefineGroup.createGroup(name);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryRefine>("HDF5", geoRefine);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&geoRefGroup);
            adaptor->setDataObject(geoRefine);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Refine Errors, Geometry : %1").arg(geoRefine->getGeometryModel()->getDataObjectName()));
            isW &= w;
            delete adaptor;
        }
        return true;
    }

    bool FITKFlowOFHDF5Writer::writeGeoMeshSize()
    {
        bool isW = true;
        //创建几何节点
        H5::Group geoMeshSizeGroup = _h5File->createGroup("GeoMeshSize");
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKGeometryMeshSizeManager* geoMeshSizeManager = meshGen->getGeometryMeshSizeManager();
        if (!geoMeshSizeManager)return false;
        //获取数据
        int count = geoMeshSizeManager->getDataCount();
        //写出长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = geoMeshSizeGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKGeometryMeshSize* geoMeshSize = geoMeshSizeManager->getDataByIndex(i);
            if (!geoMeshSize)continue;
            std::string name = std::to_string(i);
            H5::Group geoMSGroup = geoMeshSizeGroup.createGroup(name);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryMeshSize>("HDF5", geoMeshSize);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&geoMSGroup);
            adaptor->setDataObject(geoMeshSize);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Geometry Errors, GeometryComponentName : %1").arg(geoMeshSize->getDataObjectName()));
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKFlowOFHDF5Writer::writeMaterialPoint()
    {
        bool isW = true;
        //创建几何节点
        H5::Group materialPointsGroup = _h5File->createGroup("MaterialPoints");
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKZonePointManager* pointManager = meshGen->getZonePointManager();
        if (!pointManager)return false;
        //获取数据
        int count = pointManager->getDataCount();
        //写出长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = materialPointsGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKZonePoint* pointData = pointManager->getDataByIndex(i);
            if (!pointData)continue;
            std::string name = std::to_string(i);
            H5::Group materialPointGroup = materialPointsGroup.createGroup(name);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorMaterialPoint>("HDF5", "Interface::FITKZonePoint");
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&materialPointGroup);
            adaptor->setDataObject(pointData);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Geometry Errors, MaterialPointId : %1").arg(pointData->getDataObjectID()));
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKFlowOFHDF5Writer::writeRegionMesh()
    {
        //获取工具
        IO::FITKHDF5FileTool* tool = this->getHDF5FileTool();
        if (!tool) return false;
        bool isW = true;
        //创建几何节点
        H5::Group regionMeshGroup = _h5File->createGroup("RegionMesh");
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKRegionMeshSizeManager* regionMeshManager = meshGen->getRegionMeshSizeMgr();
        if (!regionMeshManager)return false;
        //获取数据
        int count = regionMeshManager->getDataCount();
        //写出长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = regionMeshGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKAbstractRegionMeshSize* regionMesh = regionMeshManager->getDataByIndex(i);
            if (!regionMesh)continue;
            std::string name = std::to_string(i);
            H5::Group regMeshGroup = regionMeshGroup.createGroup(name);
            int type = regionMesh->getRegionType();
            tool->writeGroupAttrInt(regMeshGroup, "RegionType", type);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRegionMesh>("HDF5", regionMesh);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&regMeshGroup);
            adaptor->setDataObject(regionMesh);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Geometry Errors, MaterialPointId : %1").arg(regionMesh->getDataObjectName()));
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKFlowOFHDF5Writer::writeMesh()
    {
        //创建物理场数据节点
        H5::Group meshGroup = _h5File->createGroup("MeshData");
        //获取全局数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取网格数据
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshData)return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorMesh>("HDF5", "Interface::FITKUnstructuredFluidMeshVTK");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&meshGroup);
        adaptor->setDataObject(meshData);
        adaptor->setWriter(this);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write MeshData Errors ! "));
        delete adaptor;
        return isW;
    }

    bool FITKFlowOFHDF5Writer::writePhysicsData()
    {
        //创建物理场数据节点
        H5::Group physicsGroup = _h5File->createGroup("PhysicsData");
        //获取全局数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorPhysicsData>("HDF5", "Interface::FITKOFPhysicsData");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&physicsGroup);
        adaptor->setDataObject(physicsData);
        adaptor->setWriter(this);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write PhysicsData Errors ! "));
        delete adaptor;
        return isW;
    }

    void FITKFlowOFHDF5Writer::sendCalculateProgress(int Progress)
    {
    }

    void FITKFlowOFHDF5Writer::consoleMessage(int level, const QString & str)
    {
#ifdef QT_DEBUG
        qDebug() << str;
#endif
        //判断消息等级
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }

}
