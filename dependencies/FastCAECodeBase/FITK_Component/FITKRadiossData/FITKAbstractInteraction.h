/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITK_ABSTRACT_INTERACTION_H_Radioss
#define FITK_ABSTRACT_INTERACTION_H_Radioss

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbstractRadiossData.h"

namespace Radioss
{
    /**
     * @brief  相互作用默认值基类
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-19
     */
    class FITKRadiossDataAPI FITKInteractionDefaultAbstract : public Core::FITKAbstractDataObject
    {
    public:
        enum DefaultInteractionType
        {
            DIT_None = -1,
            DIT_KinematicTied = 0,
            DIT_MultiUsageImpact,
        };

        explicit FITKInteractionDefaultAbstract() = default;
        virtual ~FITKInteractionDefaultAbstract() = 0 {};

        virtual DefaultInteractionType getDefaultInteractionType();
    };

    /**
     * @brief  相互作用默认值管理器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-19
     */
    class FITKRadiossDataAPI FITKRadiossInteractionDefaultManager : public Core::FITKAbstractDataManager<FITKInteractionDefaultAbstract>
    {
    public:
        explicit FITKRadiossInteractionDefaultManager() = default;
        virtual ~FITKRadiossInteractionDefaultManager() = default;
    };




    class FITKRadiossDataAPI FITKAbstractInteraction : public Core::FITKAbstractNDataObject,
           public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
    enum InteractionType
        {
            IT_None = -1,
            IT_Tied = 0,
            IT_KinematicTied, // 动力学绑定 /inter/type2
            IT_MultiUsageImpact, // 多用途冲击 /inter/type7
        };
        Q_ENUM(InteractionType);


    public:
        explicit FITKAbstractInteraction() = default;
        virtual ~FITKAbstractInteraction() = 0;
        /**
        * @brief 设置主面（Master Surface）ID
        * @param id 主面ID
        */
        void setMasterSurfaceID(int id);
        /**
         * @brief 获取主面（Master Surface）ID
         * @return 主面ID
         */
        int getMasterSurfaceID() const;

        /**
         * @brief 设置从节点组ID
         * @param id 节点组ID
         */
        void setNodeGroupID(int id);
        /**
         * @brief 获取从节点组ID
         * @return 节点组ID
         */
        int getNodeGroupID() const;

        virtual InteractionType getInteractionType();
    private:
        int m_masterSurfaceID{ -1 }; // 绑定主从面
        int m_nodeGroupID{ -1 };
    };

    class FITKRadiossDataAPI FITKInteractionManager 
        : public Core::FITKAbstractDataManager<FITKAbstractInteraction>
    {   
    public:
        explicit FITKInteractionManager() = default;
        virtual ~FITKInteractionManager() = default;
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_INTERACTION_H