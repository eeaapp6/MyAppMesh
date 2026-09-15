/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKThreadTaskGroup.h"
#include <TopExp_Explorer.hxx>
#include <QHash>
#include <QList>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Iterator.hxx>
#include <QDateTime>
#include <QDebug>

namespace OCC
{
    /**
     * @brief       内部结构体 - 形状字典。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-21
     */
    struct _IndiceMaps
    {
    public:
        _IndiceMaps()
        {
            _hash.insert(TopAbs_ShapeEnum::TopAbs_VERTEX, iVert);
            _hash.insert(TopAbs_ShapeEnum::TopAbs_EDGE, iEdge);
            _hash.insert(TopAbs_ShapeEnum::TopAbs_WIRE, iWire);
            _hash.insert(TopAbs_ShapeEnum::TopAbs_FACE, iFace);
            _hash.insert(TopAbs_ShapeEnum::TopAbs_SHELL, iShell);
            _hash.insert(TopAbs_ShapeEnum::TopAbs_SOLID, iSolid);
        }

        //TopTools_IndexedMapOfShape & getIndexMap(TopAbs_ShapeEnum type)
        //{
        //    return _hash[type];
        //}

    public:
        TopTools_IndexedMapOfShape iVert;
        TopTools_IndexedMapOfShape iEdge;
        TopTools_IndexedMapOfShape iWire;
        TopTools_IndexedMapOfShape iFace;
        TopTools_IndexedMapOfShape iShell;
        TopTools_IndexedMapOfShape iSolid;

        QHash<TopAbs_ShapeEnum, TopTools_IndexedMapOfShape> _hash;

    };

    //几何类型映射
    QHash<TopAbs_ShapeEnum, Interface::FITKGeoEnum::VTopoShapeType>  OCCShapeEnumHash = {
        {TopAbs_COMPOUND, Interface::FITKGeoEnum::VSAssembly},
        {TopAbs_COMPSOLID,Interface::FITKGeoEnum::VSAssembly},
        {TopAbs_SOLID,    Interface::FITKGeoEnum::VSSolid},
        {TopAbs_SHELL,    Interface::FITKGeoEnum::VSShell},
        {TopAbs_FACE,     Interface::FITKGeoEnum::VSFace},
        {TopAbs_WIRE,     Interface::FITKGeoEnum::VSWire},
        {TopAbs_EDGE,     Interface::FITKGeoEnum::VSEdge},
        {TopAbs_VERTEX,   Interface::FITKGeoEnum::VSPoint},
    };
    //支持的OCC类型，需要注意层次顺序
    QList<TopAbs_ShapeEnum> OCCShapeEnumList = {
        TopAbs_COMPOUND,TopAbs_SOLID,TopAbs_SHELL ,TopAbs_FACE,TopAbs_WIRE, TopAbs_EDGE, TopAbs_VERTEX };


    FITKOCCTopoShape::FITKOCCTopoShape(const TopoDS_Shape & shape)
    {
        _shape = new TopoDS_Shape;
        *_shape = shape;
    }

    bool FITKOCCTopoShape::isSameShape(void* s)
    {
        TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(s);
        return shape->IsEqual(*_shape);
    }

    const TopoDS_Shape& FITKOCCTopoShape::getTopoShape() const
    {
        return *_shape;
    }

    FITKOCCVirtualTopoCreator::FITKOCCVirtualTopoCreator(const TopoDS_Shape& shape, Interface::FITKVirtualTopoManager* topo, const QString& model, bool& r)
        :_shape(shape), _topoMgr(topo), _modelName(model), _runLabel(r)
    {
        _runLabel = true;
    }

    FITKOCCVirtualTopoCreator::~FITKOCCVirtualTopoCreator()
    {
        _runLabel = false;
    }

    void FITKOCCVirtualTopoCreator::run()
    {
        createOCCTopos();
    }

    void FITKOCCVirtualTopoCreator::createOCCTopos()
    {
        if (_topoMgr == nullptr) return;

        if (_shape.IsNull()) return;

        //顶层形状类型
        TopAbs_ShapeEnum shapeEnum = _shape.ShapeType();
        Interface::FITKGeoEnum::VTopoShapeType vts = OCCShapeEnumHash.value(shapeEnum);

        // 创建根节点。
        Interface::FITKAbsVirtualTopo* parent = new Interface::FITKAbsVirtualTopo;
        parent->setShapeType(vts);
        parent->setShape(new FITKOCCTopoShape(_shape));
        _topoMgr->setRootObj(parent);
        _topoMgr->appendVirtualTopoObj(parent);
        parent->setIndexLabel(0);
        int hashInt = _shape.HashCode(std::numeric_limits<int>::max());
        //      _virtualTopoHash.insert(hashInt, parent);
        _virtualTopoHash[vts][hashInt] = parent;

        // 创建生成虚拓扑任务。
        Core::FITKThreadTaskGroup g;
        FITKOCCVirtualTopoMapper* m = new FITKOCCVirtualTopoMapper(_shape, _topoMgr, _virtualTopoHash, parent);
        g.appendThreadTask(m);
        g.push2ThreadPool();
        g.wait();
    }

