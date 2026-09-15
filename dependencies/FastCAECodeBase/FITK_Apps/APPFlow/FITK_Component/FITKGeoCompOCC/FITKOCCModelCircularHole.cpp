/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelCircularHole.h"

// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <BRep_Tool.hxx>
#include <gp_Lin.hxx>
#include <gp_Pln.hxx>
#include <gp_Trsf.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

// C++
#include <cmath>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace OCC
{
    // 倒角。
    //@{
    FITKOCCModelCircularHole::FITKOCCModelCircularHole() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCircularHole::update()
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

        // 获取包围盒计算通孔深度与边偏移方向策略。
        double min3[3], max3[3];
        bool flag = partShapeAgent->getBoundaryBox(min3, max3);
        if (!flag)
        {
            return false;
        }

        double cent3[3]{ (max3[0] + min3[0]) * 0.5, (max3[1] + min3[1]) * 0.5 , (max3[2] + min3[2]) * 0.5 };
        double depth = sqrt(pow(max3[0] - min3[0], 2) + pow(max3[1] - min3[1], 2) + pow(max3[2] - min3[2], 2));
        if (qFuzzyIsNull(depth))
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 获取参考边与面。
        Interface::FITKAbsGeoCommand* cmdEdge1 = geoCmdList->getDataByID(_geoEdge01.CmdId);
        Interface::FITKAbsGeoCommand* cmdEdge2 = geoCmdList->getDataByID(_geoEdge02.CmdId);
        Interface::FITKAbsGeoCommand* cmdFace = geoCmdList->getDataByID(_geoOperFace.CmdId);
        if (!cmdEdge1 || !cmdEdge2 || !cmdFace)
        {
            return false;
        }

        FITKOCCTopoShape* edgeShape1 = cmdEdge1->getShapeT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _geoEdge01.VirtualTopoId);
        FITKOCCTopoShape* edgeShape2 = cmdEdge2->getShapeT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _geoEdge02.VirtualTopoId);
        FITKOCCTopoShape* faceShape = cmdFace->getShapeT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSFace, _geoOperFace.VirtualTopoId);
        //FITKOCCTopoShape* edgeShape1 = cmdEdge1->getShapeTopoByIndexT<FITKOCCTopoShape>
        //    (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _geoEdge01.VirtualTopoIndex);
        //FITKOCCTopoShape* edgeShape2 = cmdEdge2->getShapeTopoByIndexT<FITKOCCTopoShape>
        //    (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _geoEdge02.VirtualTopoIndex);
        //FITKOCCTopoShape* faceShape = cmdFace->getShapeTopoByIndexT<FITKOCCTopoShape>
        //    (Interface::FITKGeoEnum::VTopoShapeType::VSFace, _geoOperFace.VirtualTopoIndex);
        if (!edgeShape1 || !edgeShape2 || !faceShape)
        {
            return false;
        }

        const TopoDS_Edge & topoEdge1 = TopoDS::Edge(edgeShape1->getTopoShape());
        const TopoDS_Edge & topoEdge2 = TopoDS::Edge(edgeShape2->getTopoShape());
        const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
        if (topoEdge1.IsNull() || topoEdge2.IsNull() || topoFace.IsNull())
        {
            return false;
        }

        // 获取直线与平面。
        Standard_Real t;
        Handle(Geom_Line) cur1 = Handle(Geom_Line)::DownCast(BRep_Tool::Curve(topoEdge1, t, t));
        Handle(Geom_Line) cur2 = Handle(Geom_Line)::DownCast(BRep_Tool::Curve(topoEdge2, t, t));
        Handle(Geom_Plane) surf = Handle(Geom_Plane)::DownCast(BRep_Tool::Surface(topoFace));
        if (cur1.IsNull() || cur2.IsNull() || surf.IsNull())
        {
            return false;
        }

        // 获取模型中心在平面的投影位置。
        GeomAPI_ProjectPointOnSurf projSurf;
        projSurf.Init(gp_Pnt(cent3[0], cent3[1], cent3[2]), surf);

        if (projSurf.NbPoints() == 0)
        {
            return false;
        }

        gp_Pnt ctOnPln = projSurf.Point(1);

        // 将位置投影到直线上。
        GeomAPI_ProjectPointOnCurve projCur1(ctOnPln, cur1);
        GeomAPI_ProjectPointOnCurve projCur2(ctOnPln, cur2);
        if (projCur1.NbPoints() == 0 || projCur2.NbPoints() == 0)
        {
            return false;
        }

        gp_Pnt ctOnCur1 = projCur1.Point(1);
        gp_Pnt ctOnCur2 = projCur2.Point(1);

        // 获取直线方向平面法向。
        const gp_Dir & curDir1 = cur1->Lin().Position().Direction();
        const gp_Dir & curDir2 = cur2->Lin().Position().Direction();
        const gp_Dir & surfNor = surf->Pln().Position().Direction();

        // 获取直线绕平面法向旋转90度的垂直方向。
        gp_Trsf tsf;
        tsf.SetRotation(gp_Ax1(gp_Pnt(), surfNor), 3.1415926 * 0.5);
        gp_Dir curNPDir1 = curDir1.Transformed(tsf);
        gp_Dir curNPDir2 = curDir2.Transformed(tsf);

        // 根据直线法向与线上任意一点与模型中心投影到面上位置的连线夹角判断距离取向策略。
        gp_Dir curCtDir1(ctOnPln.Coord(1) - ctOnCur1.Coord(1), ctOnPln.Coord(2) - ctOnCur1.Coord(2), 
            ctOnPln.Coord(3) - ctOnCur1.Coord(3));
        gp_Dir curCtDir2(ctOnPln.Coord(1) - ctOnCur2.Coord(1), ctOnPln.Coord(2) - ctOnCur2.Coord(2), 
            ctOnPln.Coord(3) - ctOnCur2.Coord(3));

        if (!curNPDir1.IsEqual(curCtDir1, 1e-6))
        {
            curNPDir1.Reverse();
        }

        if (!curNPDir2.IsEqual(curCtDir2, 1e-6))
        {
            curNPDir2.Reverse();
        }

        // 根据处理的方向平移曲线。
        gp_Trsf tsfCur1;
        tsfCur1.SetTranslation(gp_Vec(curNPDir1.Coord(1) * _distanceLocateHole01, 
            curNPDir1.Coord(2) * _distanceLocateHole01, curNPDir1.Coord(3) * _distanceLocateHole01));
        Handle(Geom_Curve) transCur1 = Handle(Geom_Curve)::DownCast(cur1->Transformed(tsfCur1));

        gp_Trsf tsfCur2;
        tsfCur2.SetTranslation(gp_Vec(curNPDir2.Coord(1) * _distanceLocateHole02,
            curNPDir2.Coord(2) * _distanceLocateHole02, curNPDir2.Coord(3) * _distanceLocateHole02));
        Handle(Geom_Curve) transCur2 = Handle(Geom_Curve)::DownCast(cur2->Transformed(tsfCur2));

        if (transCur1.IsNull() || transCur2.IsNull())
        {
            return false;
        }

        // 获取两条直线交点。
        GeomAPI_ExtremaCurveCurve extCC(transCur1, transCur2);
        if (extCC.NbExtrema() == 0)
        {
            return false;
        }

        gp_Pnt pt1, pt2;
        extCC.Points(1, pt1, pt2);

        // 适当移动圆柱底面中心，防止存在容差无法完全穿透模型。
        pt1.SetCoord(1, pt1.Coord(1) + surfNor.Coord(1) * depth * 0.1);
        pt1.SetCoord(2, pt1.Coord(2) + surfNor.Coord(2) * depth * 0.1);
        pt1.SetCoord(3, pt1.Coord(3) + surfNor.Coord(3) * depth * 0.1);

        // 异常处理。
        try
        {
            // 创建圆柱进行切除。
            gp_Ax2 cylAxis;
            cylAxis.SetLocation(pt1);
            cylAxis.SetDirection(- surfNor);

            const TopoDS_Shape & cylinderShape = BRepPrimAPI_MakeCylinder(cylAxis, _diameter * 0.5, depth * 1.2);
            if (cylinderShape.IsNull())
            {
                return false;
            }

            QList<TopoDS_Shape> subShapes;
            FITKOCCComandCommon::FlattenCompShape(*partShape, subShapes);

            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            int nShape = 0;
            TopoDS_Shape result;
            for (const TopoDS_Shape & shape : subShapes)
            {
                TopoDS_Shape cutShape = BRepAlgoAPI_Cut(shape, cylinderShape);
                if (cutShape.IsNull())
                {
                    result = shape;
                }
                else
                {
                    result = cutShape;
                }

                builder.Add(compound, result);
                nShape++;
            }

            if (nShape != 1)
            {
                result = compound;
            }

            // 更新形状。
            if (part)
            {
                part->updatePartShape(result, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(result);
            }
        }
        catch (...)
        {
            printLog(tr("Invalid parameters !"), 3);
            return false;
        }

        return true;
    }
    //@}
}
