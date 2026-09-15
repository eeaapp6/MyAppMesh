/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSeedManager.h"
#include "FITKMeshSeed.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include <QSet>
#include <QDebug>

namespace Interface
{
    FITKMeshSeedManager::FITKMeshSeedManager()
    {

    }

    FITKMeshSeedManager::~FITKMeshSeedManager()
    {

    }

    void FITKMeshSeedManager::removeDataObj(FITKAbstractDataObject* obj)
    {
        if (_globalSeedDataTag == true) {
            QMutexLocker locker(&_mutex);
            Interface::FITKMeshSeed* seedObj = dynamic_cast<Interface::FITKMeshSeed*>(obj);
            if (seedObj == nullptr) {
                return;
            }
            //判断该种子对应的边是否存在
            Interface::FITKAbsVirtualTopo* vEdge = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(seedObj->getVirTopoId());
            if (vEdge) {
                seedObj->setEleSize(getGlobalSeedSize());
                seedObj->setMethod(FITKMeshSeed::FITKLocalSeedMethod::MBySize);
                seedObj->setSeedType(FITKMeshSeed::FITKSeedType::TGlobal);
                calculateSeedPoint(seedObj);
            }
            else {
                _helper->removeDataObj(obj);
            }
        }
        else {
            Core::FITKAbstractDataManager<FITKMeshSeed>::removeDataObj(obj);
        }
    }

    void FITKMeshSeedManager::removeDataByIndex(int index)
    {
        if (_globalSeedDataTag == true) {
            QMutexLocker locker(&_mutex);
            Interface::FITKMeshSeed* seedObj = dynamic_cast<Interface::FITKMeshSeed*>(_helper->getDataByIndex(index));
            if (seedObj == nullptr) {
                return;
            }
            //判断该种子对应的边是否存在
            Interface::FITKAbsVirtualTopo* vEdge = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(seedObj->getVirTopoId());
            if (vEdge) {
                seedObj->setEleSize(getGlobalSeedSize());
                seedObj->setMethod(FITKMeshSeed::FITKLocalSeedMethod::MBySize);
                seedObj->setSeedType(FITKMeshSeed::FITKSeedType::TGlobal);
                calculateSeedPoint(seedObj);
            }
            else {
                _helper->removeDataObj(seedObj);
            }
        }
        else {
            Core::FITKAbstractDataManager<FITKMeshSeed>::removeDataByIndex(index);
        }
    }

    void FITKMeshSeedManager::removeDataByID(int id)
    {
        if (_globalSeedDataTag == true) {
            QMutexLocker locker(&_mutex);
            Interface::FITKMeshSeed* seedObj = dynamic_cast<Interface::FITKMeshSeed*>(_helper->getDataByID(id));
            if (seedObj == nullptr) {
                return;
            }
            //判断该种子对应的边是否存在
            Interface::FITKAbsVirtualTopo* vEdge = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(seedObj->getVirTopoId());
            if (vEdge) {
                seedObj->setEleSize(getGlobalSeedSize());
                seedObj->setMethod(FITKMeshSeed::FITKLocalSeedMethod::MBySize);
                seedObj->setSeedType(FITKMeshSeed::FITKSeedType::TGlobal);
                calculateSeedPoint(seedObj);
            }
            else {
                _helper->removeDataObj(seedObj);
            }
        }
        else {
            Core::FITKAbstractDataManager<FITKMeshSeed>::removeDataByID(id);
        }
    }

    FITKMeshSeed* FITKMeshSeedManager::getMeshSeedByVirTopo(int virTopoID)
    {
        for (Core::FITKAbstractDataObject* data : _dataList) {
            FITKMeshSeed* meshSeed = dynamic_cast<FITKMeshSeed*>(data);
            if (meshSeed == nullptr) {
                continue;
            }
            if (meshSeed->getVirTopoId() == virTopoID) {
                return meshSeed;
            }
        }
        return nullptr;
    }

