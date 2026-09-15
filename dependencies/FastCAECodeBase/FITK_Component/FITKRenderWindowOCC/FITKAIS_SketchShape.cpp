/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAIS_SketchShape.h"

// Qt
#include <QPaintDevice>

#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeFix_SplitTool.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS.hxx>
#include <gp_Vec.hxx>
#include <ElSLib.hxx>
#include <BRepLib.hxx>
#include <BRep_Tool.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_Edge.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <ShapeBuild_ReShape.hxx>

//point
#include <Geom_CartesianPoint.hxx>
//curve
#include <GC_MakeArcOfParabola.hxx>
#include <gp_Parab.hxx>
#include <GC_MakeArcOfHyperbola.hxx>
#include <gp_Hypr.hxx>
#include <gce_MakeElips.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <BRepLib_MakePolygon.hxx>
#include <StepGeom_Polyline.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <gp_Circ.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <ShapeExtend_ComplexCurve.hxx>
#include <TopoDS_Wire.hxx>
#include <GC_MakeSegment.hxx>
#include <ShapeFix_EdgeConnect.hxx>
#include <GC_MakeArcOfCircle.hxx>

//QT
#include <QDebug>

//face
#include <ProjLib_Projector.hxx>
#include <ProjLib.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Plane.hxx>
#include <GeomPlate_Surface.hxx>
#include <Geom_Surface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>

//FITK
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCAIS_ColoredShape.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

#include <Prs3d_Drawer.hxx>
#include <Prs3d_PointAspect.hxx>

FITKAIS_SketchShape::FITKAIS_SketchShape(AIS_InteractiveContext* context) : AIS_Shape(TopoDS_Shape()), m_context(context)
{
    // 置顶对象。
    this->SetZLayer(Graphic3d_ZLayerId_Topmost);
}

void FITKAIS_SketchShape::setShapeType(CommonOCC::SketchShapeType t)
{    //开启拾取状态 只能拾取点
    if (m_context == nullptr) return;
    Attributes()->SetupOwnPointAspect();
    Attributes()->SetVertexDrawMode(Prs3d_VDM_All);
    Attributes()->PointAspect()->SetTypeOfMarker(Aspect_TOM_O);
    updateDraft = nullptr;
    _ssType = t;
    init();
}

void FITKAIS_SketchShape::addPoint(gp_Pnt & p)
{
    if (_inputPointList.size() > 0)
        if (_inputPointList.last().Distance(p) < 1e-4) return;
    _inputPointList.push_back(p);
    if (!isAddParameters()) _inputPointList.removeLast();
    update();
    //多段线做单独处理，不发送信号
    if (isParametersSatisfy() && _ssType != CommonOCC::SketchShapeType::Draft_Wire && _ssType != CommonOCC::SketchShapeType::Draft_BezierTools && _ssType != CommonOCC::SketchShapeType::Draft_Bspline)
    {
        emit sig_parametersSatisfy();
    }
}

const QList<double*> FITKAIS_SketchShape::getPoint()
{
    for (int i = 0; i < tempPList.size(); ++i)
    {
        delete[] tempPList[i];
    }
    tempPList.clear();

    for (int i = 0; i < _calculatePointList.size(); ++i)
    {
        //校验两点距离
        if (i > 0)
        {
            double* tempXYZ = tempPList.last();
            auto dis = _calculatePointList[i].Distance(gp_Pnt(tempXYZ[0], tempXYZ[1], tempXYZ[2]));
            if (dis < 1e-4) continue;
        }
        double x = _calculatePointList[i].X();
        double y = _calculatePointList[i].Y();
        double z = _calculatePointList[i].Z();
        double* xyz = new double[3]{ x,y,z };
        tempPList.append(xyz);
    }
    return tempPList;
}

void FITKAIS_SketchShape::replaceLast(gp_Pnt & p)
{
    _mouseMovePoint = p;
}

void FITKAIS_SketchShape::setInitParameter(const gp_Pnt & p, const gp_Dir& direction, const gp_Dir& upDir)
{
    _p = p;
    _direction = direction;
    _upDirection = upDir;
}

bool FITKAIS_SketchShape::drawStatus()
{
    return _drawStatus;
}

