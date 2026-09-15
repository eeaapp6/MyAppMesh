/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file SolutionWapper.h
 * @brief 求解步骤装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
#ifndef _SOLUTION_WAPPER_H___
#define _SOLUTION_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKRadiossSolution;
}

class Solution
{
public:
    explicit Solution(const QString& name);
    explicit Solution();
    virtual ~Solution() = default;

    QString getName();

    bool setName(const QString& name);

    bool createSolution(const QString& name);

    static bool setCurrentSolution(const QString& name);

    static Radioss::FITKRadiossSolution* getCurrentSolution();

    /**
     * @brief 刷新渲染
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    void updateRender();

private:
    static Radioss::FITKRadiossSolution* _solution;
    
};

/**
 * @brief 求解方案装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
class PythonInterfaceSolutionAPI SolutionWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建求解方案对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    Solution* new_Solution();

    /**
     * @brief 删除求解方案对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    void delete_Solution(Solution* obj);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
     QString getName(Solution* obj);

     /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
     void setName(Solution* obj, const QString& name);

    /**
     * @brief 创建求解方案对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
     void createSolution(Solution* obj, const QString& name);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    Solution* static_Solution_GetSolution(const QString& name);

    /**
     * @brief 设置当前求解方案对象
     * @param[i] obj 求解方案对象
     * @param[i] name 求解方案名称
     * @return 是否成功提示信息
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString static_Solution_setCurrentSolution(const QString& name);
};

#endif