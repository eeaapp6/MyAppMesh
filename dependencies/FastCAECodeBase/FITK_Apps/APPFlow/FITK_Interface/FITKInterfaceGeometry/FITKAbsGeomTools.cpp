/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeomTools.h"

#include "FITKGeoInterfaceFactory.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    // 点查询。
    //@{
    FITKAbstractGeomPointTool* FITKAbstractGeomPointTool::createTool()
    {
        //获取注册的工具生成器
        FITKAbstractGeomToolsCreator* c = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (c == nullptr) return nullptr;
        //创建工具
        return c->createPointTool();
    }

    bool FITKAbstractGeomPointTool::getXYZByID(const int & id, double* oXyz, int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getXYZ(vTopo, oXyz, iModelId);
    }
    //@}

    // 线查询。
    //@{
    FITKAbstractGeomEdgeTool* FITKAbstractGeomEdgeTool::createTool()
    {
        //获取注册的工具生成器
        FITKAbstractGeomToolsCreator* c = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (c == nullptr) return nullptr;
        //创建工具
        return c->createEdgeTool();
    }

    bool FITKAbstractGeomEdgeTool::getStartPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getStartPointDirection(vTopo, oPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getEndPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getEndPointDirection(vTopo, oPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getDirectionByID(const int & id, double* iPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getDirection(vTopo, iPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getCoedgeDirectionByID(const int & faceId, const int & edgeId, double* iPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopoFace = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(faceId);
        Interface::FITKAbsVirtualTopo* vTopoEdge = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(edgeId);
        return this->getCoedgeDirection(vTopoFace, vTopoEdge, iPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getBoundaryNormalByID(const int & faceId, const int & edgeId, double* iPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopoFace = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(faceId);
        Interface::FITKAbsVirtualTopo* vTopoEdge = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(edgeId);
        return this->getBoundaryNormal(vTopoFace, vTopoEdge, iPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::isPosOnEdgeByID(const int & id, double* iPos)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->isPosOnEdge(vTopo, iPos);
    }

    bool FITKAbstractGeomEdgeTool::isLineByID(const int & id)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->isLine(vTopo);
    }

    bool FITKAbstractGeomEdgeTool::getLengthByID(const int & id, double & len)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getLength(vTopo, len);
    }

    bool FITKAbstractGeomEdgeTool::getMidPointByID(const int & id, double * oPos, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getMidPoint(vTopo, oPos, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getMidPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getMidPointDirection(vTopo, oPos, oDir, iModelId);
    }

    bool FITKAbstractGeomEdgeTool::getFeaturePointsByID(const int & id, QList<QList<double>> & oPoses, bool skipMidPt, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getFeaturePoints(vTopo, oPoses, skipMidPt, iModelId);
    }
    bool FITKAbstractGeomEdgeTool::evalPointByID(const int & id, double param, double * pos, bool normalized, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->evalPoint(vTopo, param, pos, normalized, iModelId);
    }
    //@}

    // 面查询。
    //@{
    FITKAbstractGeomFaceTool* FITKAbstractGeomFaceTool::createTool()
    {
        //获取注册的工具生成器
        FITKAbstractGeomToolsCreator* c = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (c == nullptr) return nullptr;
        //创建工具
        return c->createFaceTool();
    }

    bool FITKAbstractGeomFaceTool::getWiresByID(const int cmdId, const int & id, QList<QList<int>> & oWires)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getWires(cmdId, vTopo, oWires);
    }

    bool FITKAbstractGeomFaceTool::getAreaByID(const int & id, double & oArea)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getArea(vTopo, oArea);
    }

    bool FITKAbstractGeomFaceTool::getFaceBoundSizeByID(const int & id, double & oLne)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getFaceBoundSize(vTopo, oLne);
    }

    bool FITKAbstractGeomFaceTool::getMeanCurvatureAtPosByID(const int & id, double* iPos, double & oCur)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getMeanCurvatureAtPos(vTopo, iPos, oCur);
    }

    bool FITKAbstractGeomFaceTool::getCurvatureAtPosByID(const int & id, double* iPos,
        double & oCur1, double & oCur2, double* oDir1, double* oDir2)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getCurvatureAtPos(vTopo, iPos, oCur1, oCur2, oDir1, oDir2);
    }

    bool FITKAbstractGeomFaceTool::getPlaneByID(const int & id, double* oPos, double* oNormal, double* oUp, int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getPlane(vTopo, oPos, oNormal, oUp, iModelId);
    }

    bool FITKAbstractGeomFaceTool::getNormalByID(const int & id, double* oPos, double* oNormal, int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getNormal(vTopo, oPos, oNormal, iModelId);
    }

    bool FITKAbstractGeomFaceTool::getNormalAtPosByID(const int & id, double * iPos, double * oNormal, int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getNormalAtPos(vTopo, iPos, oNormal, iModelId);
    }

    bool FITKAbstractGeomFaceTool::isPlaneByID(const int & id)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->isPlane(vTopo);
    }

    bool FITKAbstractGeomFaceTool::isCylindByID(const int & id)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->isCylind(vTopo);
    }

    bool FITKAbstractGeomFaceTool::getCylindSurfByID(const int & id, double* oCt, double* oDir)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getCylindSurf(vTopo, oCt, oDir);
    }

    bool FITKAbstractGeomFaceTool::getCylindSurfRadiusByID(const int & id, double & oRadius)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getCylindSurfRadius(vTopo, oRadius);
    }

    bool FITKAbstractGeomFaceTool::isConeByID(const int & id)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->isCone(vTopo);
    }

    bool FITKAbstractGeomFaceTool::getConeSurfByID(const int & id, double* oCt, double* oDir)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getConeSurf(vTopo, oCt, oDir);
    }
    //@}

    // 通用查询。
    //@{
    FITKAbstractGeomCommonTool* FITKAbstractGeomCommonTool::createTool()
    {
        //获取注册的工具生成器
        FITKAbstractGeomToolsCreator* c = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (c == nullptr) return nullptr;
        //创建工具
        return c->createCommonTool();
    }

    bool FITKAbstractGeomCommonTool::getDistanceByTopoIDs(const int vTopoId_1, const int vTopoId_2, 
        double & oDist, double oPos_1[3], double oPos_2[3], int iModelId_1, int iModelId_2)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo_1 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vTopoId_1);
        Interface::FITKAbsVirtualTopo* vTopo_2 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vTopoId_2);
        return this->getDistanceByTopos(vTopo_1, vTopo_2, oDist, oPos_1, oPos_2, iModelId_1, iModelId_2);
    }

    bool FITKAbstractGeomCommonTool::getDistanceByTopoIDAndPos(const int vTopoId, double iPos3[3],
        double & oDist, double oPos[3], int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vTopoId);
        return this->getDistanceByTopoAndPos(vTopo, iPos3, oDist, oPos, iModelId);
    }

    bool FITKAbstractGeomCommonTool::getCrossByEdgeIDAndLine(const int vEdgeId, double iPos1[3], double iPos2[3], double oPos[3])
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vEdgeId);
        return this->getCrossByEdgeAndLine(vTopo, iPos1, iPos2, oPos);
    }

    bool FITKAbstractGeomCommonTool::getFaceSamplePointsByID(const int vTopoId, QList<QList<double>> & oPoses, int iSpacing, int iModelId)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vTopoId);
        return this->getFaceSamplePoints(vTopo, oPoses, iSpacing, iModelId);
    }
    //@}

    // 特征查询。
    //@{
    FITKAbstractGeomFeatureTool* FITKAbstractGeomFeatureTool::createTool()
    {
        //获取注册的工具生成器
        FITKAbstractGeomToolsCreator* c = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (c == nullptr) return nullptr;
        //创建工具
        return c->createFeatureTool();
    }
    //@}

    FITKAbstractGeomToolsCreator::~FITKAbstractGeomToolsCreator()
    {
    }

    FITKAbstractGeomPointTool* FITKAbstractGeomToolsCreator::createPointTool()
    {
        //错误值
        return nullptr;
    }

    FITKAbstractGeomEdgeTool* FITKAbstractGeomToolsCreator::createEdgeTool()
    {
        //错误值
        return nullptr;
    }

    FITKAbstractGeomFaceTool* FITKAbstractGeomToolsCreator::createFaceTool()
    {
        //错误值
        return nullptr;
    }
    Interface::FITKAbstractGeomCommonTool* FITKAbstractGeomToolsCreator::createCommonTool()
    {
        //错误值
        return nullptr;
    }
    Interface::FITKAbstractGeomFeatureTool * FITKAbstractGeomToolsCreator::createFeatureTool()
    {
        //错误值
        return nullptr;
    }
}
