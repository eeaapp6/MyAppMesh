/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCCommons.h"

// OCC
#include <Quantity_Color.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <MeshVS_DataMapOfIntegerColor.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <BRepGProp_Face.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomLib_Tool.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <V3d_View.hxx>
#include <GeomAPI_IntCS.hxx>
#include <Geom_Curve.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Geom_Line.hxx>
#include <BRepBndLib.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <TopExp_Explorer.hxx>

namespace Exchange
{
    // Initialize color list.
    //@{
    QList<QColor> FITKOCCCommons::s_colorList =
    {
        QColor(69, 139, 116), QColor(245, 245, 220), QColor(187, 58, 58),
        QColor(0, 94, 157), QColor(193, 205, 205), QColor(222, 184, 135),
        QColor(177, 198, 237), QColor(139, 136, 120), QColor(233, 150, 122),
        QColor(189, 183, 107), QColor(139, 10, 80), QColor(49, 58, 151),
        QColor(202, 217, 187), QColor(238, 201, 0), QColor(173, 216, 230),
        QColor(110, 123, 139), QColor(139, 71, 137), QColor(238, 232, 170),
        QColor(184, 206, 198), QColor(184, 184, 219), QColor(206, 164, 107),
        QColor(106, 90, 205), QColor(238, 233, 233), QColor(0, 51, 102)
    };
    //@}

    // Initialize colors.
    //@{
    QColor FITKOCCCommons::s_vertexColor = Qt::black;
    QColor FITKOCCCommons::s_edgeColor = Qt::black;
    QColor FITKOCCCommons::s_faceColor = Qt::white;
    QColor FITKOCCCommons::s_meshFaceColor = QColor(70, 170, 70);
    QColor FITKOCCCommons::s_datumColor = QColor(0, 255, 0);
    QColor FITKOCCCommons::s_previewColor = QColor(50, 150, 230);
    QColor FITKOCCCommons::s_markColor = QColor(255, 200, 50);
    QColor FITKOCCCommons::s_markPreviewShapeColor = Qt::green;
    //@}

    // Initialize properties.
    //@{
    double FITKOCCCommons::s_transpacency = 0.6;
    double FITKOCCCommons::s_modelLineWidth = 2.;
    double FITKOCCCommons::s_datumLineWidth = 1.;
    double FITKOCCCommons::s_toolLineWidth = 3.; 
    //@}

    QColor FITKOCCCommons::GetColorByIndex(int index)
    {
        // Default color list.
        return s_colorList[index % s_colorList.count()];
    }

    QColor FITKOCCCommons::GetRandomColor()
    {
        //int index = rand() % s_colorList.count();
        //return s_colorList[index];

        srand(std::time(0));
        int r = rand() % 200 + 55;
        int g = rand() % 200 + 55;
        int b = rand() % 200 + 55;
        return QColor(r, g, b);
    }

    void FITKOCCCommons::QColorToDouble3(QColor color, double* rgbf)
    {
        rgbf[0] = color.redF();
        rgbf[1] = color.greenF();
        rgbf[2] = color.blueF();
    }

    void FITKOCCCommons::QColorToInt3(QColor color, int* rgb)
    {
        rgb[0] = color.red();
        rgb[1] = color.green();
        rgb[2] = color.blue();
    }

    Quantity_Color FITKOCCCommons::QColorToQuan(QColor color)
    {
        Quantity_Color quanColor;
        quanColor.SetValues(color.redF(), color.greenF(), color.blueF(), Quantity_TypeOfColor::Quantity_TOC_RGB);
        return quanColor;
    }

    gp_Pnt FITKOCCCommons::Double3ToPnt(double* vec3)
    {
        return gp_Pnt(vec3[0], vec3[1], vec3[2]);
    }

    gp_Vec FITKOCCCommons::Double3ToVec(double* vec3)
    {
        return gp_Vec(vec3[0], vec3[1], vec3[2]);
    }

    gp_Dir FITKOCCCommons::Double3ToDir(double* vec3)
    {
        return gp_Dir(vec3[0], vec3[1], vec3[2]);
    }

