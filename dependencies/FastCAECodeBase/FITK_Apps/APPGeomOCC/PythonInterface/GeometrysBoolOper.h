/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GeometrysBoolOper.h
 * @brief  几何布尔操作Python接口封装
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-01
 *********************************************************************/
#ifndef __GeometrysBoolOper_H___
#define __GeometrysBoolOper_H___

#include <QString>
#include <QObject>
#include "PyGeometryBase.h"

namespace Interface {
    class FITKGeoCommandList;
    class FITKAbsGeoCommand;
    class VirtualShape;
}

/**
 * @brief  几何布尔操作封装类，供装饰器装饰
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-01
 */
class BoolOper : public PyGeometryBase
{
public:
    explicit BoolOper();
    explicit BoolOper(const QString& name);
    virtual ~BoolOper() = default;

    /**
     * @brief    创建布尔操作
     * @param[i] targetName 
     * @param[i] toolName 
     * @return   是否创建成功
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-02
     */
    bool createGBTAdd(const QString& targetName, const QString& toolName);
    bool createGBTCut(const QString& targetName, const QString& toolName);
    bool createGBTCommon(const QString& targetName, const QString& toolName);

private:
    /**
     * @brief    从几何命令获取VirtualShape
     * @param[i] cmd 几何命令指针
     * @return   VirtualShape
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    Interface::VirtualShape getVirtualShapeFromCommand(Interface::FITKAbsGeoCommand* cmd);

};

/**
 * @brief  几何布尔操作PythonQt接口装饰器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-01
 */
class PythonInterfaceAPI BoolOperWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 BoolOper 实例
     * @return   新分配的 BoolOper 指针
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    BoolOper* new_BoolOper();

    /**
     * @brief    释放 BoolOper 实例
     * @param[i] obj 需要释放的 BoolOper 指针
     * @return   void
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    void delete_BoolOper(BoolOper* obj);

    /**
     * @brief    获取几何对象
     * @param[i] name 几何对象名称
     * @return   Geometrys 指针
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    BoolOper* static_BoolOper_GetGeometry(const QString& name);

    /**
     * @brief    创建布尔操作
     * @param[i] obj BoolOper 对象指针
     * @param[i] targetName 目标几何名称
     * @param[i] toolName 工具几何名称
     * @return   QString 执行结果信息
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    QString createGBTAdd(BoolOper* obj, const QString& targetName, const QString& toolName);
    QString createGBTCut(BoolOper* obj, const QString& targetName, const QString& toolName);
    QString createGBTCommon(BoolOper* obj, const QString& targetName, const QString& toolName);
    /**
     * @brief    获取名称
     * @param[i] obj BoolOper 对象指针
     * @return   几何对象名称
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    QString getName(BoolOper* obj);

    /**
     * @brief    设置名称
     * @param[i] obj BoolOper 对象指针
     * @param[i] name 几何对象名称
     * @return   QString 执行结果信息
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    QString setName(BoolOper* obj, const QString& name);
};

#endif
