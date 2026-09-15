/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbaStepRelatedDataObject.h
 * @brief  与分析步相关的数据类基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-28
 *
 */
#ifndef FITKABASTEPRELATEDDATAOBJECT_H
#define FITKABASTEPRELATEDDATAOBJECT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QObject>

namespace Interface {
    class FITKAbaStepManager;

    class FITKInterfaceStructuralAPI FITKAbaStepEnum : public QObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  数据在分析步中的状态枚举
        * @author LiJin (1003680421@qq.com)
        * @date   2024-03-28
        */
        enum StateInStep
        {
            None,
            Created,
            Propagated,
            Modified,
            Deactived,
            NoLongerActive,
            BuiltIntoBaseState,
            InValid,
            ResetToInitial, //用于温度预定义场
            Computed,//用于速度场预定义
        };
        Q_ENUM(StateInStep);
    };

   
    /**
     * @brief  与分析步相关的数据基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-28
     */
    class FITKInterfaceStructuralAPI FITKAbaStepRelatedDataObject
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaStepRelatedDataObject() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        virtual ~FITKAbaStepRelatedDataObject() = default;
        /**
         * @brief  判断数据在id为stepId的分析步中是否发生改变，由子类实现
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        virtual bool isModifiedInStep(int stepId) const = 0 ;
        /**
         * @brief  数据激活状态发生变化时的响应函数，由子类实现
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        virtual void activeStateChanged() = 0;
        /**
         * @brief  判断是否已获取分析步管理器，
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        virtual bool hasGetStepManager() = 0;
        /**
         * @brief  是否在分析步中有效,用于数据在后续分析步中的状态判断
         * @param[in]  stepId 分析步Id。
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        virtual bool isValidInStep(int stepId);
        /**
         * @brief  当创建所在的分析步被替换时，判断在新的分析步中是否有效
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        virtual bool isValidInCreatedStep();
        /**
         * @brief  获取数据在分析步中的状态
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        virtual FITKAbaStepEnum::StateInStep getStateInStep(int stepId);
        /**
         * @brief  设置数据创建所在的分析步
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        void setCreatedStep(int stepId);
        /**
         * @brief  获取数据创建所在的分析步Id，如果为-1，说明未设置
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        int getCreatedStep() const;
        /**
         * @brief  设置数据不再激活的分析步
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        void setDeactiveStep(int stepId);
        /**
         * @brief  获取取消激活的分析步Id，如果为-1，说明未被取消激活
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        int getDeactiveStep() const;
        /**
         * @brief  在分析步中激活
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setActive();

    protected:
        /**
         * @brief  通过分析步Id获取分析步索引
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        int getStepIndex(int stepId);
        /**
         * @brief  通过分析步索引获取分析步Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        int getStepIdByIndex(int index) ;
        /**
         * @brief  获取分析步Linear perturbation性质是否发生变化
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        bool isStepPerturbationChanged(int stepId);
        /**
         * @brief  数据是否重新有效
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        bool isReValid(int stepId);
        /**
         * @brief  获取分析步id列表
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        QVector<int> getStepIds();
        /**
         * @brief 获取分析步管理器
         * @param[i]  obj           算例，及算例内的任意数据对象
         * @return FITKAbaStepManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        FITKAbaStepManager* getStepManager(Core::FITKAbstractDataObject* obj) const;

    protected:
        /**
         * @brief  数据创建所在的分析步Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        int m_createdStepId{-1};
        /**
         * @brief  数据不再激活的分析步Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-28
         */
        int m_deactiveStepId{-1};
    protected:
        /**
         * @brief  分析步管理器指针，避免重复查询
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        Interface::FITKAbaStepManager* m_stepManager{nullptr};
    };
}


#endif // FITKABASTEPRELATEDDATAOBJECT_H
