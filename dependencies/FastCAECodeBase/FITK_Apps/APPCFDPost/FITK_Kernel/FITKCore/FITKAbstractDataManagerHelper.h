/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractDataManagerHelper.h
 * @brief 数据管理模板实际执行器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef  __FITKABSTRACTEDATAMANAGERHELPER_H__
#define  __FITKABSTRACTEDATAMANAGERHELPER_H__

#include <QList>
#include <QHash>
#include <QObject>
#include <QString>
#include <functional>
#include <QListIterator>
#include "FITKCoreAPI.h"



namespace Core
{
    class FITKAbstractDataObject;

    /**
     * @brief 排序类型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-11-19
     */
    enum ObjectSortType
    {
        DataObjID,
        DataName,
    };


    /**
     * @brief 数据管理模板实际执行器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class FITKCoreAPI FITKAbstractDataManagerHelper : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractDataManagerHelper object
         * @param[i]  l             数据管理器中的链表引用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKAbstractDataManagerHelper(QList<FITKAbstractDataObject*>& l);
        /**
         * @brief Destroy the FITKAbstractDataManagerHelper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        ~FITKAbstractDataManagerHelper() = default;
         /**
         * @brief 追加对象
         * @param[i] obj          追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void appendData(FITKAbstractDataObject* obj);

        /**
         * @brief 批量追加对象
         * @param[i] objs          追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-27
         */
        void appendData(const QList<FITKAbstractDataObject*>& objs);
        /**
         * @brief 获取管理的数据数量
         * @return int 数据数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        int getDataCount() const;
        /**
         * @brief 是否包含数据对象
         * @param[i] obj        待判定对象指针
         * @return true   包含该对象
         * @return false  不包含该对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        bool isContains(FITKAbstractDataObject* obj) const;
        /**
         * @brief 清空管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void clear();
        /**
         * @brief 获取第一个数据对象
         * @return FITKAbstractDataObject* 数据对象指针
         * @author LiBaojun 
         * @date 2024-02-22
         */
        FITKAbstractDataObject* getFirstOne();
        /**
         * @brief 获取最后一个数据对象
         * @return FITKAbstractDataObject* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataObject* getLastOne();
        /**
         * @brief 插入数据对象，索引越界追加，索引为负数插入到最前面
         * @param[i] index           插入的索引
         * @param[i] item            待插入的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void insertDataObj(int index, FITKAbstractDataObject* item);
        /**
         * @brief 根据索引获取数据对象，越界返回空指针
         * @param[i]  index          索引
         * @return T* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataObject* getDataByIndex(const int index);
        /**
         * @brief 根据数据ID获取对象
         * @param[i]  id             数据ID
         * @return T* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataObject* getDataByID(const int id);
        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感
         * @return FITKAbstractDataObject*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractDataObject* getDataByName(const QString& name, bool compSens);
        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感
         * @return FITKAbstractDataObject*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QList<FITKAbstractDataObject*> getDataListByName(const QString& name, bool compSens);
        /**
         * @brief 获取数据对象的索引，错误值返回-1
         * @param[i]  obj           数据对象指针
         * @return int 查询到的索引值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        int getDataIndex(FITKAbstractDataObject* obj);
         /**
         * @brief 移除对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataObj(FITKAbstractDataObject* obj);
        /**
         * @brief 移除对象,不释放对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataObjWithoutRelease(FITKAbstractDataObject* obj);
        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByIndex(int index);
         /**
         * @brief 根据数据ID移除对象
         * @param[i]  id            数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByID(int id);
        /**
         * @brief 根据名称移除对象
         * @param[i]  name         数据对象名称
         * @param[i]  CompSens       大小写敏感
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByName(const QString &name, bool CompSens);
        /**
         * @brief 设置父对象ID
         * @param[i]  id             父对象ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        void setParentDataID(const int & id);
        /**
         * @brief 获取父对象id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getParentDataID() const;
        /**
         * @brief 检查名称是否重复
         * @param[i]  name         待检查的名字
         * @return QString         不合法返回新名称，合法返回原始名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        QString checkName(const QString& name);

        /**
         * @brief       通过数据ID获取数据索引。
         * @param[in]   dataId：数据对象ID
         * @return      数据对象索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        int getIndexByID(int dataId);

        /**
         * @brief       通过数据索引获取数据ID。
         * @param[in]   dataIndex：数据对象索引
         * @return      数据对象ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        int getIDByIndex(int dataIndex);
        /**
         * @brief 对数据列表排序
         * @param[i]  type  排序类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-19
         */
        void sortObject(ObjectSortType type);

        /**
         * @brief 对数据列表排序
         * @param[i]  function  排序函数 返回值为是否小于（less than）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-19
         */
        void sortObject(std::function<bool(FITKAbstractDataObject*, FITKAbstractDataObject*)> function);
        /**
         * @brief 数据是否被使用（关联）
         * @param[i]  data           被检查的数据ID
         * @return true              管理器中使用了Data中包含的数据
         * @return false             管理器中没有使用Data中包含的数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-08
         */
        bool isUsedDataObject(const QList<int>& data);
         /**
         * @brief 获取迭代器
         * @return QListIterator<FITKAbstractDataObject*> 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-26
         */
        QListIterator<Core::FITKAbstractDataObject*> getIterator();
        

    private:
        /**
         * @brief 关联/断开数据对象与本管理器的信号关联
         * @param[i]  obj            数据对象
         * @param[i]  con            是否关联
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void connectHelper(FITKAbstractDataObject* obj, bool con = true);


    public slots :
        /**
         * @brief 对象被销毁之后的操作
         * @param[i]  obj            被销毁对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void dataObjectDestoried(FITKAbstractDataObject* obj);

    private:
        /**
         * @brief 数据管理器引用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QList<FITKAbstractDataObject*>& _dataList;
        /**
         * @brief 父对象id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int _parentDataID{ -1 };
    };
}



#endif
