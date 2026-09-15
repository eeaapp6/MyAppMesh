/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractDataManagerHelper.h"
#include "FITKAbstractDataObject.h"
#include "FITKAbstractNDataObject.h"
#include <QRegExp>

namespace Core
{
    FITKAbstractDataManagerHelper::FITKAbstractDataManagerHelper(QList<FITKAbstractDataObject*>& l)
        :_dataList(l)  //引用数据管理中的链表
    {
    }

    void FITKAbstractDataManagerHelper::appendData(FITKAbstractDataObject* item, bool changeParentID)
    {
        if (item == nullptr) return;
        //存在的就直接delete
        if ( isContains(item))
        {
            //delete item;
            return;
        }
        _dataList.append(item);
        if (changeParentID)
        {
            //将父对象id设置到item
            item->setParentDataID(_parentDataID);
        }
        
        this->connectHelper(item);
    }


    void FITKAbstractDataManagerHelper::appendData(const QList<FITKAbstractDataObject*>& objs, bool changeParentID)
    {
        _dataList.append(objs);
        for (auto item : objs)
        {
            if (changeParentID)
            {
                //将父对象id设置到item
                item->setParentDataID(_parentDataID);
            }

            this->connectHelper(item);
        }
    }

    int FITKAbstractDataManagerHelper::getDataCount() const
    {
        return _dataList.size();
    }

    bool FITKAbstractDataManagerHelper::isContains(FITKAbstractDataObject* obj) const
    {
        return _dataList.contains(obj);
    }

    void FITKAbstractDataManagerHelper::clear()
    {
        //清空内存
        while (!_dataList.isEmpty())
        {
            auto obj = _dataList.first();
            if (obj) delete obj;
//            _dataList.removeFirst();
        }
//         auto dataList = _dataList;
//         for (auto obj : dataList)
//             delete obj;
//         _dataList.clear();
    }

    FITKAbstractDataObject *FITKAbstractDataManagerHelper::getFirstOne()
    {
        if (_dataList.isEmpty()) return nullptr;
        return _dataList.first();
    }

    FITKAbstractDataObject *FITKAbstractDataManagerHelper::getLastOne()
    {
        if (_dataList.isEmpty()) return nullptr;
        return _dataList.last();
    }

    void FITKAbstractDataManagerHelper::insertDataObj(int index, FITKAbstractDataObject* item, bool changeParentID)
    {
        if(item == nullptr) return;
        // 索引越界，追加
        if (index >= _dataList.size())
        {
            this->appendData(item);
            return;
        }
        if (index < 0) index = 0;
        // 索引小于0，在最前面插入
        _dataList.insert(index, item);

        if (changeParentID)
        {
            //将父对象id设置到item
            item->setParentDataID(_parentDataID);
        }

        this->connectHelper(item);
    }

    FITKAbstractDataObject* FITKAbstractDataManagerHelper::getDataByIndex(const int index)
    {
        //错误判断
        if (index < 0 || index >= getDataCount()) return nullptr;
        return _dataList.at(index);
    }

    FITKAbstractDataObject* FITKAbstractDataManagerHelper::getDataByID(const int id)
    {
        //遍历获取数据
        for (auto obj : _dataList)
        {
            if(obj == nullptr) continue;
            if (id == obj->getDataObjectID()) return obj;
        }
        return nullptr;
    }

    FITKAbstractDataObject* FITKAbstractDataManagerHelper::getDataByName(const QString& name, bool sens)
    {
        for (auto obj : _dataList)
        {
            //如果不是FITKAbstractNDataObject派生类对象，不做判断
            auto nobj = dynamic_cast<FITKAbstractNDataObject*>(obj);
            if (nobj == nullptr) continue;
            //大小写不敏感，全部转换为小写
            if (!sens)
            {
                if (name.toLower() == nobj->getDataObjectName().toLower())
                    return obj;
            }
            else if (name == nobj->getDataObjectName() ) 
                return obj;
        }
        return nullptr;
    }

