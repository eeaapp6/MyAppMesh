/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ModelSurfaceWapper.h
 * @brief  模型面相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 *********************************************************************/
#ifndef __PYTHONINTERFACE_MODELSURFACEWAPER_H___
#define __PYTHONINTERFACE_MODELSURFACEWAPER_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceGlobalAPI.h"

namespace Interface {
    class FITKComponentManager;
    class FITKMeshSurface;
}

/**
 * @brief  模型面接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class ModelSurface
{
public:
    /**
     * @brief    构造函数
     * @param[i] name 模型面对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit ModelSurface(const QString& name);
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit ModelSurface();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    virtual ~ModelSurface() = default;

    /**
     * @brief    获取名称
     * @return   模型面对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName();

    /**
     * @brief    设置名称
     * @param[i] name 模型面对象名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool setName(const QString& name);

public:
    /**
     * @brief    Radioss模型面数据
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKMeshSurface* _modelSurfaceData{};
    /**
     * @brief    集合管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKComponentManager* _componentManager{};
};

/**
 * @brief  模型面相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class PythonInterfaceGlobalAPI ModelSurfaceWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 ModelSurface 实例
     * @return   新分配的 ModelSurface 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    ModelSurface* new_ModelSurface();

    /**
     * @brief    释放 ModelSurface 实例
     * @param[i] obj 需要释放的 ModelSurface 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void delete_ModelSurface(ModelSurface* obj);

    /**
     * @brief    获取模型面对象
     * @param[i] name 模型面对象名称
     * @return   ModelSurface 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    ModelSurface* static_ModelSurface_GetModelSurface(const QString& name);

    /**
     * @brief    获取名称
     * @param[i] obj ModelSurface 对象指针
     * @return   名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName(ModelSurface* obj);

    /**
     * @brief    设置名称
     * @param[i] obj ModelSurface 对象指针
     * @param[i] name 名称
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void setName(ModelSurface* obj, const QString& name);

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