bool FITKAIS_SketchShape::isAddParameters()
{
    if (!_drawStatus) return false;
    switch (_ssType)
    {
    case CommonOCC::SketchShapeType::Draft_Circle:
    case CommonOCC::SketchShapeType::Draft_Line: return _inputPointList.size() <= 2;
    case CommonOCC::SketchShapeType::Draft_Wire: return true;
    case CommonOCC::SketchShapeType::Draft_Fillet: return false;
    case CommonOCC::SketchShapeType::Draft_ArcTools:  return _inputPointList.size() <= 4;
    case CommonOCC::SketchShapeType::Draft_Ellipse: return _inputPointList.size() <= 2;
    case CommonOCC::SketchShapeType::Draft_Rectangle: return _inputPointList.size() <= 2;
    case CommonOCC::SketchShapeType::Draft_Polygon: return false;
    case CommonOCC::SketchShapeType::Draft_Bspline: return _inputPointList.size() < Geom_BSplineCurve::MaxDegree() + 1;
    case CommonOCC::SketchShapeType::Draft_BezierTools: return _inputPointList.size() < Geom_BezierCurve::MaxDegree() + 1;
    case CommonOCC::SketchShapeType::Draft_Hyperbola: return _inputPointList.size() < 6;
    case CommonOCC::SketchShapeType::Draft_Parabola: return _inputPointList.size() < 5;

    case CommonOCC::SketchShapeType::None:
    case CommonOCC::SketchShapeType::EMax:
    default:
        break;
        return false;
    }
    return false;
}

bool FITKAIS_SketchShape::isParametersSatisfy()
{
    switch (_ssType)
    {
    case CommonOCC::SketchShapeType::Draft_Line: return _inputPointList.size() > 1;
    case CommonOCC::SketchShapeType::Draft_Wire: return _inputPointList.size() > 1;
    case CommonOCC::SketchShapeType::Draft_Fillet: return false;
    case CommonOCC::SketchShapeType::Draft_ArcTools:  return _inputPointList.size() == 4;
    case CommonOCC::SketchShapeType::Draft_Circle:  return _inputPointList.size() > 1;
    case CommonOCC::SketchShapeType::Draft_Ellipse: return _inputPointList.size() > 1;
    case CommonOCC::SketchShapeType::Draft_Rectangle: return _inputPointList.size() > 1;
    case CommonOCC::SketchShapeType::Draft_Polygon: return false;
    case CommonOCC::SketchShapeType::Draft_Bspline: return _inputPointList.size() > 1 && _inputPointList.size() < Geom_BSplineCurve::MaxDegree() + 1;
    case CommonOCC::SketchShapeType::Draft_BezierTools: return _inputPointList.size() > 1 && _inputPointList.size() < Geom_BezierCurve::MaxDegree() + 1;
    case CommonOCC::SketchShapeType::Draft_Hyperbola: return _inputPointList.size() > 4;
    case CommonOCC::SketchShapeType::Draft_Parabola: return _inputPointList.size() > 3;

    case CommonOCC::SketchShapeType::None:
    case CommonOCC::SketchShapeType::EMax:
    default:
        break;
        return false;
    }
    return false;
}

void FITKAIS_SketchShape::setDrawStatus(bool status)
{
    if (!status)
    {
        _inputPointList.clear();
        _calculatePointList.clear();
        _mouseMovePoint = gp_Pnt();
        _p = gp_Pnt();
        _direction = gp_Dir();
        //清除再次开启时绘制的形状，因为对象没有被清楚
        SetShape(TopoDS_Shape());
    }

    _drawStatus = status;
}

void FITKAIS_SketchShape::update()
{
    if (_inputPointList.size() < 1 || !_drawStatus)return;
    //异常检测
    Standard_Real tempDis = fabs(_mouseMovePoint.Distance(_inputPointList.last()));
    if (tempDis < 1e-4) return;

    (this->*updateDraft)();
}

