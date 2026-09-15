/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshGenInterface.h"
#include "FITKAbstractMeshSizeInfoGenerator.h"
#include "FITKRegionMeshSize.h"
#include "FITKGeometryMeshSize.h"
#include "FITKRegionGeometryRefine.h"
#include "FITKAbstractMesherDriver.h"
#include "FITKAbstractMeshProcessor.h"
#include "FITKAbstractGeometryMeshSizeGenerator.h"
#include "FITKZonePoints.h"
#include "FITKAbstractMeshAlgorithmInfoGenerator.h"
#include "FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITKMeshParameterManager.h"
#include "FITKMeshSeedManager.h"

#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
FITKLIBINFOREGISTER(FITKInterfaceMeshGen, FITKInterfaceMeshGenVersion);

namespace Interface
{
    //静态变量初始化
    QMutex FITKMeshGenInterface::m_mutex;
    FITKMeshGenInterface* FITKMeshGenInterface::_instance = nullptr;

    FITKGlobalMeshSizeInfo* FITKMeshGenInterface::getGlobalMeshSizeInfo(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;
        //全局尺寸为空则创建
        if (d->_globalSize == nullptr && d->_meshSizeGenor != nullptr)
        {
            d->_globalSize = d->_meshSizeGenor->generateGlobalMeshSizeInfo();
        }
        //返回全局尺寸
        return d->_globalSize;
    }

    void FITKMeshGenInterface::regMeshSizeGenerator(FITKAbstractMeshSizeInfoGenerator* genator, const QString & mesher)
    {
        FITKMeshGenInterfaceData* d = this->getValidData(mesher);
        if (d == nullptr) return;
        //赋值
        d->_meshSizeGenor = genator;
    }

    FITKGlobalMeshGenerateAlgorithmInfo* FITKMeshGenInterface::getGlobalMeshGenerateAlgorithmInfo(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        //全局网格算法信息为空则创建
        if (d->_globalMeshAlgorithm == nullptr && d->_meshAlgorithmGenor != nullptr)
        {
            d->_globalMeshAlgorithm = d->_meshAlgorithmGenor->generateGlobalMeshAlgorithmInfo();
        }
        //返回全局尺寸
        return d->_globalMeshAlgorithm;
    }

    void FITKMeshGenInterface::regMeshAlgorithmGenerator(FITKAbstractMeshAlgorithmInfoGenerator* genator, const QString & mesher)
    {
        FITKMeshGenInterfaceData* d = this->getValidData(mesher);
        if (d == nullptr) return;
        //赋值
        d->_meshAlgorithmGenor = genator;
    }

    Interface::FITKAbstractMeshSizeInfoGenerator* FITKMeshGenInterface::getMeshSizeGenerator(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_meshSizeGenor;
    }

    void FITKMeshGenInterface::regGeometryMeshSizeGenerator(FITKAbstractGeometryMeshSizeGenerator* genator,
        const QString & mesher)
    {
        FITKMeshGenInterfaceData* d = this->getValidData(mesher);
        if (d == nullptr) return;
        //赋值
        d->_geoMeshSizeGenor = genator;
    }

    FITKAbstractGeometryMeshSizeGenerator* FITKMeshGenInterface::getGeometryMeshSizeGenerator(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_geoMeshSizeGenor;
    }

    void FITKMeshGenInterface::regMesherDriver(FITKAbstractMesherDriver* driver, const QString & mesher)
    {
        FITKMeshGenInterfaceData* d = this->getValidData(mesher);
        if (d == nullptr) return;
        //赋值
        d->_mesherDriver = driver;
    }

    Interface::FITKAbstractMesherDriver* FITKMeshGenInterface::getMesherDriver(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_mesherDriver;
    }

    void FITKMeshGenInterface::regMeshProcessor(FITKAbstractMeshProcessor* p, const QString & mesher )
    {
        FITKMeshGenInterfaceData* d = this->getValidData(mesher);
        if (d == nullptr) return;
        //赋值
        d->_meshProcessor = p;
    }

