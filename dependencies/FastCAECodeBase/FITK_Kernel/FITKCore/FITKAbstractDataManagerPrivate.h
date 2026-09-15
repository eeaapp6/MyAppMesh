/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractDataManagerPrivate.h
 * @brief  管理器抽象类，用于数据访问使用，全部都是虚函数，无实际意义
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-11-29
 * 
 */
#ifndef __FITK_ABSTRACT_DATAMANAGER_PRIVATE_H___
#define __FITK_ABSTRACT_DATAMANAGER_PRIVATE_H___
 
#include "FITKCoreAPI.h"
#include <QList>
#include <QListIterator>
#include "FITKAbstractDataManagerHelper.h"

namespace Core
{
    class FITKAbstractDataObject;
    /**
     * @brief 管理器抽象类，用于数据访问使用，全部都是虚函数，无实际意义
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-11-29
     */
    class FITKCoreAPI FITKAbstractDataManagerPrivate
    {
    public:
        explicit FITKAbstractDataManagerPrivate() = default;
        virtual ~FITKAbstractDataManagerPrivate() = 0;

        /**
         * @brief 追加对象
         * @param[i] item          追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void appendDataObj(FITKAbstractDataObject* item)=0;
        /**
        * @brief 批量追加对象
        * @param[i] items         追加的数据对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        virtual void appendDataObj(const QList<FITKAbstractDataObject*> & items) = 0;
        /**
         * @brief 获取管理的数据数量
         * @return int 数据数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual int getDataCount() const;

        /**
         * @brief 是否包含数据对象
         * @param[i] obj        待判定对象指针
         * @return true   包含该对象
         * @return false  不包含该对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual bool isContains(FITKAbstractDataObject* obj) = 0;

        /**
        * @brief 清空管理器
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        virtual void clear() = 0;

        /**
         * @brief 插入数据对象，索引越界追加，索引为负数插入到最前面
         * @param[i] index           插入的索引
         * @param[i] item            待插入的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void insertDataObj(int index, FITKAbstractDataObject* item) = 0;

        /**
         * @brief 获取数据对象的索引，错误值返回-1
         * @param[i]  obj           数据对象指针
         * @return int 查询到的索引值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual int getDataIndex(FITKAbstractDataObject* obj) const;
        /**
        * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
        * @param[i]  name           对象名称
        * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
        * @return FITKAbstractDataObject*               对象指针
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-28
        */
        virtual FITKAbstractDataObject* getDataObjectByName(const QString& name, bool CompSens = false) = 0;

        /**
         * @brief 根据名称获取数据对象，只对FITKAbstractNDataObject派生类生效
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @return FITKAbstractDataObject*               对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-28
         */
        virtual QList<FITKAbstractDataObject*> getDataObjectsByName(const QString& name, bool CompSens = false) = 0;
        /**
         * @brief 移除对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataObj(FITKAbstractDataObject* obj) = 0;

        /**
         * @brief 移除对象,不释放对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-23
         */
        virtual void removeDataObjWithoutRelease(FITKAbstractDataObject* obj) = 0;
        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByIndex(int index) =0;
        /**
         * @brief 根据数据ID移除对象
         * @param[i]  id            数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByID(int id) =0;
        /**
         * @brief 根据名称移除对象
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByName(const QString &name, bool compSens = false) =0;
        /**
        * @brief 检查名称是否重复
        * @param[i]  name         待检查的名字
        * @return QString         不合法返回新名称，合法返回原始名称
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-04-11
        */
        virtual QString checkName(const QString& name) = 0;
        /**
         * @brief       通过数据ID获取数据索引。
         * @param[in]   dataId：数据对象ID
         * @return      数据对象索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        virtual int getIndexByID(int dataId) = 0;

        /**
         * @brief       通过数据索引获取数据ID。
         * @param[in]   dataIndex：数据对象索引
         * @return      数据对象ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        virtual int getIDByIndex(int dataIndex) = 0;


        /**
        * @brief 对数据列表排序
        * @param[i]  type  排序类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-19
        */
        virtual void sortObject(ObjectSortType type) = 0;

        /**
        * @brief 对数据列表排序
        * @param[i]  function  排序函数
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-19
        */
        virtual void sortObject(std::function<bool(FITKAbstractDataObject*, FITKAbstractDataObject*)> function) = 0;

        /**
         * @brief 数据是否被使用（关联）
         * @param[i]  data           被检查的数据ID
         * @return true              管理器中使用了Data中包含的数据
         * @return false             管理器中没有使用Data中包含的数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-08
         */
        virtual bool isUsedDataObject(const QList<int>& data) = 0;
        /**
         * @brief 数据是否在管理器中被使用（关联）
         * @param[i]  dataIDs        被检查的数据ID
         * @param[i]  mgrs           被检查的管理器对象
         * @return true 
         * @return false 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-08
         */
        static bool IsUsedDataObjInManagers(const QList<int>& dataIDs,
            const QList<FITKAbstractDataManagerPrivate*>& mgrs);
        /**
         * @brief 获取迭代器
         * @return QListIterator<FITKAbstractDataObject*> 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-26
         */
        virtual QListIterator<FITKAbstractDataObject*> getIterator();
    };
}



#endif
