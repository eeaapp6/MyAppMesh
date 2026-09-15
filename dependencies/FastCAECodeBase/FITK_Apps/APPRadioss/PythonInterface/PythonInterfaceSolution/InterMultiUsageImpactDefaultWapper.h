/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InterMultiUsageImpactDefaultWapper.h
 * @brief 默认多用途冲击装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _INTER_MULTI_USAGE_IMPACT_DEFAULT_WAPPER_H___
#define _INTER_MULTI_USAGE_IMPACT_DEFAULT_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKInteractionMultiUsageImpactDefaultValue;
}

/**
 * @brief 默认多用途冲击接口封装
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class InterMultiUsageImpactDefault
{
public:
    /**
     * @brief 构造函数
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    explicit InterMultiUsageImpactDefault() = default;

    /**
     * @brief 析构函数
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    virtual ~InterMultiUsageImpactDefault() = default;

    /**
     * @brief 获取第一个默认多用途冲击对象
     * @return 默认多用途冲击对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* getInterMultiUsageImpactDefault() const;
};

/**
 * @brief 默认多用途冲击装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class PythonInterfaceSolutionAPI InterMultiUsageImpactDefaultWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建默认多用途冲击对象
     * @return 默认多用途冲击对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    InterMultiUsageImpactDefault* new_InterMultiUsageImpactDefault();

    /**
     * @brief 删除默认多用途冲击对象
     * @param[in] obj 默认多用途冲击对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    void delete_InterMultiUsageImpactDefault(InterMultiUsageImpactDefault* obj);

    /**
     * @brief 设置刚度标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 刚度标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIstf(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置间隙单元选项标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 间隙单元选项标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIgap(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置气囊通风孔关闭标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 气囊通风孔关闭标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIbag(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置节点退化表面标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 节点退化表面标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIdel(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置次节点停用标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 次节点停用标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIremGap(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置辅助节点停用标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 辅助节点停用标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIremI2(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置初始穿透刚度停用标志
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 初始穿透刚度停用标志值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setInacti(InterMultiUsageImpactDefault* obj, int value);

    /**
     * @brief 设置摩擦惩罚公式类型
     * @param[in] obj 默认多用途冲击对象
     * @param[in] value 摩擦惩罚公式类型值
     * @return 结果信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setIform(InterMultiUsageImpactDefault* obj, int value);

private:
    /**
     * @brief 获取默认多用途冲击对象
     * @param[in] obj 默认多用途冲击对象
     * @return 默认多用途冲击对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* getInterMultiUsageImpactDefault(InterMultiUsageImpactDefault* obj);
};

#endif