void FITKAIS_SketchShape::slot_updateSelectdObject()
{
    if (m_context == nullptr) return;
    for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
    {
        if (_inputPointList.size() < 1 || _ssType != CommonOCC::SketchShapeType::Draft_Wire) return;

        auto selectedShape = m_context->SelectedShape();
        if (selectedShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_VERTEX) continue;
        TopoDS_Vertex vertex = TopoDS::Vertex(selectedShape);
        gp_Pnt selectPoint = BRep_Tool::Pnt(vertex);

        for (int i = 0; i < _inputPointList.size(); ++i)
        {
            double dis = fabs(selectPoint.Distance(_inputPointList[i]));
            if (dis > 1e-6) continue;
            //只做第一个点的吸附
            if (i == 0 && isParametersSatisfy())
            {
                _inputPointList.replace(_inputPointList.size() - 1, _inputPointList[0]);
                _calculatePointList.replace(_calculatePointList.size() - 1, _calculatePointList[0]);
                emit sig_parametersSatisfy();
                return;
            }
        }
    }
}

void FITKAIS_SketchShape::init()
{
    switch (_ssType)
    {
    case CommonOCC::SketchShapeType::Draft_Line: updateDraft = &FITKAIS_SketchShape::updateDraftLine; break;
    case CommonOCC::SketchShapeType::Draft_Wire: updateDraft = &FITKAIS_SketchShape::updateDraftWire; break;
    case CommonOCC::SketchShapeType::Draft_Fillet: updateDraft = &FITKAIS_SketchShape::updateDraftFillet; break;
    case CommonOCC::SketchShapeType::Draft_ArcTools: updateDraft = &FITKAIS_SketchShape::updateDraftArcTools; break;
    case CommonOCC::SketchShapeType::Draft_Circle: updateDraft = &FITKAIS_SketchShape::updateDraftCircle; break;
    case CommonOCC::SketchShapeType::Draft_Ellipse:updateDraft = &FITKAIS_SketchShape::updateDraftEllipse; break;
    case CommonOCC::SketchShapeType::Draft_Rectangle:updateDraft = &FITKAIS_SketchShape::updateDraftRectangle; break;
    case CommonOCC::SketchShapeType::Draft_Polygon:updateDraft = &FITKAIS_SketchShape::updateDraftPolygon; break;
    case CommonOCC::SketchShapeType::Draft_Bspline:updateDraft = &FITKAIS_SketchShape::updateDraftBspline; break;
    case CommonOCC::SketchShapeType::Draft_BezierTools:updateDraft = &FITKAIS_SketchShape::updateDraftBezierTools; break;
    case CommonOCC::SketchShapeType::Draft_Hyperbola:updateDraft = &FITKAIS_SketchShape::updateDraftHyperbola; break;
    case CommonOCC::SketchShapeType::Draft_Parabola:updateDraft = &FITKAIS_SketchShape::updateDraftParabola; break;

    case CommonOCC::SketchShapeType::None:
    case CommonOCC::SketchShapeType::EMax:
    default:
        break;
    }
}

void FITKAIS_SketchShape::updateDraftLine()
{
    Standard_Real dis = fabs(_mouseMovePoint.Distance(_inputPointList[0]));
    if (dis < 1e-4) return;

    GC_MakeSegment makeLineSegment(_inputPointList[0], _mouseMovePoint);
    Handle(Geom_TrimmedCurve) v = makeLineSegment.Value();
    if (v.IsNull()) return;
    SetShape(BRepBuilderAPI_MakeEdge(v).Shape());

    //处理数据
    _calculatePointList.clear();
    _calculatePointList.append(v->StartPoint());
    _calculatePointList.append(v->EndPoint());

    //Standard_Real dis = fabs(_mouseMovePoint.Distance(_inputPointList[0]));
    //if (dis < 1e-4) return;
    //BRepBuilderAPI_MakeWire wire;
    //
    //GC_MakeSegment makeLineSegment(_inputPointList[0], _mouseMovePoint);
    //Handle(Geom_TrimmedCurve) v = makeLineSegment.Value();
    //TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(v);
    //wire.Add(BRepBuilderAPI_MakeWire(edge1));
    //
    //GC_MakeSegment makeLineSegment2(gp_Pnt(),_inputPointList[0]);
    //Handle(Geom_TrimmedCurve) v2 = makeLineSegment2.Value();
    //TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(v2);
    //wire.Add(BRepBuilderAPI_MakeWire(edge2));
    //
    //GC_MakeSegment makeLineSegment3(gp_Pnt(), gp_Pnt(100,100,100));
    //Handle(Geom_TrimmedCurve) v3 = makeLineSegment3.Value();
    //TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(v3);
    //wire.Add(BRepBuilderAPI_MakeWire(edge3));
    //wire.Build();
    //SetShape(wire);

    //Standard_Real dis = fabs(_mouseMovePoint.Distance(_inputPointList[0]));
    //if (dis < 5) return;
    //
    //GC_MakeSegment makeLineSegment(_inputPointList[0], _mouseMovePoint);
    //Handle(Geom_TrimmedCurve) v = makeLineSegment.Value();
    //TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(v);
    //if (edge.IsNull()) return;
    //TopoDS_Edge partOfE;
    //Standard_Real startParam(0.), endParam(0.);
    //Handle(Geom_Curve) c = BRep_Tool::Curve(edge, startParam, endParam);//得到底层曲线
    ////有时底层曲线没有创建，要手动创建
    //if (c.IsNull())
    //{
    //    BRepLib::BuildCurves3d(edge, 1.0e-5, GeomAbs_C1);//创建曲线
    //    c = BRep_Tool::Curve(edge, startParam, endParam);
    //}
    //if (!c.IsNull())
    //{
    //    //截取曲线并创建边
    //    Handle(Geom_TrimmedCurve) tc = new Geom_TrimmedCurve(c, startParam, (startParam + endParam) / 2);//例如截取边的的一半
    //    partOfE = BRepBuilderAPI_MakeEdge(tc);
    //    SetShape(partOfE);
    //}

}

