/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementList.h"
#include "FITKAbstractElement.h"
#include <QSet>
#include <QMutexLocker>

namespace Interface
{

    FITKElementList::~FITKElementList()
    {
        //释放全部单元内存
        this->removeAllElement();
       
    }

    void FITKElementList::appendElement(FITKAbstractElement* element)
    {
        //已经存在的单元不追加
        if (element == nullptr) return;
//        if (_elementList.contains(element)) return;

        _elementList.append(element);
       const int id = element->getEleID();
       const int index = _elementList.size() - 1;
        //建立ID与index的映射关系
       _idIndexMap.insert(id, index);
       _eleMaxID = _eleMaxID > id ? _eleMaxID : id;
    }

    int FITKElementList::getEleIDByIndex(const int index)
    {
        if (index < 0 || index >= _elementList.size()) return -1;
        FITKAbstractElement* ele = _elementList[index];
        if (ele == nullptr) return -1;

        return ele->getEleID();
    }

    int FITKElementList::getEleIndexByID(const int id) const
    {
        return _idIndexMap.value(id, -1);
    }

    FITKAbstractElement* FITKElementList::getElementAt(const int index)
    {
        //越界返回空指针
        if (index < 0 || index >= _elementList.size()) return nullptr;
        return _elementList.at(index);
    }

    Interface::FITKAbstractElement* FITKElementList::getElementByID(const int id)
    {
        //首先查到ID
        const int index = _idIndexMap.value(id, -1);
        if (index < 0 || index >= _elementList.size()) return nullptr;
        return _elementList.at(index);
    }

    int FITKElementList::getElementCount()
    {
        return _elementList.size();
    }

    void FITKElementList::reverseEleByID(const int id)
    {
        auto ele = this->getElementByID(id);
        if (ele) ele->reverse();
    }

    void FITKElementList::reverseEleByIndex(const int index)
    {
        auto ele = this->getElementAt(index);
        if (ele) ele->reverse();
    }

    int FITKElementList::getElementMaxID()
    {
        return _eleMaxID;
    }

    void FITKElementList::removeAllElement()
    {
        for (auto ele : _elementList)
            delete ele;
        //清空列表
        _elementList.clear();
        _idIndexMap.clear();
        _eleMaxID = 0;
    }

    int FITKElementList::removeElementAt(const int index)
    {
        QMutexLocker locker(&_mutex);

        //越界判断
        if (index < 0 || index >= _elementList.size()) return -1;
        FITKAbstractElement* ele = _elementList[index];
        //从链表与Map中移除
        _elementList.removeAt(index);
        if (ele == nullptr) return -1;
        const int id = ele->getEleID();
        _idIndexMap.remove(id);
        //释放内存
        delete ele;
        this->updateElementIDIndexMap();
        return id;
    }

    int FITKElementList::removeElementByID(const int id)
    {
        QMutexLocker locker(&_mutex);
        //获取索引
        const int index = _idIndexMap.value(id, -1);
        if (index < 0 || index >= _elementList.size()) return -1;
        FITKAbstractElement* ele = _elementList[index];
        //根据索引移除
        _elementList.removeAt(index);
        _idIndexMap.remove(id);
        if (ele == nullptr) return -1;
        delete ele;
        this->updateElementIDIndexMap();
        return index;
    }

    void FITKElementList::fastRemoveElementAt(const int index)
    {
        QMutexLocker locker(&_mutex);

        //越界判断
        if (index < 0 || index >= _elementList.size()) return;
        FITKAbstractElement* ele = _elementList[index];
        //从链表中移除
        _elementList.removeAt(index);
        if (ele == nullptr) return;
        const int id = ele->getEleID();
        //释放内存
        delete ele;
    }

    void FITKElementList::fastRemoveElementByID(const int id)
    {
        QMutexLocker locker(&_mutex);
        //获取索引
        const int index = _idIndexMap.value(id, -1);
        if (index < 0 || index >= _elementList.size()) return;
        FITKAbstractElement* ele = _elementList[index];
        //根据索引移除
        _elementList.removeAt(index);
        if (ele == nullptr) return;
        delete ele;
    }