    QList<FITKMeshSeed*> FITKMeshSeedManager::getMeshSeedsByType(FITKMeshSeed::FITKSeedType type)
    {
        QList<FITKMeshSeed*> seeds = {};
        for (Core::FITKAbstractDataObject* data : _dataList) {
            FITKMeshSeed* meshSeed = dynamic_cast<FITKMeshSeed*>(data);
            if (meshSeed == nullptr) {
                continue;
            }
            if (meshSeed->getSeedType() == type) {
                seeds.append(meshSeed);
            }
        }
        return seeds;
    }

    QList<FITKMeshSeed*> FITKMeshSeedManager::getMeshSeedsByMergeEdge()
    {
        QList<FITKMeshSeed*> result;
        for (Core::FITKAbstractDataObject* data : _dataList) {
            FITKMeshSeed* meshSeed = dynamic_cast<FITKMeshSeed*>(data);
            if (meshSeed == nullptr) {
                continue;
            }
            Interface::FITKAbsVirtualTopo* edgeVirTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(meshSeed->getVirTopoId());
            if (edgeVirTopo == nullptr) {
                continue;
            }
            if (edgeVirTopo->getCombinedBasicToposRecur().size() > 0) {
                result.append(meshSeed);
            }
        }
        return result;
    }

    QList<FITKMeshSeed*> FITKMeshSeedManager::getMeshSeedsByNoMergeEdge()
    {
        QList<FITKMeshSeed*> result;
        for (Core::FITKAbstractDataObject* data : _dataList) {
            FITKMeshSeed* meshSeed = dynamic_cast<FITKMeshSeed*>(data);
            if (meshSeed == nullptr) {
                continue;
            }
            Interface::FITKAbsVirtualTopo* edgeVirTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(meshSeed->getVirTopoId());
            if (edgeVirTopo == nullptr) {
                continue;
            }
            if (edgeVirTopo->getCombinedBasicToposRecur().size() == 0) {
                result.append(meshSeed);
            }
        }
        return result;
    }