    Interface::FITKAbstractMeshProcessor* FITKMeshGenInterface::getMeshProcessor(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_meshProcessor;
    }

    FITKRegionMeshSizeManager* FITKMeshGenInterface::getRegionMeshSizeMgr(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_regionMeshMgr;
    }

    FITKGeometryMeshSizeManager* FITKMeshGenInterface::getGeometryMeshSizeManager(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_geoMeshSizeMgr;
    }

    FITKMeshParameterManager * FITKMeshGenInterface::getMeshParameterManager(int dataObjId, const QString & mesher, bool isCreate)
    {
        if (_interfaceData.isEmpty()){
            return nullptr;
        }

        FITKMeshGenInterfaceData* d = nullptr;
        if (mesher.isEmpty()){
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else{
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) {
            return nullptr;
        }

        FITKMeshParameterManager* meshParamMangager = d->_meshParamManager.value(dataObjId);
        if (meshParamMangager == nullptr && isCreate == true) {
            meshParamMangager = new FITKMeshParameterManager();
            d->_meshParamManager.insert(dataObjId, meshParamMangager);
        }

        return meshParamMangager;
    }

    void FITKMeshGenInterface::removeMeshParameterManager(int dataObjId, const QString & mesher)
    {
        if (_interfaceData.isEmpty()) {
            return;
        }

        FITKMeshGenInterfaceData* d = nullptr;
        if (mesher.isEmpty()) {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) {
            return;
        }

        FITKMeshParameterManager* meshParamMangager = d->_meshParamManager.take(dataObjId);
        if (meshParamMangager) {
            delete meshParamMangager;
            meshParamMangager = nullptr;
        }
    }

    FITKMeshSeedManager * FITKMeshGenInterface::getMeshSeedManager(int dataObjId, const QString & mesher, bool isCreate)
    {
        if (_interfaceData.isEmpty()) {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d = nullptr;
        if (mesher.isEmpty()) {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) {
            return nullptr;
        }

        FITKMeshSeedManager* meshSeedMangager = d->_meshSeedManager.value(dataObjId);
        if (meshSeedMangager == nullptr && isCreate == true) {
            meshSeedMangager = new FITKMeshSeedManager();
            d->_meshSeedManager.insert(dataObjId, meshSeedMangager);
        }

        return meshSeedMangager;
    }

    void FITKMeshGenInterface::removeMeshSeedManager(int dataObjId, const QString & mesher)
    {
        if (_interfaceData.isEmpty()) {
            return;
        }

        FITKMeshGenInterfaceData* d = nullptr;
        if (mesher.isEmpty()) {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) {
            return;
        }

        FITKMeshSeedManager* meshSeedMangager = d->_meshSeedManager.take(dataObjId);
        if (meshSeedMangager) {
            delete meshSeedMangager;
            meshSeedMangager = nullptr;
        }
    }

    Interface::FITKZonePointManager* FITKMeshGenInterface::getZonePointManager(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_zonePointsManager;
    }

    FITKRegionGeometryRefineManager* FITKMeshGenInterface::getRegionGeometryRefineManager(const QString & mesher)
    {
        if (_interfaceData.isEmpty())
        {
            return nullptr;
        }

        FITKMeshGenInterfaceData* d{ nullptr };
        if (mesher.isEmpty())
        {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else
        {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) return nullptr;

        return d->_regionGeoRefineMgr;
    }

    void FITKMeshGenInterface::removeMeshGenInterfaceData(const QString &mesher)
    {
        if (_interfaceData.contains(mesher))
        {
            FITKMeshGenInterfaceData* d = _interfaceData.value(mesher);
            delete d;
            _interfaceData.remove(mesher);
        }
    }

    QStringList FITKMeshGenInterface::getMeshInterfaceNames()
    {
        return _interfaceData.keys();
    }

    void FITKMeshGenInterface::clear(const QString & mesher)
    {
        FITKMeshGenInterfaceData* d = nullptr;
        if (mesher.isEmpty()) {
            QString mesherDefault = _interfaceData.keys().first();
            d = _interfaceData.value(mesherDefault);
        }
        else {
            d = _interfaceData.value(mesher);
        }

        if (d == nullptr) {
            return;
        }

        QList<int> geoIds = d->_meshParamManager.keys();
        for (const int& geoId : geoIds) {
            FITKMeshParameterManager* meshParaManager = d->_meshParamManager.take(geoId);
            if (meshParaManager == nullptr) {
                continue;
            }
            delete meshParaManager;
            meshParaManager = nullptr;
        }
        d->_meshParamManager.clear();

        geoIds = d->_meshSeedManager.keys();
        for (const int& geoId : geoIds) {
            FITKMeshSeedManager* meshSeedManager = d->_meshSeedManager.take(geoId);
            if (meshSeedManager == nullptr) {
                continue;
            }
            delete meshSeedManager;
            meshSeedManager = nullptr;
        }
        d->_meshSeedManager.clear();
    }

    void FITKMeshGenInterface::initialize()
    {
       
    }

    void FITKMeshGenInterface::finalize()
    {
        QList<FITKMeshGenInterfaceData*> ds = _interfaceData.values();
        for (FITKMeshGenInterfaceData* d :ds)
        {
            delete d;
        }
        _interfaceData.clear();
    }

    Interface::FITKMeshGenInterfaceData* FITKMeshGenInterface::getValidData(const QString mesher)
    {
        //已经存在
        if (_interfaceData.contains(mesher)) return _interfaceData.value(mesher);
        //不存在则创建
        FITKMeshGenInterfaceData* d = new FITKMeshGenInterfaceData;
        _interfaceData.insert(mesher,d);
        return d;
    }

    FITKMeshGenInterfaceDeleator::~FITKMeshGenInterfaceDeleator()
    {
        FITKMeshGenInterface* interface = FITKMeshGenInterface::getInstance();
        delete interface;
        FITKMeshGenInterface::_instance = nullptr;
    }


    static FITKMeshGenInterfaceDeleator __DELEATOR__;

    FITKMeshGenInterfaceData::FITKMeshGenInterfaceData()
    {
        //创建对象
        _regionMeshMgr = new FITKRegionMeshSizeManager;
        _geoMeshSizeMgr = new FITKGeometryMeshSizeManager;
        _zonePointsManager = new FITKZonePointManager;
        _regionGeoRefineMgr = new FITKRegionGeometryRefineManager;
    }

    FITKMeshGenInterfaceData::~FITKMeshGenInterfaceData()
    {
        QList<int> dataIds = _meshParamManager.keys();
        for (int dataId : dataIds) {
            FITKMeshParameterManager* meshParamMangager = _meshParamManager.take(dataId);
            if (meshParamMangager) {
                delete meshParamMangager;
                meshParamMangager = nullptr;
            }
        }
        _meshParamManager.clear();

        //释放内存
        if (_meshSizeGenor) delete _meshSizeGenor;
        _meshSizeGenor = nullptr;
        if (_regionMeshMgr) delete _regionMeshMgr;
        _regionMeshMgr = nullptr;
        if (_meshAlgorithmGenor) delete _meshAlgorithmGenor;
        _meshAlgorithmGenor = nullptr;
        if (_globalMeshAlgorithm) delete _globalMeshAlgorithm;
        _globalMeshAlgorithm = nullptr;
        if (_globalSize)  delete _globalSize;
        _globalSize = nullptr;
        if (_mesherDriver) delete _mesherDriver;
        _mesherDriver = nullptr;
        if (_meshProcessor) delete _meshProcessor;
        _meshProcessor = nullptr;
        if (_geoMeshSizeMgr) delete _geoMeshSizeMgr;
        _geoMeshSizeMgr = nullptr;
        if (_geoMeshSizeGenor) delete _geoMeshSizeGenor;
        _geoMeshSizeGenor = nullptr;
        if (_zonePointsManager) delete _zonePointsManager;
        _zonePointsManager = nullptr;
        if (_regionGeoRefineMgr) delete _regionGeoRefineMgr;
        _regionGeoRefineMgr = nullptr;
    }

}




