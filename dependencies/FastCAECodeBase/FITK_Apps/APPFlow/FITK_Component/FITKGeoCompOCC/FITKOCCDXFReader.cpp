/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCDXFReader.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <TopoDS_Shape.hxx> 
#include <TopoDS_Compound.hxx> 
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx> 
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <dime/Input.h>
#include <dime/Model.h>
#include <dime/sections/EntitiesSection.h>
#include <dime/entities/Arc.h>
#include <dime/entities/Circle.h>
#include <dime/entities/Ellipse.h>
#include <dime/entities/LWPolyline.h>
#include <dime/entities/Line.h>
#include <dime/entities/Point.h>
#include <dime/entities/Polyline.h>
#include <dime/entities/Text.h>
#include <dime/util/Linear.h>
#include <dime/entities/ExtrusionEntity.h>
#include <QFile>
#include <QDebug>

namespace OCC
{
    FITKOCCDXFReader::FITKOCCDXFReader(const QString &fileName)
        :_fileName(fileName)
    {
        _resShape = new TopoDS_Compound;
    }

    FITKOCCDXFReader::~FITKOCCDXFReader()
    {
        if (_resShape != nullptr) delete _resShape;
    }

    bool FITKOCCDXFReader::read()
    {
        //文件正确性判断
        if (!QFile::exists(_fileName)) return false;
        QByteArray ba = _fileName.toLocal8Bit();
        //读取文件
        dimeInput dimeIn;
        dimeIn.setFile(ba.data());
        dimeModel model;
        bool ok = false;

        try {
            ok = model.read(&dimeIn);
        }
        catch (...) {
            return false;
        }

        if (!ok) return false;

        //  const int numLayers = model.getNumLayers();
        dimeSection* section = model.findSection("ENTITIES");
        //获取绘图部分
        dimeEntitiesSection* entities = dynamic_cast<dimeEntitiesSection*>(section);
        if (entities == nullptr)return false;

        BRep_Builder aBuilder;
        aBuilder.MakeCompound(*_resShape);

        int numEntities = entities->getNumEntities();
        //  QString name = entities->getSectionName();
        for (int i = 0; i < numEntities; ++i)
        {
            dimeEntity * entity = entities->getEntity(i);
            if (entity == nullptr)continue;

            int flag = entity->getEntityFlags();
            QString entityName = QString(entity->getEntityName());

            if (entityName == "POINT")
            {
                this->readPoint(&aBuilder, entity);
            }
            else if (entityName == "LINE")
            {
                this->readLine(&aBuilder, entity);
            }
            else if (entityName == "CIRCLE")
            {
                this->readCircle(&aBuilder, entity);
            }
            else if (entityName == "ARC")
            {
                this->readArc(&aBuilder, entity);
            }
            else if (entityName == "ELLIPSE")
            {
                this->readEllipse(&aBuilder, entity);
            }
            else if (entityName == "LWPOLYLINE")
            {
                this->readLWPolyLine(&aBuilder, entity);
            }
            else
            {
                AppFrame::FITKMessageWarning(QString(QObject::tr("UnSupport dxf entity type: %1 ")).arg(entityName));
            }



            // "POINT", "LINE", "ARC", "CIRCLE", "ELLIPSE", "LWPOLYLINE", "POLYLINE"
        }
        return true;
    }

    const TopoDS_Shape & FITKOCCDXFReader::getShape()
    {
        return *_resShape;
    }

    void FITKOCCDXFReader::readPoint(BRep_Builder* builder, dimeEntity* entity)
    {
        dimePoint* point = dynamic_cast<dimePoint*>(entity);
        if (point == nullptr) return;
        //坐标
        dimeVec3f coor = point->getCoords();
        gp_Pnt pnt(coor.x, coor.y, coor.z);
        try
        {
            builder->Add(*_resShape, BRepBuilderAPI_MakeVertex(pnt).Shape());

        }
        catch (...) {}
    }

    void FITKOCCDXFReader::readLine(BRep_Builder* builder, dimeEntity* entity)
    {
        dimeLine *line = dynamic_cast<dimeLine*>(entity);
        if (line == nullptr) return;
        //起点
        dimeVec3f coord_star = line->getCoords(0);
        //终点
        dimeVec3f coord_end = line->getCoords(1);

        gp_Pnt pnt1(coord_star.x, coord_star.y, coord_star.z);
        gp_Pnt pnt2(coord_end.x, coord_end.y, coord_end.z);
        //创建线并且追加到组合体
        try {
            builder->Add(*_resShape, BRepBuilderAPI_MakeEdge(pnt1, pnt2).Shape());
        }
        catch (...) {}
    }

