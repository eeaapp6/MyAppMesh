/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RadReadInformationMapper.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBEM.h"
#include "FITK_Component/FITKRadiossData/FITKBEMDoubleAsymptoticApproximation.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySPHCEL.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"

#include <QSet>

namespace Radioss
{
    void RadReadInformationMapper::setCaseData(FITKRadiossCase* caseData)
    {
        _caseData = caseData;
    }

    void RadReadInformationMapper::map()
    {
        if (_caseData == nullptr) return;

        //part
        FITKRadiossPartManager * partManager = _caseData->getMeshModel()->getPartsManager();
        if (partManager == nullptr) return;
        const int nPart = partManager->getDataCount();
        for (int i = 0; i< nPart; ++i)
        {
            Radioss::FITKRadiossPart* part = partManager->getDataByIndex(i);
            if(part == nullptr) continue;
            const int matid = part->getMaterialID();
            const int propid = part->getPropertyID();
            //子集ID
            const int susetID = part->getSubsetID();
            part->setMaterialID(this->getMaterialID(matid));
            part->setPropertyID(this->getPropertyID(propid));
            part->setSubsetID(this->getSubsetIDByRadID(susetID));
        }
        //子集subset
        FITKRadiossSubSetManager * subsetManager = _caseData->getMeshModel()->getSubSetManager();
        if (subsetManager == nullptr) return;
        const int nSubset = subsetManager->getDataCount();
        for (int i = 0; i < nSubset; ++i)
        {
            Radioss::FITKRadiossSubSet* subset = subsetManager->getDataByIndex(i);
            if (subset == nullptr) continue;
            //嵌套子集
            if (subset->m_subsetRadIDs.size() > 1)
            {
                for (int radID: subset->m_subsetRadIDs) 
                {
                    int id = this->getSubsetIDByRadID(radID);
                    subset->appendDataObj(subsetManager->getDataByID(id));
                }
                subset->m_subsetRadIDs.clear();
                continue;
            }
            //普通子集
            int subsetID = subset->getDataObjectID();
            for (int i = 0; i < nPart; ++i)
            {
                Radioss::FITKRadiossPart* part = partManager->getDataByIndex(i);
                if (part == nullptr) continue;
                if (part->getSubsetID() == subsetID)
                {
                    subset->addPartID(part->getDataObjectID());
                }
            }
        }
        //节点集合
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return;
        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (componentManager == nullptr) return;
        const int nComponent = componentManager->getDataCount();
        for (int i = 0; i < nComponent; ++i)
        {
            Interface::FITKAbstractModelComponent* component = componentManager->getDataByIndex(i);
            FITKRadiossNodeGroup* nodesGroup = dynamic_cast<FITKRadiossNodeGroup*>(component);
            if (!nodesGroup)continue;
            FITKRadiossNodeGroupRadIOInfo* info = nodesGroup->getRadIOInfo();
            if(!info)continue;
            QString keyWords = info->getRadiossKeyWord();
            QList<int> idList{};
            if (keyWords.isEmpty())continue;
            if (keyWords == "/GRNOD/NODE") {
                for (auto id : info->getRadiossIDs()) {
                    int nodeID = getNodeIDByRadID(id);
                    if (nodeID < 1)continue;
                    idList.append(nodeID);
                }
            }
            else if (keyWords == "/GRNOD/PART")
            {
                for (auto id : info->getRadiossIDs()) {
                    FITKRadiossPart* part = getRadiossPartByFilePartID(id, false);
                    if (!part)return;
                    idList.append(part->getDataObjectID());
                }
            }
            else if (keyWords == "/GRNOD/SURF")
            {
                for (auto id : info->getRadiossIDs()) {
                    int surfID = getSurfaceSetIDByRadID(id);
                    if (surfID < 1)continue;
                    idList.append(surfID);
                }
            }
            if (idList.isEmpty())continue;
            info->setRadiossIDs(idList);
            //数据转化
            nodesGroup->transformRadIOInfo(meshModel);
        }
        //节点集合的集合
        for (int i = 0; i < nComponent; ++i)
        {
            Interface::FITKAbstractModelComponent* component = componentManager->getDataByIndex(i);
            FITKRadiossNodeGroup* nodesGroup = dynamic_cast<FITKRadiossNodeGroup*>(component);
            if (!nodesGroup)continue;
            FITKRadiossNodeGroupRadIOInfo* info = nodesGroup->getRadIOInfo();
            if (!info)continue;
            QString keyWords = info->getRadiossKeyWord();
            QSet<int> idList{};
            if (keyWords.isEmpty())continue;
            if (keyWords == "/GRNOD/GRNOD") {
                for (auto id : info->getRadiossIDs()) {
                    int grnod_ID = getNodeGroupIDByRadID(id);
                    if (grnod_ID < 1)continue;
                    Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByID(grnod_ID));
                    if (!set)continue;
                    idList.unite(set->getAbsoluteMember().toSet());
                }
            }
            if (idList.isEmpty())return;
            info->setRadiossKeyWord("/GRNOD/NODE");
            info->setRadiossIDs(idList.toList());
            //数据转化
            nodesGroup->transformRadIOInfo(meshModel);
        }
        //单元组
        for (int i = 0; i < nComponent; ++i)
        {
            Interface::FITKAbstractModelComponent* component = componentManager->getDataByIndex(i);
            FITKRadiossElementGroup* eleGroup = dynamic_cast<FITKRadiossElementGroup*>(component);
            if (!eleGroup)continue;
            FITKRadiossElementGroupRadIOInfo* info = eleGroup->getRadIOInfo();
            if (!info)continue;
            FITKRadiossElementGroupRadIOInfo::ElementWayType wayType = info->getElementWayType();
            FITKRadiossElementGroup::ElementGroupType groupType = eleGroup->getElementGroupType();
            QList<int> idList{};

            switch(wayType)
            {
            case FITKRadiossElementGroupRadIOInfo::ElementWayType::SameProp: 
            {
                for (auto id : info->getRadiossIDs()) {
                    int eleID = getEleIDByRadID(id);
                    if (eleID < 1)continue;
                    idList.append(eleID);
                }
                break;
            }
            case FITKRadiossElementGroupRadIOInfo::ElementWayType::PART: 
            {
                for (auto id : info->getRadiossIDs()) {
                    FITKRadiossPart* part = getRadiossPartByFilePartID(id, false);
                    if (!part)continue;
                    idList.append(part->getDataObjectID());
                }
                break;
            }
            case FITKRadiossElementGroupRadIOInfo::ElementWayType::Equal:
            {
                //to do
                break;
            }
            default:
                break;
            }
            if (idList.isEmpty())continue;
            info->setRadiossIDs(idList);
            //数据转化
            eleGroup->transformRadIOInfo(meshModel);
        }
        //面集合
        for (int i = 0; i < nComponent; ++i)
        {
            Interface::FITKAbstractModelComponent* component = componentManager->getDataByIndex(i);
            FITKRadiossSurfaceSeg* surf= dynamic_cast<FITKRadiossSurfaceSeg*>(component);
            if (!surf)continue;
            FITKRadiossSurfaceRadIOInfo* info = surf->getRadIOInfo();
            if (!info)continue;
            FITKAbstractRadiossSurface::RadiossSurfaceType type = info->getSurfaceType();
            QList<int> idList{};
            switch (type)
            {
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PART:
            {
                for (auto id : info->getRadiossIDs()) {
                    FITKRadiossPart* part = getRadiossPartByFilePartID(id, false);
                    if (!part)continue;
                    idList.append(part->getDataObjectID());
                }
                break;
            }
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSHEL:
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSH3N:
            {
                for (auto id : info->getRadiossIDs()) {
                    int eleId = this->getElementGroupIDByRadID(id);
                    if (eleId < 1)continue;
                    idList.append(eleId);
                }
                break;
            }
            default:
                break;
            }
            if (idList.isEmpty())continue;
            info->setRadiossIDs(idList);
            //数据转化
            surf->transformRadIOInfo(meshModel);
        }
        //面集合的集合
        for (int i = 0; i < nComponent; ++i)
        {
            Interface::FITKAbstractModelComponent* component = componentManager->getDataByIndex(i);
            FITKRadiossSurfaceSeg* surf = dynamic_cast<FITKRadiossSurfaceSeg*>(component);
            if (!surf)continue;
            FITKRadiossSurfaceRadIOInfo* info = surf->getRadIOInfo();
            if (!info)continue;
            FITKAbstractRadiossSurface::RadiossSurfaceType type = info->getSurfaceType();
            if (type != FITKAbstractRadiossSurface::RadiossSurfaceType::RST_SURF)
                continue;
            QList<int> idList{};

            for (auto id : info->getRadiossIDs()) {
                int surfId = this->getSurfaceSetIDByRadID(id);
                if (surfId < 1)continue;
                idList.append(surfId);
            }

            if (idList.isEmpty())continue;
            info->setRadiossIDs(idList);
            //数据转化
            surf->transformRadIOInfo(meshModel);
        }
        //初始场
        FITKInitialFieldManager*  initMgr = _caseData->getInitialFieldManager();
        if (initMgr == nullptr) return ;
        for (int i = 0; i < initMgr->getDataCount(); ++i)
        {
            Radioss::FITKAbstractInitialField* init = initMgr->getDataByIndex(i);
            if (init == nullptr) continue;
            //节点组ID
            const int grnd = init->getSetID();
            init->setSetID(this->getNodeGroupIDByRadID(grnd));
            //坐标系ID to do
        }
        //重力
        FITKGravityManager*  gMgr = _caseData->getGravityManager();
        if (gMgr == nullptr) return ;
        for (int i = 0; i < gMgr->getDataCount(); ++i)
        {
            Radioss::FITKGravity* grav = gMgr->getDataByIndex(i);
            if (grav == nullptr) continue;
            //节点组ID
            const int grnd = grav->getNodeGroupID();
            grav->setNodeGroupID(this->getNodeGroupIDByRadID(grnd));
            //坐标系ID to do
            //时间函数ID to do
            //传感器ID to do
        }
        //刚性墙
        FITKRadiossRWallManager*  wallMgr = _caseData->getRWallManager();
        if (wallMgr == nullptr) return;
        for (int i = 0; i < wallMgr->getDataCount(); ++i)
        {
            Radioss::FITKAbstractRWall* wall = wallMgr->getDataByIndex(i);
            if (wall == nullptr) continue;
            //参考节点ID
            const int ndID = wall->getNodeId();
            wall->setNodeId(this->getNodeIDByRadID(ndID));
            //节点组ID
            const int grnd1 = wall->getNodeGroupId1();
            wall->setNodeGroupId1(this->getNodeGroupIDByRadID(grnd1));
            const int grnd2 = wall->getNodeGroupId2();
            if(grnd2!=0)
                wall->setNodeGroupId2(this->getNodeGroupIDByRadID(grnd2));
        }
        //边界
        FITKRadiossBCSManager* bcMgr = _caseData->getBCSManager();
        if (bcMgr == nullptr) return ;
        for (int i = 0; i < bcMgr->getDataCount(); ++i)
        {
            Radioss::FITKAbstractBCS* bc = bcMgr->getDataByIndex(i);
            if (bc == nullptr) continue;
            //节点组ID
            const int grnd1 = bc->getGroupID1();
            const int grnd2 = bc->getGroupID2();
            bc->setGroupID(this->getNodeGroupIDByRadID(grnd1));
            if (grnd2 != 0)
                bc->setGroupID(this->getNodeGroupIDByRadID(grnd1), this->getNodeGroupIDByRadID(grnd2));
            //坐标系ID to do
        }
        //边界元法
        FITKRadiossBEMManager* bemMgr =  _caseData->getBEMManager();
        if (bemMgr == nullptr) return;
        for (int i = 0; i < bemMgr->getDataCount(); ++i)
        {
            Radioss::FITKAbstractBEM* bem = bemMgr->getDataByIndex(i);
            if (bem == nullptr) continue;
            FITKBEMDoubleAsymptoticApproximation* daa = dynamic_cast<FITKBEMDoubleAsymptoticApproximation*>(bem);
            if (daa)
            {
                int surfId = daa->getSurfId();
                int gravId = daa->getGravId();
                int surfaceID = this->getSurfaceSetIDByRadID(surfId);
                int gravityID =  this->getGravIDByRadID(gravId);
                if (surfaceID > 0)
                    daa->setSurfId(surfaceID);
                if (gravityID > 0)
                    daa->setGravId(gravityID);
            }
        }
        //材料
        Interface::FITKMaterialManager* materialsMgr = _caseData->getMaterialManager();
        if (materialsMgr == nullptr) return ;
        for (int i = 0; i < materialsMgr->getDataCount(); ++i)
        {
            Interface::FITKAbstractMaterial* mat = materialsMgr->getDataByIndex(i);
            FITKMaterialPlasTab* material = dynamic_cast<FITKMaterialPlasTab*>(mat);
            if (!material)continue;
            //曲线ID
            FITKMaterialPlasTabData& data= material->getMaterialData();
            int id = data.m_fctIDp;
            data.m_fctIDp = this->getCurveIDByRadID(id);

            QList<int> fct_IDList{};
            for (auto id: data.m_fctID) {
                fct_IDList.append(this->getCurveIDByRadID(id));
            }
            data.m_fctID.clear();
            data.m_fctID = fct_IDList;
        }
        //状态方程
        FITKEquationOfStateManager* eosMgr = _caseData->getEquationOfState();
        if (eosMgr == nullptr) return;
        for (int i = 0; i < eosMgr->getDataCount(); ++i)
        {
            FITKAbstractEquationOfState* eos = eosMgr->getDataByIndex(i);
            if (!eos)continue;
            //材料ID
            QList<int> matIDs = eos->getMaterialIDs();
            for (auto radMatID : matIDs) {
                int matID = this->getMaterialID(radMatID);
                FITKMaterialHydro* mat =dynamic_cast<FITKMaterialHydro*> (materialsMgr->getDataByID(matID));
                if (!mat)continue;
                mat->setEOSID(eos->getDataObjectID());
                eos->addMaterialID(matID);
            };
        }
        //失效模型
        FITKRadiossFailureModelManager* failMgr = _caseData->getFailureModelManager();
        if (failMgr == nullptr) return;
        for (int i = 0; i < failMgr->getDataCount(); ++i)
        {
            FITKAbstractFailureModel* fail = failMgr->getDataByIndex(i);
            if (!fail)continue;
            //材料ID
            QList<int> matIDs = fail->getMaterialIDs();
            for (auto radMatID : matIDs) {
                int matID = this->getMaterialID(radMatID);
                FITKAbstractRadMaterial* mat = dynamic_cast<FITKAbstractRadMaterial*> (materialsMgr->getDataByID(matID));
                if (!mat)continue;
                mat->setFailState(true);
                mat->setFailID(fail->getDataObjectID());
                fail->addMaterialID(matID);
            };
        }
        //相互作用
        FITKInteractionManager* interactionMgr = _caseData->getInteractionManager();
        if (interactionMgr == nullptr) return;
        for (int i = 0; i < interactionMgr->getDataCount(); ++i)
        {
            FITKAbstractInteraction* Interaction = interactionMgr->getDataByIndex(i);
            if (!Interaction)continue;
            //节点组ID
            int grnd = Interaction->getNodeGroupID();
            Interaction->setNodeGroupID(this->getNodeGroupIDByRadID(grnd));
            //面ID
            int surfID = Interaction->getMasterSurfaceID();
            Interaction->setMasterSurfaceID(this->getSurfaceSetIDByRadID(surfID));

        }
        //属性BeamSPR
        Interface::FITKAbstractSectionManager* sectionMgr = _caseData->getSectionManager();
        if(!sectionMgr) return;
        for (int i = 0; i < sectionMgr->getDataCount(); ++i)
        {
            FITKPropBeamSPR* beam = dynamic_cast<FITKPropBeamSPR*>(sectionMgr->getDataByIndex(i));
            if (!beam)continue;
            FITKPropValueBeamSPR* data = beam->getValue();
            if(!data)continue;
            //传感器ID  to do
            //坐标ID  to do
            //曲线ID
            for (int i = DOF_SPR::Tension_Compression; i <= DOF_SPR::Bending_Z; ++i) {
                ValueDOF& dof = data->getValueDOF((DOF_SPR)i);
                dof.fct_ID1 = this->getCurveIDByRadID(dof.fct_ID1);
                dof.fct_ID2 = this->getCurveIDByRadID(dof.fct_ID2);
                dof.fct_ID3 = this->getCurveIDByRadID(dof.fct_ID3);
                dof.fct_ID4 = this->getCurveIDByRadID(dof.fct_ID4);
            }
        }
        //属性Sandwich Shell
        for (int i = 0; i < sectionMgr->getDataCount(); ++i)
        {
            FITKPropSandwichShell* sShell = dynamic_cast<FITKPropSandwichShell*>(sectionMgr->getDataByIndex(i));
            if (!sShell)continue;
            FITKPropSandwichShellValue* data = sShell->getValue();
            if (!data)continue;
            for (int i = 0; i < data->getN(); ++i)
            {
                SandwichShellLayerValue oldLayV = data->getLayerValue(i);
                int matID = oldLayV._matID;
                matID = this->getMaterialID(matID);
                oldLayV._matID = matID;
                data->setLayerValue(oldLayV,i);
            }

        }
        //刚性连接
        FITKConnectionManager* connectionMgr = _caseData->getCurrentSolution()->getConnectionManager();
        if (!connectionMgr) return;
        for (int i = 0; i < connectionMgr->getDataCount(); ++i)
        {
            FITKConnectionRBODY* connection = dynamic_cast<FITKConnectionRBODY*>(connectionMgr->getDataByIndex(i));
            if (!connection)continue;
            //点ID
            int ndID = connection->getMainNodeID();
            connection->setMainNodeID(this->getNodeIDByRadID(ndID));
            //节点组ID
            int grnd = connection->getSecondaryNodesGroupID();
            connection->setSecondaryNodesGroupID(this->getNodeGroupIDByRadID(grnd));
            //面ID
            int surfID = connection->getEnvelopeSurface();
            connection->setEnvelopeSurface(this->getSurfaceSetIDByRadID(surfID));
        }
        //截面探针
        FITKProbeManager* probeMgr = _caseData->getCurrentSolution()->getProbeManager();
        if (!probeMgr) return;
        for (int i = 0; i < probeMgr->getDataCount(); ++i)
        {
            FITKProbeSection* section = dynamic_cast<FITKProbeSection*>(probeMgr->getDataByIndex(i));
            if (!section)continue;
            //节点ID/*在适配器中完成*/
             
            //节点组ID
            int grnd = section->getGrndID();
            section->setGrndID(this->getNodeGroupIDByRadID(grnd));
            //壳组ID
            int grshelID = section->getGrshelID();
            section->setGrshelID(this->getElementGroupIDByRadID(grshelID));
            //梁组ID
            int grbeamID = section->getGrbeamID();
            section->setGrbeamID(this->getElementGroupIDByRadID(grbeamID));
            //其他ID to do

        }
        //时间历程
        FITKTimeHistoryManager* thMgr = _caseData->getCurrentSolution()->getTimeHietoryManager();
        if (!thMgr) return;
        for (int i = 0; i < thMgr->getDataCount(); ++i) {
            FITKAbstractTimeHistory* th = dynamic_cast<FITKAbstractTimeHistory*>(thMgr->getDataByIndex(i));
            if (!th)continue;
            FITKAbstractTimeHistory::TimeHistoryType thType = th->getTimeHistoryType();
            QList<int> objIDs{};
            if (thType == FITKAbstractTimeHistory::TimeHistory_ACCEL) {
                for (int radID : th->getObjIDs())
                    objIDs.append(this->getProbeAccelIDByRadID(radID));
            }
            else if (thType == FITKAbstractTimeHistory::TimeHistory_INTER) {
                for (int radID : th->getObjIDs())
                    objIDs.append(this->getInteractionIDByRadID(radID));
            }
            else if (thType == FITKAbstractTimeHistory::TimeHistory_RWALL) {
                for (int radID : th->getObjIDs())
                    objIDs.append(this->getRWallIDByRadID(radID));
            }
            else if (thType == FITKAbstractTimeHistory::TimeHistory_RBODY) {
                for (int radID : th->getObjIDs())
                    objIDs.append(this->getConnectionIDByRadID(radID));
            }
            else if (thType == FITKAbstractTimeHistory::TimeHistory_SECTIO) {
                for (int radID : th->getObjIDs())
                    objIDs.append(this->getProbeSectionIDByRadID(radID));
            }
            else if (thType == FITKAbstractTimeHistory::TimeHistory_SPHCEL) {
                FITKTimeHistorySPHCEL* tH = dynamic_cast<FITKTimeHistorySPHCEL*>(th);
                if (!tH) continue;
                int objID = this->getNodeIDByRadID(tH->getSPHNodeID());
                tH->setSPHNodeID(objID);
            }
            if(objIDs.isEmpty())continue;
            th->setObjIDs(objIDs);
        }
    }

    FITKRadiossPart * RadReadInformationMapper::getRadiossPartByFilePartID(int id, bool isCreate)
    {
        if(_caseData == nullptr) return nullptr;

        //获取part管理器
        FITKRadiossPartManager * partManager = _caseData->getMeshModel()->getPartsManager();
        if(partManager == nullptr) return nullptr;  
        
        if(_partIDMap.contains(id))
        {
            //如果partIDMap中存在该id，直接返回对应的part
            int partID = _partIDMap.value(id);
            return partManager->getDataByID(partID);
        }
        else
        {
            if(!isCreate)return nullptr;
            //如果不存在，返回创建 part
            FITKRadiossPart* part = new FITKRadiossPart();
            partManager->appendDataObj(part);
            _partIDMap.insert(id, part->getDataObjectID());
            return part;
        }
    }

    void RadReadInformationMapper::insertMaterialIDMap(const int radID, const int materialID)
    {
        _materialIDMap.insert(radID, materialID);
    }

    int RadReadInformationMapper::getMaterialID(const int radID)
    {
        return _materialIDMap.contains(radID) ? _materialIDMap.value(radID) : 0;
    }

    void RadReadInformationMapper::insertPropertyIDMap(const int radID, const int propertyID)
    {
        _propertyIDMap.insert(radID, propertyID);
    }

    int RadReadInformationMapper::getPropertyID(const int radID)
    {
        return _propertyIDMap.contains(radID) ? _propertyIDMap.value(radID) : 0;
    }

    void RadReadInformationMapper::insertNodeGroupIDMap(const int radID, const int nodeGroupID)
    {
        _nodeGroupIDMap.insert(radID, nodeGroupID);
    }
    int RadReadInformationMapper::getNodeGroupIDByRadID(int radID) const
    {
        return _nodeGroupIDMap.contains(radID) ? _nodeGroupIDMap.value(radID) : 0;
    }


    void RadReadInformationMapper::insertNodeIDMap(const int radID, const int nodeID)
    {
        _nodeIDMap.insert(radID, nodeID);
    }
    int RadReadInformationMapper::getNodeIDByRadID(int radID) const
    {
        return _nodeIDMap.contains(radID) ? _nodeIDMap.value(radID) : 0;
    }

    void RadReadInformationMapper::insertEleIDMap(const int radID, const int eleID)
    {
        _eleIDMap.insert(radID, eleID);
    }
    int RadReadInformationMapper::getEleIDByRadID(int eleID) const
    {
        return _eleIDMap.contains(eleID) ? _eleIDMap.value(eleID) : 0;
    }

    void RadReadInformationMapper::insertSubsetIDMap(const int radID, const int subsetID)
    {
        _subsetIDMap.insert(radID, subsetID);
    }
    int RadReadInformationMapper::getSubsetIDByRadID(int subsetID) const
    {
        return _subsetIDMap.contains(subsetID) ? _subsetIDMap.value(subsetID) : 0;
    }

    void RadReadInformationMapper::insertCurveIDMap(const int radID, const int curveID)
    {
        _curveIDMap.insert(radID, curveID);
    }
    int RadReadInformationMapper::getCurveIDByRadID(int curveID) const
    {
        return _curveIDMap.contains(curveID) ? _curveIDMap.value(curveID) : 0;
    }

    void RadReadInformationMapper::insertSurfaceSetIDMap(const int radID, const int surfaceID)
    {
        _surfaceIDMap.insert(radID, surfaceID);
    }
    int RadReadInformationMapper::getSurfaceSetIDByRadID(int surfaceID) const
    {
        return _surfaceIDMap.contains(surfaceID) ? _surfaceIDMap.value(surfaceID) : 0;
    }

    void RadReadInformationMapper::insertInteractionIDMap(const int radID, const int interactionID)
    {
        _interactionIDMap.insert(radID, interactionID);
    }
    int RadReadInformationMapper::getInteractionIDByRadID(int interactionID) const
    {
        return _interactionIDMap.contains(interactionID) ? _interactionIDMap.value(interactionID) : 0;
    }


    void RadReadInformationMapper::insertConnectionIDMap(const int radID, const int connectionID)
    {
        _connectionIDMap.insert(radID, connectionID);
    }

    int RadReadInformationMapper::getConnectionIDByRadID(int connectionID) const
    {
        return _connectionIDMap.contains(connectionID) ? _connectionIDMap.value(connectionID) : 0;
    }

    void RadReadInformationMapper::insertElementGroupIDMap(const int radID, const int elementGroupID)
    {
        _elementGroupID.insert(radID, elementGroupID);
    }
    int RadReadInformationMapper::getElementGroupIDByRadID(int elementGroupID) const
    {
        return _elementGroupID.contains(elementGroupID) ? _elementGroupID.value(elementGroupID) : 0;
    }

    void RadReadInformationMapper::insertProbeAccelIDMap(const int radID, const int probeID)
    {
        _probeAccelID.insert(radID, probeID);
    }
    int RadReadInformationMapper::getProbeAccelIDByRadID(int probeID) const
    {
        return _probeAccelID.contains(probeID) ? _probeAccelID.value(probeID) : 0;
    }

    void RadReadInformationMapper::insertProbeSectionIDMap(const int radID, const int probeID)
    {
        _probeSectionID.insert(radID, probeID);
    }

    int RadReadInformationMapper::getProbeSectionIDByRadID(int probeID) const
    {
        return _probeSectionID.contains(probeID) ? _probeSectionID.value(probeID) : 0;
    }

    void RadReadInformationMapper::insertRWallIDMap(const int radID, const int rwallID)
    {
        _rWallID.insert(radID, rwallID);
    }
    int RadReadInformationMapper::getRWallIDByRadID(int rwallID) const
    {
        return _rWallID.contains(rwallID) ? _rWallID.value(rwallID) : 0;
    }

    void RadReadInformationMapper::insertFailureIDMap(const int radID, const int failureID)
    {
        _failureID.insert(radID, failureID);
    }

    int RadReadInformationMapper::getFailureIDByRadID(int failureID) const
    {
        return _failureID.contains(failureID) ? _failureID.value(failureID) : 0;
    }

    void RadReadInformationMapper::insertBEMIDMap(const int radID, const int bemID)
    {
        _bemID.insert(radID, bemID);
    }

    int RadReadInformationMapper::getBEMIDByRadID(int bemID) const
    {
        return _bemID.contains(bemID) ? _bemID.value(bemID) : 0;
    }

    void RadReadInformationMapper::insertGravIDMap(const int radID, const int gravID)
    {
        _gravID.insert(radID, gravID);
    }

    int RadReadInformationMapper::getGravIDByRadID(int gravID) const
    {
        return _gravID.contains(gravID) ? _gravID.value(gravID) : 0;
    }

    //void RadReadInformationMapper::insertEOSIDMap(const int radID, const int eosID)
    //{
    //    _eosID.insert(radID, eosID);
    //}
    //int RadReadInformationMapper::getEOSIDByRadID(int eosID) const
    //{
    //    return _eosID.contains(eosID) ? _eosID.value(eosID) : 0;
    //}
}
