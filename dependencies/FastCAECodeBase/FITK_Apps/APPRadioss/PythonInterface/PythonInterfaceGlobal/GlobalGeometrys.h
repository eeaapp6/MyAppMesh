/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GlobalGeometrys.h
 * @brief  几何相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-11
 *********************************************************************/
#ifndef __PYTHONINTERFACE_GLOBALGEOMETRYS_H___
#define __PYTHONINTERFACE_GLOBALGEOMETRYS_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceGlobalAPI.h"

namespace Interface {
    class FITKGeoCommandList;
}

namespace Radioss {
    class FITKRadiossGeomPart;
    class FITKRadiossGeomPartManager;
}

/**
 * @brief  几何接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-11
 */
class Geometrys
{
public:
    /**
     * @brief    构造函数
     * @param[i] name 几何对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    explicit Geometrys(const QString& name);
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    explicit Geometrys();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    virtual ~Geometrys() = default;

    /**
     * @brief    创建长方体
     * @param[i] name 长方体名称
     * @param[i] x_p X坐标
     * @param[i] y_p Y坐标
     * @param[i] z_p Z坐标
     * @param[i] x_l X方向长度
     * @param[i] y_l Y方向长度
     * @param[i] z_l Z方向长度
     * @return   是否创建成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    bool createBox(const QString& name, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l);

    /**
     * @brief    创建球体
     * @param[i] name 球体名称
     * @param[i] x_c X坐标
     * @param[i] y_c Y坐标
     * @param[i] z_c Z坐标
     * @param[i] radius 球体半径
     * @return   是否创建成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    bool createSphere(const QString& name, double x_c, double y_c, double z_c, double radius);

    /**
     * @brief    创建圆柱体
     * @param[i] name 圆柱体名称
     * @param[i] x_c X坐标
     * @param[i] y_c Y坐标
     * @param[i] z_c Z坐标
     * @param[i] x_d X方向分量
     * @param[i] y_d Y方向分量
     * @param[i] z_d Z方向分量
     * @param[i] radius 圆柱体半径
     * @param[i] length 圆柱体长度
     * @return   是否创建成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    bool createCylinder(const QString& name, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length);

    /**
     * @brief    获取名称
     * @return   几何对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString getName();

    /**
     * @brief    设置名称
     * @param[i] name 几何对象名称
     * @return   是否设置成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    bool setName(const QString& name);

public:
    /**
     * @brief    几何对象命令列表
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    Interface::FITKGeoCommandList* _cmdList{};
    /**
     * @brief    Radioss几何对象
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    Radioss::FITKRadiossGeomPart* _geometryData{};
    /**
     * @brief    Radioss几何部件管理器
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    Radioss::FITKRadiossGeomPartManager* _radiossGeomPartManager{};
};

/**
 * @brief  几何相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-11
 */
class PythonInterfaceGlobalAPI GeometrysWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 Geometrys 实例
     * @return   新分配的 Geometrys 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    Geometrys* new_Geometrys();

    /**
     * @brief    释放 Geometrys 实例
     * @param[i] obj 需要释放的 Geometrys 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    void delete_Geometrys(Geometrys* obj);

    /**
     * @brief    获取几何对象
     * @param[i] name 几何对象名称
     * @return   Geometrys 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    Geometrys* static_Geometrys_GetGeometry(const QString& name);

    /**
     * @brief    创建长方体
     * @param[i] obj Geometrys 对象指针
     * @param[i] x_p 基准点X坐标
     * @param[i] y_p 基准点Y坐标
     * @param[i] z_p 基准点Z坐标
     * @param[i] x_l X方向长度
     * @param[i] y_l Y方向长度
     * @param[i] z_l Z方向长度
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString createBox(Geometrys* obj, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l);

    /**
     * @brief    创建球体
     * @param[i] obj Geometrys 对象指针
     * @param[i] x_p 中心点X坐标
     * @param[i] y_p 中心点Y坐标
     * @param[i] z_p 中心点Z坐标
     * @param[i] radius 球体半径
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString createSphere(Geometrys* obj, double x_p, double y_p, double z_p, double radius);

    /**
     * @brief    创建圆柱体
     * @param[i] obj Geometrys 对象指针
     * @param[i] x_c 基准点X坐标
     * @param[i] y_c 基准点Y坐标
     * @param[i] z_c 基准点Z坐标
     * @param[i] x_d 方向X分量
     * @param[i] y_d 方向Y分量
     * @param[i] z_d 方向Z分量
     * @param[i] radius 圆柱体半径
     * @param[i] length 圆柱体长度
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString createCylinder(Geometrys* obj, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length);

    /**
     * @brief    获取名称
     * @param[i] obj Geometrys 对象指针
     * @return   几何对象名称
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString getName(Geometrys* obj);

    /**
     * @brief    设置名称
     * @param[i] obj Geometrys 对象指针
     * @param[i] name 几何对象名称
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    QString setName(Geometrys* obj, const QString& name);

private:
    /**
     * @brief    更新装配树事件
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    void updateAssemblyTreeEvent();
    /**
     * @brief    更新图形和树事件
     * @param[i] dataObjId 数据对象ID
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-11
     */
    static void updateGraphAndTreeEvent(int dataObjId);
};

#endif