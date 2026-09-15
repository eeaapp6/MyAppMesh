/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyMaterialsBase.h
 * @brief 材料基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _PY_MATERIALS_BASE_H___
#define _PY_MATERIALS_BASE_H___


#include <QString>
#include "PythonInterfacePropertiesAPI.h"

namespace Radioss 
{
    class  FITKAbstractRadMaterial;
}
namespace Interface
{
    class FITKMaterialManager;
}
/**
 * @brief 材料基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
namespace PyInterface
{
    class PythonInterfacePropertiesAPI PyMaterialsBase
    {
    public:
        explicit PyMaterialsBase(const QString& name);
        explicit PyMaterialsBase();
        virtual ~PyMaterialsBase() = default;

        /**
         * @brief  获取名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        QString getName();
        /**
         * @brief  设置名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        bool setName(const QString& name);
        /**
         * @brief  创建材料
         * @param[i] name 材料名称
         * @param[i] type 材料类型
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        bool createMaterial(const QString& name,int type);

		//获取材料指针
        Radioss::FITKAbstractRadMaterial* getMaterial();

        //刷新渲染
		void updateRender();

    protected:
        Radioss::FITKAbstractRadMaterial* _material{};

		Interface::FITKMaterialManager* _materialManager{};
    };
}

#endif