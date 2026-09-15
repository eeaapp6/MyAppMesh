/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file MaterialHydroWapper.h
 * @brief Hydro 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _MATERIAL_HYDRO_WAPPER_H___
#define _MATERIAL_HYDRO_WAPPER_H___

#include <QObject>

#include "PyMaterialsBase.h"
#include "PythonInterfacePropertiesAPI.h"

class Hydro : public PyInterface::PyMaterialsBase
{
public:
    explicit Hydro(const QString& name);
    explicit Hydro();
    virtual ~Hydro() = default;
};

/**
 * @brief Hydro 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI MaterialHydroWapper : public QObject
{
    Q_OBJECT

public slots:
    Hydro* new_Hydro();

    void delete_Hydro(Hydro* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    Hydro* static_Hydro_GetHydro(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(Hydro* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(Hydro* obj, const QString& name);

    /**
     * @brief 创建材料
     * @param[i] name 材料名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createMaterial(Hydro* obj, const QString& name);

    /**
     * @brief 设置材料密度
     * @param[i] density 密度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDensity(Hydro* obj, double density);

    /**
     * @brief 设置参考密度
        * @param[i] rDensity 参考密度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
     QString setReferenceDensity(Hydro* obj, double rDensity);

    /**
     * @brief 设置运动粘度
     * @param[i] viscosity 运动粘度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setKinematicViscosity(Hydro* obj, double viscosity);

    /**
     * @brief 设置最小压力截断值
     * @param[i] pressureCut 压力截断值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPressureCut(Hydro* obj, double pressureCut);

    /**
     * @brief 设置状态方程
     * @param[i] eosName 状态方程名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setEOS(Hydro* obj, const QString& eosName);
};

#endif