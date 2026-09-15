/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   SphereWapper.h
 * @brief  球体python接口
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-08
 *********************************************************************/
#ifndef __SphereWapper_H___
#define __SphereWapper_H___

#include <QString>
#include <QObject>

#include "PyGeometryBase.h"

class Sphere : public PyGeometryBase
{
public:
    explicit Sphere(const QString& name);
    explicit Sphere();
    virtual ~Sphere() = default;

    /**
     * @brief    创建球体几何命令并加入当前几何列表
     * @param[i] name 几何名称
     * @param[i] x_c 球心X坐标
     * @param[i] y_c 球心Y坐标
     * @param[i] z_c 球心Z坐标
     * @param[i] radius 球体半径
     * @return   是否创建成功
     */
    bool createGeometry(const QString& name, double x_c, double y_c, double z_c, double radius);

    //编辑球心 半径 to do

};

class PythonInterfaceAPI SphereWapper : public QObject
{
    Q_OBJECT
public slots:
    Sphere* new_Sphere();
    void delete_Sphere(Sphere* obj);
    Sphere* static_Sphere_GetGeometry(const QString& name);

    /**
     * @brief    PythonQt 装饰器入口，用于创建球体并返回执行结果
     * @param[i] obj Sphere 对象指针
     * @param[i] x_c 球心X坐标
     * @param[i] y_c 球心Y坐标
     * @param[i] z_c 球心Z坐标
     * @param[i] radius 球体半径
     * @return   执行结果描述
     */
    QString createGeometry(Sphere* obj, double x_c, double y_c, double z_c, double radius);
    
    //编辑球心 半径 to do

    QString getName(Sphere* obj);
    QString setName(Sphere* obj, const QString& name);
};

#endif