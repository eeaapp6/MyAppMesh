/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelTransformPart.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKAbstractOCCModel.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCComandCommon.h"

// OCC
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax2.hxx>
#include <TopoDS_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Builder.hxx>

namespace OCC
{
    FITKOCCModelTranslatePart::FITKOCCModelTranslatePart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelTranslatePart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // 获取源部件的OCC模型代理。
        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        // 获取OCC形状数据。
        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        try
        {
            // 计算平移增量。
            double dx = m_to[0] - m_from[0];
            double dy = m_to[1] - m_from[1];
            double dz = m_to[2] - m_from[2];

            // 创建平移变换。
            gp_Trsf trans;
            trans.SetTranslation(gp_Vec(dx, dy, dz));

            // 应用变换到形状。
            BRepBuilderAPI_Transform transformer(*shape, trans, true);
            TopoDS_Shape newShape = transformer.Shape();

            // 如果存在部件，则将新形状添加到部件中。
            if (part)
            {
                // 添加变换后的新形状。
                part->updatePartShape(newShape, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                // 无部件模式，直接更新形状显示。
                _occShapeAgent->updateShape(newShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to translate shape!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelRotatePart::FITKOCCModelRotatePart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRotatePart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // 获取源部件的OCC模型代理。
        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        // 获取OCC形状数据。
        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        try
        {
            // 创建旋转轴。
            gp_Pnt originPoint(m_origin[0], m_origin[1], m_origin[2]);
            gp_Dir directionVector(m_direction[0], m_direction[1], m_direction[2]);
            gp_Ax1 rotationAxis(originPoint, directionVector);

            // 创建旋转变换。
            gp_Trsf trans;
            trans.SetRotation(rotationAxis, m_angle * M_PI / 180.);

            // 应用变换到形状。
            BRepBuilderAPI_Transform transformer(*shape, trans, true);
            TopoDS_Shape newShape = transformer.Shape();

            // 如果存在部件，则将新形状添加到部件中。
            if (part)
            {
                // 添加变换后的新形状。
                part->updatePartShape(newShape, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                // 无部件模式，直接更新形状显示。
                _occShapeAgent->updateShape(newShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to rotate shape!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelScalePart::FITKOCCModelScalePart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelScalePart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // 获取源部件的OCC模型代理。
        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        // 获取OCC形状数据。
        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        try
        {
            // 创建缩放中心点。
            gp_Pnt scaleOrigin(m_origin[0], m_origin[1], m_origin[2]);

            // 创建缩放变换。
            gp_Trsf trans;
            trans.SetScale(scaleOrigin, m_scaleFactor);

            // 应用变换到形状。
            BRepBuilderAPI_Transform transformer(*shape, trans, true);
            TopoDS_Shape newShape = transformer.Shape();

            // 如果存在部件，则将新形状添加到部件中。
            if (part)
            {
                // 添加变换后的新形状。
                part->updatePartShape(newShape, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                // 无部件模式，直接更新形状显示。
                _occShapeAgent->updateShape(newShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to scale shape!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelMirrorPart::FITKOCCModelMirrorPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMirrorPart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // 获取源部件的OCC模型代理。
        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        // 获取OCC形状数据。
        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        try
        {
            // 创建镜像平面原点。
            gp_Pnt mirrorOrigin(m_origin[0], m_origin[1], m_origin[2]);

            // 根据平面类型创建初始镜像平面法向量。
            gp_Dir planeNormal;
            switch (m_planeType)
            {
            case Interface::FITKGeoEnum::PT_XY:
            {
                // XY平面的法向量为Z轴。
                planeNormal = gp_Dir(0, 0, 1);
                break;
            }
            case Interface::FITKGeoEnum::PT_XZ:
            {
                // XZ平面的法向量为Y轴。
                planeNormal = gp_Dir(0, 1, 0);
                break;
            }
            case Interface::FITKGeoEnum::PT_YZ:
            {
                // YZ平面的法向量为X轴。
                planeNormal = gp_Dir(1, 0, 0);
                break;
            }
            case Interface::FITKGeoEnum::PT_UserDefine:
            {
                // 使用自定义方向。
                planeNormal = gp_Dir(m_customPlaneNormal[0], 
                    m_customPlaneNormal[1], m_customPlaneNormal[2]);
                break;
            }
            default:
                break;
            }

            // 创建初始镜像平面。
            gp_Ax3 planeAxis(mirrorOrigin, planeNormal);
            gp_Pln mirrorPlane(planeAxis);
            BRepBuilderAPI_MakeFace makeFace(mirrorPlane);
            TopoDS_Shape mirrorFace = makeFace.Shape();

            // 将角度转换为弧度。
            double rotX = m_rotateX * M_PI / 180.0;
            double rotY = m_rotateY * M_PI / 180.0;
            double rotZ = m_rotateZ * M_PI / 180.0;

            // 对镜像平面进行旋转。
            TopoDS_Shape finalPlane = mirrorFace;

            switch (m_planeType)
            {
            case Interface::FITKGeoEnum::PT_XY:
            {
                // XY平面可以绕X轴和Y轴旋转。
                if (!qFuzzyIsNull(rotX))
                {
                    gp_Ax1 axisX(mirrorOrigin, gp_Dir(1, 0, 0));
                    gp_Trsf trsfX;
                    trsfX.SetRotation(axisX, rotX);
                    BRepBuilderAPI_Transform transformX(finalPlane, trsfX, true);
                    finalPlane = transformX.Shape();
                }

                if (!qFuzzyIsNull(rotY))
                {
                    gp_Ax1 axisY(mirrorOrigin, gp_Dir(0, 1, 0));
                    gp_Trsf trsfY;
                    trsfY.SetRotation(axisY, rotY);
                    BRepBuilderAPI_Transform transformY(finalPlane, trsfY, true);
                    finalPlane = transformY.Shape();
                }

                break;
            }
            case Interface::FITKGeoEnum::PT_XZ:
            {
                // XZ平面可以绕X轴和Z轴旋转。
                if (!qFuzzyIsNull(rotX))
                {
                    gp_Ax1 axisX(mirrorOrigin, gp_Dir(1, 0, 0));
                    gp_Trsf trsfX;
                    trsfX.SetRotation(axisX, rotX);
                    BRepBuilderAPI_Transform transformX(finalPlane, trsfX, true);
                    finalPlane = transformX.Shape();
                }

                if (!qFuzzyIsNull(rotZ))
                {
                    gp_Ax1 axisZ(mirrorOrigin, gp_Dir(0, 0, 1));
                    gp_Trsf trsfZ;
                    trsfZ.SetRotation(axisZ, rotZ);
                    BRepBuilderAPI_Transform transformZ(finalPlane, trsfZ, true);
                    finalPlane = transformZ.Shape();
                }

                break;
            }
            case Interface::FITKGeoEnum::PT_YZ:
            {
                // YZ平面可以绕Y轴和Z轴旋转。
                if (!qFuzzyIsNull(rotY))
                {
                    gp_Ax1 axisY(mirrorOrigin, gp_Dir(0, 1, 0));
                    gp_Trsf trsfY;
                    trsfY.SetRotation(axisY, rotY);
                    BRepBuilderAPI_Transform transformY(finalPlane, trsfY, true);
                    finalPlane = transformY.Shape();
                }

                if (!qFuzzyIsNull(rotZ))
                {
                    gp_Ax1 axisZ(mirrorOrigin, gp_Dir(0, 0, 1));
                    gp_Trsf trsfZ;
                    trsfZ.SetRotation(axisZ, rotZ);
                    BRepBuilderAPI_Transform transformZ(finalPlane, trsfZ, true);
                    finalPlane = transformZ.Shape();
                }

                break;
            }
            default:
                break;
            }

            // 从旋转后的平面提取法向量，创建镜像轴。
            Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(finalPlane));
            Handle(Geom_Plane) geomPlane = Handle(Geom_Plane)::DownCast(surface);
            if (geomPlane.IsNull())
            {
                printLog(tr("Failed to get plane surface!"), 3);
                return false;
            }

            // 创建镜像变换。
            gp_Ax2 ax2;
            ax2.SetAxis(geomPlane->Axis());

            gp_Trsf mirrorTrsf;
            mirrorTrsf.SetMirror(ax2);

            // 应用镜像变换到形状。
            BRepBuilderAPI_Transform transformer(*shape, mirrorTrsf, true);
            TopoDS_Shape mirroredShape = transformer.Shape();

            if (m_keepOrigin)
            {
                TopoDS_Shape shapes;
                FITKOCCComandCommon::MergeShapes(*shape, mirroredShape, shapes);
                mirroredShape = shapes;
            }

            part->updatePartShape(mirroredShape, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        catch (...)
        {
            printLog(tr("Failed to mirror shape!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelRectangularPatternPart::FITKOCCModelRectangularPatternPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRectangularPatternPart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        try
        {
            BRep_Builder builder;
            TopoDS_Compound comp;
            builder.MakeCompound(comp);

            builder.Add(comp, *shape);

            std::array<double, 3> dir1 = firstDirection();
            std::array<double, 3> dir2 = secondDirection();

            int n1 = std::max(1, firstCount());
            int n2 = std::max(1, secondCount());

            double step1 = 0.0;
            if (firstDefinition() == FITKAbsGeoModelRectangularPatternPart::Total)
            {
                step1 = n1 > 1 ? firstTotal() / (n1 - 1) : 0.0;
            }
            else
            {
                step1 = firstSpacing();
            }

            double step2 = 0.0;
            if (secondDefinition() == FITKAbsGeoModelRectangularPatternPart::Total)
            {
                step2 = n2 > 1 ? secondTotal() / (n2 - 1) : 0.0;
            }
            else
            {
                step2 = secondSpacing();
            }

            for (int i = 0; i < n1; ++i)
            {
                for (int j = 0; j < n2; ++j)
                {
                    if (i == 0 && j == 0)
                    {
                        continue;
                    }

                    double dx = dir1[0] * step1 * i + dir2[0] * step2 * j;
                    double dy = dir1[1] * step1 * i + dir2[1] * step2 * j;
                    double dz = dir1[2] * step1 * i + dir2[2] * step2 * j;

                    gp_Trsf trans;
                    trans.SetTranslation(gp_Vec(dx, dy, dz));
                    BRepBuilderAPI_Transform trf(*shape, trans, true);
                    builder.Add(comp, trf.Shape());
                }
            }

            part->updatePartShape(comp, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        catch (...)
        {
            printLog(tr("Failed to rectangular-pattern shape!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelCircularPatternPart::FITKOCCModelCircularPatternPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCircularPatternPart::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* occModel = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        Interface::FITKAbsVirtualTopo* axisTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(axis().VirtualTopoId);
        if (!axisTopo)
        {
            return false;
        }

        try
        {
            BRep_Builder builder;
            TopoDS_Compound comp;
            builder.MakeCompound(comp);
            builder.Add(comp, *shape);

            FITKOCCTopoShape* occAxisShape = axisTopo->getShapeT<FITKOCCTopoShape>();
            if (!occAxisShape)
            {
                return false;
            }

            TopoDS_Shape axisShape = occAxisShape->getTopoShape();
            if (axisShape.IsNull() || axisShape.ShapeType() != TopAbs_EDGE)
            {
                return false;
            }

            TopoDS_Edge edge = TopoDS::Edge(axisShape);
            Standard_Real first = 0.0;
            Standard_Real last = 0.0;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve.IsNull())
            {
                return false;
            }

            gp_Pnt p1 = curve->Value(first);
            gp_Pnt p2 = curve->Value(last);
            gp_Dir axisDir(gp_Vec(p1, p2));
            if (isReverse())
            {
                axisDir.Reverse();
            }

            gp_Ax1 axisAx(p1, axisDir);

            int n = std::max(1, axisCount());
            double step = 0.0;
            if (firstDefinition() == FITKAbsGeoModelCircularPatternPart::Total)
            {
                step = n > 1 ? axisTotal() / (n - 1) : 0.0;
            }
            else
            {
                step = axisSpacing();
            }

            for (int i = 1; i < n; ++i)
            {
                double angleDeg = step * i;
                gp_Trsf trans;
                trans.SetRotation(axisAx, angleDeg * M_PI / 180.0);
                BRepBuilderAPI_Transform trf(*shape, trans, true);
                builder.Add(comp, trf.Shape());
            }

            part->updatePartShape(comp, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        catch (...)
        {
            printLog(tr("Failed to circular-pattern shape!"), 3);
            return false;
        }

        return true;
    }
}