    QList<FITKAbstractDataObject*> FITKAbstractDataManagerHelper::getDataListByName(const QString& name, bool sens)
    {
        QList<FITKAbstractDataObject*> dls;

        for (auto obj : _dataList)
        {
            //如果不是FITKAbstractNDataObject派生类对象，不做判断
            auto nobj = dynamic_cast<FITKAbstractNDataObject*>(obj);
            if (nobj == nullptr) continue;
            //大小写不敏感，全部转换为小写
            if (!sens)
            {
                if (name.toLower() == nobj->getDataObjectName().toLower())
                    dls.append(nobj);
            }
            else if (name == nobj->getDataObjectName())
                dls.append(nobj);
        }
        return dls;
    }

    int FITKAbstractDataManagerHelper::getDataIndex(FITKAbstractDataObject* obj)
    {
        if (_dataList.contains(obj))
            return _dataList.indexOf(obj);
        //错误值-1
        return -1;
    }

    void FITKAbstractDataManagerHelper::removeDataObj(FITKAbstractDataObject* obj)
    {
        //不在链表的对象不操作
        if (obj && _dataList.contains(obj))
            delete obj;
    }


    void FITKAbstractDataManagerHelper::removeDataObjWithoutRelease(FITKAbstractDataObject* obj)
    {
        if (obj && _dataList.contains(obj))
            _dataList.removeOne(obj);
        //解除信号关联
        this->connectHelper(obj, false);

    }

    void FITKAbstractDataManagerHelper::removeDataByIndex(int index)
    {
        auto d = this->getDataByIndex(index);
        this->removeDataObj(d);
    }


    void FITKAbstractDataManagerHelper::removeDataByID(int id)
    {
        auto d = this->getDataByID(id);
        this->removeDataObj(d);
    }


    void FITKAbstractDataManagerHelper::removeDataByName(const QString &name, bool senes)
    {
        auto d = this->getDataByName(name, senes);
        this->removeDataObj(d);
    }


    void FITKAbstractDataManagerHelper::setParentDataID(const int & id)
    {
        _parentDataID = id;
        //修改已有成员ID
        for (FITKAbstractDataObject* objc : _dataList)
        {
            if(objc == nullptr) continue;
            objc->setParentDataID(id);
        }
    }

    int FITKAbstractDataManagerHelper::getParentDataID() const
    {
        return _parentDataID;
    }

    void FITKAbstractDataManagerHelper::connectHelper(FITKAbstractDataObject* obj, bool con)
    {
        if (con)  //关联信号，obj对象释放的时候会触发.direct关联直接在发送进程执行
            connect(obj, &FITKAbstractDataObject::dataObjectDestoried,
                this, &FITKAbstractDataManagerHelper::dataObjectDestoried,Qt::DirectConnection);
        else  //解除关联
            disconnect(obj, &FITKAbstractDataObject::dataObjectDestoried,
                this, &FITKAbstractDataManagerHelper::dataObjectDestoried);
    }

    void FITKAbstractDataManagerHelper::dataObjectDestoried(FITKAbstractDataObject* obj)
    {
        //槽函数，处理对象被释放事件
        this->connectHelper(obj, false);
        while (_dataList.contains(obj))
            _dataList.removeOne(obj);
    }

    QString FITKAbstractDataManagerHelper::checkName(const QString& cname)
    {
        QString name = cname.simplified();
        if (name.isEmpty()) {
            return name;
        }

        // 检查是否以 -number 结尾
        QRegExp numSuffix("^(.+)-(\\d+)$", Qt::CaseInsensitive);
        int pos = numSuffix.indexIn(name);
        bool hasNumSuffix = (pos != -1);
        QString base; // 搜索前缀（不含末尾数字）
        if (hasNumSuffix) {
            base = numSuffix.cap(1); // 保留原始大小写的 base 用于返回
        } else {
            base = name;
        }

        // 构造用于匹配 "<prefix>-<number>" 的正则（在小写的对象名上匹配）
        QString prefixLower = base.toLower();
        QRegExp pattern("^" + QRegExp::escape(prefixLower) + "-(\\d+)$", Qt::CaseInsensitive);

        int maxId = -1;
        bool foundExact = false;

        for (auto a : _dataList)
        {
            auto nobj = dynamic_cast<FITKAbstractNDataObject*>(a);
            if (nobj == nullptr) continue;
            QString objname = nobj->getDataObjectName();
            if (objname == name) // 完全相同（大小写敏感比较原名）
            {
                foundExact = true;
            }
            QString objLower = objname.toLower();
            int p = pattern.indexIn(objLower);
            if (p != -1) {
                // 获取数字部分
                bool ok = false;
                int id = pattern.cap(1).toInt(&ok);
                if (ok == false)
                {
                    continue;
                }
                if (id > maxId)
                {
                    maxId = id;
                }
            }
        }

        // 没有重复，直接返回原名
        if (!foundExact) {
            return name;
        }

        // 存在重复，按不同情形返回新名字
        if (hasNumSuffix) {
            // 原名自带数字后缀，沿用 base 并在现有最大数字上 +1
            int next = (maxId >= 0) ? (maxId + 1) : (numSuffix.cap(2).toInt() + 1);
            return QString("%1-%2").arg(base).arg(next);
        }
        else {
            // 普通无数字尾缀的名称：如果已有带数字的同名，使用 max+1，否则默认 -1
            if (maxId >= 0) {
                return QString("%1-%2").arg(base).arg(maxId + 1);
            }
            else {
                return QString("%1-1").arg(base);
            }
        }
    }

