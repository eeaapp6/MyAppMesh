/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5AdaptorGeometryComponent.h"
#include "FITKFlowOFHDF5AdaptorGeometryRefine.h"
#include "FITKFlowOFHDF5AdaptorGeometryMeshSize.h"
#include "FITKFlowOFHDF5AdaptorMaterialPoint.h"
#include "FITKFlowOFHDF5AdaptorRegionMesh.h"
#include "FITKFlowOFHDF5AdaptorMesh.h"
#include "FITKFlowOFHDF5AdaptorPhysicsData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//几何
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"

//网格
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionGeometryRefine.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
//#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
//物理场
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"

#include <QDebug>

namespace IO
{
    void FITKFlowOFHDF5Reader::run()
    {
        *_resultMark = false;
        //准备工作
        if (!_h5File)return;
        //手动清理数据仓库
        this->resetDataObject();
        //开始读取
        *_resultMark = true;
        *_resultMark &= readVersion();
        *_resultMark &= readDataGeometry();
        *_resultMark &= readGeoComponent();
        *_resultMark &= readGeometryRefine();
        *_resultMark &= readGeoMeshSize();
        *_resultMark &= readMaterialPoint();
        *_resultMark &= readRegionMesh();
        *_resultMark &= readMesh();
        *_resultMark &= readPhysicsData();
    }

    bool FITKFlowOFHDF5Reader::readVersion()
    {
        //写出HDF5管理功能版本号
        std::string name = "Version";
        if (!_h5File->nameExists(name)) return false;
        H5::Group versionGroup = _h5File->openGroup(name);
        if (!versionGroup.attrExists(name)) return false;
        auto attrVersion = versionGroup.openAttribute(name);
        double version = -1;
        attrVersion.read(H5::PredType::NATIVE_DOUBLE, &version);
        if (version > _version)
        {
            consoleMessage(2, "Read Error, File Version Mismatch.");
            return false;
        }
        return true;
    }

    bool FITKFlowOFHDF5Reader::readDataGeometry()
    {
        //获取几何数据工程文件组件
        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)return false;
        IO::FITKOCCIOHDF5Interface* h5IO = FITKAPP->getComponents()->getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
        bool isR = false;
        if (h5IO)
        {
            //设置数据并读取几何的工程数据
            h5IO->setHDF5Root(getHDF5File());
            h5IO->setRunInThread(false);
            h5IO->setDataObject(geoCmdList);
            h5IO->exec(1);
            isR = h5IO->isSuccess();
        }
        if (!isR)
            consoleMessage(3, QString("Read Geometry Errors"));

