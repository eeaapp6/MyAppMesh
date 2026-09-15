/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoShapeAgent.h"
#include "FITKVirtualTopoManager.h"
#include "FITKGeometryMeshVS.h"
#include "FITKGeometryMeshEntity.h"
#include "FITKAbsGeomTools.h"
#include "FITKGeoInterfaceFactory.h"

#include <QVector3D>
#include <QList>
#include <QFile>
#include <QTextStream>

#include <cmath>

namespace Interface
{
    FITKAbsGeoShapeAgent::FITKAbsGeoShapeAgent(FITKAbsGeoCommand* command)
        :_command(command)
    {
        _vtmanager = new FITKVirtualTopoManager;
        _meshVS = new FITKGeometryMeshVS;
    }

    FITKAbsGeoShapeAgent::~FITKAbsGeoShapeAgent()
    {
        if (_vtmanager) delete _vtmanager;
        if (_meshVS) delete  _meshVS;
    }

    void FITKAbsGeoShapeAgent::clearShape()
    {

    }

    void FITKAbsGeoShapeAgent::buildVirtualTopo(bool keepTopos)
    {
        Q_UNUSED(keepTopos);
    }

    void FITKAbsGeoShapeAgent::triangulation()
    {

    }

    void FITKAbsGeoShapeAgent::updateVS()
    {
        if (_vtmanager) _vtmanager->clear();

        if (_meshVS) _meshVS->clear();

        //创建虚拓扑
        this->buildVirtualTopo(true);
        //几何三角化
        this->triangulation();
    }

    FITKGeometryMeshVS* FITKAbsGeoShapeAgent::getMeshVS()
    {
        return _meshVS;
    }

    FITKGeoEnum::FITKGeoEngine Interface::FITKAbsGeoShapeAgent::getGeoEngine()
    {
        //错误值
        return FITKGeoEnum::FGENone;
    }

