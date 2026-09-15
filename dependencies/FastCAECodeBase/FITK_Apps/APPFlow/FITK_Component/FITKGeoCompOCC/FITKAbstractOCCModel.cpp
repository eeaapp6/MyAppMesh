/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCShapeTriangulate.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCModelSketch2D.h"

#include <limits>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <StlAPI.hxx>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QVector3D>
#include <QDebug>

//void printSubShape(Interface::FITKAbsVirtualTopo* topo)
//{
//    if (topo == nullptr) return;
//    qDebug() << topo->getShapeType() << "  " << topo->getDataObjectID();
//
//    const int n = topo->getSubTopoCount();
//    QList<Interface::FITKAbsVirtualTopo*> subtopos;
//    for (int i = 0; i < n; ++i)
//    {
//        auto sub = topo->getSubTopo(i);
//        subtopos.append(sub);
//        qDebug() << sub->getShapeType() << "  " << sub->getDataObjectID();
//
//    }
//    for (auto s : subtopos)
//    {
//        printSubShape(s);
//    }
//
//    qDebug() << "";
//}


//建立映射关系
static QHash<Interface::FITKModelEnum::FITKModelSetType, TopAbs_ShapeEnum> OCCShapeEnums = {
    {Interface::FITKModelEnum::FITKModelSetType::FMSPoint,   TopAbs_VERTEX},
    {Interface::FITKModelEnum::FITKModelSetType::FMSEdge,    TopAbs_EDGE},
    {Interface::FITKModelEnum::FITKModelSetType::FMSSurface, TopAbs_FACE},
    {Interface::FITKModelEnum::FITKModelSetType::FMSSolid,   TopAbs_SOLID} };

namespace OCC
{
    FITKAbstractOCCModel::FITKAbstractOCCModel(Interface::FITKAbsGeoCommand* c)
        :Interface::FITKAbsGeoShapeAgent(c)
    {
        _shape = new TopoDS_Shape;
    }

    FITKAbstractOCCModel::~FITKAbstractOCCModel()
    {
        if (_shape) delete _shape;
    }

    int FITKAbstractOCCModel::GetShapeDim(const TopoDS_Shape & shape)
    {
        if (shape.IsNull())
        {
            return -1;
        }

        // 检查形状是否包含体。
        //@{
        int nSolid = 0;
        TopExp_Explorer solidExp(shape, TopAbs_SOLID);
        while (solidExp.More())
        {
            solidExp.Next();
            nSolid++;
        }

        if (nSolid != 0)
        {
            return 3;
        }
        //@}

        // 检查形状是否包含面。
        //@{
        int nFace = 0;
        TopExp_Explorer faceExp(shape, TopAbs_FACE);
        while (faceExp.More())
        {
            faceExp.Next();
            nFace++;
        }

        if (nFace != 0)
        {
            return 2;
        }
        //@}

        // 检查形状是否包含面。
        //@{
        int nEdge = 0;
        TopExp_Explorer edgeExp(shape, TopAbs_EDGE);
        while (edgeExp.More())
        {
            edgeExp.Next();
            nEdge++;
        }

        if (nEdge != 0)
        {
            return 1;
        }
        //@}

        // 检查形状是否包含点。
        //@{
        int nVert = 0;
        TopExp_Explorer vertExp(shape, TopAbs_VERTEX);
        while (vertExp.More())
        {
            vertExp.Next();
            nVert++;
        }

        if (nVert != 0)
        {
            return 0;
        }
        //@}

        return -1;
    }

    void FITKAbstractOCCModel::clearShape()
    {
        // 清除模型数据。
        updateShape(TopoDS_Shape());
        
        // 清除缓存。
        if (_meshVS)
        {
            _meshVS->clear();
        }

        if (_vtmanager)
        {
            _vtmanager->clear();
        }
    }

    void FITKAbstractOCCModel::setPart(FITKOCCModelPart* part)
    {
        m_part = part;
    }

    FITKOCCModelPart* FITKAbstractOCCModel::getPart()
    {
        return m_part;
    }

    void FITKAbstractOCCModel::setSketch(FITKOCCModelSketch2D* sketch)
    {
        m_sketch = sketch;
    }

    FITKOCCModelSketch2D* FITKAbstractOCCModel::getSketch()
    {
        return m_sketch;
    }

    bool FITKAbstractOCCModel::writeToFile(const QString & file)
    {
        QString lowerS = file.toLower();
        QByteArray ba = lowerS.toLocal8Bit();
        const char* c = ba.data();
        bool ok = false;
        //导出brep
        if (lowerS.endsWith("brep"))
        {
            ok = BRepTools::Write(*_shape, c);
        }
        //导出stp
        else if (lowerS.endsWith("stp") || lowerS.endsWith("step"))
        {
            STEPControl_Writer awriter;
            awriter.Transfer(*_shape, STEPControl_AsIs);
            ok = awriter.Write(c);
        }
        //写出 igs
        else if (lowerS.endsWith("igs") || lowerS.endsWith("iges"))
        {
            IGESControl_Controller::Init();
            IGESControl_Writer igesWriter;
            igesWriter.AddShape(*_shape);
            ok = igesWriter.Write(c);
        }
        return ok;
    }

