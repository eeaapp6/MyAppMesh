/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelOperImprintSolid.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

//OCC
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pnt.hxx>
#include <BRepLib.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeBuild.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>

#include <BRep_Builder.hxx>
#include <ShapeFix_Shell.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Splitter.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include <QDebug>
namespace OCC
{
    FITKOCCModelOperImprintSolid::FITKOCCModelOperImprintSolid() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelOperImprintSolid::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            OCC::FITKOCCTopoShape* vOCCShapeSolid = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, m_tempVShapes[i].VirtualTopoIndex);
            if (vOCCShapeSolid == nullptr) return false;

            shapes.append(vOCCShapeSolid->getTopoShape());
        }
        return updateImprintSolid(shapes);
    }

    bool FITKOCCModelOperImprintSolid::updateImprintSolid(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() < 2) return false;
        TopTools_ListOfShape shapesw;
        TopTools_ListOfShape shapest;

        for (int i = 0; i < shapes.size(); i++)
        {
            shapesw.Append(shapes[i]);
            shapest.Append(shapes[i]);
        }
        BRepAlgoAPI_Splitter spli;
        spli.SetArguments(shapesw);
        spli.SetTools(shapest);
        spli.Build();

        _occShapeAgent->updateShape(spli.Shape());
        return true;
    }
}