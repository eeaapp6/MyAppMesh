/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PyGeometryBase.h
 * @brief  Python几何接口基类
 * @author wangning (2185896382@qq.com)
 * @date   2026-06-05
 *********************************************************************/
#ifndef __PyGeometryBase_H___
#define __PyGeometryBase_H___

#include <QString>

#include "PythonInterfaceAPI.h"

namespace Interface {
    class FITKGeoCommandList;
    class FITKAbsGeoCommand;
}

/**
 * @brief  Python几何接口基础封装
 * @author wangning (2185896382@qq.com)
 * @date   2026-06-05
 */
class PythonInterfaceAPI PyGeometryBase
{
public:
    /**
     * @brief    默认构造函数
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    explicit PyGeometryBase();

    /**
     * @brief    通过名称构造几何对象包装
     * @param[i] name 几何对象名称
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    explicit PyGeometryBase(const QString& name);

    /**
     * @brief    析构函数
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    virtual ~PyGeometryBase() = default;

    /**
     * @brief    获取几何对象名称
     * @return   几何对象名称
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    QString getName() const;

    /**
     * @brief    设置几何对象名称
     * @param[i] name 几何对象名称
     * @return   是否设置成功
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    bool setName(const QString& name);

    /**
     * @brief    更新图形和树事件
     * @param[i] dataObjId 数据对象ID
     * @return   void
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-01
     */
    static void updateGraphAndTreeEvent();


public:
    /**
     * @brief    几何对象命令列表
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    Interface::FITKGeoCommandList* _cmdList{};

    /**
     * @brief    当前几何对象命令
     * @author   wangning (2185896382@qq.com)
     * @date     2026-06-05
     */
    Interface::FITKAbsGeoCommand* _command{};
};

#endif