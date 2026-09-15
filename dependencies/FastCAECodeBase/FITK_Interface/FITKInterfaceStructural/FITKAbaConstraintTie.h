/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConstraintTie.h
 * @brief  相互作用绑定约束
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-17
 * 
 */
#ifndef FITKABACONSTRAINTTIE_H
#define FITKABACONSTRAINTTIE_H
#include "FITKAbaAbstractConstraint.h"


namespace Interface {

    /**
     * @brief  绑定约束类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
    class FITKInterfaceStructuralAPI FITKAbaConstraintTie: public FITKAbaAbstractConstraint
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaConstraintTie)
    public:
        /**
         * @brief  离散方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        enum DiscretizationMethod
        {
            AnalysisDefault,
            NodeToSurface,
            SurfaceToSurface
        };
        Q_ENUM(DiscretizationMethod);
        /**
         * @brief  位置容差
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        enum PositionTolerance
        {
            UseComputerDefault,
            SpecifyDistance
        };
        Q_ENUM(PositionTolerance);
        /**
         * @brief  约束比
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        enum ConstraintRatio
        {
            UseAnalysisDefault,
            SpecifyValue
        };
        Q_ENUM(ConstraintRatio);  
        /**
         * @brief  绑定方法Method ,只用于软件大工IGA
         * @author wangning (2185896382@qq.com)
         * @date 2025-12-10
         */
        enum TieMethod
        {
            None = -1,
            PENALTY,//Method-1
            //Method_2,
        };
        Q_ENUM(TieMethod);
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaConstraintTie() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaConstraintTie() = default;
        /**
         * @brief  获取约束类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaConstraintType getConstraintType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取离散方法
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        DiscretizationMethod getDiscretizationMethod() const;
        /**
         * @brief  设置离散方法
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setDiscretizationMethod(DiscretizationMethod newDiscretizationMethod);
        /**
         * @brief  排除壳单元厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getExcludeShellThickness() const;
        /**
         * @brief  设置排除壳单元厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setExcludeShellThickness(bool newExcludeShellThickness);
        /**
         * @brief  位置容差类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        PositionTolerance getPostionTolerance() const;
        /**
         * @brief  设置位置容差类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setPostionTol(PositionTolerance newPostionTol);
        /**
         * @brief  获取位置容差
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double getPositionDistance() const;
        /**
         * @brief  设置位置容差
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setPositionDistance(double newPositionDistance);
        /**
         * @brief  是否调整从面节点
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getAdjustSlave() const;
        /**
         * @brief  设置是否调整从面节点
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setAdjustSlave(bool newAdjustSlave);
        /**
         * @brief  是否绑定旋转自由度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getTieRotationDofs() const;
        /**
         * @brief  设置是否绑定旋转自由度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setTieRotationDofs(bool newTieRotationDofs);
        /**
         * @brief  约束比例类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        ConstraintRatio getConstraintRatio() const;
        /**
         * @brief  设置约束比例类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setConstraintRatio(ConstraintRatio newConstraintRatio);
        /**
         * @brief  获取约束比例值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double getConstraintRatioValue() const;
        /**
         * @brief  设置约束比例值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setConstraintRatioValue(double newConstraintRatioValue);

        /**
         * @brief  获取自由度是否绑定 Abaqus中没有自由度限制，用于其他软件
         * @param[in]  index 自由度索引 1-6
         * @return 
         * @author Libaojun
         * @date   2025-05-21
         */
        bool getTieDofStatus(int index) const;
        /**
         * @brief  设置自由度是否绑定 Abaqus中没有自由度限制，用于其他软件
         * @param[in]  index 自由度索引 1-6 status为true表示绑定
         * @return 
         * @author Libaojun
         * @date   2025-05-21
         */
        void setTieDofStatus(int index, bool status);
        /**
        * @brief  设置/获取 惩罚因子，用于其他软件
        * @param[i]  penaltyFactor 惩罚因子
        * @author wangning (2185896382@qq.com)
        * @date 2025-12-10
        */
        void setPenaltyFactor(double penaltyFactor);
        double getPenaltyFactor()const;
        /**
        * @brief  设置/获取 绑定方法，只用于软件大工IGA
        * @param[i]  penaltyFactor 惩罚因子
        * @author wangning (2185896382@qq.com)
        * @date 2025-12-10
        */
        void setTieMethod(TieMethod tieMethod);
        TieMethod getTieMethod()const;
        QString getTieMethodStr()const;

    private:
        /**
         * @brief  离散方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        DiscretizationMethod m_discretizationMethod{DiscretizationMethod::AnalysisDefault};
        /**
         * @brief  排除壳单元厚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_excludeShellThickness{false};
        /**
         * @brief  位置容差类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        PositionTolerance m_postionTol{PositionTolerance::UseComputerDefault};
        /**
         * @brief  位置容差
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double m_positionDistance{0.0};
        /**
         * @brief  是否调整从面
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_adjustSlave{true};
        /**
         * @brief  是否绑定旋转自由度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_tieRotationDofs{true};
        /**
         * @brief  约束比例类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        ConstraintRatio m_constraintRatio{ConstraintRatio::UseAnalysisDefault};
        /**
         * @brief  约束比例
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double m_constraintRatioValue{0.0};

        /**
         * @brief 绑定自由度，Abaqus中没有自由度限制，用于其他软件
         * @author Libaojun
         * @date   2025-05-21
         */
        bool m_tieDofs[6]{false, false, false, false, false, false};    

        /**
         * @brief  惩罚因子 ,用于其他软件
         * @author wangning (2185896382@qq.com)
         * @date 2025-12-10
         */
        double _penaltyFactor = 0.0;
        /**
         * @brief  绑定方法Method ,只用于软件大工IGA
         * @author wangning (2185896382@qq.com)
         * @date 2025-12-10
         */
        TieMethod _tieMethod= TieMethod::None;
    };
}


#endif // FITKABACONSTRAINTTIE_H