    NCollection_DataMap<int, Quantity_Color, NCollection_DefaultHasher<int>> FITKOCCCommons::DataToColorMap(QVector<double> & values, double max, double min)
    {
        // Get the color map by the given values vector.
        MeshVS_DataMapOfIntegerColor colorMap;
        double a, r, g, b = 0;
        int index = 0;
        for (const double & value : values)
        {
            // Value to RGB.
            a = (value - min) / (max - min);
            r = a;
            b = 1 - a;
            g = 1 - ((r - b) < 0 ? b - r : r - b);

            // Bind the color to color map.
            colorMap.Bind(index + 1, Quantity_Color(r, g, b, Quantity_TypeOfColor::Quantity_TOC_RGB));

            index++;
        }

        return colorMap;
    }

    NCollection_DataMap<int, Quantity_Color, NCollection_DefaultHasher<int>> FITKOCCCommons::QColorToColorMap(QColor color, int size)
    {
        MeshVS_DataMapOfIntegerColor colorMap;

        // Check the color.
        if (!color.isValid())
        {
            return colorMap;
        }

        Quantity_Color occColor = QColorToQuan(color);

        for (int i = 1; i <= size; i++)
        {
            // Bind the color to color map.
            colorMap.Bind(i, occColor);
        }

        return colorMap;
    }

    void FITKOCCCommons::GetShapeCenter(TopoDS_Shape & shape, gp_Pnt & pt)
    {
        gp_Vec dir;

        // Check the shape type.
        TopAbs_ShapeEnum st = shape.ShapeType();
        switch (st)
        {
            // Get the face normal.
        case TopAbs_ShapeEnum::TopAbs_FACE:
        {
            // Cast to the face shape.
            TopoDS_Face & face = TopoDS::Face(shape);

            // Get the mid point and normal.( on the mid point )
            BRepGProp_Face bf(face);
            double uMin, uMax, vMin, vMax;
            bf.Bounds(uMin, uMax, vMin, vMax);
            double midU = (uMin + uMax) / 2;
            double midV = (vMin + vMax) / 2;
            bf.Normal(midU, midV, pt, dir);

            break;
        }
        // Get the edge direction.
        case TopAbs_ShapeEnum::TopAbs_EDGE:
        {
            // Cast to the edge shape.
            TopoDS_Edge & edge = TopoDS::Edge(shape);

            // Cast to the geom curve.
            double first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (!curve)
            {
                return;
            }

            // Get the mid point of the edge and the direction from the start.
            curve->D1((first + last) * 0.5, pt, dir);

            break;
        }
        default:
        {
            // Get the shape center by the bound box.
            Bnd_Box bnd;
            BRepBndLib::Add(shape, bnd, false);

            pt = (bnd.CornerMax().XYZ() + bnd.CornerMin().XYZ()) / 2.;

            break;
        }
        }
    }

    bool FITKOCCCommons::GetShapeDirection(TopoDS_Shape & iShape, gp_Vec & oDir, gp_Pnt & oPt)
    {
        // Check the shape type.
        TopAbs_ShapeEnum st = iShape.ShapeType();
        switch (st)
        {
        // Get the face normal.
        case TopAbs_ShapeEnum::TopAbs_FACE:
        {
            // Cast to the face shape.
            TopoDS_Face & face = TopoDS::Face(iShape);

            // Get the mid point and normal.( on the mid point )
            BRepGProp_Face bf(face);
            double uMin, uMax, vMin, vMax;
            bf.Bounds(uMin, uMax, vMin, vMax);
            double midU = (uMin + uMax) / 2;
            double midV = (vMin + vMax) / 2;
            bf.Normal(midU, midV, oPt, oDir);

            break;
        }
        // Get the edge direction.
        case TopAbs_ShapeEnum::TopAbs_EDGE:
        {
            // Cast to the edge shape.
            TopoDS_Edge & edge = TopoDS::Edge(iShape);

            // Cast to the geom curve.
            double first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (!curve)
            {
                return false;
            }

            // Get the start point of the edge and the direction from the start.
            curve->D1(first, oPt, oDir);

            break;
        }
        default:
            return false;
        }

        return true;
    }

