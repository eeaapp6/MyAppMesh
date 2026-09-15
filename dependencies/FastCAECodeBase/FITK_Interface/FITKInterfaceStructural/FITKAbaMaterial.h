/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterial.h
 * @brief  结构模板（Abaqus）材料类型。通过管理各种类型的属性行为，实现属性参数管理
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef __FITKABAMATERIAL_H__
#define __FITKABAMATERIAL_H__

#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITKAbaMaterialBehavior.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include <QList>

namespace Interface
{
    class FITKAbaMaterialBehavior;
 //   enum class FITKAbaMaterialBehaviorType;
    /**
     * @brief  结构模板材料类型
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterial : public Interface::FITKAbstractMaterial
    {
        FITKCLASS(Interface, FITKAbaMaterial);
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaMaterial() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaMaterial();
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  判断当前是否添加了type类型的属性行为
         * @param[in]  type 属性行为类型，如密度、弹性等
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool hasMaterialBehavior(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType type);
        /**
         * @brief  添加type类型的属性行为。添加成功则返回true，失败则返回false。如果当前已具备该类型
         * 属性行为，则返回true
         * @param[in]  type 属性行为枚举类型 FITKAbaMaterialBehaviorType
         * @return bool值。
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool addMaterialBehavior(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType type);
        /**
         * @brief  移除type类型的属性行为
         * @param[in]  type 属性行为枚举类型 FITKAbaMaterialBehaviorType
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void removeMaterialBehavior(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType type);
        /**
         * @brief  获取当前具备的所有属性行为
         * @param[in]  void
         * @return QList<FITKAbaMaterialBehavior*> 类型属性行为链表
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QList<FITKAbaMaterialBehavior*> getMaterialBeahviors() const;
        /**
         * @brief  获取type类型的属性行为
         * @param[in]  type 属性行为枚举类型 FITKAbaMaterialBehaviorType
         * @return 属性行为指针。若尚未添加该类型属性行为，则返回nullptr
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        FITKAbaMaterialBehavior *getMaterialBehavior(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType type);
    private:
        /**
         * @brief  清空属性行为数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void clearBehaviors();
    private:
        /**
         * @brief  属性行为链表
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QList<FITKAbaMaterialBehavior*> m_materialBehaviors{};
    };
}



#endif
