/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKNodeList.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include <QMutexLocker>
#include <QDebug>

 

namespace Interface
{
    FITKNodeList::~FITKNodeList()
    {
        this->removeAllNode();
    }

    int FITKNodeList::addNode(Core::FITKNode* node)
    {
        QMutexLocker locker(&_mutex);
        if (node == nullptr)
            return -1;
        _nodeList.append(node);

        //获取索引与ID，形成映射关系
        const int index = _nodeList.size() - 1;
        int id = node->getNodeID();
        if (id < 0)
        {
            id = ++_nodeMaxID;
            node->setNodeID(id);
        }
        _idIndexMap.insert(id, index);

        //静态变量增大
        _nodeMaxID = _nodeMaxID >= id ? _nodeMaxID : id;

        return id;
    }

    int FITKNodeList::addNode(const double x, const double y, const double z, bool isNative, unsigned int tag)
    {
        Q_UNUSED(isNative);
        Q_UNUSED(tag);

        QMutexLocker locker(&_mutex);
        //创建节点
        Core::FITKNode* node = new Core::FITKNode(x, y, z);
        _nodeList.append(node);
        //获取索引与ID，形成映射关系
        const int index = _nodeList.size()-1;
        const int id = ++_nodeMaxID;
        node->setNodeID(id);
        _idIndexMap.insert(id, index);
        return id;
    }


    void FITKNodeList::addNode(const int id, const double x, const double y, const double z, bool isNative, unsigned int tag)
    {
        Q_UNUSED(isNative);
        Q_UNUSED(tag);

        QMutexLocker locker(&_mutex);
        //创建对象
        Core::FITKNode* node = new Core::FITKNode(id, x, y, z);
        _nodeList.append(node);
       //获取索引与ID，形成映射关系
        const int index = _nodeList.size() - 1;
        _idIndexMap.insert(id, index);
        //静态变量增大
        _nodeMaxID = _nodeMaxID >= id ? _nodeMaxID : id;
    }

    void FITKNodeList::removeNodeAt(const int index)
    {
        QMutexLocker locker(&_mutex);
        //越界判断
        if (index < 0 || index >= _nodeList.size()) return;
        Core::FITKNode* node = _nodeList[index];
        //从链表与Map中移除
        _nodeList.removeAt(index);
        if (node == nullptr) return;
        const int id = node->getNodeID();
        _idIndexMap.remove(id);
        //释放内存
        delete node;
        this->updateNodeIDIndexMap();
    }

    void FITKNodeList::removeReplceNodesAt(QList<int> indice)
    {
        QMutexLocker locker(&_mutex);

        if (indice.isEmpty())
        {
            return;
        }

        std::sort(indice.begin(), indice.end());
        std::reverse(indice.begin(), indice.end());
        for (const int & index : indice)
        {
            delete _nodeList[index];
            _nodeList[index] = nullptr;
        }

        QVector<Core::FITKNode*> newNodes;
        for (Core::FITKNode* n : _nodeList)
        {
            if (!n)
            {
                continue;
            }

            newNodes.push_back(n);
        }

        _nodeList.clear();
        _nodeList = newNodes;

        this->updateNodeIDIndexMap();
    }

    void FITKNodeList::removeNodesAt(QList<int> indice)
    {
        QMutexLocker locker(&_mutex);

        if (indice.isEmpty())
        {
            return;
        }

        std::sort(indice.begin(), indice.end());
        std::reverse(indice.begin(), indice.end());
        for (const int & index : indice)
        {
            delete _nodeList.takeAt(index);
        }

        this->updateNodeIDIndexMap();
    }

    void FITKNodeList::removeNodeByID(const int id)
    {
        QMutexLocker locker(&_mutex);
       //获取索引
        const int index = _idIndexMap.value(id,-1);
        if (index < 0 || index >= _nodeList.size()) return;
        Core::FITKNode* node = _nodeList[index];
        //根据索引移除
        _nodeList.removeAt(index);
        _idIndexMap.remove(id);
        if (node == nullptr) return;
        delete node;
        this->updateNodeIDIndexMap();

    }

    void FITKNodeList::removeNodeByIDs(const QList<int>& ids)
    {
        QMutexLocker locker(&_mutex);
        QList<Core::FITKNode*> nodes = {};
        for (const int& id : ids) {
            const int index = _idIndexMap.value(id);
            if (index < 0 || index >= _nodeList.size()) {
                continue;
            }
            Core::FITKNode* node = _nodeList[index];
            nodes.append(node);
        }

        for (Core::FITKNode* node : nodes) {
            if (node == nullptr) {
                continue;
            }
            _nodeList.removeOne(node);
            delete node;
            node = nullptr;
        }
        nodes.clear();
        this->updateNodeIDIndexMap();
    }

