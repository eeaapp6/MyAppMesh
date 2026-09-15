/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFromSketch.h"

// OCC
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

#include "FITKOCCModelSketch2D.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"

namespace OCC
{
    FITKOCCModelFromSketch::FITKOCCModelFromSketch() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFromSketch::update()
    {
        // 获取部件。
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 获取草图。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        FITKOCCModelSketch2D* sketch2D = dynamic_cast<FITKOCCModelSketch2D*>(cmdList->getDataByID(m_sketchDataID));
        
        if (!part || !sketch2D)
        {
            return false;
        }

        // 获取部件模型。
        FITKAbstractOCCModel* occPart = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occPart)
        {
            return false;
        }

        TopoDS_Shape* partShape = occPart->getShape();
        if (!partShape)
        {
            return false;
        }

        // 合并草图。
        sketch2D->update();

        FITKAbstractOCCModel* occModel = sketch2D->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        TopoDS_Shape* sketchShape = occModel->getShape();
        if (sketchShape->IsNull())
        {
            return false;
        }

        // 创建线框。
        QList<TopoDS_Wire> wiresList;
        FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, 1e-5);

        if (wiresList.isEmpty())
        {
            return false;
        }

        // 合并模型。
        TopoDS_Shape fuseShape = wiresList.takeFirst();

        for (const TopoDS_Wire& wire : wiresList)
        {
            try
            {
                BRepAlgoAPI_Fuse fuse(fuseShape, wire);
                if (fuse.IsDone() && !fuse.Shape().IsNull())
                {
                    fuseShape = fuse.Shape();
                }
            }
            catch (...) {}
        }

        if (part && part->getAutoBool())
        {
            TopoDS_Shape* partShape{ nullptr };
            FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
            if (!partAgent)
            {
                return false;
            }

            partShape = partAgent->getShape();

            TopoDS_Shape result;
            bool flag = FITKOCCComandCommon::MergeShapes(*partShape, fuseShape, result, !part->getAutoMergeSolid());
            if (!flag)
            {
                return false;
            }

            part->updatePartShape(result, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else if (part && part->getAutoBool())
        {
            TopoDS_Shape* partShape = part->getPartShape();
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
            {
                builder.Add(compound, *partShape);
            }

            builder.Add(compound, fuseShape);
            part->updatePartShape(compound, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else
        {
            _occShapeAgent->updateShape(fuseShape);
        }

        return true;
    }
}
