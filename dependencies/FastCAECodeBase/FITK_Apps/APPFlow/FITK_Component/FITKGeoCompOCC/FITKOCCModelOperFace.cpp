/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelOperFace.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKShapeFix_FixSmallFace.h"
#include "FITKOCCComandCommon.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// OCC
#include <BRepTools_ReShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pnt.hxx>
#include <BRepLib.hxx>
#include <BRepBndLib.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeBuild.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeBuild_ReShape.hxx>

#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>
#include <ShapeFix_Shell.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <GeomLib.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include <cmath>

#include <QDebug>

namespace OCC
{
    FITKOCCModelOperFace::FITKOCCModelOperFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelOperFace::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i].VirtualTopoId);
            if (vOCCShapeEdge == nullptr) return false;

            shapes.append(vOCCShapeEdge->getTopoShape());
        }

        //按类型处理
        if (_faceOperType == Interface::FITKAbsGeoModelOperFace::GBTFillHoles)
        {
            return updateFillHoles(shapes);
        }
        else if (_faceOperType == Interface::FITKAbsGeoModelOperFace::GBTImprintMapPointToFace)
        {
            return updateImprintMapPointToFace(shapes);
        }
        else if (_faceOperType == Interface::FITKAbsGeoModelOperFace::GBTImprintMapLineToFace)
        {
            return updateImprintMapLineToFace(shapes);
        }
        else if (_faceOperType == Interface::FITKAbsGeoModelOperFace::GBTImprintMapCircleToFace)
        {
            return updateImprintMapCircleToFace(shapes);
        }
        return false;
    }

    bool FITKOCCModelOperFace::updateFillHoles(QList<TopoDS_Shape> shapes)
    {
        TopoDS_Face face;
        QList<TopoDS_Edge> holes;

        //数据筛选
        for (int i = 0; i < shapes.size(); ++i)
        {
            if (shapes[i].ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE && face.IsNull())
                face = TopoDS::Face(shapes[i]);
            else if (shapes[i].ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
                holes.append(TopoDS::Edge(shapes[i]));
            else
                return false;
        }
        if (face.IsNull() || holes.size() < 1) return false;

        BRepFill_Filling fill;
        fill.LoadInitSurface(face);
        for (int i = 0; i < holes.size(); ++i)
        {
            fill.Add(holes[i], GeomAbs_Shape::GeomAbs_C0);
        }
        fill.Build();
        auto topoDsShape = fill.Face();
        TopoDS_Compound aComp;
        BRep_Builder BuildTool;
        BuildTool.MakeCompound(aComp);
        BuildTool.Add(aComp, face);
        BuildTool.Add(aComp, topoDsShape);
        //缝合
        BRepOffsetAPI_Sewing sewing;
        //缝合初始化
        sewing.Init(_tolerance, Standard_True);

        sewing.Load(aComp);
        sewing.Perform();
        //检查是否修改成功
        if (!sewing.IsModified(sewing.SewedShape())) return false;
        //消除缝合线
        ShapeUpgrade_UnifySameDomain unif(sewing.SewedShape(), true, true, false);
        unif.Build();
        _occShapeAgent->updateShape(unif.Shape());
        return true;
    }

    bool FITKOCCModelOperFace::updateImprintMapPointToFace(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() != 2) return false;
        TopoDS_Face face;
        TopoDS_Vertex point;

        if (shapes[0].ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE)
        {
            face = TopoDS::Face(shapes[0]);
            point = TopoDS::Vertex(shapes[1]);
        }
        else
        {
            point = TopoDS::Vertex(shapes[0]);
            face = TopoDS::Face(shapes[1]);
        }

        if (point.IsNull() || face.IsNull()) return false;




        return false;
    }

    bool FITKOCCModelOperFace::updateImprintMapLineToFace(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() != 2) return false;

        return false;
    }

    bool FITKOCCModelOperFace::updateImprintMapCircleToFace(QList<TopoDS_Shape> shapes)
    {
        return false;
    }

    ///////////////FITKOCCModelOperFaceFillHoles///////////////////////////

    FITKOCCModelOperFaceFillHoles::FITKOCCModelOperFaceFillHoles() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOperFaceFillHoles::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i].VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;

            shapes.append(vOCCShapeEdge->getTopoShape());
        }

        return updateFillHoles(shapes);
    }
    bool FITKOCCModelOperFaceFillHoles::updateFillHoles(QList<TopoDS_Shape> shapes)
    {
        TopoDS_Face face;
        QList<TopoDS_Edge> holes;

        //数据筛选
        for (int i = 0; i < shapes.size(); ++i)
        {
            if (shapes[i].ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE && face.IsNull())
                face = TopoDS::Face(shapes[i]);
            else if (shapes[i].ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
                holes.append(TopoDS::Edge(shapes[i]));
            else
                return false;
        }
        if (face.IsNull() || holes.size() < 1) return false;

        BRepFill_Filling fill;
        fill.LoadInitSurface(face);
        for (int i = 0; i < holes.size(); ++i)
        {
            fill.Add(holes[i], GeomAbs_Shape::GeomAbs_C0);
        }
        fill.Build();
        auto topoDsShape = fill.Face();
        TopoDS_Compound aComp;
        BRep_Builder BuildTool;
        BuildTool.MakeCompound(aComp);
        BuildTool.Add(aComp, face);
        BuildTool.Add(aComp, topoDsShape);
        //缝合
        BRepOffsetAPI_Sewing sewing;
        //缝合初始化
        sewing.Init(_tolerance, Standard_True);

        sewing.Load(aComp);
        sewing.Perform();
        //检查是否修改成功
        if (!sewing.IsModified(sewing.SewedShape())) return false;
        //消除缝合线
        ShapeUpgrade_UnifySameDomain unif(sewing.SewedShape(), true, true, false);
        unif.Build();
        _occShapeAgent->updateShape(unif.Shape());
        return true;
    }

    ///////////////FITKOCCModelOperFaceFillGaps///////////////////////////

    FITKOCCModelOperFaceFillGaps::FITKOCCModelOperFaceFillGaps() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOperFaceFillGaps::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i].VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;

            shapes.append(vOCCShapeEdge->getTopoShape());
        }

        return updateFillGaps(shapes);
    }

    bool FITKOCCModelOperFaceFillGaps::updateFillGaps(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() < 1) return false;
        //主面
        TopoDS_Face mFace;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            // 向上查找所选边对应体数据。
            auto vTopoManager = comm->getVirtualTopoManager()->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);

            // 获取查询结果。
            for (int i = 0; i < vTopoManager->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vFace = vTopoManager->getDataByIndex(i);
                auto occShape = vFace->getShapeT<OCC::FITKOCCTopoShape>();
                if (occShape == nullptr) return false;
                mFace = TopoDS::Face(occShape->getTopoShape());
            }
        }
        //主面判断是否为空（填充面）
        if (mFace.IsNull() || mFace.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE)
        {
            printLog(tr("data retrieval error."), 3);
            return false;
        }

        bool isSuccessful = false;
        auto tempWireShape = CheckAndForceCurveClosure(shapes, isSuccessful);
        if (!isSuccessful || tempWireShape.IsNull()) return false;
        TopoDS_Wire closedCurve = TopoDS::Wire(tempWireShape);
        if (closedCurve.IsNull()) return false;
        TopoDS_Face nFace = BRepBuilderAPI_MakeFace(closedCurve);

        //TopoDS_Compound aComp;
        //BRep_Builder BuildTool;
        //BuildTool.MakeCompound(aComp);
        //BuildTool.Add(aComp, mFace);
        //BuildTool.Add(aComp, nFace);

        ////使用 BRepBuilderAPI_Sewing 缝合
        BRepOffsetAPI_Sewing sewing;
        //sewing.Init(1, Standard_True);
        //sewing.Load(aComp);
        sewing.Add(mFace);
        sewing.Add(nFace);
        sewing.Perform();
        auto sewingFace = sewing.SewedShape();
        //检查是否修改成功
        if (sewing.IsModified(sewingFace)) return false;
        //消除缝合线 统一拓扑
        ShapeUpgrade_UnifySameDomain unif(sewingFace, false, true, false);
        unif.Build();
        _occShapeAgent->updateShape(unif.Shape());
        return true;

        //TopTools_ListOfShape aLSObjects; //进行连接的形状
        //aLSObjects.Append(shape1);
        //aLSObjects.Append(shape2);

        //BOPAlgo_Builder aBuilder;
        //aBuilder.SetArguments(aLSObjects);
        ////设置GF的选项
        ////设置并行处理模式（默认为false）
        //Standard_Boolean bRunParallel = Standard_True;
        //aBuilder.SetRunParallel(bRunParallel);
        ////设置模糊值（默认为Precision :: Confusion（））  
        //Standard_Real aFuzzyValue = 1e-5;
        //aBuilder.SetFuzzyValue(aFuzzyValue);
        ////设置安全处理模式（默认为false）
        //Standard_Boolean bSafeMode = Standard_True;
        //aBuilder.SetNonDestructive(bSafeMode);
        ////为重合的参数设置粘合模式（默认为关闭）
        //BOPAlgo_GlueEnum aGlue = BOPAlgo_GlueShift;
        //aBuilder.SetGlue(aGlue);
        ////禁用/启用对倒置固体的检查（默认为true）
        //Standard_Boolean bCheckInverted = Standard_False;
        //aBuilder.SetCheckInverted(bCheckInverted);
        ////设置OBB用法（默认为false）
        //Standard_Boolean bUseOBB = Standard_True;
        //aBuilder.SetUseOBB(bUseOBB);
        ////执行操作 
        //aBuilder.Perform();
        ////检查错误
        //if (aBuilder.HasErrors()) return TopoDS_Shape();
        //if (aBuilder.HasWarnings()) return TopoDS_Shape();
        //// result of the operation
        //return aBuilder.Shape();
    }

    TopoDS_Shape FITKOCCModelOperFaceFillGaps::CheckAndForceCurveClosure(QList<TopoDS_Shape> shapes, bool& isSuccessful)
    {
        isSuccessful = false;
        BRepBuilderAPI_MakeWire mergeWires;
        TopTools_ListOfShape edgeList;
        for (int i = 0; i < shapes.size(); i++)
        {
            //if (shapes[i].ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return TopoDS_Shape;
            qDebug() << "shape Type : " << shapes[i].ShapeType();
            TopoDS_Edge edge = TopoDS::Edge(shapes[i]);
            if (edge.IsNull()) return TopoDS_Shape();
            edgeList.Append(edge);
        }
        mergeWires.Add(edgeList);
        mergeWires.Build();
        if (!mergeWires.IsDone())
        {
            printLog(tr("parameter check error."), 3);
            return TopoDS_Shape();
        }
        isSuccessful = true;
        return mergeWires.Shape();
    }

    ///////////////FITKOCCModelOperFaceDeleteFloatingEdge///////////////////////////

    FITKOCCModelOperFaceDeleteFloatingEdge::FITKOCCModelOperFaceDeleteFloatingEdge() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOperFaceDeleteFloatingEdge::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            if (m_tempVShapes[i].CmdId == 0) continue;
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr)
            {
                printLog(tr("data retrieval error."), 3);
                return false;
            }
            //OCC::FITKOCCTopoShape* vOCCShapeFace = comm->getShapeT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_tempVShapes[i].VirtualTopoId);

            auto model = comm->getTShapeAgent<FITKAbstractOCCModel>();

            TopoDS_Compound aComp;
            BRep_Builder BuildTool;
            BuildTool.MakeCompound(aComp);

            auto sVManager = comm->getVirtualTopoManager()->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
            for (int i = 0; i < sVManager->getDataCount(); i++)
            {
                bool isReusePoint = false;
                auto vObject = sVManager->getDataByIndex(i);
                int subCount = vObject->getSubTopoCount();
                for (int j = 0; j < subCount; j++)
                {
                    auto childVTopo = vObject->getSubTopo(j);
                    int parentVTopoCount = childVTopo->getParentTopoCount();
                    isReusePoint = parentVTopoCount == 1;
                }
                if (isReusePoint) BuildTool.Add(aComp, *vObject->getShapeT<FITKAbstractOCCModel>()->getShape());
            }

            if (aComp.IsNull()) return false;
            BRepTools_ReShape reShape;
            reShape.Remove(aComp);
            auto rShape = reShape.Apply(*model->getShape());
            _occShapeAgent->updateShape(rShape);
            return true;
        }
        return false;
    }

    bool FITKOCCModelOperFaceDeleteFloatingEdge::updateDeleteFloatingEdge(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() < 1 || shapes[0].IsNull())
        {
            printLog(tr("parameter error."), 3);
            return false;
        }
        BRepTools_ReShape reShape;
        reShape.Remove(shapes[0]);

        //消除缝合线 统一拓扑
        ShapeUpgrade_UnifySameDomain unif(shapes[0], true, true, false);
        TopTools_MapOfShape shapeList;
        for (int i = 1; i < shapes.size(); i++)
        {
            shapeList.Add(shapes[i]);
        }
        unif.KeepShapes(shapeList);
        unif.Build();

        _occShapeAgent->updateShape(unif.Shape());
        return true;
    }

    // 移除面。
    //@{
    FITKOCCModelRemoveFace::FITKOCCModelRemoveFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRemoveFace::update()
    {
        QList<TopoDS_Shape*> fullShapes;
        QList<const TopoDS_Face*> faces;
        for (const Interface::VirtualShape & shape : m_tempVShapes)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(shape.CmdId);
            if (!cmd)
            {
                return false;
            }

            TopoDS_Shape* topoShape{ nullptr };
            OCC::FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FITKAbstractOCCModel>();
            if (occModel)
            {
                topoShape = occModel->getShape();
            }

            OCC::FITKOCCTopoShape* vOCCShapeFace = cmd->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSFace, shape.VirtualTopoIndex);
            if (!vOCCShapeFace)
            {
                continue;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(vOCCShapeFace->getTopoShape());
            if (topoFace.IsNull())
            {
                continue;
            }

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }
            
            faces.push_back(&topoFace);
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 移除面。
        //@{
        try
        {
            Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
            for (int i = 0; i < faces.count(); i++)
            {
                const TopoDS_Face* face = faces[i];
                reShape->Remove(*face);
            }

            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            for (TopoDS_Shape* fullShape : fullShapes)
            {
                // TopAbs_ShapeEnum sType = fullShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, newShape);
            }   

            if (part)
            {
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(compound);
            }
        }
        catch (...)
        {
            return false;
        }
        //@}

        return true;
    }
    //@}

    // 修复小面类。
    //@{
    FITKOCCModelRepairSmallFaces::FITKOCCModelRepairSmallFaces() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRepairSmallFaces::update()
    {
        QHash<TopoDS_Shape*, QList<const TopoDS_Face*>> fullShapeFacesHash;
        for (const Interface::VirtualShape & shape : m_Faces)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(shape.CmdId);
            if (!cmd)
            {
                return false;
            }

            TopoDS_Shape* topoShape{ nullptr };
            OCC::FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FITKAbstractOCCModel>();
            if (occModel)
            {
                topoShape = occModel->getShape();
            }

            OCC::FITKOCCTopoShape* vOCCShapeFace = cmd->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSFace, shape.VirtualTopoIndex);
            if (!vOCCShapeFace)
            {
                continue;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(vOCCShapeFace->getTopoShape());
            if (topoFace.IsNull())
            {
                continue;
            }

            fullShapeFacesHash[topoShape].push_back(&topoFace);
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 移除小面。
        //@{
        try
        {
            ////Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

            ShapeAnalysis_CheckSmallFace checkSmall;
            
            int nSmallFacesTotal = 0;
            //QList<const TopoDS_Shape*> fixedShapes;
            QList<TopoDS_Shape> fixedShapes;
            QHashIterator<TopoDS_Shape*, QList<const TopoDS_Face*>> iter(fullShapeFacesHash);
            while (iter.hasNext())
            {
                iter.next();

                double tol = 3;

                Bnd_Box box;
                BRepBndLib::Add(*iter.key(), box, false);
                tol = sqrt(box.SquareExtent()) * 0.05;

                checkSmall.SetTolerance(tol);

                FITKShapeFix_FixSmallFace shapeFix;
                shapeFix.Init(*iter.key());
                shapeFix.SetPrecision(tol);

                int nSmallFaces = 0;
                for (const TopoDS_Face* face : iter.value())
                {
                    bool isSpot = checkSmall.CheckSpotFace(*face, tol);
                    bool isSmall = isSpot;

                    TopoDS_Edge e1, e2;
                    //isSmall |= checkSmall.CheckSingleStrip(*face, e1, e2, tol);
                    bool isStrip = checkSmall.CheckStripFace(*face, e1, e2, tol);
                    isSmall |= isStrip;

                    //TopTools_DataMapOfShapeListOfShape mes;
                    //ShapeAnalysis_DataMapOfShapeListOfReal mp;
                    //TopoDS_Compound vs;
                    //isSmall |= checkSmall.CheckSplittingVertices(*face, mes, mp, vs);

                    //Standard_Real u, v;
                    //isSmall |= checkSmall.CheckTwisted(*face, u, v);
                    //
                    //TopTools_DataMapOfShapeShape es;
                    //isSmall |= checkSmall.CheckPinFace(*face, es, tol);

                    bool flag = false;
                    if (isSpot)
                    {
                        flag = shapeFix.FixSpotFace(*face);
                    }

                    if (isStrip)
                    {
                        flag = shapeFix.FixStripFace(*face);
                    }

                    if (isSmall && flag)
                    {
                        nSmallFaces++;
                        nSmallFacesTotal++;
                    }
                }

                if (nSmallFaces == 0)
                {
                    fixedShapes.push_back(*iter.key());
                }
                else
                {
                    shapeFix.Update();
                    TopoDS_Shape newShape = shapeFix.Shape();
                    if (newShape.IsNull())
                    {
                        fixedShapes.push_back(*iter.key());
                    }
                    else
                    {
                        fixedShapes.push_back(newShape);
                    }
                }
            }

            if (nSmallFacesTotal == 0)
            {
                return false;
            }
            
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            //for (const TopoDS_Shape* fixedShape : fixedShapes)
            for (const TopoDS_Shape & fixedShape : fixedShapes)
            {
                //builder.Add(compound, *fixedShape);
                builder.Add(compound, fixedShape);
            }

            if (part)
            {
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(compound);
            }
        }
        catch (...)
        {
            return false;
        }
        //@}

        return true;
    }
    //@}

    // 延伸面类。
    //@{
    FITKOCCModelExtendFace::FITKOCCModelExtendFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelExtendFace::update()
    {
        QList<Interface::VirtualShape> vShapeEdges;
        double tempDistance = 0;

        if (_selectedMode.first == Interface::FITKAbsGeoModelOperFaceExtendFace::SpecifyModel::SpecifyEdge)
        {
            vShapeEdges = getEdges();
        }
        else if (_selectedMode.first == Interface::FITKAbsGeoModelOperFaceExtendFace::SpecifyModel::SpecifyFace)
        {
            QList<Interface::VirtualShape> faces = getFaces();

            for (int i = 0; i < faces.count(); i++)
            {
                Interface::FITKAbsVirtualTopo* vFaceTopo = getRefModelVirtualTopo(Interface::FITKGeoEnum::VTopoShapeType::VSFace, faces[i].CmdId, faces[i].VirtualTopoId);
                int subTopoCount = vFaceTopo->getSubTopoCount();
                for (int j = 0; j < subTopoCount; j++)
                {
                    Interface::FITKAbsVirtualTopo* vEdgeTopo = vFaceTopo->getSubTopo(j);
                    Interface::VirtualShape vShape(faces[i].CmdId, vEdgeTopo->getDataObjectID(), vEdgeTopo->getIndexLabel());
                    vShapeEdges.append(vShape);
                }
            }
        }

        if (_selectedMode.second == Interface::FITKAbsGeoModelOperFaceExtendFace::MethodModel::MethodDistance)
        {
            tempDistance = _distance;
        }
        else if (_selectedMode.second == Interface::FITKAbsGeoModelOperFaceExtendFace::MethodModel::MethodFace)
        {
            //计算 vShapeEdges 与 tagFace 的距离。
            //tempDistance = getFaces();
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        //// 延伸面并替换。
        ////@{
        //try
        //{
        //    BRepFill_Filling nSideFill;
        //    for (const Interface::VirtualShape & vShapeEdge : vShapeEdges)
        //    {

        //    }

        //    if (part)
        //    {
        //        part->updatePartShape(, false);
        //        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        //    }
        //    else
        //    {
        //        _occShapeAgent->updateShape();
        //    }
        //}
        //catch (...)
        //{
        //    return false;
        //}
        ////@}

        return true;
    }
    //@}

    // 替换面类。
    //@{
    FITKOCCModelReplaceFaces::FITKOCCModelReplaceFaces() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelReplaceFaces::update()
    {
        if (m_Faces.isEmpty())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 检查面连接性。
        QList<Interface::FITKAbsVirtualTopo*> vFaces;

        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vFace = faceCmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!vFace)
            {
                return false;
            }

            if (!vFaces.contains(vFace))
            {
                vFaces.push_back(vFace);
            }
        }

        if (vFaces.isEmpty())
        {
            return false;
        }   

        // 寻找共边。
        QHash<Interface::FITKAbsVirtualTopo*, int> vEdgesHash;
        QList<Interface::FITKAbsVirtualTopo*> vEdgesSides;
        QList<Interface::FITKAbsVirtualTopo*> vFacesHasCommon;
        Interface::FITKVirtualTopoMapper vMapperFE;
        Interface::FITKVirtualTopoMapper vMapperFEOther;

        int nFaces = vFaces.count();;
        for (int i = 0 ; i < nFaces; i ++)
        {
            Interface::FITKAbsVirtualTopo* vFace = vFaces[i];
            vMapperFE.mapTopo(vFace, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
            int nFaceEdges = vMapperFE.length();
            if (nFaceEdges == 0)
            {
                continue;
            }

            QList<Interface::FITKAbsVirtualTopo*> vEdges;
            for (int j = 0; j < nFaceEdges; j++)
            {
                Interface::FITKAbsVirtualTopo* vEdge = vMapperFE.virtualTopo(j);
                if (vEdge)
                {
                    vEdgesHash.insert(vEdge, vEdgesHash.value(vEdge, 0) + 1);
                    vEdges.push_back(vEdge);
                }
            }

            bool hasCommonEdges = false;
            for (int j = i; j < nFaces; j++)
            {
                Interface::FITKAbsVirtualTopo* vFaceOther = vFaces[j];
                if (vFaceOther == vFace)
                {
                    continue;
                }

                vMapperFEOther.mapTopo(vFaceOther, Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
                int nFaceEdgesOther = vMapperFEOther.length();
                if (nFaceEdgesOther == 0)
                {
                    continue;
                }

                for (int k = 0; k < nFaceEdgesOther; k++)
                {
                    Interface::FITKAbsVirtualTopo* vEdgeOther = vMapperFEOther.virtualTopo(k);
                    if (!vEdgeOther)
                    {
                        continue;
                    }

                    // 共边。
                    if (vEdges.contains(vEdgeOther))
                    {
                        if (!vFacesHasCommon.contains(vFaceOther))
                        {
                            vFacesHasCommon.push_back(vFaceOther);
                        }
                        
                        hasCommonEdges = true;
                    }
                }
            }

            if (hasCommonEdges)
            {
                if (!vFacesHasCommon.contains(vFace))
                {
                    vFacesHasCommon.push_back(vFace);
                }
            }
        }

        // 提取非共边。
        QHashIterator<Interface::FITKAbsVirtualTopo*, int> iter(vEdgesHash);
        while (iter.hasNext())
        {
            iter.next();

            if (iter.value() == 1)
            {
                vEdgesSides.push_back(iter.key());
            }
        }

        // 部分面不相连。
        if (vFacesHasCommon.count() != vFaces.count())
        {
            return false;
        }

        Interface::FITKVirtualTopoMapper vMapperFS;
        Interface::FITKVirtualTopoMapper vMapperFW;
        QList<Interface::FITKAbsVirtualTopo*> shells;
        QHash<Interface::FITKAbsVirtualTopo*, int> shellNumHash;
        for (Interface::FITKAbsVirtualTopo* vFace : vFaces)
        {
            // 判断面是否仅包含一个闭合线框。
            vMapperFW.mapTopo(vFace, Interface::FITKGeoEnum::VTopoShapeType::VSWire);
            int nFaceWires = vMapperFW.length();
            if (nFaceWires != 1)
            {
                return false;
            }

            // 检查面是否在同一个壳内。
            vMapperFS.mapTopo(vFace, Interface::FITKGeoEnum::VTopoShapeType::VSShell);
            int nFaceShells = vMapperFS.length();
            if (nFaceShells == 0)
            {
                return false;
            }

            QList<Interface::FITKAbsVirtualTopo*> vEdges;
            for (int i = 0; i < nFaceShells; i++)
            {
                Interface::FITKAbsVirtualTopo* vShell = vMapperFS.virtualTopo(i);
                if (vShell)
                {
                    shellNumHash.insert(vShell, shellNumHash.value(vShell, 0) + 1);

                    if (!shells.contains(vShell))
                    {
                        shells.push_back(vShell);
                    }
                }
            }
        }

        // 不在同壳内则跳出。
        if (shells.count() != 1)
        {
            return false;
        }

        // 壳被计数次数与面数量匹配。
        Interface::FITKAbsVirtualTopo* vShell = shellNumHash.key(vFacesHasCommon.count(), nullptr);
        if (!vShell)
        {
            return false;
        }

        FITKOCCTopoShape* occShapeShell = vShell->getShapeT<FITKOCCTopoShape>();
        if (!occShapeShell)
        {
            return false;
        }

        const TopoDS_Shape & shapeShell = occShapeShell->getTopoShape();
        if (shapeShell.IsNull())
        {
            return false;
        }

        // 延伸面并替换。
        //@{
        try
        {
            BRep_Builder builder;
            TopoDS_Compound compoundEdges;
            builder.MakeCompound(compoundEdges);

            // 创建闭合曲面。
            BRepFill_Filling nSideFill;
            for (Interface::FITKAbsVirtualTopo* vEdge : vEdgesSides)
            {
                FITKOCCTopoShape* occShape = vEdge->getShapeT<FITKOCCTopoShape>();
                if (!occShape)
                {
                    return false;
                }

                const TopoDS_Shape & shape = occShape->getTopoShape();
                const TopoDS_Edge & edge = TopoDS::Edge(shape);
                if (edge.IsNull())
                {
                    return false;
                }

                builder.Add(compoundEdges, edge);
                nSideFill.Add(edge, GeomAbs_Shape::GeomAbs_C0, true);
            }

            // 重组。
            QList<TopoDS_Wire> wires;
            FITKOCCComandCommon::MakeWires(compoundEdges, wires, 1e-5);

            // 如果产生嵌套多段线则跳出。
            if (wires.count() != 1)
            {
                return false;
            }

            //// 根据线框周长排序。
            //QList<TopoDS_Wire> wiresSort;
            //for (const TopoDS_Wire & wireUnsort : wires)
            //{
            //    GProp_GProps props;
            //    BRepGProp::LinearProperties(wireUnsort, props);
            //    double len = props.Mass();

            //    bool hasInstered = false;
            //    for (int i = 0 ; i < wiresSort.count() ; i ++)
            //    {
            //        const TopoDS_Wire & wireSorted = wiresSort[i];

            //        GProp_GProps propsOther;
            //        BRepGProp::LinearProperties(wireSorted, propsOther);
            //        double lenOther = propsOther.Mass();

            //        if (len > lenOther)
            //        {
            //            hasInstered = true;
            //            wiresSort.insert(i, wireUnsort);
            //            break;
            //        }
            //    }

            //    if (!hasInstered)
            //    {
            //        wiresSort.push_back(wireUnsort);
            //    }
            //}

            //if (wiresSort.isEmpty())
            //{
            //    return false;
            //}

            //BRepBuilderAPI_MakeFace mkFace(wiresSort.takeFirst());
            //for (const TopoDS_Wire & wire : wiresSort)
            //{
            //    mkFace.Add(wire);
            //}

            nSideFill.Build();

            if (!nSideFill.IsDone())
            {
                return false;
            }

            //mkFace.Build();

            //if (!mkFace.IsDone())
            //{
            //    return false;
            //}

            const TopoDS_Face & newFace = nSideFill.Face();
            //const TopoDS_Face & newFace = mkFace.Face();
            if (newFace.IsNull())
            {
                return false;
            }

            // 移除全部相连面。
            BRepTools_ReShape reShape;
            for (Interface::FITKAbsVirtualTopo* vFace : vFaces)
            {
                FITKOCCTopoShape* occShape = vFace->getShapeT<FITKOCCTopoShape>();
                if (!occShape)
                {
                    return false;
                }

                const TopoDS_Shape & shape = occShape->getTopoShape();
                if (shape.IsNull())
                {
                    return false;
                }

                reShape.Remove(shape);
            }

            TopoDS_Shape newShellRemoved = reShape.Apply(shapeShell);

            // 组合壳。
            BRepBuilderAPI_Sewing sewer;
            sewer.Add(newFace);
            for (TopExp_Explorer iter(newShellRemoved, TopAbs_ShapeEnum::TopAbs_FACE); iter.More(); iter.Next())
            {
                const TopoDS_Shape & sf = iter.Value();
                sewer.Add(sf);
            }

            sewer.Perform();

            //BRep_Builder builder;
            //TopoDS_Shell finalShell;
            //builder.MakeShell(finalShell);
            //builder.Add(finalShell, newFace);
            //for (TopExp_Explorer iter(newShellRemoved, TopAbs_ShapeEnum::TopAbs_FACE); iter.More(); iter.Next())
            //{
            //    const TopoDS_Shape & sf = iter.Value();
            //    builder.Add(finalShell, sf);
            //}

            // 替换壳。
            reShape.Replace(shapeShell, sewer.SewedShape());

            TopoDS_Shape newShape = reShape.Apply(*partShape);

            part->updatePartShape(newShape, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        catch (...)
        {
            return false;
        }
        //@}

        return true;
    }
    //@}
}