void FITKAIS_SketchShape::updateDraftWire()
{
    BRepBuilderAPI_MakeWire wires;

    _calculatePointList.clear();
    for (int i = 0; i + 1 < _inputPointList.size(); ++i)
    {
        auto dis = fabs(_inputPointList[i].Distance(_inputPointList[i + 1]));
        if (dis < 1e-4) return;
        GC_MakeSegment makeLineSegment(_inputPointList[i], _inputPointList[i + 1]);
        Handle(Geom_TrimmedCurve) v = makeLineSegment.Value();
        wires.Add(BRepBuilderAPI_MakeEdge(v));
    }

    _calculatePointList.append(_inputPointList);
    _calculatePointList.append(_mouseMovePoint);
    auto p = _inputPointList.last();
    if (fabs(p.Distance(_mouseMovePoint)) > 1e-4)
    {
        GC_MakeSegment makeLineSegment(p, _mouseMovePoint);
        Handle(Geom_TrimmedCurve) v = makeLineSegment.Value();
        wires.Add(BRepBuilderAPI_MakeEdge(v));
    }
    wires.Build();
    if (wires.Error() == BRepBuilderAPI_WireError::BRepBuilderAPI_WireDone)
    {
        SetShape(wires.Shape());
        //处理数据
    }
}

void FITKAIS_SketchShape::updateDraftFillet()
{

}

void FITKAIS_SketchShape::updateDraftArcTools()
{
    TopoDS_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);
    gp_Pnt tempP = _inputPointList.size() > 1 ? _inputPointList[1] : _mouseMovePoint;

    //圆
    //{
    gp_Ax2 ax2(_inputPointList[0], _direction);
    gp_Circ circle(ax2, _defaultValue);
    Standard_Real dis = fabs(tempP.Distance(_inputPointList[0]));
    circle.SetRadius(dis);
    auto circleShape = BRepBuilderAPI_MakeEdge(circle).Shape();
    if (_inputPointList.size() < 3)
        builder.Add(comp, circleShape);
    //}

    //线
    //{
    if (_inputPointList.size() > 1)
    {
        if (_mouseMovePoint.Distance(_inputPointList[0].XYZ()) < 1e-4) return;
        gp_Lin lin(_inputPointList[0], _mouseMovePoint.XYZ() - _inputPointList[0].XYZ());
        //异常检测
        Standard_Real tempLineDis = fabs(_inputPointList[1].Distance(_inputPointList[0]));
        GC_MakeSegment makeLineSegment(lin, _inputPointList[0], tempLineDis);
        Handle(Geom_TrimmedCurve) line = makeLineSegment.Value();
        auto lineShape = BRepBuilderAPI_MakeEdge(line).Shape();
        builder.Add(comp, lineShape);
    }
    else
    {
        GC_MakeSegment makeLineSegment(_inputPointList[0], _mouseMovePoint);
        Handle(Geom_TrimmedCurve) line = makeLineSegment.Value();
        auto lineShape = BRepBuilderAPI_MakeEdge(line).Shape();
        builder.Add(comp, lineShape);
    }
    //}

    //圆弧
    //{
    if (_inputPointList.size() > 2)
    {
        GC_MakeArcOfCircle mCircle(circle, _inputPointList[2], _mouseMovePoint, true);
        auto arcCircleLine = BRepBuilderAPI_MakeEdge(mCircle.Value()).Shape();
        builder.Add(comp, arcCircleLine);

        //如果参数满足则替换
        if (_inputPointList.size() > 2)
        {
            auto curv = mCircle.Value();
            //替换点
            _calculatePointList.clear();
            _calculatePointList.append(_inputPointList[0]);
            _calculatePointList.append(curv->StartPoint());
            _calculatePointList.append(curv->EndPoint());
        }
    }
    //}
    SetShape(comp);
}

