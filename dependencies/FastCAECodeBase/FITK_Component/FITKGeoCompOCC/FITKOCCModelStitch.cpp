/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelStitch.h"

// OCC
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRep_Builder.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_Copy.hxx>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

#include <QDebug>

namespace OCC
{
    // 延伸面分割实体。
    //@{
    FITKOCCModelStitch::FITKOCCModelStitch() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelStitch::update()
    {
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

        BRepBuilderAPI_Sewing sewing;
        sewing.Load(*partShape);
        sewing.SetTolerance(m_Tolerance);

        //if (m_Edges.isEmpty())
        //{
        //    return false;
        //}

        for (const Interface::VirtualShape & edge : m_Edges)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(edge.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            FITKOCCTopoShape* occEdge = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!occEdge)
            {
                return false;
            }

            const TopoDS_Shape & edgeShape = occEdge->getTopoShape();
            sewing.Add(edgeShape);
        }

        TopoDS_Shape resultShape;

        try
        {
            // 执行缝合。
            sewing.Perform();
            resultShape = sewing.SewedShape();

            // 将所有闭合壳升维为实体。
            BRep_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            BRepTools_ReShape reShape;

            QList<QList<TopoDS_Shell>> shellsGroups;
            for (TopExp_Explorer iter(resultShape, TopAbs_ShapeEnum::TopAbs_SHELL); iter.More(); iter.Next())
            {
                const TopoDS_Shell & ss = TopoDS::Shell(iter.Value());
                reShape.Remove(ss);

                //if (!ss.Closed())
                //{
                //    continue;
                //}

                TopoDS_Shell newShell = TopoDS::Shell(BRepBuilderAPI_Copy(ss));

                Bnd_Box box;
                BRepBndLib::Add(ss, box, false);

                // 分组判断包围盒。
                bool breakFlag = false;
                bool inGroup = false;
                for (QList<TopoDS_Shell> & shells : shellsGroups)
                {
                    int index = -1;
                    int nShells = shells.count();
                    for (const TopoDS_Shell & shell : shells)
                    {
                        index++;

                        Bnd_Box boxTemp;
                        BRepBndLib::Add(shell, boxTemp, false);

                        // 比较盒子。
                        bool notThisGroup = false;
                        BoxCompare boxRet = FITKOCCComandCommon::CompareBox(box, boxTemp);
                        switch (boxRet)
                        {
                        case OCC::BC_1_in_2:
                        {
                            if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                            {
                                if (nShells - 1 == index)
                                {
                                    inGroup = true;
                                    breakFlag = true;
                                    shells.push_back(newShell);
                                    break;
                                }
                                else
                                {
                                    continue;
                                    break;
                                }
                            }
                            else
                            {
                                notThisGroup = true;
                                break;
                            }
                        }          
                        case OCC::BC_2_in_1:
                        {
                            if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                            {
                                inGroup = true;
                                breakFlag = true;
                                shells.insert(index, newShell);
                                break;
                            }
                            else
                            {
                                if (index > 0)
                                {
                                    inGroup = true;
                                    breakFlag = true;
                                    shells.insert(index - 1, newShell);
                                }

                                break;
                            }
                        }
                        default:
                        {
                            notThisGroup = true;
                            break;
                        }
                        }

                        // 跳出当前组。
                        if (notThisGroup || breakFlag)
                        {
                            break;
                        }
                    }

                    if (breakFlag)
                    {
                        break;
                    }
                }

                if (!inGroup)
                {
                    shellsGroups.push_back(QList<TopoDS_Shell>{ newShell });
                }
            }

            for (QList<TopoDS_Shell> & shells : shellsGroups)
            {
                QList<TopoDS_Shell> newList = shells;
                BRepBuilderAPI_MakeSolid ms(newList.takeFirst());

                for (const TopoDS_Shell & shell : newList)
                {
                    ms.Add(shell);
                }

                ms.Build();

                bool hasSolid = false;
                if (ms.IsDone())
                {
                    const TopoDS_Solid & solid = ms;
                    if (!solid.IsNull())
                    {
                        builder.Add(compound, solid);
                        hasSolid = true;
                    }
                }

                if (!hasSolid)
                {
                    for (const TopoDS_Shell & s : newList)
                    {
                        builder.Add(compound, s);
                    }
                }
            }

            TopoDS_Shape newShape = reShape.Apply(resultShape);
            if (!newShape.IsNull())
            {
                builder.Add(compound, newShape);
            }     

            resultShape = compound;
        }
        catch (...)
        {
            return false;
        }

        part->updatePartShape(resultShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}
}
