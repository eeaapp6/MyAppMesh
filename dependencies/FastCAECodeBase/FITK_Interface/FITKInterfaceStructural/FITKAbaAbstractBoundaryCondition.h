/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractBoundaryCondition.h
 * @brief  边界条件基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABAABSTRACTBOUNDARYCONDITION_H
#define FITKABAABSTRACTBOUNDARYCONDITION_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractLoadBC.h"
#include "FITKAbaStepRelatedDataObject.h"


namespace Interface
{

    class FITKAbstractFiled;
    /**
     * @brief  边界条件类型基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractBoundaryCondition
        : public FITKAbstractBC, public FITKAbaStepRelatedDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  边界条件类型枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        enum FITKAbaBoundaryConditionType
        {
            SymmetryAntisymmetryEncastre,
            DisplacementRotation,
            VelocityAngularVelocity,
            Temperature,
        };
        Q_ENUM(FITKAbaBoundaryConditionType);

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractBoundaryCondition() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractBoundaryCondition() = default;
        /**
         * @brief  是否成功获取分析步管理器
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool hasGetStepManager() override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取边界条件类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        virtual FITKAbaBoundaryConditionType getBoundaryConditionType() const = 0;
        /**
         * @brief  获取边界条件的激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool getActiveState() const;
        /**
         * @brief  设置边界条件的激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setActiveState(bool stat);
        /**
       * @brief  设置解析场/离散场ID
       * @param[i] id 
       * @author Libaojun (libaojunqd@foxmail.com)
       * @date   2024-07-12
       */
        void setFieldID(const int id);
        /**
         * @brief  获取解析场/离散场
         * @return FITKAbstractFiled*
         * @author Libaojun (libaojunqd@foxmail.com)
         * @date   2024-07-12
         */
        FITKAbstractFiled* getField();

    protected:
        /**
         * @brief  激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool m_activeState{ true };

        /**
       * @brief  解析场/离散场ID
       * @author Libaojun (libaojunqd@foxmail.com)
       * @date   2024-07-12
       */
        int _fieldID{ -1 };
    };
}


#endif // FITKABAABSTRACTBOUNDARYCONDITION_H