    int FITKAbstractDataManagerHelper::getIndexByID(int dataId)
    {
        int dataIndex = -1;
        if (_dataList.count() == 0)
        {
            return dataIndex;
        }

        // 查询数据对象记录索引。
        int iObj = -1;
        for (FITKAbstractDataObject* obj : _dataList)
        {
            iObj++;

            if (!obj)
            {
                continue;
            }

            if (obj->getDataObjectID() == dataId)
            {
                dataIndex = iObj;
                break;
            }
        }

        return dataIndex;
    }

    int FITKAbstractDataManagerHelper::getIDByIndex(int dataIndex)
    {
        // 获取对象。
        FITKAbstractDataObject* obj = getDataByIndex(dataIndex);
        if (!obj)
        {
            return -1;
        }

        // 获取ID。
        return obj->getDataObjectID();
    }

    void FITKAbstractDataManagerHelper::sortObject(ObjectSortType type)
    {
        //根据名字排序函数
        auto SortByName = [](FITKAbstractDataObject* obj1, FITKAbstractDataObject* obj2)->bool
        {
            FITKAbstractNDataObject* nd1 = dynamic_cast<FITKAbstractNDataObject*>(obj1);
            FITKAbstractNDataObject* nd2 = dynamic_cast<FITKAbstractNDataObject*>(obj2);
            if (nd1 == nullptr || nd2 == nullptr) return false;
            QString name1 = nd1->getDataObjectName();
            QString name2 = nd2->getDataObjectName();
            
            return (name1.compare(name2)  < 0);
        };
        //根据ID排序函数
        auto SortByID = [](FITKAbstractDataObject* obj1, FITKAbstractDataObject* obj2)->bool
        {
           if (obj1 == nullptr || obj2 == nullptr) return false;
            return obj1->getDataObjectID() < obj2->getDataObjectID();
        };

        //正式开始执行排序
        if (_dataList.isEmpty()) return;
        
        //根据不同类型排序
        switch (type)
        {
        case DataName: //根据名字排序
            //没有名字属性，不执行排序
            if (dynamic_cast<FITKAbstractNDataObject*>(_dataList[0]) == nullptr) return;
            qSort(_dataList.begin(), _dataList.end(), SortByName); break;
        case DataObjID:  //根据ID排序
            qSort(_dataList.begin(), _dataList.end(), SortByID); break;
        default: break;
        }
    }

    void FITKAbstractDataManagerHelper::sortObject(std::function<bool(FITKAbstractDataObject*, FITKAbstractDataObject*)> function)
    {
        if (function == nullptr) return;
        //执行排序
        qSort(_dataList.begin(), _dataList.end(), function);
    }

    bool FITKAbstractDataManagerHelper::isUsedDataObject(const QList<int>& data)
    {
        for (auto obj : _dataList)
        {
            if (!obj)continue;
            //首先检查成员本身
            if (data.contains(obj->getDataObjectID())) return true;
            //检查关联项
            if (obj->isUsedDataObject(data))
                return true;
        }
        return false;
    }

    QListIterator<Core::FITKAbstractDataObject*> FITKAbstractDataManagerHelper::getIterator()
    {
         return QListIterator<Core::FITKAbstractDataObject*>(_dataList);
    }

}
