/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyFITKGeoBehavior.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoModelOperSolid.h"
#include "FITKAbsGeoModelSurface.h"
#include "FITKGeoInterfaceFactory.h"
#include "FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKAbsGeoModelCut.h"
#include "PyFITKSketch2D.h"
#include "FITKAbsGeoModelPartitionFace.h"
#include "FITKAbsGeoReferencePlane.h"
#include "FITKAbsGeoRepairSmallEdges.h"
#include "FITKAbsGeoModelOperFace.h"
#include "FITKAbsGeoRemoveWireEdges.h"
#include "FITKAbsGeoCoverEdges.h"
#include "FITKAbsGeoStitch.h"
#include "FITKAbsGeoReplaceFaces.h"
#include "FITKAbsGeoRepairSmallFaces.h"
#include "FITKAbsGeoReferenceLine.h"
#include "FITKAbsGeomTools.h"
#include "FITKAbsGeoModelPartitionSolid.h"
#include "FITKAbsGeoModelPartitionEdge.h"
#include "FITKAbsGeoVirtualTopoCombine.h"
#include "FITKAbsGeoMergeEdges.h"
#include <memory>

namespace Interface
{
    PyFITKGeoBehavior::PyFITKGeoBehavior(int geoCmdID)
    {
        setGeoCmd(geoCmdID);
    }

    PyFITKGeoBehavior::PyFITKGeoBehavior(Interface::FITKAbsGeoCommand * geoCmd)
    {
        _geoCmd = geoCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::chamfer(QList<int> indexs, double length)
    {
        QList<Interface::VirtualShape> edges = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty()) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto chamferCmd = fac->createCommandT<Interface::FITKAbsGeoModelChamferSolid>(Interface::FITKGeoEnum::FGTChamferSolid);
        if (!chamferCmd)return nullptr;
        chamferCmd->setInputCmdId(edges[0].CmdId);
        chamferCmd->setEdgeTopos(edges);
        chamferCmd->setDistance(length);

        return chamferCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::fillet(QList<int> indexs, double length)
    {
        QList<Interface::VirtualShape> edges = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty()) return nullptr; 

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto chamferCmd = fac->createCommandT<Interface::FITKAbsGeoModelFilletSolid>(Interface::FITKGeoEnum::FGTFilletSolid);
        if (!chamferCmd)return nullptr;
        chamferCmd->setInputCmdId(edges[0].CmdId);
        chamferCmd->setEdgeTopos(edges);
        chamferCmd->setRadius(length);

        return chamferCmd;
    }

