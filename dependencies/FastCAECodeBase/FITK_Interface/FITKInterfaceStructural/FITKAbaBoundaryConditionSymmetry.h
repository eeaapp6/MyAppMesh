/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaBoundaryConditionSymmetry.h
 * @brief  对称/反对称/完全固定边界条件类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABABOUNDARYCONDITIONSYMMETRY_H
#define FITKABABOUNDARYCONDITIONSYMMETRY_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractBoundaryCondition.h"


namespace Interface {
    class FITKCoordinateSystem;
    /**
     * @brief  对称/反对称/固定约束类型枚举
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI AbaSymmetryAntisymmetryEncastreType: public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  约束类型枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        enum SymmetryAntisymmetryEncastreType
        {
            None,
            XSYMM,
            YSYMM,
            ZSYMM,
            XASYMM,
            YASYMM,
            ZASYMM,
            PINNED,
            ENCASTRE
        };
        Q_ENUM(SymmetryAntisymmetryEncastreType)
    };
    /**
     * @brief  对称/反对称/固定边界条件
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaBoundaryConditionSymmetry: public FITKAbaAbstractBoundaryCondition
    {
        FITKCLASS(Interface, FITKAbaBoundaryConditionSymmetry)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaBoundaryConditionSymmetry() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaBoundaryConditionSymmetry() = default;
        /**
         * @brief  获取边界条件类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaBoundaryConditionType getBoundaryConditionType() const override;
        /**
         * @brief  数据在分析步中是否被修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  数据在分析步中的激活状态发生变化时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void activeStateChanged() override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取约束类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        AbaSymmetryAntisymmetryEncastreType::SymmetryAntisymmetryEncastreType getType() const;
        /**
         * @brief  设置约束类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setType(AbaSymmetryAntisymmetryEncastreType::SymmetryAntisymmetryEncastreType type);
        /**
         * @brief  获取坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int getCsysId() const;
        /**
         * @brief  设置坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setCsysId(int id);
        /**
         * @brief  获取坐标系对象
         * @return FITKCoordinateSystem*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date   2024-05-28
         */
        FITKCoordinateSystem* getCsysObj();
    private:
        /**
         * @brief  约束类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        AbaSymmetryAntisymmetryEncastreType::SymmetryAntisymmetryEncastreType m_type{ AbaSymmetryAntisymmetryEncastreType::SymmetryAntisymmetryEncastreType::None};
        /**
         * @brief  坐标系Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int m_csysId{0};
    };
}


#endif // FITKABABOUNDARYCONDITIONSYMMETRY_H
