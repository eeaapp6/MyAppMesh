/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeomTools.h"

#include "FITKGeoInterfaceFactory.h"
#include "FITKAbsVirtualTopoMapper.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

#include <QSet>

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

    QList<QPair<int, QPair<int, int>>> FITKAbstractGeomEdgeTool::sortEdgeInfos(QList<int> edgeVirTopoIDs)
    {
        QList<QPair<int, QPair<int, int>>> edgeInfoTemp = {};
        // 从端点开始遍历构建有序边列表
        QList<QPair<int, QPair<int, int>>> sortedEdges;
        if (edgeVirTopoIDs.size() <= 0) {
            return sortedEdges;
        }
        else if (edgeVirTopoIDs.size() == 1) {
            Interface::FITKAbsVirtualTopo* subEdgeVirTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(edgeVirTopoIDs[0]);
            if (subEdgeVirTopo == nullptr) {
                return sortedEdges;
            }
            Interface::FITKVirtualTopoMapper topoMapper;
            topoMapper.mapTopo(subEdgeVirTopo->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
            int pointNum = topoMapper.length();
            if (pointNum > 1) {
                Interface::FITKAbsVirtualTopo* firPointTopo = topoMapper.virtualTopo(0);
                Interface::FITKAbsVirtualTopo* lastPointTopo = topoMapper.virtualTopo(pointNum - 1);
                if (firPointTopo == nullptr || lastPointTopo == nullptr) {
                    return sortedEdges;
                }
                QPair<int, int> pointInfo(firPointTopo->getDataObjectID(), lastPointTopo->getDataObjectID());
                sortedEdges.append(QPair<int, QPair<int, int>>(subEdgeVirTopo->getDataObjectID(), pointInfo));
                return sortedEdges;
            }
            else if (pointNum == 1) {
                Interface::FITKAbsVirtualTopo* firPointTopo = topoMapper.virtualTopo(0);
                Interface::FITKAbsVirtualTopo* lastPointTopo = topoMapper.virtualTopo(0);
                if (firPointTopo == nullptr || lastPointTopo == nullptr) {
                    return sortedEdges;
                }
                QPair<int, int> pointInfo(firPointTopo->getDataObjectID(), lastPointTopo->getDataObjectID());
                sortedEdges.append(QPair<int, QPair<int, int>>(subEdgeVirTopo->getDataObjectID(), pointInfo));
                return sortedEdges;
            }
            else {
                return sortedEdges;
            }
        }
        else {
            //查找虚拟边中信息
            for (int edgeVirTopoID : edgeVirTopoIDs) {
                Interface::FITKAbsVirtualTopo* subEdgeVirTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(edgeVirTopoID);
                if (subEdgeVirTopo == nullptr) {
                    continue;
                }
                Interface::FITKVirtualTopoMapper topoMapper;
                topoMapper.mapTopo(subEdgeVirTopo->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
                int pointNum = topoMapper.length();
                if (pointNum <= 1) {
                    continue;
                }

                Interface::FITKAbsVirtualTopo* firPointTopo = topoMapper.virtualTopo(0);
                Interface::FITKAbsVirtualTopo* lastPointTopo = topoMapper.virtualTopo(pointNum - 1);
                if (firPointTopo == nullptr || lastPointTopo == nullptr) {
                    continue;
                }
                QPair<int, int> pointInfo(firPointTopo->getDataObjectID(), lastPointTopo->getDataObjectID());
                edgeInfoTemp.append(QPair<int, QPair<int, int>>(subEdgeVirTopo->getDataObjectID(), pointInfo));
            }

            if (edgeInfoTemp.size() < 1) {
                return sortedEdges;
            }

            // 构建邻接表：记录每个点连接的边
            QHash<int, QList<int>> pointToEdges;
            // 边ID到边信息的映射
            QHash<int, QPair<int, int>> edgeIdToPoints;

            for (const QPair<int, QPair<int, int>>& edge : edgeInfoTemp) {
                int edgeId = edge.first;
                int point1 = edge.second.first;
                int point2 = edge.second.second;

                edgeIdToPoints[edgeId] = edge.second;
                pointToEdges[point1].append(edgeId);
                pointToEdges[point2].append(edgeId);
            }

            // 找到端点（只连接一条边的点）
            int startPoint = -1;
            for (auto it = pointToEdges.begin(); it != pointToEdges.end(); ++it) {
                if (it.value().size() == 1) {
                    startPoint = it.key();
                    break;
                }
            }

            //判断是否可能是环形边
            if (startPoint == -1) {
                startPoint = edgeInfoTemp[0].second.first;
            }

            int currentPoint = startPoint;
            // 记录已使用的边
            QSet<int> usedEdges;

            while (true) {
                // 获取当前点连接的边
                const QList<int>& connectedEdges = pointToEdges[currentPoint];

                // 找出未使用的边
                int nextEdge = -1;
                for (int edgeId : connectedEdges) {
                    if (!usedEdges.contains(edgeId)) {
                        nextEdge = edgeId;
                        break;
                    }
                }
                // 没有更多边了
                if (nextEdge == -1) break;

                // 将边添加到结果中
                QPair<int, int> points = edgeIdToPoints[nextEdge];
                //判断当前点是首点还是末点
                if (points.first == currentPoint) {
                    sortedEdges.append(qMakePair(nextEdge, points));
                }
                else if (points.second == currentPoint) {
                    sortedEdges.append(qMakePair(nextEdge, QPair<int, int>(points.second, points.first)));
                }
                usedEdges.insert(nextEdge);

                // 移动到下一个点
                currentPoint = (points.first == currentPoint) ? points.second : points.first;
            }
        }

        return sortedEdges;
    }

    void FITKAbstractGeomEdgeTool::getPointByRatio(int edgeVirTopoId, double ratio, double * point, int & mergeEdgeTopoId, double & mergerRatio)
    {
        Interface::FITKAbsVirtualTopo* edgeVirTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(edgeVirTopoId);
        if (edgeVirTopo == nullptr) {
            return;
        }

        //判断是实际的几何边还是虚拓扑边
        QList<Interface::FITKAbsVirtualTopo*> subEdgeVirTopos = edgeVirTopo->getCombinedBasicToposRecur();
        if (subEdgeVirTopos.size() == 0) {
            Interface::FITKAbstractGeomEdgeTool* tool = Interface::FITKAbstractGeomEdgeTool::createTool();
            if (tool == nullptr) {
                return;
            }
            tool->evalPointByID(edgeVirTopoId, ratio, point, true);
            delete tool;
            tool = nullptr;
            //返回该点所在的边
            mergeEdgeTopoId = edgeVirTopoId;
            return;
        }
        else {
            QList<int> subEdgeVirTopoIDs = {};
            for (Interface::FITKAbsVirtualTopo* subEdgeVirTopo : subEdgeVirTopos) {
                if (subEdgeVirTopo == nullptr) {
                    continue;
                }
                subEdgeVirTopoIDs << subEdgeVirTopo->getDataObjectID();
            }
            //对边进行有序排序
            QList<QPair<int, QPair<int, int>>> edgeInfos = sortEdgeInfos(subEdgeVirTopoIDs);

            //计算数据结构中边的方向是否与排序后的一致
            QList<bool> edgeDirs = {};
            for (QPair<int, QPair<int, int>> edgeInfo : edgeInfos) {
                int edgeId = edgeInfo.first;
                int firPointId = edgeInfo.second.first;
                int secPointId = edgeInfo.second.second;
                Interface::FITKVirtualTopoMapper topoMapper;
                topoMapper.mapTopo(edgeId, Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
                int pointNum = topoMapper.length();
                if (pointNum <= 1) {
                    continue;
                }

                Interface::FITKAbsVirtualTopo* firPointTopo = topoMapper.virtualTopo(0);
                Interface::FITKAbsVirtualTopo* lastPointTopo = topoMapper.virtualTopo(pointNum - 1);
                if (firPointTopo == nullptr || lastPointTopo == nullptr) {
                    continue;
                }

                if (firPointTopo->getDataObjectID() == firPointId && lastPointTopo->getDataObjectID() == secPointId) {
                    edgeDirs.append(true);
                }
                else if (firPointTopo->getDataObjectID() == secPointId && lastPointTopo->getDataObjectID() == firPointId) {
                    edgeDirs.append(false);
                }
                else {

                }
            }

            //计算每段长度与总边长
            double allLength = 0.0;
            QList<double> lengths = {};
            for (QPair<int, QPair<int, int>> edgeInfo : edgeInfos) {
                Interface::FITKAbstractGeomEdgeTool* tool = Interface::FITKAbstractGeomEdgeTool::createTool();
                if (tool == nullptr) {
                    return;
                }
                double length = 0.0;
                tool->getLengthByID(edgeInfo.first, length);
                allLength += length;
                lengths.append(length);
                delete tool;
                tool = nullptr;
            }

            if (lengths.size() != edgeInfos.size() || lengths.size() != edgeDirs.size()) {
                return;
            }

            double radioLength = allLength * ratio;
            for (int i = 0; i < lengths.size(); i++) {
                //添加精度
                double length = lengths[i] + 1e-07;
                //判断比例长度是否大于当前长度
                if (radioLength > length) {
                    radioLength = radioLength - length;
                    continue;
                }
                else {
                    //如果小于，就计算该长度在当前边中的比例
                    double newRadio = radioLength / length;

                    int curEdgeId = edgeInfos[i].first;
                    bool edgeDir = edgeDirs[i];
                    Interface::FITKAbstractGeomEdgeTool* tool = Interface::FITKAbstractGeomEdgeTool::createTool();
                    if (tool == nullptr) {
                        return;
                    }

                    if (edgeDir == false) {
                        newRadio = 1 - newRadio;
                    }

                    tool->evalPointByID(curEdgeId, newRadio, point, true);
                    delete tool;
                    tool = nullptr;
                    mergeEdgeTopoId = curEdgeId;
                    mergerRatio = newRadio;
                    return;
                }
            }
        }
    }

    bool FITKAbstractGeomEdgeTool::sampleEdgeByID(int vEdgeId, int nPoses, QVector<QVector<double>> & oPoses, QVector<QVector<double>> & oDirs, int iModelId)
    {
        // 调用虚拓扑对象接口。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vEdgeId);
        return this->sampleEdge(vTopo, nPoses, oPoses, oDirs, iModelId);

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

    bool FITKAbstractGeomFaceTool::getCylindSurfInfoByID(const int & id, double* oCt, double* oDir, double & oRadius, double* oBds)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getCylindSurfInfo(vTopo, oCt, oDir, oRadius, oBds);
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

    bool FITKAbstractGeomFaceTool::getConeSurfInfoByID(const int & id, double* oCt, double* oApex, double* oDir, double & oRadius, double* oBds)
    {
        // Cast the data.
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
        return this->getConeSurfInfo(vTopo, oCt, oDir, oApex, oRadius, oBds);
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
