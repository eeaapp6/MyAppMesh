/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPlanarShell.h"

// OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

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
    FITKOCCModelPlanarShell::FITKOCCModelPlanarShell() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPlanarShell::update()
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

        double pos3[3], nor3[3], up3[3];
        sketch2D->getPlane(pos3, nor3, up3);
        gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

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
        QList<EncloseWire> wiresList;
        FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);

        QList<EncloseWire> wiresListFlat;
        FITKOCCComandCommon::FlattenWires(wiresList, wiresListFlat);

        // 创建面。
        QList<TopoDS_Shape> faceList;
        for (EncloseWire & ew : wiresListFlat)
        {
            const TopoDS_Face& fOut = BRepBuilderAPI_MakeFace(ew.Wire);
            if (fOut.IsNull())
            {
                continue;
            }

            if (ew.Children.isEmpty())
            {
                faceList.push_back(fOut);
            }
            else
            {
                TopoDS_Shape cutShape = fOut;
                for (EncloseWire& ewHole : ew.Children)
                {
                    const TopoDS_Face& fInner = BRepBuilderAPI_MakeFace(ewHole.Wire);
                    if (fInner.IsNull())
                    {
                        continue;
                    }

                    const TopoDS_Shape& newShape = BRepAlgoAPI_Cut(cutShape, fInner);
                    if (newShape.IsNull())
                    {
                        continue;
                    }

                    cutShape = newShape;
                }

                faceList.push_back(cutShape);
            }
        }

        if (faceList.isEmpty())
        {
            return false;
        }

        // 合并模型。
        TopoDS_Shape fuseShape = faceList.takeFirst();

        for (const TopoDS_Shape& face : faceList)
        {
            try
            {
                BRepAlgoAPI_Fuse fuse(fuseShape, face);
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