void FITKAIS_SketchShape::updateDraftCircle()
{

    if (_inputPointList.size() > 1)return;
    gp_Ax2 ax2(_inputPointList[0], _direction);
    Handle(Geom_Circle) circle = new Geom_Circle(ax2, _defaultValue);
    Standard_Real dis = fabs(_mouseMovePoint.Distance(_inputPointList[0]));
    if (dis < 1e-4) return;
    circle->SetRadius(dis);
    SetShape(BRepBuilderAPI_MakeEdge(circle).Shape());

    _calculatePointList.clear();
    _calculatePointList.append(_inputPointList[0]);
    _calculatePointList.append(_mouseMovePoint);
}

void FITKAIS_SketchShape::updateDraftEllipse()
{
    TopoDS_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);

    // 定义三维点
    gp_Pnt P1(_inputPointList[0]);
    gp_Pnt P2(_mouseMovePoint);
    gp_Ax3 ax3(_inputPointList[0], _direction, _upDirection);
    gp_Pln pln(ax3);

    //计算2D向量长度
    gp_Pnt2d p12D = ProjLib::Project(pln, P1);
    gp_Pnt2d p22D = ProjLib::Project(pln, P2);
    gp_Pnt2d p32D(p22D.X(), p12D.Y());
    double dis2DX = p32D.Distance(p12D);
    double dis2DY = p32D.Distance(p22D);
    // 定义平移向量
    gp_Vec translation(_upDirection);
    translation = translation.Multiplied(dis2DX);

    // 按照向量方向 创建平移变换
    if (p32D.X() < 0 || p32D.Y() < 0)
    {
        translation = translation.Reversed();
    }

    gp_Trsf p3Trsf;
    gp_Trsf p4Trsf;
    p3Trsf.SetTranslation(translation);
    p4Trsf.SetTranslation(translation.Reversed());

    // 平移点
    gp_Pnt P3 = P1.Transformed(p3Trsf);
    gp_Pnt P4 = P2.Transformed(p4Trsf);
    // 创建矩形
    BRepBuilderAPI_MakePolygon polygonMaker;
    polygonMaker.Add(P1);
    polygonMaker.Add(P3);
    polygonMaker.Add(P2);
    polygonMaker.Add(P4);
    polygonMaker.Add(P1);
    TopoDS_Wire wire = polygonMaker.Wire();
    builder.Add(comp, wire);

    //创建圆
    gp_Pnt centerPoint = (P1.XYZ() + P2.XYZ()) / 2;

    gp_Trsf p1RightTrsf;
    gp_Trsf centerLeftTrsf;
    p1RightTrsf.SetTranslation(translation.Multiplied(0.5));
    centerLeftTrsf.SetTranslation(translation.Multiplied(0.5).Reversed());

    gp_Pnt majorPoint, minorPoint;
    if (dis2DX < dis2DY)
    {
        majorPoint = P1.Transformed(p1RightTrsf);
        minorPoint = centerPoint.Transformed(centerLeftTrsf);
    }
    else
    {
        minorPoint = P1.Transformed(p1RightTrsf);
        majorPoint = centerPoint.Transformed(centerLeftTrsf);
    }

    gce_MakeElips elips(majorPoint, minorPoint, centerPoint);
    if (!elips.IsDone()) return;
    BRepBuilderAPI_MakeEdge edge(elips.Value());
    if (!edge.IsDone()) return;
    builder.Add(comp, edge);
    SetShape(comp);

    //添加计算后点的列表
    _calculatePointList.clear();
    _calculatePointList.append(centerPoint);
    _calculatePointList.append(majorPoint);
    _calculatePointList.append(minorPoint);
}