    void FITKMeshSeedManager::calculateSeedPoint(FITKMeshSeed * seedObj)
    {
        if (seedObj == nullptr) {
            return;
        }

        Interface::FITKMeshSeed::FITKLocalSeedMethod methodType = seedObj->getMethod();
        Interface::FITKMeshSeed::FITKLocalSeedBias biasType = seedObj->getBias();
        switch (methodType){
        case Interface::FITKMeshSeed::MBySize:
            switch (biasType){
            case Interface::FITKMeshSeed::BNone:calSeedPointByEleSize(seedObj); break;
            case Interface::FITKMeshSeed::BSingle:break;
            case Interface::FITKMeshSeed::BDouble:break;
            }
            break;
        case Interface::FITKMeshSeed::MByNum:
            switch (biasType) {
            case Interface::FITKMeshSeed::BNone:calNoneBiasSeedPointByEleNum(seedObj); break;
            case Interface::FITKMeshSeed::BSingle:break;
            case Interface::FITKMeshSeed::BDouble:break;
            }
            break;
        }

        //存储种子点所对应的原几何点id
        QList<FITKMeshSeedInfo> seedInfo = seedObj->getSeedPointInfos();
        if (seedInfo.size() < 2) {
            return;
        }
        for (int i = 0; i < seedInfo.size(); i++) {
            if (i > 0 && i < seedInfo.size() - 1) {
                continue;
            }
            Interface::FITKVirtualTopoMapper mapper;
            mapper.mapTopo(seedInfo[i]._edgeId, Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
            for (int nodeIndex = 0; nodeIndex < mapper.length(); nodeIndex++) {
                Interface::FITKAbsVirtualTopo* nodeVirTopo = mapper.virtualTopo(nodeIndex);
                if (nodeVirTopo == nullptr) {
                    continue;
                }
                Interface::FITKAbstractGeomPointTool* geoPointTool = Interface::FITKAbstractGeomPointTool::createTool();
                if (geoPointTool == nullptr) {
                    continue;
                }
                double pos[3] = { 0,0,0 };
                geoPointTool->getXYZ(nodeVirTopo, pos);
                delete geoPointTool;
                geoPointTool = nullptr;

                double dis = Core::Distance(Core::FITKPoint(pos[0], pos[1], pos[2]), seedInfo[i]._point);
                if (dis <1e-6 && dis >-1e-6) {
                    seedInfo[i]._nodeID = nodeVirTopo->getDataObjectID();
                    break;
                }
            }
        }
        seedObj->setSeedPointInfos(seedInfo);
    }

    QList<int> FITKMeshSeedManager::getCurGeoEdgeIds(Interface::FITKAbsGeoCommand* geoCom)
    {
        QList<int> result = {};

        //获取所有的几何边（包含合并边与所有几何边）
        Interface::FITKAbsGeoShapeAgent* geoShapeAgent = geoCom->getShapeAgent();
        if (geoShapeAgent == nullptr) {
            return result;
        }

        Interface::FITKVirtualTopoManager* topoMan = geoShapeAgent->getVirtualTopoManager();
        if (topoMan == nullptr) {
            return result;
        }

        Interface::FITKShapeVirtualTopoManager* edgeTopoMan = topoMan->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edgeTopoMan == nullptr) {
            return result;
        }

        QList<int> mergeSubEdgeIds = {};
        for (int i = 0; i < edgeTopoMan->getDataCount(); i++) {
            Interface::FITKAbsVirtualTopo* edgeTopo = edgeTopoMan->getDataByIndex(i);
            if (edgeTopo == nullptr) {
                continue;
            }
            result.append(edgeTopo->getDataObjectID());
            QList<Interface::FITKAbsVirtualTopo*> subEdgeTopos = edgeTopo->getCombinedBasicToposRecur();
            for (Interface::FITKAbsVirtualTopo* subEdgeTopo : subEdgeTopos) {
                if (subEdgeTopo == nullptr) {
                    continue;
                }
                mergeSubEdgeIds.append(subEdgeTopo->getDataObjectID());
            }
        }
        for (int mergeSubEdgeId : mergeSubEdgeIds) {
            result.removeOne(mergeSubEdgeId);
        }

        return result;
    }

    void FITKMeshSeedManager::removeInvalidEdgeSeeds(Interface::FITKAbsGeoCommand* cmd)
    {
        if (!cmd)
        {
            clear();
            return;
        }

        Interface::FITKShapeVirtualTopoManager* vEdgeMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        if (!vEdgeMgr)
        {
            clear();
            return;
        }

        // 检查不存在的边。
        int nSeedEdges = getDataCount();
        for (int i = nSeedEdges - 1; i >= 0; i--)
        {
            FITKMeshSeed* seeds = getDataByIndex(i);
            if (!seeds)
            {
                continue;
            }

            Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByID(seeds->getVirTopoId());
            if (!vEdge)
            {
                Core::FITKAbstractDataManager<FITKMeshSeed>::removeDataByIndex(i);
                continue;
            }

            // 跳过被合并的边。
            if (vEdge->getCombinedToTopo())
            {
                Core::FITKAbstractDataManager<FITKMeshSeed>::removeDataByIndex(i);
                continue;
            }

            // 重新计算种子点坐标。
            calculateSeedPoint(seeds);
        }

        // 补充缺失的全局种子点。
        if (_globalSeedDataTag)
        {
            double size = getGlobalSeedSize();

            int nEdges = vEdgeMgr->getDataCount();
            for (int i = 0; i < nEdges; i++)
            {
                Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByIndex(i);
                if (!vEdge)
                {
                    continue;
                }

                // 跳过被合并的边。
                if (vEdge->getCombinedToTopo())
                {
                    continue;
                }

                int edgeId = vEdge->getDataObjectID();
                FITKMeshSeed* meshSeeds = getMeshSeedByVirTopo(edgeId);
                if (meshSeeds)
                {
                    continue;
                }

                meshSeeds = new FITKMeshSeed(edgeId);
                meshSeeds->setMethod(FITKMeshSeed::FITKLocalSeedMethod::MBySize);
                meshSeeds->setSeedType(FITKMeshSeed::FITKSeedType::TGlobal);
                meshSeeds->setEleSize(size);
                calculateSeedPoint(meshSeeds);
                appendDataObj(meshSeeds);
            }
        }
    }

