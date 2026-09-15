/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCShapeTriangulate.h"
#include "FITKAbstractOCCModel.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"
#include <TopoDS_Shape.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <Standard_Version.hxx>
#include <QDebug>
#include <QDateTime>

namespace OCC
{
    FITKOCCShapeTriangulate::FITKOCCShapeTriangulate(FITKAbstractOCCModel* occModel) : _occModel(occModel)
    {
        // 初始化
        if (occModel == nullptr) return;
        // 从模型中获取需要处理的对象
        _vtmanager = occModel->getVirtualTopoManager();
        _meshVS = occModel->getMeshVS();
    }

    void FITKOCCShapeTriangulate::triangulate()
    {
        qDebug() << "tri: " << QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

        //错误判断
        if (_meshVS == nullptr || _vtmanager == nullptr || _occModel == nullptr) return;

        // 清除历史三角面片数据。
        _meshVS->clear();

        // 整体三角面片化。
        this->triangulate(*_occModel->getShape());

        // 遍历点虚拟拓扑数据
        Interface::FITKShapeVirtualTopoManager* sm =
            _vtmanager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSPoint);
        if (sm != nullptr)
            for (int i = 0; i < sm->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
                if (vtopo == nullptr) continue;
                FITKOCCTopoShape* occShape = vtopo->getShapeT<FITKOCCTopoShape>();
                if (occShape == nullptr) continue;
                const TopoDS_Shape& shape = occShape->getTopoShape();
                if (shape.IsNull()) continue;
                const int id = vtopo->getDataObjectID();
                this->discretePoint(id, shape);
            }

