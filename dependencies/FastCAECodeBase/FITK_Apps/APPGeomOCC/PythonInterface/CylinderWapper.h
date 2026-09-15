/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   CylinderWapper.h
 * @brief  圆柱体python接口
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-08
 *********************************************************************/
#ifndef __CylinderWapper_H___
#define __CylinderWapper_H___

#include <QString>
#include <QObject>

#include "PyGeometryBase.h"

class Cylinder : public PyGeometryBase
{
public:
    explicit Cylinder(const QString& name);
    explicit Cylinder();
    virtual ~Cylinder() = default;

    /**
     * @brief    创建圆柱体几何命令并加入当前几何列表
     * @param[i] name 几何名称
     * @param[i] x_c 基准点X坐标
     * @param[i] y_c 基准点Y坐标
     * @param[i] z_c 基准点Z坐标
     * @param[i] x_d 方向X分量
     * @param[i] y_d 方向Y分量
     * @param[i] z_d 方向Z分量
     * @param[i] radius 圆柱体半径
     * @param[i] length 圆柱体长度
     * @return   是否创建成功
     */
    bool createGeometry(const QString& name, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length);

    //编辑基准点 方向 半径 长度 to do

};

class PythonInterfaceAPI CylinderWapper : public QObject
{
    Q_OBJECT
public slots:
    Cylinder* new_Cylinder();
    void delete_Cylinder(Cylinder* obj);
    Cylinder* static_Cylinder_GetGeometry(const QString& name);

    /**
     * @brief    PythonQt 装饰器入口，用于创建圆柱体并返回执行结果
     * @param[i] obj Cylinder 对象指针
     * @param[i] x_c 基准点X坐标
     * @param[i] y_c 基准点Y坐标
     * @param[i] z_c 基准点Z坐标
     * @param[i] x_d 方向X分量
     * @param[i] y_d 方向Y分量
     * @param[i] z_d 方向Z分量
     * @param[i] radius 圆柱体半径
     * @param[i] length 圆柱体长度
     * @return   执行结果描述
     */
    QString createGeometry(Cylinder* obj, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length);
    
    //编辑基准点 方向 半径 长度 to do
    
    QString getName(Cylinder* obj);
    QString setName(Cylinder* obj, const QString& name);
};

#endif