    bool FITKMeshSeedManager::getGlobalSeedDataTag()
    {
        return _globalSeedDataTag;
    }

    void FITKMeshSeedManager::setGlobalSeedDataTag(bool seedDataTag)
    {
        _globalSeedDataTag = seedDataTag;
    }

    double FITKMeshSeedManager::getGlobalSeedSize()
    {
        return _globalSeedEleSize;
    }

    void FITKMeshSeedManager::setGlobalSeedSize(double size)
    {
        _globalSeedEleSize = size;
    }

    void FITKMeshSeedManager::calNoneBiasSeedPointByEleNum(FITKMeshSeed* seedObj)
    {
        if (seedObj == nullptr) {
            return;
        }
        int eleNum = seedObj->getEleNumber();

        QList<FITKMeshSeedInfo> points = {};
        for (int i = 0; i <= eleNum; i++) {
            double pointD[3] = { 0,0,0 };
            double ratio = (i*1.0) / (eleNum*1.0);

            FITKMeshSeedInfo seedInfo;
            Interface::FITKAbstractGeomEdgeTool::getPointByRatio(seedObj->getVirTopoId(), ratio, pointD, seedInfo._edgeId, seedInfo._mergeRatio);
            
            seedInfo._point = Core::FITKPoint(pointD);
            seedInfo._ratio = ratio;
            
            points.append(seedInfo);
        }
        seedObj->setSeedPointInfos(points);
    }

    void FITKMeshSeedManager::calSeedPointByEleSize(FITKMeshSeed * seedObj)
    {
        if (seedObj == nullptr) {
            return;
        }
        Interface::FITKAbsVirtualTopo* edgeTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(seedObj->getVirTopoId());
        if (edgeTopo == nullptr) {
            return;
        }

        //获取单元大小
        double eleSize = seedObj->getEleSize();

        //获取线长
        Interface::FITKAbstractGeomEdgeTool* tool = Interface::FITKAbstractGeomEdgeTool::createTool();
        if (tool == nullptr) {
            return;
        }
        double length = 0.0;

        //判断是否是合并边
        QList<Interface::FITKAbsVirtualTopo*> mergeSubEdgeTopos = edgeTopo->getCombinedBasicToposRecur();
        if (mergeSubEdgeTopos.size() == 0) {
            tool->getLengthByID(seedObj->getVirTopoId(), length);
        }
        else {
            for (Interface::FITKAbsVirtualTopo* mergeSubEdgeTopo : mergeSubEdgeTopos) {
                if (mergeSubEdgeTopo == nullptr) {
                    continue;
                }
                double subLength = 0.0;
                tool->getLength(mergeSubEdgeTopo, subLength);
                length += subLength;
            }
        }
        
        delete tool;
        tool = nullptr;

        //计算单元数
        int eleNum = length / eleSize;
        if (eleNum < 1) {
            eleNum = 1;
        }

        QList<FITKMeshSeedInfo> points = {};
        for (int i = 0; i <= eleNum; i++) {
            double pointD[3] = { 0,0,0 };
            double ratio = (i*1.0) / (eleNum*1.0);
            FITKMeshSeedInfo seedInfo;
            Interface::FITKAbstractGeomEdgeTool::getPointByRatio(seedObj->getVirTopoId(), ratio, pointD, seedInfo._edgeId, seedInfo._mergeRatio);

            seedInfo._point = Core::FITKPoint(pointD);
            seedInfo._ratio = ratio;
            points.append(seedInfo);
        }
        seedObj->setSeedPointInfos(points);
    }
}