        // 遍历边虚拟拓扑数据
        sm = _vtmanager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        if (sm != nullptr)
            for (int i = 0; i < sm->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
                if (vtopo == nullptr) continue;
                FITKOCCTopoShape* occShape = vtopo->getShapeT<FITKOCCTopoShape>();
                if (occShape == nullptr) continue;
                const TopoDS_Shape& shape = occShape->getTopoShape();
                if (shape.IsNull()) continue;
                const int id = vtopo->getDataObjectID();
                this->discreteEdge(id, shape);
            }
        // 遍历面虚拟拓扑数据
        sm = _vtmanager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace);
        if (sm != nullptr)
            for (int i = 0; i < sm->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vtopo = sm->getDataByIndex(i);
                if (vtopo == nullptr) continue;
                FITKOCCTopoShape* occShape = vtopo->getShapeT<FITKOCCTopoShape>();
                if (occShape == nullptr) continue;
                const TopoDS_Shape& shape = occShape->getTopoShape();
                if (shape.IsNull()) continue;
                const int id = vtopo->getDataObjectID();
                this->discreteFace(id, shape);
            }
        qDebug() << "tri finished: " << QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

    }

    void FITKOCCShapeTriangulate::discretePoint(const int id, const TopoDS_Shape& shape)
    {
        //记录点坐标
        TopoDS_Vertex vertex = TopoDS::Vertex(shape);
        gp_Pnt pt = BRep_Tool::Pnt(vertex);
        Interface::FITKGeoMeshVSPt * vpt = new Interface::FITKGeoMeshVSPt();
        vpt->setXYZ(pt.X(), pt.Y(), pt.Z());
        _meshVS->insertPoint(id, vpt);
    }

    void FITKOCCShapeTriangulate::discreteEdge(const int id, const TopoDS_Shape& shape)
    {
        //三角化边
        this->triangulate(shape, 0.0005);
        TopLoc_Location loc;
        const TopoDS_Edge & edge = TopoDS::Edge(shape);
        //获取离散点
        const Handle(Poly_Polygon3D) & mesh = BRep_Tool::Polygon3D(edge, loc);
        if (!mesh) return;
        const int nPts = mesh->NbNodes();
        gp_Trsf trans(loc);
        Interface::FITKGeoMeshVSEdgeEntity* vsedge = new Interface::FITKGeoMeshVSEdgeEntity;

        const TColgp_Array1OfPnt & nodes = mesh->Nodes();
        for (Standard_Integer i = 1; i <= nPts; i++)
        {
            //记录点坐标
            gp_Pnt pt = nodes.Value(i);
            pt.Transform(trans);
            Interface::FITKGeoMeshVSPt * vpt = new Interface::FITKGeoMeshVSPt();
            vpt->setXYZ(pt.X(), pt.Y(), pt.Z());
            vsedge->appendPoint(vpt);
        }

        //记录边
        _meshVS->insertEdge(id, vsedge);
    }

    void FITKOCCShapeTriangulate::discreteFace(const int id, const TopoDS_Shape& shape)
    {
        //三角化面
        // this->triangulate(shape);
        //获取面
        TopLoc_Location loc;
        const TopoDS_Face & face = TopoDS::Face(shape);
        const Handle(Poly_Triangulation) & mesh = BRep_Tool::Triangulation(face, loc);
        if (!mesh) return;

        int nPts = mesh->NbNodes();
        int nCells = mesh->NbTriangles();

        gp_Trsf trans(loc);
        Interface::FITKGeoMeshVSFaceEntity* vsface = new Interface::FITKGeoMeshVSFaceEntity;

#if OCC_VERSION_HEX < 0x070600
        const TColgp_Array1OfPnt & nodes = mesh->Nodes();
        const Poly_Array1OfTriangle & triangles = mesh->Triangles();

        //记录点坐标      
        for (Standard_Integer i = 1; i <= nPts; i++)
        {
            gp_Pnt pt = nodes.Value(i);
            pt.Transform(trans);
            Interface::FITKGeoMeshVSPt * vpt = new Interface::FITKGeoMeshVSPt();
            vpt->setXYZ(pt.X(), pt.Y(), pt.Z());
            vsface->appendPoint(vpt);
        }

        //记录三角形
        for (Standard_Integer i = 1; i <= nCells; i++)
        {
            Poly_Triangle tri = triangles.Value(i);
            int pt1{ -1 }, pt2{ -1 }, pt3{ -1 };
            tri.Get(pt1, pt2, pt3);
            //判断方向
            if (face.Orientation() == TopAbs_REVERSED)
            {
                Interface::FITKGeoMeshVSTri * vsTri = new Interface::FITKGeoMeshVSTri(pt3 - 1, pt2 - 1, pt1 - 1);
                vsface->appendMeshTri(vsTri);
            }
            else
            {
                Interface::FITKGeoMeshVSTri * vsTri = new Interface::FITKGeoMeshVSTri(pt1 - 1, pt2 - 1, pt3 - 1);
                vsface->appendMeshTri(vsTri);
            }
        }
#else       
        //记录点坐标
        for (Standard_Integer i = 1; i <= nPts; i++)
        {
            gp_Pnt pt = mesh->Node(i);
            pt.Transform(trans);
            Interface::FITKGeoMeshVSPt * vpt = new Interface::FITKGeoMeshVSPt();
            vpt->setXYZ(pt.X(), pt.Y(), pt.Z());
            vsface->appendPoint(vpt);
        }

        //记录三角形
        for (Standard_Integer i = 1; i <= nCells; i++)
        {
            Poly_Triangle tri = mesh->Triangle(i);
            int pt1{ -1 }, pt2{ -1 }, pt3{ -1 };
            tri.Get(pt1, pt2, pt3);
            //判断方向
            if (face.Orientation() == TopAbs_REVERSED)
            {
                Interface::FITKGeoMeshVSTri * vsTri = new Interface::FITKGeoMeshVSTri(pt3 - 1, pt2 - 1, pt1 - 1);
                vsface->appendMeshTri(vsTri);
            }
            else
            {
                Interface::FITKGeoMeshVSTri * vsTri = new Interface::FITKGeoMeshVSTri(pt1 - 1, pt2 - 1, pt3 - 1);
                vsface->appendMeshTri(vsTri);
            }
        }
#endif

        //记录面对象
        _meshVS->insertFace(id, vsface);
    }

    void FITKOCCShapeTriangulate::triangulate(const TopoDS_Shape & shape, double factor)
    {
        if (shape.IsNull())
        {
            return;
        }

        // 获取包围盒尺寸。
        try
        {
            Bnd_Box box;
            BRepBndLib::Add(shape, box);
            double xMin, xMax, yMin, yMax, zMin, zMax;
            box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
            double sizeFactor = sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2));
            double lineDef = factor * sizeFactor;

            // 划分参数。
            IMeshTools_Parameters params;
            params.Deflection = lineDef;

            // 网格划分。（三角面片化）
            BRepMesh_IncrementalMesh(shape, params);
        }
        catch (...)
        {
            qWarning() << "Error occured while tessellating!";
        }
    }
}


