/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   MeshGenerateWapper.h
 * @brief  网格生成相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 *********************************************************************/
#ifndef __PYTHONINTERFACE_MESHGENERATEWAPER_H___
#define __PYTHONINTERFACE_MESHGENERATEWAPER_H___

#include <QString>
#include <QObject>
#include <QEventLoop>
#include "PythonInterfaceGlobalAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Interface {
    class FITKGeoCommandList;
    class FITKAbstractMesherDriver;
}

namespace Radioss {
    class FITKRadiossMeshModel;
}

/**
 * @brief  网格生成接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class MeshGenerate
{
public:
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    explicit MeshGenerate();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    virtual ~MeshGenerate() = default;

    /**
     * @brief    添加几何面用于网格面生成
     * @param[i] g_name 几何对象名称
     * @param[i] index_faces 面的索引列表
     * @return   是否添加成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool addFace(const QString& g_name, const QVariantList& index_faces);

    /**
     * @brief    添加几何体用于网格体生成
     * @param[i] g_name 几何对象名称
     * @param[i] index_volumes 体的索引列表
     * @return   是否添加成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    bool addVolume(const QString& g_name, const QVariantList& index_volumes);

public:
    /**
     * @brief    几何面虚拓扑数据用于网格面生成
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QList<Interface::VirtualShape> _vShape_faces{};
    /**
     * @brief    几何体虚拓扑数据用于网格体生成
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QList<Interface::VirtualShape> _vShape_volumes{};
    /**
     * @brief    几何命令列表
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    Interface::FITKGeoCommandList* _cmdList{};
    /**
     * @brief    网格生成尺寸范围(最小尺寸, 最大尺寸)
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    double _size[2]{ 0.0,0.0 };
};

/**
 * @brief  网格生成相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-12
 */
class PythonInterfaceGlobalAPI MeshGenerateWapper : public QObject
{
    Q_OBJECT
public slots:

    MeshGenerate* new_MeshGenerate();
    void delete_MeshGenerate(MeshGenerate* obj);

    /**
     * @brief    静态方法生成网格
     * @param[i] g_name 几何对象名称
     * @param[i] dim 网格维度(2或3)
     * @param[i] size_mesh 网格尺寸(可选，默认为0表示使用默认尺寸)
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString static_MeshGenerate_GenerateMesh(const QString& g_name, int dim, double size_mesh);

    /**
     * @brief    添加几何面用于网格面生成
     * @param[i] obj MeshGenerate 对象指针
     * @param[i] g_name 几何对象名称
     * @param[i] index_faces 面的索引列表(空列表表示添加所有面)
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString addFace(MeshGenerate* obj, const QString& g_name, const QVariantList& index_faces = QVariantList());

    /**
     * @brief    添加几何体用于网格体生成
     * @param[i] obj MeshGenerate 对象指针
     * @param[i] g_name 几何对象名称
     * @param[i] index_volumes 体的索引列表(空列表表示添加所有体)
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString addVolume(MeshGenerate* obj, const QString& g_name, const QVariantList& index_volumes = QVariantList());

    /**
     * @brief    设置网格尺寸
     * @param[i] obj MeshGenerate 对象指针
     * @param[i] size_min 最小网格尺寸
     * @param[i] size_max 最大网格尺寸
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString setMeshSize(MeshGenerate* obj, double size_min, double size_max);

    /**
     * @brief    执行网格生成
     * @param[i] obj MeshGenerate 对象指针
     * @param[i] dim 网格维度(2或3)
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString generate(MeshGenerate* obj, int dim);

private:
    /**
     * @brief    内部网格生成方法
     * @param[i] dim 网格维度
     * @param[i] size_min 最小网格尺寸
     * @param[i] size_max 最大网格尺寸
     * @param[i] vShape 虚拓扑数据列表
     * @param[i] cmdID 几何命令ID(-1表示使用vShape)
     * @return   QString meshPartName
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    QString meshGenerate(int dim, double size_min, double size_max, const QList<Interface::VirtualShape> vShape, int cmdID = -1);
    /**
     * @brief    更新图形和树形事件
     * @param[i] dataObjId 数据对象ID
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-12
     */
    void updateGraphAndTreeEvent(int dataObjId);

};

#endif