        return isR;
    }

    bool FITKFlowOFHDF5Reader::readGeoComponent()
    {
        if (!_h5File->nameExists("GeoComponent")) return false;
        Interface::FITKGeoCommandList* commList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (!commList) return false;
        bool isR = true;
        // create cases group
        H5::Group geometryGroup = _h5File->openGroup("GeoComponent");

        //获取几何命令长度
        if (!geometryGroup.attrExists("Count")) return true;
        H5::Attribute att = geometryGroup.openAttribute("Count");
        int count = 0;
        att.read(H5::PredType::NATIVE_INT, &count);

        for (int i = 0; i < count; i++)
        {
            std::string name = std::to_string(i);
            //根据索引映射读取几何名称
            if (!geometryGroup.attrExists(name))continue;
            auto geoNameAttribute = geometryGroup.openAttribute(std::to_string(i));
            std::string geoName;
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            geoNameAttribute.read(datatype, geoName);

            if (!geometryGroup.nameExists(geoName))
            {
                consoleMessage(3, QString("Read Geo Error, CaseName : %1").arg(QString::fromStdString(geoName)));
                return false;
            }
            H5::Group geoComGroup = geometryGroup.openGroup(geoName);
            Interface::FITKAbsGeoCommand* comm = commList->getDataByName(QString::fromStdString(geoName));
            if (!comm)return false;
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryComponent>("HDF5", "Interface::FITKGeoComponentManager");
            if (adaptor == nullptr)
            {
                delete comm;
                return false;
            }
            adaptor->setH5Group(&geoComGroup);
            adaptor->setDataObject(comm);
            adaptor->setReader(this);
            bool r = adaptor->adaptR();
            if (!r) consoleMessage(3, QString("Read Command Errors, Command : %1").arg(comm->getDataObjectName()));
            isR &= r;
            delete adaptor;
        }
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readGeometryRefine()
    {
        if (!_h5File->nameExists("GeoRefine")) return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取几何优化管理器
        Interface::FITKRegionGeometryRefineManager*geoRefineManager = meshGen->getRegionGeometryRefineManager();
        if (!geoRefineManager) return false;
        bool isR = true;
        //获取几何优化数据
        H5::Group geoRefineGroup = _h5File->openGroup("GeoRefine");
        //获取长度
        if (!geoRefineGroup.attrExists("Count")) return true;
        H5::Attribute att = geoRefineGroup.openAttribute("Count");
        int count = 0;
        att.read(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!geoRefineGroup.nameExists(name))
                continue;
            Interface::FITKRegionGeometryRefine* geoRefine = new Interface::FITKRegionGeometryRefine();

            //创建几何优化工厂
            H5::Group geoRefGroup = geoRefineGroup.openGroup(name);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryRefine>("HDF5", geoRefine);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&geoRefGroup);
            adaptor->setDataObject(geoRefine);
            adaptor->setReader(this);
            bool R = adaptor->adaptR();
            if (!R)
            {//失败销毁几何数据对象
                delete geoRefine;
            }
            else
            {//成功插入数据
                geoRefineManager->appendDataObj(geoRefine);
            }
            //销毁工厂
            delete adaptor;
            isR &= R;
        }
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readGeoMeshSize()
    {
        if (!_h5File->nameExists("GeoMeshSize")) return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKGeometryMeshSizeManager* geoMeshSizeManager = meshGen->getGeometryMeshSizeManager();
        if (!geoMeshSizeManager)return false;
        bool isR = true;
        //获取hdf5几何节点数据
        H5::Group geometryMeshSizeGroup = _h5File->openGroup("GeoMeshSize");
        //获取长度
        if (!geometryMeshSizeGroup.attrExists("Count")) return true;
        H5::Attribute att = geometryMeshSizeGroup.openAttribute("Count");
        int count = 0;
        att.read(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!geometryMeshSizeGroup.nameExists(name))
                continue;
            H5::Group geoDataGroup = geometryMeshSizeGroup.openGroup(name);
            Interface::FITKGeometryMeshSize* geoMeshsize = new Interface::FITKGeometryMeshSize;
            //创建几何读取工厂
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorGeometryMeshSize>("HDF5", geoMeshsize);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&geoDataGroup);
            adaptor->setDataObject(geoMeshsize);
            adaptor->setReader(this);
            bool R = adaptor->adaptR();
            if (!R)
            {//失败销毁几何数据对象
                delete geoMeshsize;
            }
            else
            {//成功插入数据
                geoMeshSizeManager->appendDataObj(geoMeshsize);
            }
            //销毁工厂
            delete adaptor;
            isR &= R;
        }
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readMaterialPoint()
    {
        if (!_h5File->nameExists("MaterialPoints")) return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKZonePointManager* pointManager = meshGen->getZonePointManager();
        if (!pointManager)return false;
        bool isR = true;
        //获取hdf5几何节点数据
        H5::Group materialPointGroup = _h5File->openGroup("MaterialPoints");
        //获取长度
        if (!materialPointGroup.attrExists("Count")) return true;
        H5::Attribute att = materialPointGroup.openAttribute("Count");
        int count = 0;
        att.read(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!materialPointGroup.nameExists(name))
                continue;
            H5::Group pointGroup = materialPointGroup.openGroup(name);
            Interface::FITKZonePoint* ponitMaterial = new Interface::FITKZonePoint;
            //创建几何读取工厂
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorMaterialPoint>("HDF5", "Interface::FITKZonePoint");
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&pointGroup);
            adaptor->setDataObject(ponitMaterial);
            adaptor->setReader(this);
            bool R = adaptor->adaptR();
            if (!R)
            {//失败销毁几何数据对象
                delete ponitMaterial;
            }
            else
            {//成功插入数据
                pointManager->appendDataObj(ponitMaterial);
            }
            //销毁工厂
            delete adaptor;
            isR &= R;
        }
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readRegionMesh()
    {
        if (!_h5File->nameExists("RegionMesh")) return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKRegionMeshSizeManager* regionMeshManager = meshGen->getRegionMeshSizeMgr();
        if (!regionMeshManager)return false;
        //获取区域网格尺寸生成器
        Interface::FITKAbstractMeshSizeInfoGenerator* regionMeshGenerator = meshGen->getMeshSizeGenerator();
        if (!regionMeshGenerator)return false;
        bool isR = true;
        //获取hdf5几何节点数据
        H5::Group regionMeshGroup = _h5File->openGroup("RegionMesh");
        //获取长度
        if (!regionMeshGroup.attrExists("Count")) return true;
        H5::Attribute att = regionMeshGroup.openAttribute("Count");
        int count = 0;
        att.read(H5::PredType::NATIVE_INT, &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!regionMeshGroup.nameExists(name))
                continue;
            H5::Group regionGroup = regionMeshGroup.openGroup(name);
            //获取类型
            H5::Attribute regionTypeAttribute = regionGroup.openAttribute("RegionType");
            int regionType = 0;
            regionTypeAttribute.read(H5::PredType::NATIVE_INT, &regionType);
            //生成数据
            Interface::FITKAbstractRegionMeshSize* regionMeshData = regionMeshGenerator->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType(regionType));
            //创建几何读取工厂
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRegionMesh>("HDF5", regionMeshData);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&regionGroup);
            adaptor->setDataObject(regionMeshData);
            adaptor->setReader(this);
            bool R = adaptor->adaptR();
            if (!R)
            {//失败销毁几何数据对象
                delete regionMeshData;
            }
            else
            {//成功插入数据
                regionMeshManager->appendDataObj(regionMeshData);
            }
            //销毁工厂
            delete adaptor;
            isR &= R;
        }
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readMesh()
    {
        if (!_h5File->nameExists("MeshData")) return false;
        H5::Group meshGroup = _h5File->openGroup("MeshData");
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
        adaptor->setReader(this);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read MeshData Errors ! "));
        delete adaptor;
        return isR;
    }

    bool FITKFlowOFHDF5Reader::readPhysicsData()
    {
        if (!_h5File->nameExists("PhysicsData")) return false;
        H5::Group physicsGroup = _h5File->openGroup("PhysicsData");
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
        adaptor->setReader(this);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read PhysicsData Errors ! "));
        delete adaptor;
        return isR;
    }

    void FITKFlowOFHDF5Reader::resetDataObject()
    {
        //获取全局数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return;
        //清理区域网格尺寸管理器
        Interface::FITKRegionMeshSizeManager* regionMeshManager = meshGen->getRegionMeshSizeMgr();
        if (!regionMeshManager) return;
        regionMeshManager->clear();
        //清理几何网格区域尺寸管理器
        Interface::FITKGeometryMeshSizeManager* geoMeshSizeManager = meshGen->getGeometryMeshSizeManager();
        if (!regionMeshManager) return;
        geoMeshSizeManager->clear();
        //清理网格区域材料点
        Interface::FITKZonePointManager* pointManager = meshGen->getZonePointManager();
        if (!regionMeshManager) return;
        pointManager->clear();
        //清理几何区域优化管理器
        Interface::FITKRegionGeometryRefineManager* geoRefineManager = meshGen->getRegionGeometryRefineManager();
        if (!regionMeshManager) return;
        geoRefineManager->clear();
        //清理求解物理数据
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return;
        factoryData->clearPhysicsData();
    }

    void FITKFlowOFHDF5Reader::sendCalculateProgress(int Progress)
    {
    }

    void FITKFlowOFHDF5Reader::consoleMessage(int level, const QString & str)
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