    bool FITKOCCCommons::GetShapeDirectionByPos(TopoDS_Shape & iShape, gp_Vec & oDir, gp_Pnt & oPt, gp_Pnt & oPpt)
    {
        // Check the shape type.
        TopAbs_ShapeEnum st = iShape.ShapeType();
        switch (st)
        {
        // Get the face normal.
        case TopAbs_ShapeEnum::TopAbs_FACE:
        {
            // Cast to the face shape.
            TopoDS_Face & face = TopoDS::Face(iShape);

            Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
            if (surface.IsNull())
            {
                return false;
            }

            double pu, pv;

            // If the point has already been contained by th surface.
            bool flag = GeomLib_Tool::Parameters(surface, oPt, Precision::Confusion(), pu, pv);
            if (!flag)
            {
                // Project the point to surface.
                GeomAPI_ProjectPointOnSurf pPOS(oPt, surface);
                if (pPOS.NbPoints() == 0)
                {
                    return false;
                }

                // Get the first point by default.
                oPpt = pPOS.Point(1);

                // Get the U, V parameters of the point.
                flag = GeomLib_Tool::Parameters(surface, oPpt, Precision::Confusion(), pu, pv);
            }
            else
            {
                oPpt = oPt;
            }

            if (!flag)
            {
                return false;
            }

            // Get the normal of the point with the U, V.
            BRepGProp_Face bf(face);
            bf.Normal(pu, pv, oPt, oDir);

            break;
        }
        // Get the edge direction.
        case TopAbs_ShapeEnum::TopAbs_EDGE:
        {
            // Cast to the edge shape.
            TopoDS_Edge & edge = TopoDS::Edge(iShape);

            // Cast to the geom curve.
            double first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (!curve)
            {
                return false;
            }

            double pu;

            // If the point has already been contained by th curve.
            bool flag = GeomLib_Tool::Parameter(curve, oPt, Precision::Confusion(), pu);
            if (!flag)
            {
                // Project the point to the curve.
                GeomAPI_ProjectPointOnCurve pPOC(oPt, curve);
                if (pPOC.NbPoints() == 0)
                {
                    return false;
                }

                // Get the first point by default.
                oPpt = pPOC.Point(1);

                // Get U parameter with the projected point.
                flag = GeomLib_Tool::Parameter(curve, oPpt, Precision::Confusion(), pu);
            }
            else
            {
                oPpt = oPt;
            }

            curve->D1(pu, oPpt, oDir);

            break;
        }
        default:
            return false;
        }

        return true;
    }

    TopoDS_Shape FITKOCCCommons::CreateArrow(gp_Pnt & pos, gp_Dir & dir, double scaleFactor)
    {
        // The radius and height for the cylinder and cone.
        double cyR = 2. * scaleFactor;
        double cyH = 40. * scaleFactor;
        double cR = 6. * scaleFactor;
        double cH = 12. * scaleFactor;

        // The ax2 of the cylinder and cone.
        gp_Ax2 cylinderAx2(pos, gp_Dir(dir.XYZ()));
        gp_Ax2 coneAx2(dir.XYZ() * cyH + pos.XYZ(), gp_Dir(dir.XYZ()));

        // Create the cylinder as arrow shaft.
        TopoDS_Shape arrowShaft = BRepPrimAPI_MakeCylinder(cylinderAx2, cyR, cyH);

        // Create the cone as arrow tip.
        TopoDS_Shape arrowTip = BRepPrimAPI_MakeCone(coneAx2, cR, 0., cH);

        // Merge tip and shaft.
        BRep_Builder builder;
        TopoDS_Compound arrow;
        builder.MakeCompound(arrow);
        builder.Add(arrow, arrowShaft);
        builder.Add(arrow, arrowTip);

        return arrow;
    }