    FITKOCCVirtualTopoMapper::FITKOCCVirtualTopoMapper(const TopoDS_Shape& shape, Interface::FITKVirtualTopoManager* topomgr,
        QHash<Interface::FITKGeoEnum::VTopoShapeType, QHash<int, Interface::FITKAbsVirtualTopo*>> &topos,
        Interface::FITKAbsVirtualTopo* rootTopo)
        : _shape(shape), _topos(topos), _topoMgr(topomgr), _rootTopo(rootTopo)
    {
        // 创建字典。
        _maps = new _IndiceMaps;

        // 初始化形状字典。
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_VERTEX, _maps->_hash[TopAbs_ShapeEnum::TopAbs_VERTEX]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_EDGE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_EDGE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_WIRE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_WIRE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_FACE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_FACE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_SHELL, _maps->_hash[TopAbs_ShapeEnum::TopAbs_SHELL]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_SOLID, _maps->_hash[TopAbs_ShapeEnum::TopAbs_SOLID]);
    }

    FITKOCCVirtualTopoMapper::~FITKOCCVirtualTopoMapper()
    {
        // 析构字典列表。
        if (_maps)
        {
            delete _maps;
            _maps = nullptr;
        }
    }

    void FITKOCCVirtualTopoMapper::run()
    {
        // 递归生成虚拓扑。
        QList<Interface::FITKAbsVirtualTopo*> topos;
        recurCreateTopos(_shape, topos, _rootTopo, _maps);
        _topoMgr->appendVirtualTopoObjs(topos);
    }

    void FITKOCCVirtualTopoMapper::recurCreateTopos(const TopoDS_Shape & shape, QList<Interface::FITKAbsVirtualTopo*> & topos,
        Interface::FITKAbsVirtualTopo* parent, _IndiceMaps* maps)
    {
        // 组合对象直接拆分形状。
        TopAbs_ShapeEnum shapeType = shape.ShapeType();
        if (shapeType == TopAbs_ShapeEnum::TopAbs_COMPOUND || shapeType == TopAbs_ShapeEnum::TopAbs_COMPSOLID)
        {
            // 遍历其他形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                TopAbs_ShapeEnum subShapeType = subShape.ShapeType();

                const int sHash = subShape.HashCode(std::numeric_limits<int>::max());
                Interface::FITKGeoEnum::VTopoShapeType subCvts = OCCShapeEnumHash.value(subShapeType);          
                Interface::FITKAbsVirtualTopo* vTopo = _topos[subCvts][sHash];
                if (vTopo)
                {
                    continue;
                }

                // 创建虚拓扑对象并进行临时存储。
                vTopo = new Interface::FITKAbsVirtualTopo;
                vTopo->setShapeType(subCvts);
                vTopo->setShape(new FITKOCCTopoShape(subShape));

                if (subShapeType == TopAbs_ShapeEnum::TopAbs_COMPOUND || subShapeType == TopAbs_ShapeEnum::TopAbs_COMPSOLID)
                {
                    // 将已有组合对象数量设置为标签。
                    int indexLabel = _topos[subCvts].count() - 1;
                    vTopo->setIndexLabel(indexLabel);
                }
                else
                {
                    // 检查标签。
                    const TopTools_IndexedMapOfShape & map = maps->_hash[subShapeType];
                    int indexLabel = map.FindIndex(subShape);
                    vTopo->setIndexLabel(indexLabel - 1);
                }

                _topos[subCvts][sHash] = vTopo;

                // 保存到列表。
                topos.push_back(vTopo);

                parent->addSubTopo(vTopo);

                // 递归拆分。
                recurCreateTopos(subShape, topos, vTopo, maps);
            }
        }
        // 普通形状向下拆分。
        else
        {
            TopAbs_ShapeEnum subShapeType = TopAbs_ShapeEnum(shapeType + 1);
            const TopTools_IndexedMapOfShape & map = maps->_hash[subShapeType];

            TopExp_Explorer iter;
            for (iter.Init(shape, subShapeType); iter.More(); iter.Next())
            {
                const TopoDS_Shape & subShape = iter.Value();
                const int sHash = subShape.HashCode(std::numeric_limits<int>::max());
                Interface::FITKGeoEnum::VTopoShapeType subCvts = OCCShapeEnumHash.value(subShapeType);

                // 创建虚拓扑对象并进行临时存储。
                Interface::FITKAbsVirtualTopo* vTopo = _topos[subCvts][sHash];
                if (!vTopo)
                {
                    // 检查标签。   
                    int indexLabel = map.FindIndex(subShape);
                    vTopo = new Interface::FITKAbsVirtualTopo;
                    vTopo->setShapeType(subCvts);
                    vTopo->setShape(new FITKOCCTopoShape(subShape));
                    vTopo->setIndexLabel(indexLabel - 1);
                    _topos[subCvts][sHash] = vTopo;

                    // 保存到列表。
                    topos.push_back(vTopo);
                }

                parent->addSubTopo(vTopo);

                // 递归拆分。
                recurCreateTopos(subShape, topos, vTopo, maps);
            }
        }
    }
}
