/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file BCSWapper.h
 * @brief 边界条件装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
#ifndef _BCS_WAPPER_H___
#define _BCS_WAPPER_H___

#include <QObject>

#include "PyBCSBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKRadiossBCS;
}

class PythonInterfaceSolutionAPI BCS : public PyInterface::PyBCSBase
{
public:
    explicit BCS(const QString& name);
    explicit BCS();
    virtual ~BCS() = default;

    /**
     * @brief 获取边界条件对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    Radioss::FITKRadiossBCS* getBCS();
};

/**
 * @brief 边界条件装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
class PythonInterfaceSolutionAPI BCSWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建边界条件对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    BCS* new_BCS();

    /**
     * @brief 删除边界条件对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    void delete_BCS(BCS* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    BCS* static_BCS_GetBCS(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getName(BCS* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setName(BCS* obj, const QString& name);

    /**
     * @brief 创建边界条件对象
     * @param[i] name 边界条件名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString createBCS(BCS* obj, const QString& name = QString());

    // /**
    //  * @brief 设置坐标系 ID
    //  * @param[i] id 坐标系 ID
    //  * @author wangning (2185896382@qq.com)
    //  * @date 2026-05-22
    //  */
    // QString setCoordinateSystemID(BCS* obj, int id);

    /**
     * @brief 设置第一节点组
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setNodeGroup(BCS* obj, const QString& name);

    /**
     * @brief 设置自由度约束状态
     * @param[i] index 自由度索引，范围 1-6
     * @param[i] fixed 是否固定
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setFreedomFixed(BCS* obj, int index, bool fixed);
};

#endif