    bool FITKOCCCommons::ScreenToWorldWithSurf(int sx, int sy, TopoDS_Shape & face, V3d_View* view, gp_Pnt & ret)
    {
        if (!view)
        {
            return false;
        }

        // Create a ray with the eye direction.
        //@{
        double wx, wy, wz, vx, vy, vz;
        view->Convert(sx, sy, wx, wy, wz);
        view->Proj(vx, vy, vz);

        gp_Pnt ppt(wx, wy, wz);
        gp_Lin ray(ppt, gp_Dir(vx, vy, vz));
        const TopoDS_Edge & rayEdge = BRepBuilderAPI_MakeEdge(ray);
        //@}

        // Get the closed cross points.
        int iPt = 0;
        double dist = 0.;
        const TopoDS_Shape & points = BRepAlgoAPI_Section(face, rayEdge);
        for (TopExp_Explorer iter(points, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
        {
            const TopoDS_Vertex & vertShape = TopoDS::Vertex(iter.Value());
            gp_Pnt cpt = BRep_Tool::Pnt(vertShape);
            if (iPt == 0)
            {
                ret = cpt;
                dist = cpt.Distance(ppt);
            }
            else
            {
                // Get the closet point to the given project point.
                double newDist = cpt.Distance(ppt);
                if (newDist < dist)
                {
                    ret = cpt;
                    dist = newDist;
                }
            }

            iPt++;
        }

        if (iPt == 0)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCCommons::ScreenToWorldWithCurve(int sx, int sy, TopoDS_Shape & edge, V3d_View* view, gp_Pnt & ret)
    {
        if (!view)
        {
            return false;
        }

        // Cast to the face shape.
        TopoDS_Edge & edgeShape = TopoDS::Edge(edge);

        // Create a ray with the eye direction.
        //@{
        double wx, wy, wz, vx, vy, vz;
        view->Convert(sx, sy, wx, wy, wz);
        view->Proj(vx, vy, vz);

        gp_Pnt ppt(wx, wy, wz);
        gp_Lin ray(ppt, gp_Dir(vx, vy, vz));
        TopoDS_Edge edgeShape2 = BRepBuilderAPI_MakeEdge(ray);
        //@}

        // Insect the line with the given surface.
        BRepExtrema_ExtCC ext(edgeShape, edgeShape2);
        if (!ext.IsDone())
        {
            return false;
        }

        int nPt = ext.NbExt();
        if (nPt == 0)
        {
            return false;
        }

        // Get the closet extrema point.
        double dist = 0.;
        for (int i = 1; i <= nPt; i++)
        {
            gp_Pnt cpt = ext.PointOnE1(i);
            if (i == 1)
            {
                ret = cpt;
                dist = cpt.Distance(ppt);
            }
            else
            {
                // Get the closet point to the given project point.
                double newDist = cpt.Distance(ppt);
                if (newDist < dist)
                {
                    ret = cpt;
                    dist = newDist;
                }
            }
        }

        return true;
    }

    bool FITKOCCCommons::GetEdgeMidPoint(TopoDS_Shape & edge, gp_Pnt & ret)
    {
        // Cast the shape to a curve.
        TopoDS_Edge & edgeShape = TopoDS::Edge(edge);

        double first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edgeShape, first, last);
        if (!curve)
        {
            return false;
        }

        // Get the mid point.
        gp_Vec temp;
        curve->D1((first + last) * 0.5, ret, temp);

        return true;
    }

    bool FITKOCCCommons::GetFaceMidPoint(TopoDS_Shape & face, gp_Pnt & ret)
    {
        // Cast the shape to a face.
        TopoDS_Face & faceShape = TopoDS::Face(face);

        // Get the mid point.
        BRepGProp_Face bf(faceShape);
        double uMin, uMax, vMin, vMax;
        bf.Bounds(uMin, uMax, vMin, vMax);
        double midU = (uMin + uMax) / 2;
        double midV = (vMin + vMax) / 2;

        gp_Vec dir;
        bf.Normal(midU, midV, ret, dir);

        return true;
    }
}   // namespace Exchange