    QList<FITKAbstractElement*> FITKElementList::getNeighborElement(const int nodeID)
    {
        QList<FITKAbstractElement*> eles;
        //遍历单元
        for (FITKAbstractElement* ele : _elementList)
        {
            if (!ele) continue;
            const int index = ele->getNodeIndex(nodeID);
            //index >=0 单元包含该节点
            if(index<0) continue;
            eles.append(ele);
        }
        return eles;
    }

    QList<int> FITKElementList::getAllNodeIDs(bool isUnite) const
    {
        if (isUnite) {
            QSet<int> all;
            //遍历全部单元
            for (FITKAbstractElement* e : _elementList)
            {
                if (e == nullptr) continue;
                QList<int> ids = e->getAllNodes();
                //交集
                all.unite(ids.toSet());
            }
            return all.toList();
        }
        QList<int> all;
        //遍历全部单元
        for (FITKAbstractElement* e : _elementList)
        {
            if(e == nullptr) continue;
            QList<int> ids = e->getAllNodes();

            all.append(ids);
        }
        return all;
    }

    int FITKElementList::getAllElementNodesCount() const
    {
        int count = 0;
        //遍历全部单元
        for (FITKAbstractElement* e : _elementList)
        {
            if(e == nullptr) continue;
            count += e->getNodeCount();
        }
        return count;
    }

    void FITKElementList::replaceNodesID(const QHash<int, int> & nodes)
    {
        //遍历全部单元
        QVector<FITKAbstractElement* >::iterator eleIter = _elementList.begin();
        QVector<FITKAbstractElement* >::iterator eleEndIter = _elementList.end();

        QHash<int, int>::ConstIterator nodesIter = nodes.begin();
        QHash<int, int>::ConstIterator nodesEndIter = nodes.end();
 
        for (;eleIter != eleEndIter; ++eleIter)
        {
            if (*eleIter == nullptr) continue;
            //点遍历
            nodesIter = nodes.begin();
            for (; nodesIter != nodesEndIter; ++nodesIter)
            {
                (*eleIter)->replaceNodeID(nodesIter.key(), nodesIter.value());
            }
            
        }
      
    }

    bool FITKElementList::replaceElement(int orgId, FITKAbstractElement* newEle, bool delOrg)
    {
        // 查询单元索引。
        const int index = _idIndexMap.value(orgId, -1);
        if (index < 0 || index >= _elementList.size())
        {
            return false;
        }

        FITKAbstractElement* orgEle = _elementList.at(index);
        if (orgEle)
        {
            // 拷贝ID以及其他属性。
            newEle->setEleID(orgEle->getEleID());
            newEle->setNativeFlag(orgEle->getNativeFlag());
            newEle->setTag(orgEle->getTag());
        }
        else
        {
            _idIndexMap.take(orgId);
            _idIndexMap.insert(orgEle->getEleID(), index);
        }

        _elementList.replace(index, newEle);

        if (delOrg && orgEle)
        {
            delete orgEle;
        }

        return true;
    }

    bool FITKElementList::replaceElementAt(int orgIndex, FITKAbstractElement* newEle, bool delOrg)
    {
        // 查询单元索引。
        if (orgIndex < 0 || orgIndex >= _elementList.size())
        {
            return false;
        }

        FITKAbstractElement* orgEle = _elementList.at(orgIndex);
        if (orgEle)
        {
            // 拷贝ID以及其他属性。
            newEle->setEleID(orgEle->getEleID());
            newEle->setNativeFlag(orgEle->getNativeFlag());
            newEle->setTag(orgEle->getTag());
        }
        else
        {
            _idIndexMap.insert(orgEle->getEleID(), orgIndex);
        }

        _elementList.replace(orgIndex, newEle);

        if (delOrg && orgEle)
        {
            delete orgEle;
        }

        return true;
    }

    void FITKElementList::updateElementIDIndexMap()
    {
        _idIndexMap.clear();
        int index = 0;
        _eleMaxID = 0;

        for (FITKAbstractElement* ele : _elementList)
        {
            if (ele == nullptr) continue;
            const int eId = ele->getEleID();
            _idIndexMap[eId] = index++;
            _eleMaxID = qMax(_eleMaxID, eId);
        }
    }
}



