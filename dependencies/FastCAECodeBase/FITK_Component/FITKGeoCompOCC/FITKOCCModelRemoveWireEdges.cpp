/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelRemoveWireEdges.h"

// Geometry
#include "FITKOCCModelPart.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

// OCC
#include <BRepTools_ReShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace OCC
{
    FITKOCCModelRemoveWireEdges::FITKOCCModelRemoveWireEdges() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRemoveWireEdges::update()
    {
        QHash<TopoDS_Shape*, QList<const TopoDS_Edge*>> fullShapeEdgesHash;

        int invalidEdgeCount = 0;
        int nonWireEdgeCount = 0;

        for (const Interface::VirtualShape & shape : m_tempVShapes)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(shape.CmdId);
            if (!cmd)
            {
                invalidEdgeCount++;
                continue;
            }

            TopoDS_Shape* topoShape{ nullptr };
            OCC::FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FITKAbstractOCCModel>();
            if (occModel)
            {
                topoShape = occModel->getShape();
            }
            if (!topoShape)
            {
                invalidEdgeCount++;
                continue;
            }

            Interface::FITKAbsVirtualTopo* vEdge = cmd->getVirtualTopoByIndex(Interface::FITKGeoEnum::VSEdge, shape.VirtualTopoIndex);
            if (!vEdge)
            {
                invalidEdgeCount++;
                continue;
            }

            // 非线边（属于面的边）跳过并报错。
            Interface::FITKVirtualTopoMapper vMapperEF;
            vMapperEF.mapTopo(vEdge, Interface::FITKGeoEnum::VSFace);
            if (vMapperEF.length() > 0)
            {
                nonWireEdgeCount++;
                continue;
            }

            OCC::FITKOCCTopoShape* vOCCShapeEdge = vEdge->getShapeT<OCC::FITKOCCTopoShape>();
            if (!vOCCShapeEdge)
            {
                invalidEdgeCount++;
                continue;
            }

            const TopoDS_Edge & topoEdge = TopoDS::Edge(vOCCShapeEdge->getTopoShape());
            if (topoEdge.IsNull())
            {
                invalidEdgeCount++;
                continue;
            }

            fullShapeEdgesHash[topoShape].push_back(&topoEdge);
        }

        if (fullShapeEdgesHash.isEmpty())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            QHashIterator<TopoDS_Shape*, QList<const TopoDS_Edge*>> iter(fullShapeEdgesHash);
            while (iter.hasNext())
            {
                iter.next();

                TopoDS_Shape* fullShape = iter.key();
                if (!fullShape)
                {
                    continue;
                }

                Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
                for (const TopoDS_Edge* edge : iter.value())
                {
                    if (edge)
                    {
                        reShape->Remove(*edge);
                    }
                }

                TopoDS_Shape newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, newShape);
            }

            if (compound.IsNull())
            {
                return false;
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

        return true;
    }
}
