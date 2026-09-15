/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractDataManager.hpp
 * @brief 数据管理器模板声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _FITKDATAMANAGERABSTRACT_H__
#define _FITKDATAMANAGERABSTRACT_H__

#include "FITKAbstractDataManagerHelper.h"
#include "FITKAbstractObject.hpp"
#include "FITKAbstractDataManagerPrivate.h"
#include <QMutex>
#include <QMutexLocker>


namespace Core
{
    class FITKAbstractDataObject;
    class FITKAbstractDataManagerHelper;

    template <class T>
    /**
     * @brief 数据管理器模板，采用list维护， 线程安全
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class  FITKAbstractDataManager : public FITKAbstractDataManagerPrivate
    {
    public:
        /**
         * @brief Construct a new FITKAbstractDataManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataManager()
        {
            _helper = new FITKAbstractDataManagerHelper(_dataList);
        }
        /**
         * @brief Destroy the FITKAbstractDataManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKAbstractDataManager()
        {
            this->clear();
            if (_helper) delete _helper;
        }
        /**
         * @brief 追加对象
         * @param[i] item          追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void appendDataObj(FITKAbstractDataObject* item) override
        {
            QMutexLocker locker(&_mutex);
            if (!isT(item))
                delete item;
            else
                _helper->appendData(item);
        }
        /**
         * @brief 批量追加对象
         * @param[i] items         追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void appendDataObj(const QList<FITKAbstractDataObject*> & items) override
        {
            QMutexLocker locker(&_mutex);
             _helper->appendData(items);
        }
 
        /**
         * @brief 获取管理的数据数量
         * @return int 数据数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual int getDataCount() const override
        {
            QMutexLocker locker(&_mutex);
            return _dataList.size();
        }
        /**
         * @brief 是否包含数据对象
         * @param[i] obj        待判定对象指针
         * @return true   包含该对象
         * @return false  不包含该对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual bool isContains(FITKAbstractDataObject* obj) override
        {
            QMutexLocker locker(&_mutex);
            return (_helper && _helper->isContains(obj));
        }
        /**
         * @brief 清空管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void clear() override
        {
            QMutexLocker locker(&_mutex);
            _helper->clear();
        }
        /**
         * @brief 插入数据对象，索引越界追加，索引为负数插入到最前面
         * @param[i] index           插入的索引
         * @param[i] item            待插入的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void insertDataObj(int index, FITKAbstractDataObject* item) override
        {
            QMutexLocker locker(&_mutex);
            if (!isT(item))
                delete item;
            else
                _helper->insertDataObj(index, item);
        }
        /**
         * @brief 根据索引获取数据对象，越界返回空指针
         * @param[i]  index          索引
         * @return T* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        T* getDataByIndex(const int index)
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getDataByIndex(index));
        }
        /**
         * @brief 运算符重载，根据索引获取数据对象，越界返回空指针
         * @param[i]  index          索引
         * @return T* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-25
         */
        T* operator[](const int index)
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getDataByIndex(index));
        }
        /**
         * @brief 根据数据ID获取对象
         * @param[i]  id             数据ID
         * @return T* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual T* getDataByID(const int id)
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getDataByID(id));
        }
        /**
         * @brief 获取数据对象的索引，错误值返回-1
         * @param[i]  obj           数据对象指针
         * @return int 查询到的索引值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual int getDataIndex(FITKAbstractDataObject* obj) const override
        {
            QMutexLocker locker(&_mutex);
            return _helper->getDataIndex(obj);
        }
        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @return T*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        T* getDataByName(const QString& name, bool CompSens = false)
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getDataByName(name,CompSens));
        }
        /**
         * @brief 获取第一个数据对象
         * @return FITKAbstractDataObject* 数据对象指针
         * @author LiBaojun 
         * @date 2024-02-22
         */
        T* getFirstOne()
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getFirstOne());
        }
        /**
         * @brief 获取最后一个数据对象
         * @return FITKAbstractDataObject* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        T* getLastOne()
        {
            QMutexLocker locker(&_mutex);
            return transfe2T(_helper->getLastOne());
        }



        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @return FITKAbstractDataObject*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-28
         */
        virtual FITKAbstractDataObject* getDataObjectByName(const QString& name, bool CompSens = false) override
        {
            QMutexLocker locker(&_mutex);
            return (_helper->getDataByName(name, CompSens));
        }
        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @return FITKAbstractDataObject*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-28
         */
        virtual QList<FITKAbstractDataObject*> getDataObjectsByName(const QString& name, bool CompSens = false) override
        {
            QMutexLocker locker(&_mutex);
            return (_helper->getDataListByName(name, CompSens));
        }
        /**
         * @brief 移除对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataObj(FITKAbstractDataObject* obj) override
        {
            QMutexLocker locker(&_mutex);
            _helper->removeDataObj(obj);
        }
        /**
         * @brief 移除对象,不释放对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-23
         */
        virtual void removeDataObjWithoutRelease(FITKAbstractDataObject* obj) override
        {
            QMutexLocker locker(&_mutex);
            _helper->removeDataObjWithoutRelease(obj);
        }
        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByIndex(int index) override
        {
            QMutexLocker locker(&_mutex);
            _helper->removeDataByIndex(index);
        }
        /**
         * @brief 根据数据ID移除对象
         * @param[i]  id            数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByID(int id) override
        {
            QMutexLocker locker(&_mutex);
            _helper->removeDataByID(id);
        }
        /**
         * @brief 根据名称移除对象
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByName(const QString &name, bool compSens = false) override
        {
            QMutexLocker locker(&_mutex);
            _helper->removeDataByName(name,compSens);
        }
        /**
         * @brief 设置父对象ID
         * @param[i]  id             父对象ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        void setParentDataIDM(const int & id)
        {
            if (_helper)
                _helper->setParentDataID(id);
        }
        /**
         * @brief 获取父对象id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getMParentDataIDM() const
        {
            if (_helper)
                return _helper->getParentDataID();
            return -1;
        }
        /**
         * @brief 检查名称是否重复
         * @param[i]  name         待检查的名字
         * @return QString         不合法返回新名称，合法返回原始名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        virtual QString checkName(const QString& name) override
        {
            if (_helper)
                return _helper->checkName(name);
            return name;
        }

        /**
         * @brief       通过数据ID获取数据索引。
         * @param[in]   dataId：数据对象ID
         * @return      数据对象索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        int getIndexByID(int dataId) override
        {
            QMutexLocker locker(&_mutex);
            return _helper->getIndexByID(dataId);
        }

        /**
         * @brief       通过数据索引获取数据ID。
         * @param[in]   dataIndex：数据对象索引
         * @return      数据对象ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        int getIDByIndex(int dataIndex)
        {
            QMutexLocker locker(&_mutex);
            return _helper->getIDByIndex(dataIndex);
        }


        /**
        * @brief 对数据列表排序
        * @param[i]  type  排序类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-19
        */
        void sortObject(ObjectSortType type) override
        {
            QMutexLocker locker(&_mutex);
            return _helper->sortObject(type);
        }
        /**
         * @brief 对数据列表排序
         * @param[i]  function  排序函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-19
         */
        void sortObject(std::function<bool(FITKAbstractDataObject*, FITKAbstractDataObject*)> function) override
        {
            QMutexLocker locker(&_mutex);
            return _helper->sortObject(function);
        }
        /**
         * @brief 数据是否被使用（关联）
         * @param[i]  data           被检查的数据ID
         * @return true              管理器中使用了Data中包含的数据
         * @return false             管理器中没有使用Data中包含的数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-08
         */
        bool isUsedDataObject(const QList<int>& data) override
        {
            QMutexLocker locker(&_mutex);
            return _helper->isUsedDataObject(data);
        }
        /**
         * @brief 获取迭代器
         * @return QListIterator<FITKAbstractDataObject*> 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-26
         */
        QListIterator<Core::FITKAbstractDataObject*> getIterator() override
        {
            QMutexLocker locker(&_mutex);
            return  _helper->getIterator();
        }

    private:
        /**
         * @brief 是否为模板类型对象
         * @param[i]  obj          待判断的指针
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        bool isT(FITKAbstractDataObject* obj)
        {
            auto p = dynamic_cast<T*>(obj);
            return nullptr != p;
        }
        /**
         * @brief 转换为模板T类型
         * @param[i]  data        数据对象
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        T* transfe2T(FITKAbstractDataObject* data)
        {
            return dynamic_cast<T*>(data);
        }

        

     protected:
        /**
         * @brief 实际执行插入删除等操作的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataManagerHelper * _helper{};
        /**
         * @brief 互斥锁，保证线程安全
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        mutable QMutex _mutex{};
        /**
         * @brief 数据对象链表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QList<FITKAbstractDataObject*> _dataList{};
    };



}

#endif
