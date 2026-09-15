/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCTransformation.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

#include "FITKOCCExtendTool.h"

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Trsf.hxx>
#include <gp_GTrsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Ax1.hxx>
#include <TopLoc_Location.hxx>
#include <Precision.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>
#include <BRep_Builder.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <Geom_Line.hxx>
#include <gp_Lin.hxx>
#include <BRepBuilderAPI_Copy.hxx>
namespace OCC {
    // 构造函数
    FITKOCCModelTransform::FITKOCCModelTransform() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelTransform::update()
    {
        if (m_SourceShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;

        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        try {
            gp_Trsf trans;
            trans.SetTranslation(gp_Vec(m_Vector[0], m_Vector[1], m_Vector[2]));

            auto copyShape = BRepBuilderAPI_Copy(*shape).Shape();
            auto newShape = copyShape.Located(trans * copyShape.Location());
            _occShapeAgent->updateShape(newShape);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelTransformByTwoPoints::FITKOCCModelTransformByTwoPoints() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelTransformByTwoPoints::update()
    {
        if (m_SourceShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;

        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        m_Vector[0] = m_EndPoint[0] - m_StartPoint[0];
        m_Vector[1] = m_EndPoint[1] - m_StartPoint[1];
        m_Vector[2] = m_EndPoint[2] - m_StartPoint[2];

        try {
            gp_Trsf trans;
            trans.SetTranslation(gp_Vec(m_Vector[0], m_Vector[1], m_Vector[2]));

            auto copyShape = BRepBuilderAPI_Copy(*shape).Shape();
            auto newShape = copyShape.Located(trans * copyShape.Location());
            _occShapeAgent->updateShape(newShape);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelTransformByDirAndDis::FITKOCCModelTransformByDirAndDis() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelTransformByDirAndDis::update()
    {
        if (m_SourceShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;

        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        gp_Vec vec(m_Direction[0], m_Direction[1], m_Direction[2]);
        auto mag = vec.Magnitude();
        if (mag <= Precision::Confusion()) {
            printLog(tr("The direction cannot be zero vector!"), 3);
            return false;
        }
        vec = m_Distance / mag * vec;
        m_Vector = { vec.X(), vec.Y(), vec.Z() };

        try {
            gp_Trsf trans;
            trans.SetTranslation(vec);
            auto copyShape = BRepBuilderAPI_Copy(*shape).Shape();
            auto newShape = copyShape.Located(trans * copyShape.Location());
            _occShapeAgent->updateShape(newShape);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelRotate::FITKOCCModelRotate() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelRotate::update()
    {
        if (m_SourceShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;
        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        try {
            gp_Trsf trans;
            gp_Ax1 rotateAxis(
                gp_Pnt(m_AxisStartPoint[0], m_AxisStartPoint[1], m_AxisStartPoint[2]),
                gp_Dir(m_AxisEndPoint[0] - m_AxisStartPoint[0], m_AxisEndPoint[1] - m_AxisStartPoint[1], m_AxisEndPoint[2] - m_AxisStartPoint[2])
            );
            trans.SetRotation(rotateAxis, m_Angle);
            auto copyShape = BRepBuilderAPI_Copy(*shape).Shape();
            auto newShape = copyShape.Located(trans * copyShape.Location());
            _occShapeAgent->updateShape(newShape);

        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelScale::FITKOCCModelScale() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelScale::update()
    {
        if (m_SourceShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;
        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        try {
            // 如果基准点不在原点的话需要进行两次平移操作
            bool isOriginBase = (std::abs(m_BasePoint[0]) <= Precision::Confusion()) &&
                (std::abs(m_BasePoint[1]) <= Precision::Confusion()) &&
                (std::abs(m_BasePoint[2]) <= Precision::Confusion());

            TopoDS_Shape newShape = *shape;

            // 平移基准点到原点
            if (!isOriginBase) {
                gp_Trsf trsf;
                trsf.SetTranslation(gp_Vec(-m_BasePoint[0], -m_BasePoint[1], -m_BasePoint[2]));
                newShape = newShape.Located(newShape.Location() * trsf);
            }

            // 进行缩放
            gp_GTrsf gtrsf;
            gtrsf.SetValue(1, 1, m_Factors[0]);
            gtrsf.SetValue(2, 2, m_Factors[1]);
            gtrsf.SetValue(3, 3, m_Factors[2]);
            BRepBuilderAPI_GTransform transform(newShape, gtrsf);
            newShape = transform.Shape();

            // 从原点移动到基准点
            if (!isOriginBase) {
                gp_Trsf trsf;
                trsf.SetTranslation(gp_Vec(m_BasePoint[0], m_BasePoint[1], m_BasePoint[2]));
                newShape = newShape.Located(newShape.Location() * trsf);
            }
            //auto newShape = shape->Located(shape->Location() * trans);
            _occShapeAgent->updateShape(newShape);

        }
        catch (...)
        {
            printLog(tr("Failed to scale!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelMirror::FITKOCCModelMirror() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelMirror::update()
    {
        if (m_SourceShape.isNull() || m_Reference.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;
        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        TopoDS_Shape refShape{};
        auto refCmd = geoCmdList->getDataByID(m_Reference.CmdId);
        if (refCmd == nullptr) {
            refCmd = geoCmdList->getDatumManager()->getDataByID(m_Reference.CmdId);
            if (refCmd == nullptr) return false;
            auto refAgent = dynamic_cast<OCC::FITKAbstractOCCModel*>(refCmd->getShapeAgent());
            if (refAgent == nullptr) return false;
            refShape = *(refAgent->getShape());
        }
        else {
            auto vshape = refCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_Reference.VirtualTopoId);
            if (vshape == nullptr) return false;
            refShape = vshape->getTopoShape();
        }

        if (refShape.IsNull()) return false;
        try {
            gp_Trsf trans;

            switch (refShape.ShapeType())
            {
            case TopAbs_ShapeEnum::TopAbs_VERTEX:
            {
                TopoDS_Vertex vertex = TopoDS::Vertex(refShape);
                if (vertex.IsNull()) return false;
                trans.SetMirror(BRep_Tool::Pnt(vertex));
                break;
            }
            case TopAbs_ShapeEnum::TopAbs_EDGE:
            {
                TopoDS_Edge edge = TopoDS::Edge(refShape);
                if (edge.IsNull()) return false;
                Standard_Real first, last;
                Handle(Geom_Curve)curve = BRep_Tool::Curve(edge, first, last);
                gp_Pnt start = curve->Value(first);
                gp_Pnt end = curve->Value(last);
                trans.SetMirror(gp_Ax1(start, gp_Dir(end.XYZ() - start.XYZ())));
                break;
            }
            case TopAbs_ShapeEnum::TopAbs_FACE:
            {
                TopoDS_Face face = TopoDS::Face(refShape);
                if (face.IsNull()) return false;

                Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
                if (surface->DynamicType() != STANDARD_TYPE(Geom_Plane)) return false;
                Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
                if (plane.IsNull()) return false;

                gp_Ax2 ax2;
                ax2.SetAxis(plane->Axis());
                trans.SetMirror(ax2);
                break;
            }
            default:
                auto t = refShape.ShapeType();
                return false;
                break;
            }

            auto newShape = BRepBuilderAPI_Transform(*shape, trans, true);
            _occShapeAgent->updateShape(newShape);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelRectangularPattern::FITKOCCModelRectangularPattern() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelRectangularPattern::update()
    {
        if (m_SourceShape.isNull()) return false;
        /*@{*/
        // 参数处理
        if (m_FirstDefinition != FITKAbsGeoModelPattern::Definition::InstanceAndSpacing) {
            switch (m_FirstDefinition)
            {
            case Interface::FITKAbsGeoModelPattern::InstanceAndTotal:
                if (m_FirstCount <= 0 || m_FirstTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_FirstSpacing = m_FirstTotal / m_FirstCount;
                break;
            case Interface::FITKAbsGeoModelPattern::SpacingAndTotal:
                if (m_FirstSpacing <= 0 || m_FirstTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_FirstCount = std::floor(m_FirstTotal / m_FirstSpacing);
                break;
            default:
                break;
            }
        }

        if (m_SecondDefinition != FITKAbsGeoModelPattern::Definition::InstanceAndSpacing) {
            switch (m_SecondDefinition)
            {
            case Interface::FITKAbsGeoModelPattern::InstanceAndTotal:
                if (m_FirstCount <= 0 || m_FirstTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_SecondSpacing = m_SecondTotal / m_SecondCount;
                break;
            case Interface::FITKAbsGeoModelPattern::SpacingAndTotal:
                if (m_FirstSpacing <= 0 || m_FirstTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_SecondCount = std::floor(m_SecondTotal / m_SecondSpacing);
                break;
            default:
                break;
            }
        }

        if (m_FirstCount <= 0 || m_FirstSpacing <= 0 || m_SecondCount <= 0 || m_SecondSpacing <= 0) {
            printLog(tr("Value should be grater than 0."), 3);
            return false;
        }
        /*@}*/

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;
        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;

        BRep_Builder aBuilder;
        TopoDS_Compound aCompound;
        aBuilder.MakeCompound(aCompound);

        try {
            gp_Trsf trans1;

            QList<TopoDS_Shape> firstShapeList{};
            firstShapeList.append(BRepBuilderAPI_Copy(*shape));
            /*@{*/
            // 第一方向上复制
            gp_Dir dir(m_FirstDirection[0], m_FirstDirection[1], m_FirstDirection[2]);
            gp_Vec vec(gp_Vec(dir)*m_FirstSpacing);

            for (int i = 1; i < m_FirstCount; ++i) {
                trans1.SetTranslation(vec * i);
                auto newShape = shape->Located(shape->Location() * trans1);
                firstShapeList.append(BRepBuilderAPI_Copy(newShape));
                aBuilder.Add(aCompound, newShape);
            }
            /*@}*/


            /*@{*/
            // 第二方向上复制
            dir = gp_Dir(m_SecondDirection[0], m_SecondDirection[1], m_SecondDirection[2]);
            vec = gp_Vec(gp_Vec(dir)*m_SecondSpacing);

            gp_Trsf trans2;
            for (auto s : firstShapeList) {
                for (int i = 1; i < m_SecondCount; ++i) {
                    trans2.SetTranslation(vec * i);
                    auto newShape = s.Located(s.Location() * trans2);
                    aBuilder.Add(aCompound, newShape);
                }
            }
            /*@}*/
            _occShapeAgent->updateShape(aCompound);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelCircularPattern::FITKOCCModelCircularPattern() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCModelCircularPattern::update()
    {
        if (m_SourceShape.isNull() || m_AxisShape.isNull()) return false;
        /*@{*/
        // 参数处理
        if (m_FirstDefinition != FITKAbsGeoModelPattern::Definition::InstanceAndSpacing) {
            switch (m_FirstDefinition)
            {
            case Interface::FITKAbsGeoModelPattern::InstanceAndTotal:
                if (m_AxisCount <= 0 || m_AxisTotalDegree <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_AxisSpacingDegree = m_AxisTotalDegree / m_AxisCount;
                break;
            case Interface::FITKAbsGeoModelPattern::SpacingAndTotal:
                if (m_AxisSpacingDegree <= 0 || m_AxisTotalDegree <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_AxisCount = std::floor(m_AxisTotalDegree / m_AxisSpacingDegree);
                break;
            default:
                break;
            }
        }

        if (m_SecondDefinition != FITKAbsGeoModelPattern::Definition::InstanceAndSpacing) {
            switch (m_SecondDefinition)
            {
            case Interface::FITKAbsGeoModelPattern::InstanceAndTotal:
                if (m_CrownCount <= 0 || m_CrownTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_CrownSpacing = m_CrownTotal / m_CrownCount;
                break;
            case Interface::FITKAbsGeoModelPattern::SpacingAndTotal:
                if (m_CrownSpacing <= 0 || m_CrownTotal <= 0) {
                    printLog(tr("Value should be grater than 0."), 3);
                    return false;
                }
                m_CrownCount = std::floor(m_CrownTotal / m_CrownSpacing);
                break;
            default:
                break;
            }
        }

        if (m_AxisCount <= 0 || m_AxisSpacingDegree <= 0 || m_CrownCount <= 0 || m_CrownSpacing <= 0) {
            printLog(tr("Value should be grater than 0."), 3);
            return false;
        }
        /*@}*/

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        /*@{*/
        // 源形状
        auto cmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (cmd == nullptr) return false;
        auto agent = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (agent == nullptr) return false;
        TopoDS_Shape* shape = agent->getShape();
        if (shape->IsNull()) return false;
        /*@}*/

        /*@{*/
        // 旋转轴
        auto axisCmd = geoCmdList->getDataByID(m_AxisShape.CmdId);
        if (axisCmd == nullptr)
        {
            axisCmd = geoCmdList->getDatumManager()->getDataByID(m_AxisShape.CmdId);
            if (axisCmd == nullptr) return false;
        }

        auto vshape = axisCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_AxisShape.VirtualTopoId);
        if (vshape == nullptr) return false;
        auto axisShape = vshape->getTopoShape();
        if (axisShape.IsNull()) return false;
        if (axisShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(axisShape);
        if (edge.IsNull()) return false;
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve->DynamicType() != STANDARD_TYPE(Geom_Line)) return false;

        gp_Pnt start = curve->Value(first);
        gp_Pnt end = curve->Value(last);
        /*@}*/

        BRep_Builder aBuilder;
        TopoDS_Compound aCompound;
        aBuilder.MakeCompound(aCompound);

        try {
            QList<TopoDS_Shape> firstShapeList{};
            firstShapeList.append(BRepBuilderAPI_Copy(*shape));
            /*@{*/
            // 第一方向上复制
            gp_Ax1 rotateAxis;
            if (m_IsReverse) {
                rotateAxis = gp_Ax1(end, gp_Dir(start.XYZ() - end.XYZ()));
            }
            else {
                rotateAxis = gp_Ax1(start, gp_Dir(end.XYZ() - start.XYZ()));
            }
            gp_Trsf trans1;
            for (int i = 1; i < m_AxisCount; ++i) {
                trans1.SetRotation(rotateAxis, m_AxisSpacingDegree / 180 * 3.14159265358979323846 * i);
                auto newShape = shape->Located(trans1*shape->Location());
                firstShapeList.append(BRepBuilderAPI_Copy(newShape));
                aBuilder.Add(aCompound, newShape);
            }
            /*@}*/


            /*@{*/
            // 第二方向上复制
            gp_Lin lin(start, gp_Dir(end.XYZ() - start.XYZ()));
            int index = 0;
            for (const TopoDS_Shape& s : firstShapeList) {
                gp_Pnt c = FITKOCCExtendTool::center(s);
                auto dis = m_CrownSpacing; //lin.Distance(c);
                gp_Lin dirLin = lin.Normal(c);
                gp_Dir dir = dirLin.Direction();
                for (int i = 1; i < m_CrownCount; ++i) {
                    gp_Vec vec(gp_Vec(dir) * dis * i);
                    gp_Trsf trans2;
                    trans2.SetTranslation(vec);
                    auto newShape = s.Located(trans2 * s.Location());
                    aBuilder.Add(aCompound, newShape);
                }
            }
            /*@}*/
            _occShapeAgent->updateShape(aCompound);
        }
        catch (...)
        {
            printLog(tr("Failed to transform!"), 3);
            return false;
        }
        return true;
    }

}