    int FITKAbstractOCCModel::getShapeCount(Interface::FITKModelEnum::FITKModelSetType type)
    {
        if (!OCCShapeEnums.contains(type)) return -1;
        TopAbs_ShapeEnum shapeType = OCCShapeEnums.value(type);

        //建立映射
        TopTools_IndexedMapOfShape maps;
        TopExp::MapShapes(*_shape, shapeType, maps);
        return maps.Extent();
    }

    const TopoDS_Shape FITKAbstractOCCModel::getShape(Interface::FITKModelEnum::FITKModelSetType type, const int id)
    {
        if (!OCCShapeEnums.contains(type)) return TopoDS_Shape();
        TopAbs_ShapeEnum shapeType = OCCShapeEnums.value(type);
        //建立映射
        TopTools_IndexedMapOfShape maps;
        TopExp::MapShapes(*_shape, shapeType, maps);
        //key 从1开始
        return maps.FindKey(id);
    }

    TopoDS_Shape* FITKAbstractOCCModel::getShape()
    {
        return _shape;
    }


    Interface::FITKGeoEnum::FITKGeoEngine FITKAbstractOCCModel::getGeoEngine()
    {
        return Interface::FITKGeoEnum::FGEOCC;
    }

    Interface::FITKModelEnum::AbsModelType FITKAbstractOCCModel::getAbsModelType()
    {
        return Interface::FITKModelEnum::AbsModelType::AMTGeometry;
    }

    bool FITKAbstractOCCModel::getBoundaryBox(double* min, double* max)
    {
        if (min == nullptr || max == nullptr || _shape == nullptr) return false;

        if (_shape->IsNull())
        {
            return false;
        }

        try
        {
            //计算包围盒
            Bnd_Box box;
            BRepBndLib::Add(*_shape, box, false);

            gp_Pnt minpt = box.CornerMin();
            gp_Pnt maxpt = box.CornerMax();

            //复制值
            min[0] = minpt.X();
            min[1] = minpt.Y();
            min[2] = minpt.Z();
            max[0] = maxpt.X();
            max[1] = maxpt.Y();
            max[2] = maxpt.Z();
        }
        catch (...)
        {
            qWarning() << "Error occured while calculating geometry bounding box!";
            return false;
        }

        return true;
    }

    void FITKAbstractOCCModel::updateShape(const TopoDS_Shape& shape, bool builVTopo)
    {
        //// 存在部件则更新部件形状。
        //if (m_part)
        //{
        //    // 保存形状与状态值。
        //    *_shape = shape;
        //    _hashCode = shape.HashCode(INT_MAX);

        //    FITKAbstractOCCModel* partShapeAgent = m_part->getTShapeAgent<FITKAbstractOCCModel>();
        //    if (!partShapeAgent)
        //    {
        //        return;
        //    }

        //    partShapeAgent->updateShape(shape);
        //}
        //// 否则视为独立命令填充自身虚拓扑。
        //else
        //{
            // 模型未发生变化不进行虚拓扑重构。
            if (_hashCode != -1 && _shape->HashCode(INT_MAX) == shape.HashCode(INT_MAX))
            {
                return;
            }

            // 保存形状与状态值。
            *_shape = shape;
            _hashCode = shape.HashCode(INT_MAX);

            //正在执行构建拓扑
            if (_buildingTopo) return;

            if (builVTopo)
            {
                _vtmanager->clear();
                _meshVS->clear();
            }

            if (_shape->IsNull()) return;

            if (builVTopo)
            {
                this->buildVirtualTopo();
                this->triangulation();
            }
        //}
    }

    void FITKAbstractOCCModel::buildVirtualTopo(bool keepTopos)
    {
        Q_UNUSED(keepTopos);

        if (_buildingTopo) return;
        _vtmanager->clear();
        QString name;
        if (_command)
            name = _command->getDataObjectName();
        FITKOCCVirtualTopoCreator* creator = new FITKOCCVirtualTopoCreator(*_shape, _vtmanager, name, _buildingTopo);
        creator->createOCCTopos();
        delete creator;
        // Core::FITKThreadPool::getInstance()->execTask(creator);
//        v.creatOCCVirtualTopo(*_shape, _vtmanager);
//        this->triangulation();
    }

    void FITKAbstractOCCModel::triangulation()
    {
        //        this->triangulate(*_shape);
        FITKOCCShapeTriangulate t(this);
        t.triangulate();
    }

    int FITKAbstractOCCModel::getDim()
    {
        return GetShapeDim(*_shape);
    }

    bool FITKAbstractOCCModel::createShapeState(QVariant & stateVal)
    {
        if (!_shape)
        {
            return false;
        }

        stateVal = _shape->HashCode(INT_MAX);
        return true;
    }

    bool FITKAbstractOCCModel::isSameState(QVariant & stateVal_1, QVariant & stateVal_2)
    {
        return stateVal_1.toInt() == stateVal_2.toInt();
    }

    OCCShapeAgent::OCCShapeAgent(Interface::FITKAbsGeoCommand* c)
    {
        _occShapeAgent = new FITKAbstractOCCModel(c);
    }
}
