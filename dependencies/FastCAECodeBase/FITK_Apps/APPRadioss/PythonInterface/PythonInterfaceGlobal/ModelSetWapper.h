/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ModelSetWapper.h
 * @brief  模型集相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 *********************************************************************/
#ifndef __PYTHONINTERFACE_MODELSETWAPER_H___
#define __PYTHONINTERFACE_MODELSETWAPER_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceGlobalAPI.h"

namespace Interface {
    class FITKComponentManager;
    class FITKModelSet;
}

namespace Radioss {
    class FITKRadiossMeshModel;
}

/**
 * @brief  模型集接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class ModelSet
{
public:
    /**
     * @brief    构造函数
     * @param[i] name 模型集对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit ModelSet(const QString& name);
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit ModelSet();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    virtual ~ModelSet() = default;

    /**
     * @brief    创建模型集合
     * @param[i] type 集合类型 (0:节点集合, 1:单元集合)
     * @param[i] member 集合成员列表
     * @return   是否创建成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool createModelSet(int type, const QVariantList& member);

    /**
     * @brief    获取名称
     * @return   模型集对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName();

    /**
     * @brief    设置名称
     * @param[i] name 模型集对象名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool setName(const QString& name);

public:
    /**
     * @brief    Radioss模型集合数据
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKModelSet* _modelSetData{};
    /**
     * @brief    集合管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKComponentManager* _componentManager{};
    /**
     * @brief    Radioss模型数据
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Radioss::FITKRadiossMeshModel* _meshModel{};
};

/**
 * @brief  模型集相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class PythonInterfaceGlobalAPI ModelSetWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 ModelSet 实例
     * @return   新分配的 ModelSet 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    ModelSet* new_ModelSet();

    /**
     * @brief    释放 ModelSet 实例
     * @param[i] obj 需要释放的 ModelSet 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void delete_ModelSet(ModelSet* obj);

    /**
     * @brief    获取模型集对象
     * @param[i] name 模型集对象名称
     * @return   ModelSet 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    ModelSet* static_ModelSet_GetModelSet(const QString& name);

    /**
     * @brief    创建模型集合对象
     * @param[i] obj ModelSet 对象指针
     * @param[i] type 集合类型 (0:节点集合, 1:单元集合)
     * @param[i] member 集合成员列表
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void createModelSet(ModelSet* obj, int type, const QVariantList& member);

    /**
     * @brief    创建模型集合对象(按范围)
     * @param[i] obj ModelSet 对象指针
     * @param[i] type 集合类型 (0:节点集合, 1:单元集合)
     * @param[i] m_minID 起始ID
     * @param[i] m_maxID 结束ID
     * @param[i] m_interval 间隔
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void createModelSet(ModelSet* obj, int type, int m_minID, int m_maxID, int m_interval);

    /**
     * @brief    获取名称
     * @param[i] obj ModelSet 对象指针
     * @return   名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName(ModelSet* obj);

    /**
     * @brief    设置名称
     * @param[i] obj ModelSet 对象指针
     * @param[i] name 名称
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void setName(ModelSet* obj, const QString& name);

private:
    /**
     * @brief    更新图形和树形事件
     * @param[i] dataObjId 数据对象ID
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void updateGraphAndTreeEvent(int dataObjId);
};

#endif