    Interface::FITKAbsGeoCommand* PyFITKGeoBehavior::extrudeCutByDepth(int sketchId, const QVector<double>& direction, double depth)
    {
        if (direction.size() != 3|| depth<=0)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeCut);
        if (!extrudeCutCmd) return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeCutCmd->setSourceSurface(shape);
        extrudeCutCmd->setLength(depth);
        extrudeCutCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeCutCmd->setExtrudeType(1);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeCutCmd->getDataObjectID(), sketchId);
        return extrudeCutCmd;
    }

    Interface::FITKAbsGeoCommand* PyFITKGeoBehavior::extrudeCutByFace(int sketchId, const QVector<double>& direction, int index)
    {
        if (direction.size() != 3 )return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeCut);
        if (!extrudeCutCmd) return nullptr;

        Interface::VirtualShape toFace = getShape(index, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeCutCmd->setSourceSurface(shape);
        extrudeCutCmd->setUpToFace(toFace);
        extrudeCutCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeCutCmd->setExtrudeType(2);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeCutCmd->getDataObjectID(), sketchId);
        return extrudeCutCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::extrudeSolidByLength(int sketchId, const QVector<double>& direction, double depth)
    {
        if (direction.size() != 3 || depth <= 0)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSolid);
        if (!extrudeSolidCmd) return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeSolidCmd->setSourceSurface(shape);
        extrudeSolidCmd->setLength(depth);
        extrudeSolidCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeSolidCmd->setExtrudeType(1);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeSolidCmd->getDataObjectID(), sketchId);
        return extrudeSolidCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::extrudeSolidByFace(int sketchId, const QVector<double>& direction, int index)
    {
        if (direction.size() != 3)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSolid);
        if (!extrudeSolidCmd) return nullptr;

        Interface::VirtualShape toFace = getShape(index, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeSolidCmd->setSourceSurface(shape);
        extrudeSolidCmd->setUpToFace(toFace);
        extrudeSolidCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeSolidCmd->setExtrudeType(2);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeSolidCmd->getDataObjectID(), sketchId);
        return extrudeSolidCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::extrudeShellByLength(int sketchId, const QVector<double>& direction, double depth)
    {
        if (direction.size() != 3 || depth <= 0)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSurface);
        if (!extrudeShellCmd) return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeShellCmd->setSourceCurve(shape);
        extrudeShellCmd->setLength(depth);
        extrudeShellCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeShellCmd->setExtrudeType(1);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeShellCmd->getDataObjectID(), sketchId);
        return extrudeShellCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::extrudeShellByFace(int sketchId, const QVector<double>& direction, int index)
    {
        if (direction.size() != 3)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extrudeShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSurface);
        if (!extrudeShellCmd) return nullptr;

        Interface::VirtualShape toFace = getShape(index, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        extrudeShellCmd->setSourceCurve(shape);
        extrudeShellCmd->setUpToFace(toFace);
        extrudeShellCmd->setDirection(direction[0], direction[1], direction[2]);
        extrudeShellCmd->setExtrudeType(2);
        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(extrudeShellCmd->getDataObjectID(), sketchId);
        return extrudeShellCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::revolveCut(int sketchId, const QVector<double>& points, double angle)
    {
        if (points.size() != 6)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto revolCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolCut);
        if (!revolCutCmd)return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        revolCutCmd->setSourceSurface(shape);

        revolCutCmd->setRotateAxisPoint1(points[0], points[1], points[2]);
        revolCutCmd->setRotateAxisPoint2(points[3], points[4], points[5]);
        revolCutCmd->setAngle(angle);

        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(revolCutCmd->getDataObjectID(), sketchId);
        return revolCutCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::revolveSolid(int sketchId, const QVector<double>& points, double angle)
    {
        if (points.size() != 6)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto revolSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolSolid);
        if (!revolSolidCmd)return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        revolSolidCmd->setSourceSurface(shape);

        revolSolidCmd->setRotateAxisPoint1(points[0], points[1], points[2]);
        revolSolidCmd->setRotateAxisPoint2(points[3], points[4], points[5]);
        revolSolidCmd->setAngle(angle);

        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(revolSolidCmd->getDataObjectID(), sketchId);
        return revolSolidCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::revolveShell(int sketchId, const QVector<double>& points, double angle)
    {
        if (points.size() != 6)return nullptr;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto revolShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolSurface);
        if (!revolShellCmd)return nullptr;

        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        revolShellCmd->setSourveCurve(shape);

        revolShellCmd->setRotateAxisPoint1(points[0], points[1], points[2]);
        revolShellCmd->setRotateAxisPoint2(points[3], points[4], points[5]);
        revolShellCmd->setAngle(angle);

        //记录特征和草图，用于删除
        _FeaturesUseSketches.insert(revolShellCmd->getDataObjectID(), sketchId);
        return revolShellCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::sweepByEdgeFace(const QList<int>& indexEdge, int indexFace, int type, bool flip)
    {
        QList<Interface::VirtualShape> edge = getShapes(indexEdge, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::VirtualShape face = getShape(indexFace, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (type == 1) {
            auto sweepSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSolid);
            if(!sweepSolidCmd)return nullptr;
            sweepSolidCmd->setCurves(edge);
            sweepSolidCmd->setProfile(face);
            sweepSolidCmd->setFlipPath(flip);
            return sweepSolidCmd;
        }
        else if (type == 2) {
            auto sweepShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSurface);
            if (!sweepShellCmd)return nullptr;
            sweepShellCmd->setCurves(edge);
            sweepShellCmd->setProfiles(QList<Interface::VirtualShape>{face});
            sweepShellCmd->setFlipPath(flip);
            return sweepShellCmd;
        }
        else if (type == 3) {
            auto sweepCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepCut);
            if (!sweepCutCmd)return nullptr;
            sweepCutCmd->setCurves(edge);
            sweepCutCmd->setProfile(face);
            sweepCutCmd->setFlipPath(flip);
            return sweepCutCmd;
        }
        return nullptr;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::sweepBySketchFace(const QString & sketchNameEdge, int indexFace, int type)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByName(sketchNameEdge);
        if (!sketch) return nullptr;
        Interface::VirtualShape edge;
        edge.CmdId = sketch->getDataObjectID();
        Interface::VirtualShape face = getShape(indexFace, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        QList<Interface::VirtualShape> edges{ edge };

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (type == 1) {
            auto sweepSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSolid);
            if (!sweepSolidCmd)return nullptr;
            sweepSolidCmd->setCurves(edges);
            sweepSolidCmd->setProfile(face);
            sweepSolidCmd->setFlipPath(false);
            return sweepSolidCmd;
        }
        else if (type == 2) {
            auto sweepShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSurface);
            if (!sweepShellCmd)return nullptr;
            sweepShellCmd->setCurves(edges);
            sweepShellCmd->setProfiles(QList<Interface::VirtualShape>{face});
            sweepShellCmd->setFlipPath(false);
            return sweepShellCmd;
        }
        else if (type == 3) {
            auto sweepCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepCut);
            if (!sweepCutCmd)return nullptr;
            sweepCutCmd->setCurves(edges);
            sweepCutCmd->setProfile(face);
            sweepCutCmd->setFlipPath(false);
            return sweepCutCmd;
        }
        return nullptr;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::sweepByEdgeSketch(const QList<int>& indexEdge, const QString & sketchNameFace, int type, bool flip)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByName(sketchNameFace);
        if (!sketch) return nullptr;
        Interface::VirtualShape face;
        face.CmdId = sketch->getDataObjectID();
        QList<Interface::VirtualShape> edge = getShapes(indexEdge, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (type == 1) {
            auto sweepSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSolid);
            if (!sweepSolidCmd)return nullptr;
            sweepSolidCmd->setCurves(edge);
            sweepSolidCmd->setProfile(face);
            sweepSolidCmd->setFlipPath(flip);
            return sweepSolidCmd;
        }
        else if (type == 2) {
            auto sweepShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSurface);
            if (!sweepShellCmd)return nullptr;
            sweepShellCmd->setCurves(edge);
            sweepShellCmd->setProfiles(QList<Interface::VirtualShape>{face});
            sweepShellCmd->setFlipPath(flip);
            return sweepShellCmd;
        }
        else if (type == 3) {
            auto sweepCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepCut);
            if (!sweepCutCmd)return nullptr;
            sweepCutCmd->setCurves(edge);
            sweepCutCmd->setProfile(face);
            sweepCutCmd->setFlipPath(flip);
            return sweepCutCmd;
        }
        return nullptr;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::sweepBySketchSketch(const QString & sketchNameEdge, const QString & sketchNameFace, int type)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketchedge = cmdList->getDataByName(sketchNameEdge);
        Interface::FITKAbsGeoCommand* sketchface = cmdList->getDataByName(sketchNameFace);
        if (!sketchedge||!sketchface) return nullptr;
        Interface::VirtualShape edge;
        edge.CmdId = sketchedge->getDataObjectID();
        Interface::VirtualShape face;
        face.CmdId = sketchface->getDataObjectID();
        QList<Interface::VirtualShape> edges{ edge };

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (type == 1) {
            auto sweepSolidCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSolid);
            if (!sweepSolidCmd)return nullptr;
            sweepSolidCmd->setCurves(edges);
            sweepSolidCmd->setProfile(face);
            sweepSolidCmd->setFlipPath(false);
            return sweepSolidCmd;
        }
        else if (type == 2) {
            auto sweepShellCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSurface);
            if (!sweepShellCmd)return nullptr;
            sweepShellCmd->setCurves(edges);
            sweepShellCmd->setProfiles(QList<Interface::VirtualShape>{face});
            sweepShellCmd->setFlipPath(false);
            return sweepShellCmd;
        }
        else if (type == 3) {
            auto sweepCutCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepCut>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepCut);
            if (!sweepCutCmd)return nullptr;
            sweepCutCmd->setCurves(edges);
            sweepCutCmd->setProfile(face);
            sweepCutCmd->setFlipPath(false);
            return sweepCutCmd;
        }
        return nullptr;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::circularHoleThrough(int faceIndex, int edgeIndex1, int edgeIndex2, double distance1, double distance2, double diameter, bool dir)
    {
        Interface::VirtualShape face = getShape(faceIndex, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        Interface::VirtualShape edge1 = getShape(edgeIndex1, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::VirtualShape edge2 = getShape(edgeIndex2, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (face.VirtualTopoId < 0 || edge1.VirtualTopoId < 0 || edge2.VirtualTopoId < 0) return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto circularHoleCmd = fac->createCommandT<Interface::FITKAbsGeoModelCircularHoleSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularHole);
        if (!circularHoleCmd) return nullptr;
        circularHoleCmd->setOperFace(face);
        circularHoleCmd->setFlip(dir);
        circularHoleCmd->setOperEdge1(edge1);
        circularHoleCmd->setDistanceLocateHole1(distance1);
        circularHoleCmd->setOperEdge2(edge2);
        circularHoleCmd->setDistanceLocateHole2(distance2);
        circularHoleCmd->setDiameter(diameter);
        return circularHoleCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::removeFeaturesOnFaces(const QList<int>& indexs)
    {
        QList<Interface::VirtualShape> faces= getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto defeatureCmd = fac->createCommandT<Interface::FITKAbsGeoModelDefeatureSolid>(Interface::FITKGeoEnum::FGTDefeature);
        if (!defeatureCmd) return nullptr;
        defeatureCmd->setInputCmdId(faces[0].CmdId);
        defeatureCmd->setFaceTopos(faces);
        return defeatureCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceSketch(int sketchId, const QList<int>& indexs)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        QList<Interface::VirtualShape> faces = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithSketch>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithSketch);
        if(!partitionCmd) return nullptr;
        partitionCmd->setFaces(faces);
        partitionCmd->setSketch(sketchId);

        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceTwoPoints(const QVector<double>& twoPoints, const QList<int>& indexs)
    {
        if (twoPoints.size() != 6)return nullptr;
        QList<Interface::VirtualShape> faces = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithTwoPoints);
        if (!partitionCmd) return nullptr;
        partitionCmd->setFaces(faces);
        partitionCmd->setStartPoint(QVector<double> {twoPoints[0], twoPoints[1], twoPoints[2]});
        partitionCmd->setEndPoint(QVector<double> {twoPoints[3], twoPoints[4], twoPoints[5]});
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceCurvedPath(int faceIndex, int edgeIndex1, int edgeIndex2, const QVector<double>& twoPoints)
    {
        if (twoPoints.size() != 6)return nullptr;
        Interface::VirtualShape face = getShape(faceIndex, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        Interface::VirtualShape edge1 = getShape(edgeIndex1, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::VirtualShape edge2 = getShape(edgeIndex2, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (face.VirtualTopoId < 0 || edge1.VirtualTopoId < 0 || edge2.VirtualTopoId < 0) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithCurvedPath);
        if (!partitionCmd) return nullptr;
        partitionCmd->setFace(face);
        partitionCmd->setFirstEdge(edge1);
        partitionCmd->setSecondEdge(edge2);
        partitionCmd->setFirstPoint(QVector<double> {twoPoints[0], twoPoints[1], twoPoints[2]});
        partitionCmd->setSecondPoint(QVector<double> {twoPoints[3], twoPoints[4], twoPoints[5]});
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceExtendFace(int extendfaceIndex, const QList<int>& indexs)
    {
        QList<Interface::VirtualShape> faces = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;
        Interface::VirtualShape extendface = getShape(extendfaceIndex, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if(extendface.VirtualTopoId<0) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithExtendFace>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithExtendFace);
        if (!partitionCmd) return nullptr;
        partitionCmd->setFaces(faces);
        partitionCmd->setExtendFace(extendface);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceWithDatumPlane(const QList<int>& indexs, const QString & planeName)
    {
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;
        QList<Interface::VirtualShape> faces = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;
        if(!datumMgr->getDataByName(planeName))return nullptr;
        auto plane = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(datumMgr->getDataByName(planeName));
        if(!plane)return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithDatumPlane);
        if (!partitionCmd)return nullptr;
        partitionCmd->setFaces(faces);
        partitionCmd->setDatumPlane(plane);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceProjectEdges(const QList<int>& edgeIndexs, const QList<int>& faceIndexs)
    {
        QList<Interface::VirtualShape> faces = getShapes(faceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (faces.isEmpty()||edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithProjectEdges);
        if (!partitionCmd)return nullptr;
        partitionCmd->setFaces(faces);
        partitionCmd->setProjectEdges(edges);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionFaceWithIntersectFace(const QList<int>& partitionIndexs, const QList<int>& toolIndexs)
    {
        QList<Interface::VirtualShape> partitions = getShapes(partitionIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        QList<Interface::VirtualShape> tools = getShapes(toolIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (partitions.isEmpty() || tools.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionFaceWithIntersectFace);
        if (!partitionCmd)return nullptr;
        partitionCmd->setFaces(partitions);
        partitionCmd->setToolFaces(tools);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlaneOffsetPrincipal(int type, double offset,const QString& name)
    {
        if(type<1||type>3) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto offsetPlaneCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceOffsetPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane);
        if (!offsetPlaneCmd)return nullptr;
        switch (type)
        {
        case 1:
            offsetPlaneCmd->setSourcePlaneOrigin(0, 0, 0);
            offsetPlaneCmd->setSourcePlaneNormal(0, 0, 1);
            offsetPlaneCmd->setSourcePlaneUp(0, 1, 0);
            break;
        case 2:
            offsetPlaneCmd->setSourcePlaneOrigin(0, 0, 0);
            offsetPlaneCmd->setSourcePlaneNormal(1, 0, 0);
            offsetPlaneCmd->setSourcePlaneUp(0, 0, 1);
            break;
        case 3:
            offsetPlaneCmd->setSourcePlaneOrigin(0, 0, 0);
            offsetPlaneCmd->setSourcePlaneNormal(0, -1, 0);
            offsetPlaneCmd->setSourcePlaneUp(0, 0, 1);
            break;
        default:
            break;
        }
        offsetPlaneCmd->setOffset(offset);
        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        offsetPlaneCmd->setDataObjectName(fName);

        if (!offsetPlaneCmd->update()) return nullptr;

        datumMgr->appendDataObj(offsetPlaneCmd);

        return offsetPlaneCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlaneOffsetPlane(const QVector<double>& pos_nor_up, double offset, const QString & name)
    {
        if (pos_nor_up.size()!=9) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto offsetPlaneCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceOffsetPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane);
        if (!offsetPlaneCmd) return nullptr;
        offsetPlaneCmd->setSourcePlaneOrigin(pos_nor_up[0], pos_nor_up[1], pos_nor_up[2]);
        offsetPlaneCmd->setSourcePlaneNormal(pos_nor_up[3], pos_nor_up[4], pos_nor_up[5]);
        offsetPlaneCmd->setSourcePlaneUp(pos_nor_up[6], pos_nor_up[7], pos_nor_up[8]);
        offsetPlaneCmd->setOffset(offset);
        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        offsetPlaneCmd->setDataObjectName(fName);

        if (!offsetPlaneCmd->update()) return nullptr;

        datumMgr->appendDataObj(offsetPlaneCmd);

        return offsetPlaneCmd;
    }


    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlaneThreePoints(const QVector<double>& points, const QString & name)
    {
        if (points.size() != 9) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto planeCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane);
        if (planeCmd == nullptr) return nullptr;
        planeCmd->setPoint(0, points.at(0), points.at(1), points.at(2));
        planeCmd->setPoint(1, points.at(3), points.at(4), points.at(5));
        planeCmd->setPoint(2, points.at(6), points.at(7), points.at(8));

        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        planeCmd->setDataObjectName(fName);

        if (!planeCmd->update()) return nullptr;

        datumMgr->appendDataObj(planeCmd);

        return planeCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlanePointAndNormal(const QVector<double>& point, const QVector<double>& dir, const QString & name)
    {
        if (point.size() != 3|| dir.size() != 3) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto planeCmd = fac->createDatumT<Interface::FITKAbsGeoReferencePointAndDirectionPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane);
        if (planeCmd == nullptr) return nullptr;
        planeCmd->setPoint(point[0], point[1], point[2]);
        planeCmd->setDirection(dir[0], dir[1], dir[2]);

        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        planeCmd->setDataObjectName(fName);

        if (!planeCmd->update()) return nullptr;

        datumMgr->appendDataObj(planeCmd);

        return planeCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlaneMidwaybetween(const QVector<double>& points, const QString & name)
    {
        if (points.size() != 6) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto planeCmd = fac->createDatumT<Interface::FITKAbsGeoReferencePointAndDirectionPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane);
        if (planeCmd == nullptr) return nullptr;

        auto x = points[3] - points[0];
        auto y = points[4] - points[1];
        auto z = points[5] - points[2];

        planeCmd->setPoint(points[0] + x / 2.0, points[1] + y / 2.0, points[2] + z / 2.0);
        planeCmd->setDirection(x, y, z);

        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        planeCmd->setDataObjectName(fName);

        if (!planeCmd->update()) return nullptr;

        datumMgr->appendDataObj(planeCmd);

        return planeCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPlaneRotate(const QVector<double>& pos_nor_up, const QVector<double>& axis, double angle, const QString & name)
    {
        if (pos_nor_up.size() != 9 || axis.size() != 6) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto planeCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceRotateFromPlanePlane>(Interface::FITKGeoEnum::FITKDatumType::FDTRotateFromPlanePlane);
        if (planeCmd == nullptr) return nullptr;
        planeCmd->setPlane(pos_nor_up);
        planeCmd->setAxis(QVector<double>{axis[0], axis[1], axis[2]}, QVector<double>{axis[3], axis[4], axis[5]});
        planeCmd->setAngle(angle);

        QString fName = name;
        if (fName.isEmpty())
            fName = datumMgr->checkName(QString("Datum plane-1"));
        else
            fName = datumMgr->checkName(name);
        planeCmd->setDataObjectName(fName);

        if (!planeCmd->update()) return nullptr;

        datumMgr->appendDataObj(planeCmd);

        return planeCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumPoint(double x, double y, double z, const QString& name)
    {
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto datumPoint = fac->createDatumT<Interface::FITKAbsGeoDatumPoint>(Interface::FITKGeoEnum::FITKDatumType::FDTPoint);
        if (!datumPoint) return nullptr;
        double pos3[3]{ x, y, z };
        datumPoint->setPosition(pos3);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum pt-1"));
        else
            pName = datumMgr->checkName(name);
        datumPoint->setDataObjectName(pName);

        datumMgr->appendDataObj(datumPoint);

        return datumPoint;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::repairSmallEdges(const QList<int>& edgeIndexs)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if(edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto repairSmallEdgesCmd = fac->createCommandT<Interface::FITKAbsGeoRepairSmallEdges>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRepairSmallEdges);
        if(!repairSmallEdgesCmd) return nullptr;
        repairSmallEdgesCmd->setEdges(edges);
        return repairSmallEdgesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::mergeEdges(const QList<int>& edgeIndexs)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto repairSmallEdgesCmd = fac->createCommandT<Interface::FITKAbsGeoMergeEdges>(Interface::FITKGeoEnum::FITKGeometryComType::FGTMergeEdges);
        if (!repairSmallEdgesCmd) return nullptr;
        repairSmallEdgesCmd->setEdges(edges);
        return repairSmallEdgesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::repairSmallFaces(const QList<int>& faceIndexs)
    {
        QList<Interface::VirtualShape> faces = getShapes(faceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto repairSmallFacesCmd = fac->createCommandT<Interface::FITKAbsGeoRepairSmallFaces>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRepairSmallFaces);
        if(!repairSmallFacesCmd)return nullptr;
        repairSmallFacesCmd->setFaces(faces);
        return repairSmallFacesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::removeFaces(const QList<int>& faceIndexs)
    {
        QList<Interface::VirtualShape> faces = getShapes(faceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto removeFacesCmd = fac->createCommandT<Interface::FITKAbsGeoModelOperFaceRemoveFace>(Interface::FITKGeoEnum::FGTRemoveFace);
        if (!removeFacesCmd) return nullptr;
        removeFacesCmd->set(faces);
        return removeFacesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::removeWireEdges(const QList<int>& edgeIndexs)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty()) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto removeWireEdgesCmd = fac->createCommandT<Interface::FITKAbsGeoRemoveWireEdges>(
            Interface::FITKGeoEnum::FGTRemoveWireEdges);
        if (!removeWireEdgesCmd) return nullptr;
        removeWireEdgesCmd->set(edges);
        return removeWireEdgesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::coverEdges(const QList<int>& edgeIndexs)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto coverEdgesCmd = fac->createCommandT<Interface::FITKAbsGeoCoverEdges>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCoverEdges);
        if (!coverEdgesCmd) return nullptr;
        coverEdgesCmd->setEdges(edges);
        return coverEdgesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::stitch(double tolerance, const QList<int>& edgeIndexs)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto stitchCmd = fac->createCommandT<Interface::FITKAbsGeoStitch>(Interface::FITKGeoEnum::FITKGeometryComType::FGTStitch);
        if(!stitchCmd) return nullptr;
        stitchCmd->setTolerance(tolerance);
        stitchCmd->setEdges(edges);
        return stitchCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::replaceFaces(const QList<int>& faceIndexs, bool extendNeighboringFaces)
    {
        QList<Interface::VirtualShape> faces = getShapes(faceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        // 只选一个面并且不扩张邻面时直接返回
        if (faces.size() == 1 && !extendNeighboringFaces)  return nullptr;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto replaceFacesCmd = fac->createCommandT<Interface::FITKAbsGeoReplaceFaces>(Interface::FITKGeoEnum::FITKGeometryComType::FGTReplaceFaces);
        replaceFacesCmd->setFaces(faces);
        replaceFacesCmd->setExtend(extendNeighboringFaces);
        return replaceFacesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::extendFaces(const QList<int>& edgeIndexs, const QList<int>& faceIndexs,
        const QList<int>&  tagFaceIndexs, double distance, int SpecifyModel, int MethodModel)
    {
        QList<Interface::VirtualShape> edges = getShapes(edgeIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        QList<Interface::VirtualShape> faces = getShapes(faceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        QList<Interface::VirtualShape> tagFaces = getShapes(tagFaceIndexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (edges.isEmpty()&& faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto extendFacesCmd = fac->createCommandT<Interface::FITKAbsGeoModelOperFaceExtendFace>(Interface::FITKGeoEnum::FGTExtendFace);
        if (extendFacesCmd == nullptr) return nullptr;
        extendFacesCmd->setEdges(edges);
        extendFacesCmd->set(faces);
        extendFacesCmd->setDistance(distance);
        extendFacesCmd->setTagFaces(tagFaces);

        extendFacesCmd->setSpecifyMode((Interface::FITKAbsGeoModelOperFaceExtendFace::SpecifyModel)SpecifyModel);
        extendFacesCmd->setMethodModel((Interface::FITKAbsGeoModelOperFaceExtendFace::MethodModel)MethodModel);

        return extendFacesCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisPrincipal(int type, const QString& name)
    {
        if (type < 1 || type>3) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoDatumLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLine);
        if (!lineCmd)return nullptr;
        double origin[3] = { 0., 0., 0. };
        lineCmd->setPosition(origin);
        switch (type)
        {
        case 1: {
            double pos[3] = { 1., 0., 0. };
            lineCmd->setPosition2(pos);
            break;
        }
        case 2: {
            double pos[3] = { 0., 1., 0. };
            lineCmd->setPosition2(pos);
            break;
        }
        case 3: {
            double pos[3] = { 0., 0., 1. };
            lineCmd->setPosition2(pos);
            break;
        }
        default:break;
        }
        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);
        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisIntersectPlanes(const QVector<double>& pos_nor_up_1, const QVector<double>& pos_nor_up_2, const QString & name)
    {
        if(pos_nor_up_1.size()!=9|| pos_nor_up_2.size()!=9) return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLineByIntersectPlanes>(Interface::FITKGeoEnum::FITKDatumType::FDTLineByIntersectPlanes);
        if (!lineCmd)return nullptr;

        lineCmd->setPlane1(pos_nor_up_1);
        lineCmd->setPlane2(pos_nor_up_2);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisStraightEdge(int index, const QString & name)
    {
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;
        Interface::VirtualShape edges = getShape(index, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if(edges.VirtualTopoId<0)return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLine);
        if (!lineCmd)return nullptr;
        // 获取几何模型工具接口。
        Interface::FITKAbstractGeomToolsCreator* geomTools = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (geomTools == nullptr) return nullptr;

        // 获取点查询工具
        std::unique_ptr<Interface::FITKAbstractGeomEdgeTool> edgeTool(geomTools->createEdgeTool());
        if (edgeTool == nullptr) return nullptr;

        double pos[3]{}, dir[3]{};

        if (!edgeTool->getStartPointDirectionByID(edges.VirtualTopoId, pos, dir)) return nullptr;

        double pos2[3];
        for (int i = 0; i < 3; ++i) {
            pos2[i] = pos[i] + dir[i];
        }
        lineCmd->setPosition(pos);
        lineCmd->setPosition2(pos2);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisTwoPoints(const QVector<double>& points, const QString & name)
    {
        if(points.size()!=6)return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLine);
        if (!lineCmd)return nullptr;
        double point1[3]{ points[0], points[1], points[2] };
        double point2[3]{ points[3], points[4], points[5] };
        lineCmd->setPosition(point1);
        lineCmd->setPosition2(point2);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisNormalToPlane(const QVector<double>& pos_nor_up_1, QVector<double> point, const QString & name)
    {
        if (point.size() != 3|| pos_nor_up_1.size()!=9)return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLine);
        if (!lineCmd)return nullptr;
        lineCmd->setPosition(point.data());
        double secondPoint[3]{};
        for (int i = 0; i < 3; ++i) {
            secondPoint[i] = point[i] + pos_nor_up_1[i + 3];
        }
        lineCmd->setPosition2(secondPoint);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisPointsOnCircle(const QVector<double>& points, const QString & name)
    {
        if(points.size()!=9)return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        // 获取几何模型工具接口。
        Interface::FITKAbstractGeomToolsCreator* geomTools = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (geomTools == nullptr) return nullptr;
        // 创建几何线工具。
        std::unique_ptr<Interface::FITKAbstractGeomCommonTool> commonTool(geomTools->createCommonTool());
        if (commonTool == nullptr) return nullptr;
        QVector<double> firstPoint{ points[0],points[1], points[2] }, secondPoint{ points[3],points[4], points[5] }, thirdPoint{ points[6],points[7], points[8] };
        QVector<double> center{}, normal{};
        double radius{};
        if (!commonTool->calcCircleInfoBy3Points(firstPoint, secondPoint, thirdPoint, center, normal, radius))  return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLine);
        if(!lineCmd)return nullptr;
        lineCmd->setPosition(center.data());
        double pos2[3]{};
        for (int i = 0; i < 3; ++i) {
            pos2[i] = center[i] + normal[i];
        }
        lineCmd->setPosition2(pos2);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::datumAxisRotateFromLine(const QVector<double>& linePoints, const QVector<double>& rotatePoints, double angle, const QString & name)
    {
        if (linePoints.size() != 6 || rotatePoints.size() != 6)return nullptr;
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto lineCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceLineByRotateLine>(Interface::FITKGeoEnum::FITKDatumType::FDTLineByRotateLine);
        if (!lineCmd)return nullptr;
        lineCmd->setLine(QVector<double>{linePoints[0], linePoints[1], linePoints[2]}, QVector<double>{linePoints[3], linePoints[4], linePoints[5]});
        lineCmd->setAxis(QVector<double>{rotatePoints[0], rotatePoints[1], rotatePoints[2]}, QVector<double>{rotatePoints[3], rotatePoints[4], rotatePoints[5]});
        lineCmd->setAngle(angle);

        QString pName = name;
        if (pName.isEmpty())
            pName = datumMgr->checkName(tr("Datum axis-1"));
        else
            pName = datumMgr->checkName(name);
        lineCmd->setDataObjectName(pName);

        if (!lineCmd->update()) return nullptr;
        datumMgr->appendDataObj(lineCmd);

        return lineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionCellWithPlane(QList<int> indexSolid, const QVector<double>& point, const QVector<double>& normal)
    {
        if (point.size() != 3 || normal.size() != 3)return nullptr;
        QList<Interface::VirtualShape> solids = getShapes(indexSolid, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (solids.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionSolidWithPlane>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionSolidWithPlane);
        if (!partitionCmd) return nullptr;
        partitionCmd->setSolids(solids);
        partitionCmd->setPlane(point, normal);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionCellWithExtendFace(QList<int> indexSolid, int faceIndex)
    {
        QList<Interface::VirtualShape> solids = getShapes(indexSolid, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        Interface::VirtualShape extendFace = getShape(faceIndex, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (solids.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionSolidWithExtendFace>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionSolidWithExtendFace);
        if (!partitionCmd) return nullptr;
        partitionCmd->setSolids(solids);
        partitionCmd->setExtendFace(extendFace);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionCellWithSweepEdge(QList<int> indexSolid, QList<int> edgeIndex, const QVector<double>& dir)
    {
        QList<Interface::VirtualShape> solids = getShapes(indexSolid, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        QList<Interface::VirtualShape> edges = getShapes(edgeIndex, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (solids.isEmpty()|| edges.isEmpty())return nullptr;
        if(dir.size()!=3)return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionSolidWithSweepEdge);
        if (!partitionCmd)return nullptr;
        partitionCmd->setSolids(solids);
        partitionCmd->setEdges(edges);
        partitionCmd->setSweepDirection(dir);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionCellWithSweepEdge(QList<int> indexSolid, QList<int> edgeIndex, int sweepIndex)
    {
        QList<Interface::VirtualShape> solids = getShapes(indexSolid, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        QList<Interface::VirtualShape> edges = getShapes(edgeIndex, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::VirtualShape sweepEdge = getShape(sweepIndex, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (solids.isEmpty() || edges.isEmpty()|| sweepEdge.VirtualTopoId<0)return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionSolidWithSweepEdge);
        if (!partitionCmd)return nullptr;
        partitionCmd->setSolids(solids);
        partitionCmd->setEdges(edges);
        partitionCmd->setSweepPath(sweepEdge);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionCellWithSketch(QList<int> indexSolid, int sketchId)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return nullptr;
        Interface::FITKAbsGeoCommand* sketch = cmdList->getDataByID(sketchId);
        if (!sketch) return nullptr;
        QList<Interface::VirtualShape> solids = getShapes(indexSolid, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (solids.isEmpty())return nullptr;
        Interface::VirtualShape shape;
        shape.CmdId = sketchId;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionSolidWithSketchPlanar);
        if (!partitionCmd) return nullptr;

        partitionCmd->setSolids(solids);
        partitionCmd->setSketch(shape);
        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionEdgeWithLocation(QList<int> indexEdge, const QVector<double>& point)
    {
        QList<Interface::VirtualShape> edges = getShapes(indexEdge, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionEdgeWithPoint>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithPoint);
        if (!partitionCmd) return nullptr;
        partitionCmd->setEdges(edges);
        partitionCmd->setPoint(point);

        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionEdgeWithParameter(QList<int> indexEdge, double parameter)
    {
        QList<Interface::VirtualShape> edges = getShapes(indexEdge, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionEdgeWithParameter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithParameter);
        if (!partitionCmd)return nullptr;
        partitionCmd->setEdges(edges);
        partitionCmd->setParameter(parameter);

        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::partitionEdgeWithDatumPlane(QList<int> indexEdge, const QString & planeName)
    {
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoList)return nullptr;
        Interface::FITKDatumList* datumMgr = geoList->getDatumManager();
        if (!datumMgr) return nullptr;
        QList<Interface::VirtualShape> edges = getShapes(indexEdge, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;
        if (!datumMgr->getDataByName(planeName))return nullptr;
        auto plane = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(datumMgr->getDataByName(planeName));
        if (!plane)return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto partitionCmd = fac->createCommandT<Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithDatumPlane);
        if (!partitionCmd)  return nullptr;
        partitionCmd->setEdges(edges);
        partitionCmd->setDatumPlane(plane);

        return partitionCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::virtualTopologyCombineFaces(QList<int> indexs, bool skipSharpAngle, bool combineBounding)
    {
        QList<Interface::VirtualShape> faces = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (faces.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto combineCmd = fac->createCommandT<Interface::FITKAbsGeoVirtualTopoCombineFaces>(Interface::FITKGeoEnum::FITKGeometryComType::FGTVirtualTopoCombineFaces);
        if (!combineCmd) return nullptr;
        combineCmd->setCombineTopos(faces);
        combineCmd->setSkipSharpAngle(skipSharpAngle);
        combineCmd->setCombineBounding(combineBounding);

        return combineCmd;
    }

    Interface::FITKAbsGeoCommand * PyFITKGeoBehavior::virtualTopologyCombineEdges(QList<int> indexs, bool skipSharpAngle)
    {
        QList<Interface::VirtualShape> edges = getShapes(indexs, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (edges.isEmpty())return nullptr;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto combineCmd = fac->createCommandT<Interface::FITKAbsGeoVirtualTopoCombineEdges>(Interface::FITKGeoEnum::FITKGeometryComType::FGTVirtualTopoCombineEdges);
        if (!combineCmd) return nullptr;
        combineCmd->setCombineTopos(edges);
        combineCmd->setSkipSharpAngle(skipSharpAngle);

        return combineCmd;
    }

    bool PyFITKGeoBehavior::removeFeature(const QString & name)
    {
        if (!_geoCmd) return false;

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return false;
        auto partCmdMgr = dynamic_cast<Interface::FITKGeoCommandManager*>(_geoCmd);
        if (!partCmdMgr)return false;
        if(!partCmdMgr->getDataByName(name))return false;

        int objID = partCmdMgr->getDataByName(name)->getDataObjectID();
        const int nbCmds = partCmdMgr->getDataCount();
        int firstIndex = partCmdMgr->getIndexByID(objID);

        for (int i = nbCmds - 1; i >= 0 && i >= firstIndex; --i) {
            int featureId = partCmdMgr->getIDByIndex(i);
            partCmdMgr->removeDataByIndex(i);
            //如果特征使用了草图，则删除草图
            int sketchId = _FeaturesUseSketches.value(featureId);
            if (sketchId > 0) {
                cmdList->removeDataByID(sketchId);
                emit deleteSig(sketchId);
            }
        }
        return true;
    }

    void PyFITKGeoBehavior::setGeoCmd(int geoCmdID)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList) return;
        _geoCmd = cmdList->getDataByID(geoCmdID);
    }

    QList<Interface::VirtualShape> PyFITKGeoBehavior::getShapes(const QList<int>& indexs, Interface::FITKGeoEnum::VTopoShapeType type)
    {
        QList<Interface::VirtualShape> shapes{};
        if (!_geoCmd) return shapes;

        auto vmgr = _geoCmd->getShapeVirtualTopoManager(type);
        if (!vmgr) return shapes;

        for (auto index : indexs) {
            int shapeid = vmgr->getIDByIndex(index);
            if (shapeid < 0)continue;
            Interface::VirtualShape shape = Interface::VirtualShape(_geoCmd->getDataObjectID(), shapeid, index);
            shape.Type = type;
            shapes.append(shape);
        }
        return shapes;
    }

    Interface::VirtualShape PyFITKGeoBehavior::getShape(int indexs, Interface::FITKGeoEnum::VTopoShapeType type)
    {
        QList<int> indexList{ indexs }; 
        QList<Interface::VirtualShape>list = this->getShapes(indexList, type);
        if (list.isEmpty())
            return Interface::VirtualShape();
        return list.first();
    }

}