void FITKAIS_SketchShape::updateDraftRectangle()
{
    // 定义三维点
    gp_Pnt P1(_inputPointList[0]);
    gp_Pnt P2(_mouseMovePoint);
    gp_Ax3 ax3(_inputPointList[0], _direction, _upDirection);
    gp_Pln pln(ax3);

    //计算2D向量长度
    gp_Pnt2d p12D = ProjLib::Project(pln, P1);
    gp_Pnt2d p22D = ProjLib::Project(pln, P2);
    gp_Pnt2d p32D(p22D.X(), p12D.Y());
    double dis2D = p32D.Distance(p12D);
    // 定义平移向量
    gp_Vec translation(_upDirection);
    translation = translation.Multiplied(dis2D);

    // 按照向量方向 创建平移变换
    if (p32D.X() < 0 || p32D.Y() < 0)
    {
        translation = translation.Reversed();
    }

    gp_Trsf p3Trsf;
    gp_Trsf p4Trsf;
    p3Trsf.SetTranslation(translation);
    p4Trsf.SetTranslation(translation.Reversed());

    // 平移点
    gp_Pnt P3 = P1.Transformed(p3Trsf);
    gp_Pnt P4 = P2.Transformed(p4Trsf);

    // 创建矩形
    BRepBuilderAPI_MakePolygon polygonMaker;
    polygonMaker.Add(P1);
    polygonMaker.Add(P3);
    polygonMaker.Add(P2);
    polygonMaker.Add(P4);
    polygonMaker.Add(P1);
    TopoDS_Wire wire = polygonMaker.Wire();
    SetShape(wire);

    //添加计算后点的列表
    _calculatePointList.clear();
    _calculatePointList.append(P1);
    _calculatePointList.append(P3);
    _calculatePointList.append(P2);
    _calculatePointList.append(P4);
}

void FITKAIS_SketchShape::updateDraftPolygon()
{
    updateDraftCircle();
}

void FITKAIS_SketchShape::updateDraftBspline()
{
    //添加计算后点的列表
    _calculatePointList.clear();

    Handle(TColgp_HArray1OfPnt) aPoles = new TColgp_HArray1OfPnt(1, _inputPointList.size() + 1);
    for (int i = 0; i < _inputPointList.size(); ++i)
    {
        aPoles->SetValue(i + 1, _inputPointList[i]);
        _calculatePointList.append(_inputPointList[i]);

    }
    aPoles->SetValue(_inputPointList.size() + 1, _mouseMovePoint);
    _calculatePointList.append(_mouseMovePoint);

    GeomAPI_Interpolate bspline_generator(aPoles, false, 1e-7);
    bspline_generator.Perform();
    if (!bspline_generator.IsDone()) return;

    Handle(Geom_BSplineCurve) bSplineCurve = bspline_generator.Curve();
    BRepBuilderAPI_MakeEdge anBCurve(bSplineCurve);
    SetShape(anBCurve.Shape());
}

void FITKAIS_SketchShape::updateDraftBezierTools()
{
    _calculatePointList.clear();

    // Add points to the curve poles array.
    TColgp_Array1OfPnt aPoles(1, _inputPointList.size() + 1);
    for (int i = 0; i < _inputPointList.size(); ++i)
    {
        aPoles.SetValue(i + 1, _inputPointList[i]);
        _calculatePointList.append(_inputPointList[i]);
    }
    aPoles.SetValue(_inputPointList.size() + 1, _mouseMovePoint);
    _calculatePointList.append(_mouseMovePoint);

    // Create Bezier curve.
    Handle(Geom_BezierCurve) aBezierCurve = new Geom_BezierCurve(aPoles);
    BRepBuilderAPI_MakeEdge anAisBezierCurve(aBezierCurve);
    SetShape(anAisBezierCurve.Shape());
}

