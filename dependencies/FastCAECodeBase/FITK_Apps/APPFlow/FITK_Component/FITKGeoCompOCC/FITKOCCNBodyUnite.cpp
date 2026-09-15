/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCNBodyUnite.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_Copy.hxx>

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"

namespace OCC
{
    TopoDS_Shape FITKOCCNBodyUnite::removeSolidFromCompsolid(const TopoDS_CompSolid & compsolid, QList<const TopoDS_Solid*> solids)
    {
        TopoDS_Builder builder;
        TopoDS_CompSolid result;
        builder.MakeCompSolid(result);

        for (TopoDS_Iterator iter(compsolid); iter.More(); iter.Next())
        {
            TopoDS_Shape s = iter.Value();
            if (s.ShapeType() != TopAbs_ShapeEnum::TopAbs_SOLID) 
            {
                builder.Add(result, s);
            }
            else
            {
                bool inList = false;
                for (const TopoDS_Solid* iSolid : solids)
                {
                    if (TopoDS::Solid(s).IsSame(*iSolid))
                    {
                        inList = true;
                        break;
                    }
                }

                if (!inList)
                {
                    builder.Add(result, s);
                }
            }
        }

        return result;
    }

    TopoDS_Shape FITKOCCNBodyUnite::removeSolidFromComp(const TopoDS_Shape & comp, QList<const TopoDS_Solid*> solids)
    {
        if (comp.ShapeType() != TopAbs_ShapeEnum::TopAbs_COMPOUND && comp.ShapeType() != TopAbs_ShapeEnum::TopAbs_COMPSOLID) 
            return TopoDS_Shape();

        TopoDS_Builder builder;
        TopoDS_Compound result;
        builder.MakeCompound(result);

        for (TopoDS_Iterator iter(comp); iter.More(); iter.Next()) 
        {
            TopoDS_Shape s = iter.Value();
            if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) 
            {
                builder.Add(result, removeSolidFromComp(TopoDS::Compound(s), solids));
            }
            else if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPSOLID) 
            {
                builder.Add(result, removeSolidFromCompsolid(TopoDS::CompSolid(s), solids));
            }
            else if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_SOLID) 
            {
                bool inList = false;
                for (const TopoDS_Solid* iSolid : solids)
                {
                    if (TopoDS::Solid(s).IsSame(*iSolid))
                    {
                        inList = true;
                        break;
                    }
                }

                if (!inList)
                {
                    builder.Add(result, s);
                }
            }
            else 
            {
                builder.Add(result, s);
            }
        }

        return result;
    }

    FITKOCCNBodyUnite::FITKOCCNBodyUnite() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCNBodyUnite::update()
    {
        TopoDS_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            printLog(tr("Invalid data!"), 3);
            return false;
        }

        if (m_shapes.count() < 2)
        {
            printLog(tr("Please set the solids need to be united!"), 3);
            return false;
        }

        int iSolid = 0;
        //TopTools_ListOfShape solidShapes;
        QList<TopoDS_Solid> solids;
        QHash<int, TopoDS_Shape*> cmdShapeHash;
        QHash<int, QList<const TopoDS_Solid*>> cmdSolidsHash;
        for (const Interface::VirtualShape & vSolid : m_shapes)
        {
            int cmdId = vSolid.CmdId;

            Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(cmdId);
            if (!cmd)
            {
                printLog(tr("Invalid geometry data!"), 3);
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            FITKOCCTopoShape* vShapeSolid = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VSSolid, vSolid.VirtualTopoIndex);
            if (!occModel || !vShapeSolid)
            {
                printLog(tr("Invalid geometry data!"), 3);
                return false;
            }

            // 初始化模型数据。
            TopoDS_Shape* orgShape{ nullptr };
            if (!cmdShapeHash.contains(cmdId))
            {
                TopoDS_Shape* topoShape = occModel->getShape();
                if (!topoShape || topoShape->IsNull())
                {
                    printLog(tr("Invalid geometry data!"), 3);
                    return false;
                }

                // 实体则不追加至组合体合并列表。
                if (topoShape->ShapeType() != TopAbs_ShapeEnum::TopAbs_SOLID)
                {
                    cmdShapeHash.insert(cmdId, topoShape);
                }

                orgShape = topoShape;
            }
            else
            {
                orgShape = cmdShapeHash[cmdId];
            }

            if (!orgShape)
            {
                printLog(tr("Invalid geometry data!"), 3);
                return false;
            }

            // 追加至合并实体列表。
            const TopoDS_Solid & topoSolid = TopoDS::Solid(vShapeSolid->getTopoShape());
            //solidShapes.Append(topoSolid);
            solids.push_back(topoSolid);

            // 分别处理不同类型形状。
            switch (orgShape->ShapeType())
            {
            case TopAbs_ShapeEnum::TopAbs_SOLID:
            {
                // 无需处理。
                break;
            }
            case TopAbs_ShapeEnum::TopAbs_COMPOUND:
            case TopAbs_ShapeEnum::TopAbs_COMPSOLID:
            {
                // 记录要移除的体对象。
                cmdSolidsHash[cmdId].push_back(&topoSolid);
                break;
            }
            default:
                break;
            }

            iSolid++;
        }

        // 批量移除体并组合形状。
        QHashIterator<int, QList<const TopoDS_Solid*>> iter(cmdSolidsHash);
        while (iter.hasNext())
        {
            iter.next();
            QList<const TopoDS_Solid*> solids = iter.value();
            TopoDS_Shape* orgShape = cmdShapeHash.value(iter.key(), nullptr);
            if (!orgShape || solids.isEmpty())
            {
                continue;
            }

            if (orgShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND || 
                orgShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPSOLID)
            {
                builder.Add(comp, removeSolidFromComp(*orgShape, solids));
            }
        }

        TopoDS_Shape ret = BRepAlgoAPI_Fuse(solids.takeFirst(), solids.takeFirst());
        //ret = FITKOCCComandCommon::removeSplitter(ret);
        while (!solids.isEmpty())
        {
            ret = BRepAlgoAPI_Fuse(ret, solids.takeFirst());
            //ret = FITKOCCComandCommon::removeSplitter(ret);
        }

        ret = FITKOCCComandCommon::removeSplitter(ret);

        if (FITKOCCExtendTool::IsEmptyShape(ret))
        {
            printLog(tr("Failed to perform a unite operation!"), 3);
            return false;
        }

        builder.Add(comp, ret);

        //BRepAlgoAPI_Fuse unites;
        //unites.SetTools(solidShapeTool);
        //unites.SetArguments(solidShapes);

        //try
        //{
        //    unites.Build();
        //}
        //catch (...)
        //{
        //    printLog(tr("Failed to perform a unite operation!"), 3);
        //    return false;
        //}

        //const TopoDS_Shape & solids = unites.Shape();  
        //if (FITKOCCExtendTool::IsEmptyShape(solids))
        //{
        //    printLog(tr("Failed to perform a unite operation!"), 3);
        //    return false;
        //}

        //builder.Add(comp, solids);

        _occShapeAgent->updateShape(comp);

        return true;
    }
}