    void FITKNodeList::fastRemoveNodeAt(const int index)
    {
        QMutexLocker locker(&_mutex);
        //越界判断
        if (index < 0 || index >= _nodeList.size()) return;
        Core::FITKNode* node = _nodeList[index];
        //从链表与Map中移除
        _nodeList.removeAt(index);
        if (node == nullptr) return;
        const int id = node->getNodeID();
        //释放内存
        delete node;
    }

    void FITKNodeList::fastRemoveNodeByID(const int id)
    {
        QMutexLocker locker(&_mutex);
        //获取索引
        const int index = _idIndexMap.value(id);
        if (index < 0 || index >= _nodeList.size()) return;
        Core::FITKNode* node = _nodeList[index];
        //根据索引移除
        _nodeList.removeAt(index);
        if (node == nullptr) return;
        delete node;
    }

    int FITKNodeList::getNodeCount()
    {
        QMutexLocker locker(&_mutex);

        return _nodeList.size();
    }

    Core::FITKNode* FITKNodeList::getNodeAt(const int index)
    {
        QMutexLocker locker(&_mutex);
        //越界判断
        if (index < 0 || index >= _nodeList.size()) return nullptr;
        return _nodeList[index];
    }


    Core::FITKNode* FITKNodeList::getNodeByID(const int id)
    {
        QMutexLocker locker(&_mutex);
        //id合法性判断
   //     if (!_idIndexMap.contains(id)) return nullptr;
        const int index = _idIndexMap.value(id);
        //越界判断
        if (index < 0 || index >= _nodeList.size()) return nullptr;
        return _nodeList[index];
    }


    int FITKNodeList::getNodeIDByIndex(const int index)
    {
        //查找辅助存储ID
        QMutexLocker locker(&_mutex);
        
        if (index < 0 || index >= _nodeList.size()) return -1;
        Core::FITKNode* node = _nodeList[index];
        if (node == nullptr) return -1;

        return node->getNodeID();
    }

    int FITKNodeList::getNodeIndexByID(const int id)
    {
        //根据辅助存储查找索引
        QMutexLocker locker(&_mutex);
        return _idIndexMap.value(id);
    }

    int FITKNodeList::getNodeMaxID()
    {
        return _nodeMaxID;
    }

    void FITKNodeList::removeAllNode()
    {
        //释放全部内存
        for (auto node : _nodeList)
            delete node;
        _nodeList.clear();
        _idIndexMap.clear();
        _nodeMaxID = 0;
    }

    void FITKNodeList::updateNodeIDIndexMap()
    {
        //清空
        _idIndexMap.clear();
        int index = 0;
        _nodeMaxID = 0;

        //遍历重新生成
        for (Core::FITKNode* n : _nodeList)
        {
            if (n == nullptr) continue;
            int nId = n->getNodeID();
            _idIndexMap[nId] = index++;
            _nodeMaxID = qMax(_nodeMaxID, nId);
        }
    }


    //@param[out]  bndBox      边界框数组，大小为6，依次为xmin, xmax, ymin, ymax, zmin, zmax
    void  FITKNodeList:: getNodeBndBox(double *bndBox) const
    {
        if(bndBox == nullptr) return;
        bndBox[0] = bndBox[2] = bndBox[4] = 1e64;
        bndBox[1] = bndBox[3] = bndBox[5] = -1e64;
        //遍历节点
        for (QVector<Core::FITKNode*>::const_iterator it = _nodeList.constBegin(); it != _nodeList.constEnd(); ++it)
        {
            const Core::FITKNode* node = *it;
            if (node == nullptr) continue;
            const double x = node->x();
            const double y = node->y();
            const double z = node->z();
            //坐标判断
            bndBox[0] = x < bndBox[0] ? x : bndBox[0];
            bndBox[1] = x > bndBox[1] ? x : bndBox[1];
            bndBox[2] = y < bndBox[2] ? y : bndBox[2];
            bndBox[3] = y > bndBox[3] ? y : bndBox[3];
            bndBox[4] = z < bndBox[4] ? z : bndBox[4];
            bndBox[5] = z > bndBox[5] ? z : bndBox[5];
        }
    }

}



