/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InterTiedDefaultWapper.h
 * @brief 默认动力学绑定装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _INTER_TIED_DEFAULT_WAPPER_H___
#define _INTER_TIED_DEFAULT_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKInteractionKinematicTiedDefault;
}

/**
 * @brief 默认动力学绑定接口封装
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class InterTiedDefault
{
public:
    /**
     * @brief 构造函数
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    explicit InterTiedDefault() = default;

    /**
     * @brief 析构函数
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    virtual ~InterTiedDefault() = default;

    /**
     * @brief 获取第一个默认动力学绑定对象
     * @return 默认动力学绑定对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    Radioss::FITKInteractionKinematicTiedDefault* getInterTiedDefault() const;
};

/**
 * @brief 默认动力学绑定装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class PythonInterfaceSolutionAPI InterTiedDefaultWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建默认动力学绑定对象
     * @return 默认动力学绑定对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    InterTiedDefault* new_InterTiedDefault();

    /**
     * @brief 删除默认动力学绑定对象
     * @param[in] obj 默认动力学绑定对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    void delete_InterTiedDefault(InterTiedDefault* obj);


    /**
     * @brief 设置忽略标志
     * @param[in] obj 默认动力学绑定对象
     * @param[in] flag 忽略标志
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIgnoreFlag(InterTiedDefault* obj, int flag);

    /**
     * @brief 设置点焊公式标志
     * @param[in] obj 默认动力学绑定对象
     * @param[in] flag 点焊公式标志
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setSpotWeldFormulationFlag(InterTiedDefault* obj, int flag);

    /**
     * @brief 设置搜索公式标志
     * @param[in] obj 默认动力学绑定对象
     * @param[in] flag 搜索公式标志
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setSearchFormulationFlag(InterTiedDefault* obj, int flag);

    /**
     * @brief 设置节点删除阈值标志
     * @param[in] obj 默认动力学绑定对象
     * @param[in] flag 节点删除阈值标志
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setNodeDeletionFlag(InterTiedDefault* obj, int flag);

    /**
     * @brief 设置接口刚度定义标志
     * @param[in] obj 默认动力学绑定对象
     * @param[in] flag 接口刚度定义标志
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIstfFlagDefault(InterTiedDefault* obj, int flag);
    
private:
    /**
     * @brief 获取默认动力学绑定对象
     * @param[in] obj 默认动力学绑定对象
     * @return 默认动力学绑定对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    Radioss::FITKInteractionKinematicTiedDefault* getInterTiedDefault(InterTiedDefault* obj);
};

#endif