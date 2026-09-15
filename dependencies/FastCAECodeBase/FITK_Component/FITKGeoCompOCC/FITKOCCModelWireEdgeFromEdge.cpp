/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelWireEdgeFromEdge.h"

#include "FITKOCCComandCommon.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

#include <BRepBuilderAPI_Copy.hxx>
#include <BRepTools_ReShape.hxx>
#include <TopAbs.hxx>
#include <TopExp.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <ShapeFix_Shape.hxx>

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace OCC
{
    FITKOCCModelWireEdgeFromEdge::FITKOCCModelWireEdgeFromEdge() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelWireEdgeFromEdge::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        TopoDS_Shape* partShape = part->getPartShape();
        if (!partShape || partShape->IsNull())
        {
            return false;
        }

        const QList<Interface::VirtualShape> edgeShapes = edges();
        if (edgeShapes.isEmpty())
        {
            return false;
        }

        TopTools_IndexedMapOfShape wireEdgeMap;
        QList<const TopoDS_Shape*> removeShapes;

        for (const Interface::VirtualShape & edgeShape : edgeShapes)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(edgeShape.CmdId);
            if (!cmd)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vEdge = cmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edgeShape.VirtualTopoIndex);
            if (!vEdge)
            {
                return false;
            }

            FITKOCCTopoShape* occEdgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            if (!occEdgeShape)
            {
                return false;
            }

            const TopoDS_Edge & topoEdge = TopoDS::Edge(occEdgeShape->getTopoShape());
            if (topoEdge.IsNull())
            {
                return false;
            }

            wireEdgeMap.Add(topoEdge);

            Interface::FITKVirtualTopoMapper edgeFaceMapper;
            edgeFaceMapper.mapTopo(vEdge, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            for (int i = 0; i < edgeFaceMapper.length(); i++)
            {
                Interface::FITKAbsVirtualTopo* vFace = edgeFaceMapper.virtualTopo(i);
                if (!vFace)
                {
                    continue;
                }

                FITKOCCTopoShape* occFaceShape = vFace->getShapeT<FITKOCCTopoShape>();
                if (!occFaceShape)
                {
                    return false;
                }

                const TopoDS_Face & topoFace = TopoDS::Face(occFaceShape->getTopoShape());
                if (topoFace.IsNull())
                {
                    return false;
                }

                TopExp::MapShapes(topoFace, TopAbs_EDGE, wireEdgeMap);

                if (!removeShapes.contains(&topoFace))
                {
                    removeShapes.append(&topoFace);
                }
            }
        }

        if (wireEdgeMap.IsEmpty() || removeShapes.isEmpty())
        {
            return false;
        }

        QList<TopoDS_Shape> wireEdges;
        for (int i = 1; i <= wireEdgeMap.Extent(); i++)
        {
            const TopoDS_Edge & edge = TopoDS::Edge(wireEdgeMap(i));
            if (!edge.IsNull())
            {
                wireEdges.append(BRepBuilderAPI_Copy(edge).Shape());
            }
        }

        if (wireEdges.isEmpty())
        {
            return false;
        }

        try
        {
            BRepTools_ReShape reShape;
            for (const TopoDS_Shape* removeShape : removeShapes)
            {
                if (removeShape)
                {
                    reShape.Remove(*removeShape);
                }
            }

            TopoDS_Shape remainShape = reShape.Apply(*partShape);

            ShapeFix_Shape fixer(remainShape);
            fixer.Perform();
            remainShape = fixer.Shape();

            TopoDS_Shape result;
            if (!remainShape.IsNull())
            {
                wireEdges.push_back(remainShape);
            }

            if (!FITKOCCComandCommon::MergeShapes(wireEdges, result))
            {
                return false;
            }

            part->updatePartShape(result, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        catch (...)
        {
            return false;
        }

        return true;
    }
}
