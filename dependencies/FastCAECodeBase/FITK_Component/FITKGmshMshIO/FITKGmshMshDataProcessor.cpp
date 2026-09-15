/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshMshDataProcessor.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace Gmsh
{

    FITKGmshMshDataReaderProcessor::FITKGmshMshDataReaderProcessor()
    {
        //构造
    }

    FITKGmshMshDataReaderProcessor::~FITKGmshMshDataReaderProcessor()
    {
        //析构
    }

    void FITKGmshMshDataReaderProcessor::setReaderConfig(const ReaderGmshMshConfig & config)
    {
        _readConfig = config;
    }

    const ReaderGmshMshConfig & FITKGmshMshDataReaderProcessor::getReaderConfig()
    {
        //获取读取配置
        return _readConfig;
    }

    void FITKGmshMshDataReaderProcessor::addNode(const int nID, const double x, const double y, const double z)
    {
        //添加Msh文件中节点数据
        if (_nodeCount == _node_coordX.size())
        {
            _node_coordX.resize(100 + _nodeCount);
            _node_coordY.resize(100 + _nodeCount);
            _node_coordZ.resize(100 + _nodeCount);
        }
        _node_coordX[_nodeCount] = x;
        _node_coordY[_nodeCount] = y;
        _node_coordZ[_nodeCount] = z;
        _nodesId.append(nID);
        _nodeMapping.insert(nID, _nodeCount);
        ++_nodeCount;
    }

    void FITKGmshMshDataReaderProcessor::addElement(const int eID, const int eType, const int dim, QList<int>& nodeList)
    {
        if (_readConfig._filterDim.contains(dim)) return;//过滤不需要的维度
        //添加Msh文件中单元数据
        if (_elementCount == _element.size())
        {
            _element.resize(100 + _elementCount);
            _element_type.resize(100 + _elementCount);
        }
        _element_type[_elementCount] = eType;
        _element[_elementCount] = nodeList;
        _elementsId.append(eID);
        _elementMapping.insert(eID, _elementCount);
        ++_elementCount;
        if (_readConfig._isFilterUnusedNode)
        {
            for (int id : nodeList)
                _nodeIdUsed.insert(id);
        }
    }

    void FITKGmshMshDataReaderProcessor::addElementSet(const int dim, QList<int>& elementIDs)
    {
        if (_readConfig._filterDim.contains(dim)) return;//过滤不需要的维度                                                                                                   
        //添加Msh文件中单元集合数据
        if (_setCount == _array_dim.size())
        {
            _array_dim.resize(10 + _setCount);
            _array.resize(10 + _setCount);
        }
        _array_dim[_setCount] = dim;
        _array[_setCount] = elementIDs;
        ++_setCount;
    }

    void FITKGmshMshDataReaderProcessor::update(Interface::FITKUnstructuredMesh * meshData, Interface::FITKComponentManager * componentMgr)
    {
        //更新
        this->mshDtaCheck();
        if (!_readConfig._isExtractData) return;
        this->FITKGmshMshData::update(meshData, componentMgr);
    }

    void FITKGmshMshDataReaderProcessor::mshDtaCheck()
    {
        //移除多余数据
        _node_coordX.erase(_node_coordX.begin() + _nodeCount, _node_coordX.end());
        _node_coordY.erase(_node_coordY.begin() + _nodeCount, _node_coordY.end());
        _node_coordZ.erase(_node_coordZ.begin() + _nodeCount, _node_coordZ.end());
        _element_type.erase(_element_type.begin() + _elementCount, _element_type.end());
        _element.erase(_element.begin() + _elementCount, _element.end());
        _array_dim.erase(_array_dim.begin() + _setCount, _array_dim.end());
        _array.erase(_array.begin() + _setCount, _array.end());
        //Gmsh Msh数据检查 将Msh文件中的数据与映射表结合提取Msh数据
        if (_readConfig._isFilterUnusedNode)
            this->filterUnusedNode();

        //去除节点映射表对Msh数据的影响
        for (QVector<QList<int>>::iterator iter = _element.begin(); iter != _element.end(); ++iter)
        {
            QList<int>& nodeList = *iter;
            for (int i = 0; i < nodeList.size(); ++i)
            {
                int nID = nodeList[i];
                int nIndex = _nodeMapping[nID];
                nodeList[i] = nIndex;
            }
        }
        //去除单元映射表对Msh数据的影响
        for (QVector<QList<int>>::iterator iter = _array.begin(); iter != _array.end(); ++iter)
        {
            QList<int>& elementList = *iter;
            for (int i = 0; i < elementList.size(); ++i)
            {
                int eID = elementList[i];
                int eIndex = _elementMapping[eID];
                elementList[i] = eIndex;
            }
        }
        //清理数据
        _nodesId.clear();
        _elementsId.clear();
        _nodeIdUsed.clear();
        _nodeMapping.clear();
        _elementMapping.clear();
        //过滤包含的低维单元
        if (_readConfig._isFilterHighToLowCells)
            this->filterLowDim();
    }

    void FITKGmshMshDataReaderProcessor::filterUnusedNode()
    {
        if (_nodeIdUsed.size() >= _node_coordX.size()) return;
        //重新记录节点ID
        const QVector<double> x(_node_coordX), y(_node_coordY), z(_node_coordZ);
        _node_coordX.resize(_nodeIdUsed.size());
        _node_coordY.resize(_nodeIdUsed.size());
        _node_coordZ.resize(_nodeIdUsed.size());
        int count = _nodesId.size();
        for (int i = 0, nodeCount = 0; i < count; ++i)
        {
            int id = _nodesId.at(i);
            if (!_nodeIdUsed.contains(id))continue;
            QHash<int, int>::iterator iter = _nodeMapping.find(id);
            int index = iter.value();
            if (index != nodeCount) {
                _node_coordX[nodeCount] = x[index];
                _node_coordY[nodeCount] = y[index];
                _node_coordZ[nodeCount] = z[index];
                iter.value() = nodeCount;
            }
            ++nodeCount;
        }
    }

    void FITKGmshMshDataReaderProcessor::filterLowDim()
    {
        //创建节点与单元的映射
        int nCount = _node_coordX.size();
        QVector<QSet<int>> node_element_mapping(nCount);//节点与单元Index的映射
        QSet<int> noFilterElement;//不需要过滤的单元Index
        QVector<QList<int>> set_array_noFilterElement(_array.size());//集合中不需要过滤的单元
        //维度与各维度网格数据映射
        QList<int> setIndex_dim[4]{};//0-0维 1-1维 2-2维 3-3维
        int max_dim = 0;
        for (int i = 0; i < _array_dim.size(); ++i) {
            int dim = _array_dim[i];
            if (dim < 0 || dim > 3) continue;
            setIndex_dim[dim].append(i);
            if (max_dim < dim)
                max_dim = dim;
        }
        //过滤包含的低维单元函数接口
        auto functionFilterLowDim = [&](int set_index) {
            QList<int>& set_noFilterElement = set_array_noFilterElement[set_index];
            int set_dim = _array_dim[set_index];
            const QList<int>& elementList = _array[set_index];
            if (set_dim == max_dim)
            {//维度是最高维度，初始化映射表
                for (QList<int>::const_iterator eIter = elementList.constBegin(); eIter != elementList.constEnd(); ++eIter) {
                    const QList<int>& nodeList = _element[*eIter];
                    for (int nIndex : nodeList)
                        node_element_mapping[nIndex].insert(*eIter);
                    set_noFilterElement.append(*eIter);
                    noFilterElement.insert(*eIter);
                }
                return;
            }
            //维度不是最高维度，需要根据映射表里的高维单元映射判断单元是否包含
            for (QList<int>::const_iterator eIter = elementList.constBegin(); eIter != elementList.constEnd(); ++eIter) {
                const QList<int>& nodeList = _element[*eIter];
                QVector<QSet<int>> mapper(nodeList.size());//节点周围高阶单元Index
                for (int index = 0; index < nodeList.size(); ++index)
                {
                    int nIndex = nodeList[index];
                    mapper[index] = node_element_mapping[nIndex];
                }
                //判断单元是否被其他单元包含
                QSet<int> intersection = mapper.first();
                for (int i = 1; i < mapper.size(); ++i) {
                    intersection.intersect(mapper[i]);
                }
                //
                if (intersection.isEmpty())
                {//没有包含的节点，不需要过滤，记录到映射表
                    for (int nIndex : nodeList)
                        node_element_mapping[nIndex].insert(*eIter);
                    noFilterElement.insert(*eIter);
                    set_noFilterElement.append(*eIter);
                }
            }
        };
        //执行过滤判断
        for (int dim = max_dim; dim >= 0; --dim)
        {
            QList<int> setIndexs = setIndex_dim[dim];
            for (int set_index : setIndexs)
            {
                functionFilterLowDim(set_index);
            }
        }
        if (noFilterElement.size() == _element.size()) return;
        //根据上述获取的不过滤的单元，重新分配空间
        const QVector<QList<int>> element(_element);
        const QVector<int> element_type(_element_type);
        QHash<int, int> map{};
        _element.resize(noFilterElement.size());
        _element_type.resize(noFilterElement.size());
        for (int i = 0, elementCount = 0; i < element.size(); ++i)
        {//重新记录单元
            if (!noFilterElement.contains(i)) continue;
            if (elementCount != i)
            {
                _element_type[elementCount] = element_type[i];
                _element[elementCount] = element[i];
            }
            map.insert(i, elementCount);
            ++elementCount;
        }
        const QVector<int> set_array_dim(_array_dim);
        _array.clear();
        _array_dim.clear();
        for (int i = 0; i < set_array_noFilterElement.size(); ++i)
        {//重新记录集合
            const QList<int>& elementList = set_array_noFilterElement[i];
            if (elementList.isEmpty()) continue;
            int dim = set_array_dim[i];
            QList<int> eIndexs;
            for (int eIndexOld : elementList)
            {
                eIndexs.append(map[eIndexOld]);
            }
            _array.push_back(eIndexs);
            _array_dim.push_back(dim);
        }
    }

    

    FITKGmshMshDataWriterProcessor::FITKGmshMshDataWriterProcessor()
    {
        //构造
    }

    FITKGmshMshDataWriterProcessor::~FITKGmshMshDataWriterProcessor()
    {
        //析构
    }

}