    void FITKOCCDXFReader::readCircle(BRep_Builder* builder, dimeEntity* entity)
    {
        dimeCircle* circle = dynamic_cast<dimeCircle*>(entity);
        if (circle == nullptr) return;
        dimeVec3f center = circle->getCenter();
        double r = circle->getRadius();
        dimeVec3f dir = circle->getExtrusionDir();
        //圆形几何表达
        gp_Ax2 ax(gp_Pnt(center.x, center.y, center.z), gp_Dir(dir.x, dir.y, dir.z));
        gp_Circ cir(ax, r);
        //创建圆形
        try {
            builder->Add(*_resShape, BRepBuilderAPI_MakeEdge(cir).Shape());
        }
        catch (...) {}
    }

    void FITKOCCDXFReader::readArc(BRep_Builder* builder, dimeEntity* entity)
    {
        dimeArc *arc = dynamic_cast<dimeArc*>(entity);
        if (arc == nullptr) return;
        //获取信息
        dimeVec3f center;
        arc->getCenter(center);
        const double r = arc->getRadius();
        const double starAng = arc->getStartAngle();
        const double endAng = arc->getEndAngle();
        dimeVec3f dir = arc->getExtrusionDir();
        //计算局部坐标系
        gp_Pnt ori(center.x, center.y, center.z);
        gp_Dir z(dir.x, dir.y, dir.z);
        gp_Dir x;
        if (fabs(dir.x) < 1.0 / 64.0 && fabs(dir.x) < 1.0 / 64.0) //  1/64 阈值来自手册
            x = gp_Dir(0, 1, 0).Crossed(z);
        else x = gp_Dir(0, 0, 1).Crossed(z);

        //整圆几何表达
        gp_Ax2 ax(ori, z, x);
        gp_Circ cir(ax, r);
        //创建圆弧
        try {
            builder->Add(*_resShape, BRepBuilderAPI_MakeEdge(cir, starAng* M_PI / 180.0, endAng * M_PI / 180.0).Shape());
        }
        catch (...) {}
    }

    void FITKOCCDXFReader::readEllipse(BRep_Builder* builder, dimeEntity* entity)
    {
        dimeEllipse* ellipse = dynamic_cast<dimeEllipse*>(entity);
        if (ellipse == nullptr) return;

        dimeVec3f center = ellipse->getCenter();
        dimeVec3f majEnd = ellipse->getMajorAxisEndpoint();
        double ratio = ellipse->getMinorMajorRatio();
        dimeVec3f dir = ellipse->getExtrusionDir();
        double startAng = ellipse->getStartParam();
        double endAng = ellipse->getEndParam();

        //计算短轴终点
        gp_Pnt ori(center.x, center.y, center.z);
        gp_Pnt endMaj(majEnd.x, majEnd.y, majEnd.z);
        gp_Vec z(dir.x, dir.y, dir.z);
        gp_Vec maj = gp_Vec(majEnd.x - center.x, majEnd.y - center.y, majEnd.z - center.z);
        gp_Vec min = z.Crossed(maj);
        min.Normalize();
        double hafMajLength = ori.Distance(endMaj);
        gp_Pnt endMin(ori.X() + min.X()*hafMajLength* ratio, ori.Y() + min.Y()*hafMajLength* ratio,
            ori.Z() + min.Z()*hafMajLength* ratio);
        //局部坐标系
        //计算局部坐标系
//         gp_Pnt o(center.x, center.y, center.z);
//         gp_Dir zd(dir.x, dir.y, dir.z);
//         gp_Dir x;
//         if (fabs(dir.x) < 1.0 / 64.0 && fabs(dir.x) < 1.0 / 64.0) //  1/64 阈值来自手册
//             x = gp_Dir(0, 1, 0).Crossed(zd);
//         else x = gp_Dir(0, 0, 1).Crossed(zd);

        //整椭圆几何表达
        gp_Ax2 ax(ori, z, maj);
        gp_Elips elip(ax, ori.Distance(endMaj), ori.Distance(endMin));
        //创建椭圆弧
        try {
            builder->Add(*_resShape, BRepBuilderAPI_MakeEdge(elip, startAng, endAng).Shape());
        }
        catch (...) {}


    }

    void FITKOCCDXFReader::readLWPolyLine(BRep_Builder* builder, dimeEntity* entity)
    {
        dimeLWPolyline * polyLine = dynamic_cast<dimeLWPolyline*>(entity);
        if (polyLine == nullptr) return;
        const int nPt = polyLine->getNumVertices();
        //最少两个点
        if (nPt < 2) return;
        const dxfdouble* xCoor = polyLine->getXCoords();
        const dxfdouble* yCoor = polyLine->getYCoords();
        int flag = polyLine->getFlags();

        for (int index = 1; index < nPt; ++index)
        {
            try
            {
                gp_Pnt pnt1(xCoor[index - 1], yCoor[index - 1], 0);
                gp_Pnt pnt2(xCoor[index], yCoor[index], 0);
                //创建线并且追加到组合体
                builder->Add(*_resShape, BRepBuilderAPI_MakeEdge(pnt1, pnt2).Shape());

            }
            catch (...)
            {

            }
        }




    }

}