void FITKAIS_SketchShape::updateDraftHyperbola()
{
    TopoDS_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);

    // 定义三维点
    gp_Pnt P1(_inputPointList[0]);
    gp_Pnt P2(_inputPointList.size() > 1 ? _inputPointList[1] : _mouseMovePoint);
    gp_Pnt P3(_inputPointList.size() > 2 ? _inputPointList[2] : _mouseMovePoint);
    gp_Pnt P4(_inputPointList.size() > 3 ? _inputPointList[3] : _mouseMovePoint);
    gp_Pnt P5(_inputPointList.size() > 4 ? _inputPointList[4] : _mouseMovePoint);

    //计算向量
    gp_Vec vec1(P2, P1);
    gp_Trsf trsf;

    if (P3.Distance(P1) > vec1.Magnitude() * 0.97)
    {
        trsf.SetTranslation(vec1 * 0.03);
        P3 = P2.Transformed(trsf);
    }
    else if (P3.Distance(P2) > vec1.Magnitude() * 0.97)
    {
        trsf.SetTranslation(-vec1 * 0.03);
        P3 = P1.Transformed(trsf);
    }
    gp_Vec vec2(P2, P3);

    //绘制直线
    GC_MakeSegment makeLineSegment(P1, P2);
    if (!makeLineSegment.IsDone()) return;
    BRepBuilderAPI_MakeEdge lineShape(makeLineSegment.Value());
    builder.Add(comp, lineShape);

    gp_Ax2 ax2(P2, _direction, vec1);

    double a = vec2.Magnitude();
    double c = vec1.Magnitude();
    double b = std::sqrt(std::pow(c, 2) - std::pow(a, 2));

    double majorRadius = a;
    double minorRadius = b;

    //绘制双曲线
    gp_Hypr aHyperbola(ax2, majorRadius, minorRadius);

    //添加计算后点的列表
    _calculatePointList.clear();
    _calculatePointList.append(P1);
    _calculatePointList.append(P2);
    _calculatePointList.append(P3);

    if (_inputPointList.size() < 4)
    {
        BRepBuilderAPI_MakeEdge hyperCurce(aHyperbola);
        builder.Add(comp, hyperCurce.Shape());
    }
    else
    {
        GC_MakeArcOfHyperbola mCircle(aHyperbola, P4, P5, true);
        if (!mCircle.IsDone()) return;

        //添加计算后点的列表
        _calculatePointList.append(mCircle.Value()->StartPoint());
        _calculatePointList.append(mCircle.Value()->EndPoint());

        BRepBuilderAPI_MakeEdge arcHyperbolaLine(mCircle.Value());
        builder.Add(comp, arcHyperbolaLine.Shape());
    }

    SetShape(comp);
}

void FITKAIS_SketchShape::updateDraftParabola()
{
    TopoDS_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);

    // 定义三维点
    gp_Pnt P1(_inputPointList[0]);
    gp_Pnt P2(_inputPointList.size() > 1 ? _inputPointList[1] : _mouseMovePoint);
    gp_Pnt P3(_inputPointList.size() > 2 ? _inputPointList[2] : _mouseMovePoint);
    gp_Pnt P4(_inputPointList.size() > 3 ? _inputPointList[3] : _mouseMovePoint);

    //绘制直线
    GC_MakeSegment makeLineSegment(P1, P2);
    BRepBuilderAPI_MakeEdge lineShape(makeLineSegment.Value());
    if (!lineShape.IsDone()) return;
    builder.Add(comp, lineShape);

    //计算向量
    gp_Vec vec(P2, P1);
    gp_Ax2 ax2(P2, _direction, vec);

    //计算半径
    double dis = P2.Distance(P1);
    dis = dis < 1e-4 ? 1e-4 : dis;
    //绘制抛物线 不确定对不对
    gp_Parab aParabola(ax2, dis);

    //添加计算后点的列表
    _calculatePointList.clear();
    _calculatePointList.append(P1);
    _calculatePointList.append(P2);

    if (_inputPointList.size() < 3)
    {
        BRepBuilderAPI_MakeEdge hyperCurce(aParabola);
        builder.Add(comp, hyperCurce.Shape());
    }
    else
    {
        GC_MakeArcOfParabola mCircle(aParabola, P3, P4, true);
        if (!mCircle.IsDone())  return;
        //添加计算后点的列表
        _calculatePointList.append(mCircle.Value()->StartPoint());
        _calculatePointList.append(mCircle.Value()->EndPoint());

        BRepBuilderAPI_MakeEdge arcaParabolaLine(mCircle.Value());
        builder.Add(comp, arcaParabolaLine.Shape());
    }
    SetShape(comp);
}
