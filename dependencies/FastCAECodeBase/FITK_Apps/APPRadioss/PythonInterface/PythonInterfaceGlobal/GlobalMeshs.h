/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GlobalMeshs.h
 * @brief  网格相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 *********************************************************************/
#ifndef __PYTHONINTERFACE_GLOBALMESHS_H___
#define __PYTHONINTERFACE_GLOBALMESHS_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceGlobalAPI.h"

namespace Interface {
    class FITKGeoCommandList;
    class FITKMaterialManager;
    class FITKAbstractSectionManager;
}

namespace Radioss {
    class FITKRadiossMeshModel;
    class FITKRadiossPartManager;
    class FITKRadiossPart;
}

/**
 * @brief  网格接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class Meshs
{
public:
    /**
     * @brief    构造函数
     * @param[i] name 网格对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit Meshs(const QString& name);
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit Meshs();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    virtual ~Meshs() = default;

    /**
     * @brief    获取名称
     * @return   网格对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName();

    /**
     * @brief    设置名称
     * @param[i] name 网格对象名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool setName(const QString& name);

    /**
     * @brief    设置属性
     * @param[i] propertyName 属性名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool setProperty(const QString& propertyName);

    /**
     * @brief    设置材料
     * @param[i] materialName 材料名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool setMaterial(const QString& materialName);
    /**
     * @brief    转化 SPH 粒子
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool toSPHParticle();

public:
    /**
     * @brief    Radioss网格模型
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Radioss::FITKRadiossMeshModel* _meshData{};
    /**
     * @brief    材料管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKMaterialManager* _materialManager{};
    /**
     * @brief    属性管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKAbstractSectionManager* _sectionManager{};
    /**
     * @brief    Radioss部件管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Radioss::FITKRadiossPartManager* _radiossPartManager{};
    /**
     * @brief    Radioss部件
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Radioss::FITKRadiossPart* _meshPart{};
};

/**
 * @brief  网格相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class PythonInterfaceGlobalAPI MeshsWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 Meshs 实例
     * @return   新分配的 Meshs 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Meshs* new_Meshs();

    /**
     * @brief    释放 Meshs 实例
     * @param[i] obj 需要释放的 Meshs 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void delete_Meshs(Meshs* obj);

    /**
     * @brief    获取网格对象
     * @param[i] name 网格对象名称
     * @return   Meshs 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Meshs* static_Meshs_GetMesh(const QString& name);

    /**
     * @brief    网格转化 SPH 粒子
     * @param[i] name 网格对象名称
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString static_Meshs_MeshToSPHParticleConversion(const QString& name);

    /**
     * @brief    获取名称
     * @param[i] obj Meshs 对象指针
     * @return   名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString getName(Meshs* obj);

    /**
     * @brief    设置名称
     * @param[i] obj Meshs 对象指针
     * @param[i] name 名称
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString setName(Meshs* obj, const QString& name);

    /**
     * @brief    设置属性
     * @param[i] obj Meshs 对象指针
     * @param[i] propertyName 属性名称
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString setProperty(Meshs* obj, const QString& propertyName);

    /**
     * @brief    设置材料
     * @param[i] obj Meshs 对象指针
     * @param[i] materialName 材料名称
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString setMaterial(Meshs* obj, const QString& materialName);


private:

    /**
     * @brief    更新装配树形事件
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void updateAssemblyTreeEvent();
    /**
     * @brief    更新图形和树形事件
     * @param[i] dataObjId 数据对象ID
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void updateGraphAndTreeEvent(int dataObjId);
};

#endif