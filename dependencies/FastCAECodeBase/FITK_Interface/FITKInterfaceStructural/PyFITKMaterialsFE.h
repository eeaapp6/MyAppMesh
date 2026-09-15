/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __PY__MATERIALS__FE__H___
#define __PY__MATERIALS__FE__H___

#include <QString>
#include "FITKInterfaceStructuralAPI.h"

namespace Interface
{
    class FITKAbaMaterial;
    class FITKMaterialManager;
}
namespace Acoustics
{

    /**
     * @brief  FE 材料 Python脚本接口
     * @author wangning (2185896382@qq.com)
     * @date   2024-12-03
     */
    class FITKInterfaceStructuralAPI PyFITKMaterialsFE
    {
    public:
        explicit PyFITKMaterialsFE(const QString& name);
        explicit PyFITKMaterialsFE() = default;
        virtual ~PyFITKMaterialsFE() = default;

        /**
         * @brief  设置属性
         * @param[i]  type  1：各向同性杨氏模量   E
                            2：Shear Modules
                            3：各向同性泊松比     NU
                            4：密度值             RHO
                            5：Thermal Exp Coeff 
                            6：Ref Temp object
                            7：阻尼         GE
                            8: Stress Limit Tension
                            9: Stress Limit Compress
                            10: Stress Limit Shear
                            11: Coordinate Sys
         * @param[i]  value 给定值
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        bool setField(int type, double value);
        /**
         * @brief  获取名称
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        QString getName();
        /**
        * @brief  设置名称
        * @author wangning (2185896382@qq.com)
        * @date   2024-12-03
        */
        bool setName(const QString& name);
        /**
         * @brief  创建材料 
         * @param[i]  name 名字
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        bool createMaterial(const QString& name);
        /**
         * @brief  设置材料管理器
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        void setMaterialManager(Interface::FITKMaterialManager * mgr);

    private:
        /**
         * @brief  FE材料
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        Interface::FITKAbaMaterial * _FEMaterial{};
        /**
         * @brief  材料管理器
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        Interface::FITKMaterialManager * _mgr{};
        /**
         * @brief  材料对象名称
         * @author wangning (2185896382@qq.com)
         * @date   2024-12-03
         */
        QString _FEMaterialName{};
    };
}


#endif
