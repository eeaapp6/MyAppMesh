/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractElement.h"

namespace Interface
{
    FITKAbstractElement::~FITKAbstractElement()
    {
        emit elementDestroiedSignal(this);
    }


    FITKModelEnum::FITKEleType FITKAbstractElement::getEleType()
    {
        //抽象类，不override返回无效类型
        return FITKModelEnum::FITKEleType::EleNone;
    }

    int FITKAbstractElement::getNodeCount()
    {
        //纯虚函数，子类必须重写
        return -1;
    }


    void FITKAbstractElement::setEleID(const int id)
    {
        _eleID = id;
    }

    int FITKAbstractElement::getEleID() const
    {
        return _eleID;
    }


    int FITKAbstractElement::getNodeID(const int index) const
    {
        //数组越界返回-1
        if (index < 0 || index >= _elemenTopo.size()) return -1;
        return _elemenTopo.at(index);
    }

    int FITKAbstractElement::getNodeIndex(const int id) const
    {
        return _elemenTopo.indexOf(id);
    }

    void FITKAbstractElement::setNodeID(const int index, int id)
    {
        //getNodeCount在子类中需要强制重写
        //数组越界不进行操作
        if (index < 0 || index >= this->getNodeCount()) return;
        _elemenTopo[index] = id;
    }

    void FITKAbstractElement::setNodeID(const QList<int>& ids)
    {
        //数组大小不匹配不进行赋值操作
        if (ids.size() != this->getNodeCount()) return;
        _elemenTopo = ids;
    }

    int FITKAbstractElement::getFaceCount()
    {
        // 返回错误值
        return -1;
    }

    int FITKAbstractElement::getElementDim()
    {
        //错误值
        return -1;
    }

    int FITKAbstractElement::getEdgeCount()
    {
         // 返回错误值tai
        return -1;
    }

    QList<int> FITKAbstractElement::getFace(const int index, const bool & lowerOrder)
    {
         // 返回错误值
        return QList<int>();
    }

    QList<int> FITKAbstractElement::getEdge(const int index)
    {
         // 返回错误值
        return QList<int>();
    }

    void FITKAbstractElement::reverse()
    {
       const int c =  _elemenTopo.size();
       //首尾元素交换
       for (int k = 0; k < c/2 ; ++k)
           _elemenTopo.swap(k, c - 1 - k);
    }

    int FITKAbstractElement::getFaceIndex(const QList<int>& face, bool normalSens /*= false*/)
    {
        if (normalSens)
            return this->getFaceIndexNorSen(face);
        else
            return this->getFaceIndexNorUnSen(face);
    }

    int FITKAbstractElement::getEdgeIndex(const QList<int>& edge)
    {
        const int nEdge = this->getEdgeCount();
        for (int i = 0; i < nEdge; i++){
            QList<int> cEdge = this->getEdge(i);
            const int nNode = cEdge.size();
            //判断点的数量
            if (cEdge.size() != edge.size() || nNode == 0) {
                continue;
            }
            for (int iNode : edge) {
                cEdge.removeOne(iNode);
            }
            //为空说明节点对应正确
            if (cEdge.isEmpty()) {
                return i;
            }
        }
        return -1;
    }

    int FITKAbstractElement::getFaceIndexNorSen(const QList<int>& face, const bool & lowerOrder)
    {
        const int nFace = this->getFaceCount();
        for (int i = 0; i < nFace; i++)
        {
            QList<int> cFace = this->getFace(i, lowerOrder);
            const int nNode = cFace.size();
            //判断点的数量
            if(nNode != face.size()|| nNode == 0) continue;
            const int node1 = face.at(0);
            int cFNodeIndex = cFace.indexOf(node1);
            //不包含第一个点
            if(cFNodeIndex == -1) continue;
            bool isthisFace = true;
            for (int iNode = 0 ; iNode < nNode ; ++iNode)
            {
                const int nodeID = face.at(iNode);
                const int cnodeID = cFace.at((iNode + cFNodeIndex)% nNode);
                //一个节点不一致则不是同一个面
                if (cnodeID != nodeID)
                {
                    isthisFace = false;
                    break;
                }
            }
            //返回面索引
            if (isthisFace) return i;
        }
        //没找到面
        return -1;
    }

    int FITKAbstractElement::getFaceIndexNorUnSen(const QList<int>& face,const bool & lowerOrder)
    {
        const int nFace = this->getFaceCount();
        for (int i = 0; i < nFace; i++)
        {
            QList<int> cFace = this->getFace(i, lowerOrder);
            const int nNode = cFace.size();
            //判断点的数量
            if (cFace.size() != face.size() || nNode == 0) continue;
            for (int iNode : face)
                cFace.removeOne(iNode);
            //面为空说明节点对应正确
            if (cFace.isEmpty())
                return i;
        }
        return -1;
    }
    
    void FITKAbstractElement::setBlockTag(unsigned int id)
    {
        _blockTag = id;
    }

    unsigned int FITKAbstractElement::getBlockTag() const
    {
        return _blockTag;
    }

    int FITKAbstractElement::getPreviousNodeID(const int id)
    {
        const int index = _elemenTopo.indexOf(id);
        //不包含节点
        if (index < 0) return -1;
        //已经是第一个点
        if (index == 0) return _elemenTopo.last();
        return _elemenTopo.at(index - 1);

    }

    int FITKAbstractElement::getNextNodeID(const int id)
    {
        const int index = _elemenTopo.indexOf(id);
        //不包含节点
        if (index < 0) return -1;
        //已经是第最后一个点
        if (index == _elemenTopo.size() -1) return _elemenTopo.first();
        return _elemenTopo.at(index + 1);
    }

    QList<int> FITKAbstractElement::getAllNodes()
    {
        return _elemenTopo;
    }
    
    void FITKAbstractElement::replaceNodeID(const int& oldID, const int& newID)
    {
        QList<int>::iterator iter;
        QList<int>::iterator endIter = _elemenTopo.end();
        for (iter= _elemenTopo.begin(); iter != endIter; ++iter)
        {
            if (oldID == *iter)
                *iter = newID;
        }
            
    }

    

    void FITKAbstractElement::setNodeCount(const int c)
    {
        //对数组进行初始化操作
        _elemenTopo.clear();
        for (int i = 0; i < c; ++i)
            _elemenTopo.append(-1);
    }

    void FITKAbstractElement::setTag(unsigned int tag)
    {
        _tag = tag;
    }
    unsigned int FITKAbstractElement::getTag() const
    {
        return _tag;
    }

    void FITKAbstractElement::setNativeFlag(bool flag)
    {
        // 保存关联标记。
        _nativeFlag = flag;
    }

    bool FITKAbstractElement::getNativeFlag()
    {
        return _nativeFlag;
    }
}
