/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKThreadTaskGroup.h"
#include "FITKAbstractOCCModel.h"
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

    FITKOCCVirtualTopoCreator::FITKOCCVirtualTopoCreator(const TopoDS_Shape& shape, FITKAbstractOCCModel* shapeAgent,
        const QString& model, bool& r, bool keepTopos)
        :_shape(shape), _shapeAgent(shapeAgent), _modelName(model), _runLabel(r), _keepTopos(keepTopos)
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
        if (_shapeAgent == nullptr) return;

        Interface::FITKVirtualTopoManager* vTopoMgr = _shapeAgent->getVirtualTopoManager();
        if (!vTopoMgr)
        {
            return;
        }

        if (_shape.IsNull()) return;

        // 获取当前拆分拓扑源数据ID。
        int cmdId = -1;
        Interface::FITKAbsGeoCommand* cmd = _shapeAgent->getGeoCommand();
        Interface::FITKGeoCommandManager* cmdMgr = dynamic_cast<Interface::FITKGeoCommandManager*>(cmd);
        if (cmdMgr)
        {
            cmdId = cmdMgr->getIDByIndex(cmdMgr->getDataCount() - 1);
        }
        else
        {
            cmdId = cmd != nullptr ? cmd->getDataObjectID() : -1;
        }

        // 创建生成虚拓扑任务。
        FITKOCCVirtualTopoMapper(_shape, _shapeAgent, _virtualTopoHash, _keepTopos, cmdId).run();
    }

    FITKOCCVirtualTopoMapper::FITKOCCVirtualTopoMapper(const TopoDS_Shape& shape, FITKAbstractOCCModel* shapeAgent,
        QHash<Interface::FITKGeoEnum::VTopoShapeType, QHash<int, Interface::FITKAbsVirtualTopo*>> &topos,
        bool keepTopos, int cmdId)
        : _shape(shape), _topos(topos), _keepTopos(keepTopos), m_cmdId(cmdId)
    {
        if (shapeAgent)
        {
            _topoMgr = shapeAgent->getVirtualTopoManager();
        }

        // 创建字典。
        _maps = new _IndiceMaps;

        // 初始化形状字典。
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_VERTEX, _maps->_hash[TopAbs_ShapeEnum::TopAbs_VERTEX]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_EDGE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_EDGE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_WIRE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_WIRE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_FACE, _maps->_hash[TopAbs_ShapeEnum::TopAbs_FACE]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_SHELL, _maps->_hash[TopAbs_ShapeEnum::TopAbs_SHELL]);
        TopExp::MapShapes(_shape, TopAbs_ShapeEnum::TopAbs_SOLID, _maps->_hash[TopAbs_ShapeEnum::TopAbs_SOLID]);

        // 几何数据预处理：保留虚拓扑对象。
        if (_keepTopos)
        {
            for (int i = Interface::FITKGeoEnum::VTopoShapeType::VSPoint;
                i < Interface::FITKGeoEnum::VTopoShapeType::VSAssembly; i++) // 不检查组合体拓扑。
            {
                Interface::FITKShapeVirtualTopoManager* vTopoMgr = _topoMgr->
                    getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)i);
                if (!vTopoMgr)
                {
                    continue;
                }

                int nVTopos = vTopoMgr->getDataCount();
                for (int j = nVTopos - 1; j >= 0; j--)
                {
                    Interface::FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByIndex(j);

                    // 暂时跳过合并拓扑对象。
                    if (vTopo->getCombinedTopoCount() != 0)
                    {
                        continue;
                    }

                    int sHash = vTopo->getTag();
                    if (sHash <= 0)
                    {
                        continue;
                    }

                    FITKOCCTopoShape* vShape = vTopo->getShapeT<FITKOCCTopoShape>();
                    if (!vShape)
                    {
                        continue;
                    }

                    //const TopoDS_Shape & s = vShape->getTopoShape();

                    // 移除并保存对象至拓扑字典。
                    vTopoMgr->removeDataObjWithoutRelease(vTopo);

                    // 清理缓存。
                    vTopo->clearCombinedTopo();
                    vTopo->clearParentTopo();
                    vTopo->clearSubTopo();
                    _orgKeptTopoHash.insert(sHash, vTopo);
                }
            }
        }

        _topoMgr->clear();

        // 顶层形状类型
        TopAbs_ShapeEnum shapeEnum = _shape.ShapeType();
        Interface::FITKGeoEnum::VTopoShapeType vts = OCCShapeEnumHash.value(shapeEnum);

        // 创建根节点。
        _rootTopo = new Interface::FITKAbsVirtualTopo;
        _rootTopo->setShapeType(vts);
        _rootTopo->setShape(new FITKOCCTopoShape(_shape));
        _topoMgr->setRootObj(_rootTopo);
        _topoMgr->appendVirtualTopoObj(_rootTopo);
        _rootTopo->setIndexLabel(0);

        int hashInt = _shape.HashCode(std::numeric_limits<int>::max());
        topos[vts][hashInt] = _rootTopo;
    }

    FITKOCCVirtualTopoMapper::~FITKOCCVirtualTopoMapper()
    {
        // 析构字典列表。
        if (_maps)
        {
            delete _maps;
            _maps = nullptr;
        }

        for (Interface::FITKAbsVirtualTopo* vTopo : _orgKeptTopoHash.values())
        {
            if (vTopo)
            {
                delete vTopo;
            }
        }

        _orgKeptTopoHash.clear();
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

                Interface::FITKAbsVirtualTopo* vTopo = _topos[subCvts].value(sHash, nullptr);
                if (vTopo)
                {
                    if (parent)
                    {
                        parent->addSubTopo(vTopo);
                    }

                    continue;
                }
                
                vTopo = _orgKeptTopoHash.take(sHash);
                if (vTopo)
                {
                    if (vTopo->getShapeType() != subCvts)
                    {
                        delete vTopo;
                        vTopo = nullptr;
                    }
                }

                // 存在历史拓扑。
                if (vTopo)
                {
                    Interface::FITKAbsVirtualTopoShape* vs = vTopo->getShape();
                    if (vs)
                    {
                        delete vs;
                    }
                }
                else
                {
                    // 创建虚拓扑对象并进行临时存储。
                    vTopo = new Interface::FITKAbsVirtualTopo;
                    vTopo->setShapeType(subCvts);
                    vTopo->setCreatedGeoCmdID(m_cmdId);
                }

                vTopo->setShape(new FITKOCCTopoShape(subShape));
                vTopo->setTag(sHash);
                _topos[subCvts][sHash] = vTopo;

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

                // 保存到列表。
                topos.push_back(vTopo);

                if (parent)
                {
                    parent->addSubTopo(vTopo);
                }

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

                Interface::FITKAbsVirtualTopo* vTopo = _topos[subCvts].value(sHash, nullptr);
                if (vTopo)
                {
                    if (parent)
                    {
                        parent->addSubTopo(vTopo);
                    }

                    continue;
                }

                vTopo = _orgKeptTopoHash.take(sHash);
                if (vTopo)
                {
                    if (vTopo->getShapeType() != subCvts)
                    {
                        delete vTopo;
                        vTopo = nullptr;
                    }
                }

                // 存在历史拓扑。
                if (vTopo)
                {
                    Interface::FITKAbsVirtualTopoShape* vs = vTopo->getShape();
                    if (vs)
                    {
                        delete vs;
                    }
                }
                else
                {
                    // 创建虚拓扑对象并进行临时存储。
                    vTopo = new Interface::FITKAbsVirtualTopo;
                    vTopo->setShapeType(subCvts);
                    vTopo->setCreatedGeoCmdID(m_cmdId);
                }

                int indexLabel = map.FindIndex(subShape);
                vTopo->setShape(new FITKOCCTopoShape(subShape));
                vTopo->setIndexLabel(indexLabel - 1);
                vTopo->setTag(sHash);
                _topos[subCvts][sHash] = vTopo;

                // 保存到列表。
                topos.push_back(vTopo);
                if (parent)
                {
                    parent->addSubTopo(vTopo);
                }

                // 递归拆分。
                recurCreateTopos(subShape, topos, vTopo, maps);
            }
        }
    }
}
