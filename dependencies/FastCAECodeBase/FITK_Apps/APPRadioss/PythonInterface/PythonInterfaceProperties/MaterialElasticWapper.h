/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file MaterialElasticWapper.h
 * @brief 弹性材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _MATERIAL_ELASTIC_WAPPER_H___
#define _MATERIAL_ELASTIC_WAPPER_H___

#include <QObject>
#include "PyMaterialsBase.h"
#include "PythonInterfacePropertiesAPI.h"


class Elastic : public PyInterface::PyMaterialsBase
{
public:
    explicit Elastic(const QString& name);
    explicit Elastic();
    virtual ~Elastic() = default;

};

/**
 * @brief 弹性材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI MaterialElasticWapper : public QObject
{
    Q_OBJECT
public slots:
    Elastic* new_Elastic();

    void delete_Elastic(Elastic* obj);
    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    Elastic* static_Elastic_GetElastic(const QString& name);
    /**
    * @brief 获取名称
    * @author wangning (2185896382@qq.com)
    * @date 2026-05-12
    */
    QString getName(Elastic* obj);
    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(Elastic* obj, const QString& name);
    /**
     * @brief  创建材料
     * @param[i]  name：材料名称
     * @author wangning (2185896382@qq.com)
     * @date   2026-05-12
     */
    QString createMaterial(Elastic* obj, const QString& name);
    /**
     * @brief 设置材料密度
     * @param[i] density 密度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDensity(Elastic* obj, double density);

    /**
     * @brief 设置杨氏模量
     * @param[i] modulus 杨氏模量值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setYoungsModulus(Elastic* obj, double modulus);

    /**
     * @brief 设置泊松比
     * @param[i] ratio 泊松比值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPoissonsRatio(Elastic* obj, double ratio);

};


#endif