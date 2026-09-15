/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   BoxWapper.h
 * @brief  长方体python接口
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-08
 *********************************************************************/
#ifndef __BoxWapper_H___
#define __BoxWapper_H___

#include <QString>
#include <QObject>

#include "PyGeometryBase.h"

class Box : public PyGeometryBase
{
public:
    explicit Box(const QString& name);
    explicit Box();
    virtual ~Box() = default;

    /**
     * @brief    创建长方体几何命令并加入当前几何列表
     * @param[i] name 几何名称
     * @param[i] x_p 基准点X坐标
     * @param[i] y_p 基准点Y坐标
     * @param[i] z_p 基准点Z坐标
     * @param[i] x_l X方向长度
     * @param[i] y_l Y方向长度
     * @param[i] z_l Z方向长度
     * @return   是否创建成功
     */
    bool createGeometry(const QString& name, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l);

    //编辑长 宽 高 基准点 to do
};

class PythonInterfaceAPI BoxWapper : public QObject
{
    Q_OBJECT
public slots:
    Box* new_Box();
    void delete_Box(Box* obj);
    Box* static_Box_GetGeometry(const QString& name);

    /**
     * @brief    PythonQt 装饰器入口，用于创建长方体并返回执行结果
     * @param[i] obj Box 对象指针
     * @param[i] x_p 基准点X坐标
     * @param[i] y_p 基准点Y坐标
     * @param[i] z_p 基准点Z坐标
     * @param[i] x_l X方向长度
     * @param[i] y_l Y方向长度
     * @param[i] z_l Z方向长度
     * @return   执行结果描述
     */
    QString createGeometry(Box* obj, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l);
    
    //编辑长 宽 高 基准点 to do
    
    
    QString getName(Box* obj);
    QString setName(Box* obj, const QString& name);
};

#endif