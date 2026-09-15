/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionPropertyContact.h
 * @brief  相互作用属性-接触
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAINTERACTIONPROPERTYCONTACT_H
#define FITKABAINTERACTIONPROPERTYCONTACT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractInteractionProperty.h"
#include "FITKAbaInteractionContactBehavior.h"
#include <QList>


namespace Interface {
    /**
     * @brief  相互作用属性-接触类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionPropertyContact: public FITKAbaAbstractInteractionProperty
    {
        FITKCLASS(Interface, FITKAbaInteractionPropertyContact)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionPropertyContact() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionPropertyContact();
        /**
         * @brief  获取相互作用属性类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionPropertyType getInteractionPropertyType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取所有接触属性行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<FITKAbaInteractionContactBehavior*> getBehaviors();
        /**
         * @brief  是否具备type类型的接触属性行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool hasBehavior(FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType type) const;
        /**
         * @brief  增加type类型的接触属性行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void addBehavior(FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType type);
        /**
         * @brief  移除type类型的接触属性行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void removeBehavior(FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType type);
        /**
         * @brief  获取type类型的接触属性行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaInteractionContactBehavior* getBehavoir(FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType type);

    private:
        /**
         * @brief  清空所有接触行为
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void clearAllBehaviors();

    private:
        /**
         * @brief  接触行为容器
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<FITKAbaInteractionContactBehavior*> m_behaviors{};
    };
}


#endif // FITKABAINTERACTIONPROPERTYCONTACT_H