    bool Interface::FITKAbsGeoShapeAgent::isComponentValid(FITKModelEnum::FITKModelSetType compType, const QList<int> & members)
    {
        if (!_vtmanager)
        {
            return false;
        }

        FITKShapeVirtualTopoManager* vMgr{ nullptr };

        // 获取对应类型虚拓扑管理器。
        switch (compType)
        {
        case Interface::FITKModelEnum::FMSPoint:
            vMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType::VSPoint);
            break;
        case Interface::FITKModelEnum::FMSEdge:
            vMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType::VSEdge);
            break;
        case Interface::FITKModelEnum::FMSSurface:
            vMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType::VSFace);
            break;
        case Interface::FITKModelEnum::FMSSolid:
            vMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType::VSSolid);
            break;
        default:
            return false;
        }

        if (!vMgr)
        {
            return false;
        }

        // 检查虚拓扑对象是否存在。
        for (const int & member : members)
        {
            if (!vMgr->getDataByID(member))
            {
                return false;
            }
        }

        return true;
    }

    FITKAbsGeoCommand* Interface::FITKAbsGeoShapeAgent::getGeoCommand()
    {
        return _command;
    }

    FITKVirtualTopoManager* Interface::FITKAbsGeoShapeAgent::getVirtualTopoManager()
    {
        return _vtmanager;
    }

    bool FITKAbsGeoShapeAgent::writeSTLFile(QString filePath)
    {
        if (!_meshVS || !_vtmanager)
        {
            return false;
        }

        FITKShapeVirtualTopoManager* faceMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType::VSFace);
        if (!faceMgr)
        {
            return false;
        }

        // 写入文件。
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }

        QTextStream stream(&file);

        // 获取所有面数据。
        const QHash<int, FITKGeoMeshVSFaceEntity*> faceHash = _meshVS->getFaceItems();

        // 遍历所有面写入STL文件。
        QHashIterator<int, FITKGeoMeshVSFaceEntity*> iter(faceHash);

        while (iter.hasNext())
        {
            iter.next();

            int meshFaceId = iter.key();
            FITKAbsVirtualTopo* faceTopo = faceMgr->getDataByID(meshFaceId);
            if (!faceTopo)
            {
                continue;
            }

            int faceIndex = faceTopo->getIndexLabel();

            FITKGeoMeshVSFaceEntity* face = iter.value();

            // 写入面起始标题行。
            stream << "solid " << QString("face_%1").arg(faceIndex) << "\n";

            // 获取属于面的所有点数据。
            const QList<FITKGeoMeshVSPt*> & facePoints = face->getPoints();

            // 获取三角面片。
            const QList<FITKGeoMeshVSTri*> & tris = face->getTriangles();

            for (FITKGeoMeshVSTri* tri : tris)
            {
                // 获取三角面片节点索引。
                const int* nodes = tri->getNode();

                // 获取三角面片节点。
                FITKGeoMeshVSPt* pt1 = facePoints[nodes[0]];
                FITKGeoMeshVSPt* pt2 = facePoints[nodes[1]];
                FITKGeoMeshVSPt* pt3 = facePoints[nodes[2]];

                // 计算三角面片法向。
                QList<QVector3D> facePointsPos;
                double nor3[3]{ 0., 0., 0. };
                facePointsPos.push_back(QVector3D(pt1->x(), pt1->y(), pt1->z()));
                facePointsPos.push_back(QVector3D(pt2->x(), pt2->y(), pt2->z()));
                facePointsPos.push_back(QVector3D(pt3->x(), pt3->y(), pt3->z()));
                GetFaceNormal(facePointsPos, nor3);

                // 写入三角面片法向。
                stream << "    facet normal " << nor3[0] << " " << nor3[1] << " " << nor3[2] << "\n";

                // 依次写入面片节点坐标。
                stream << "        outer loop\n";
                stream << "            vertex " << pt1->x() << " " << pt1->y() << " " << pt1->z() << "\n";
                stream << "            vertex " << pt2->x() << " " << pt2->y() << " " << pt2->z() << "\n";
                stream << "            vertex " << pt3->x() << " " << pt3->y() << " " << pt3->z() << "\n";
                stream << "        endloop\n";

                // 写入三角面片结束标识。
                stream << "    endfacet\n";
            }

            // 写入分组结束标识。
            stream << "endsolid " << QString("face_%1").arg(faceIndex) << "\n";

            faceIndex++;
        }

        file.close();

        return true;
    }

    void FITKAbsGeoShapeAgent::GetFaceNormal(QList<QVector3D> & points, double* normal)
    {
        // 临时数据。
        double v[3][3], *v0 = v[0], *v1 = v[1], *v2 = v[2], *tmp;
        double ax, ay, az, bx, by, bz;

        // 检查节点数量。
        int nPts = points.count();
        if (nPts < 3)
        {
            return;
        }

        // 三角形法向。
        if (nPts == 3)
        {
            for (int i = 0; i < 3; i++)
            {
                v0[i] = points[0][i];
                v1[i] = points[1][i];
                v2[i] = points[2][i];
            }

            // 根据节点连接顺序获取法向区分正反。
            ax = v2[0] - v1[0];
            ay = v2[1] - v1[1];
            az = v2[2] - v1[2];
            bx = v0[0] - v1[0];
            by = v0[1] - v1[1];
            bz = v0[2] - v1[2];

            normal[0] = (ay * bz - az * by);
            normal[1] = (az * bx - ax * bz);
            normal[2] = (ax * by - ay * bx);
        }
        // 多边形法向。
        else
        {

            for (int i = 0; i < 3; i++)
            {
                v1[i] = points[0][i];
                v2[i] = points[1][i];
            }

            for (int i = 0; i < nPts; i++)
            {
                tmp = v0;
                v0 = v1;
                v1 = v2;
                v2 = tmp;

                for (int j = 0; j < 3; j++)
                {
                    v2[j] = points[(i + 2) % nPts][i];
                }

                // 根据节点连接顺序获取法向区分正反。
                ax = v2[0] - v1[0];
                ay = v2[1] - v1[1];
                az = v2[2] - v1[2];
                bx = v0[0] - v1[0];
                by = v0[1] - v1[1];
                bz = v0[2] - v1[2];

                normal[0] += (ay * bz - az * by);
                normal[1] += (az * bx - ax * bz);
                normal[2] += (ax * by - ay * bx);
            }
        }

        // 归一化。
        double fac = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
        if (qFuzzyCompare(fac, 0.))
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            normal[i] /= fac;
        }
    }

    bool FITKAbsGeoShapeAgent::isSameState(QVariant& stateVal_1, QVariant& stateVal_2)
    {
        Q_UNUSED(stateVal_1);
        Q_UNUSED(stateVal_2);
        return false;
    }

    bool FITKAbsGeoShapeAgent::isValid()
    {
        return true;
    }

    int FITKAbsGeoShapeAgent::getShapeStateID()
    {
        return _geoStateID;
    }
    void FITKAbsGeoShapeAgent::setShapeStateID(int stateID)
    {
        _geoStateID = stateID;
    }

    bool FITKAbsGeoShapeAgent::getPointCoor(int pointID, double* coor, int modelIndex)
    {
        Q_UNUSED(modelIndex);

        if (!_vtmanager)
        {
            return false;
        }

        FITKShapeVirtualTopoManager* vPtMgr = _vtmanager->getShapeVirtualTopoManager(FITKGeoEnum::VSPoint);
        if (!vPtMgr)
        {
            return false;
        }

        // 获取虚拓扑对象并查询坐标。
        FITKAbsVirtualTopo* vPt = vPtMgr->getDataByID(pointID);
        if (!vPt)
        {
            return false;
        }

        FITKAbstractGeomToolsCreator* toolCreator = FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (!toolCreator)
        {
            return false;
        }

        FITKAbstractGeomPointTool* ptTool = toolCreator->createPointTool();
        if (!ptTool)
        {
            return false;
        }

        bool flag = ptTool->getXYZ(vPt, coor);

        delete ptTool;

        return flag;
    }
}