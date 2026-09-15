/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaBoundaryConditionDisplacement.h
 * @brief  位移/旋转边界条件
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABABOUNDARYCONDITIONDISPLACEMENT_H
#define FITKABABOUNDARYCONDITIONDISPLACEMENT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractBoundaryCondition.h"


namespace Interface {

    class FITKCoordinateSystem;
    /**
     * @brief  位移/旋转边界条件数据类。在分析步中可编辑
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class BCDispalcementRotationData
    {
    public:
        /**
         * @brief  获取U1激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        bool getU1State() const;
        /**
         * @brief  设置U1激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setU1State(bool state);
        /**
         * @brief  获取U2激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool getU2State() const;
        /**
         * @brief  设置U2激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setU2State(bool state);
        /**
         * @brief  获取U3激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool getU3State() const;
        /**
         * @brief  设置U3激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setU3State(bool state);
        /**
         * @brief  获取UR1激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool getUR1State() const;
        /**
         * @brief  设置UR1激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR1State(bool state);
        /**
         * @brief  获取UR2激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool getUR2State() const;
        /**
         * @brief  设置UR2激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR2State(bool state);
        /**
         * @brief  获取UR3激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool getUR3State() const;
        /**
         * @brief  设置UR3激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR3State(bool state);
        /**
         * @brief  获取U1数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        double getU1() const;
        /**
         * @brief  设置U1数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setU1(double value);
        /**
         * @brief  获取U2数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getU2() const;
        /**
         * @brief  设置U2数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setU2(double value);
        /**
         * @brief  获取U3数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getU3() const;
        /**
         * @brief  设置U3数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setU3(double value);
        /**
         * @brief  获取UR1数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getUR1() const;
        /**
         * @brief  设置UR1数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR1(double value);
        /**
         * @brief  获取UR2数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getUR2() const;
        /**
         * @brief  设置UR2数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR2(double value);
        /**
         * @brief  获取UR3数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getUR3() const;
        /**
         * @brief  设置UR3数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setUR3(double value);
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        int getAmplitudeId() const;
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setAmplitudeId(int id);
    private:
        /**
         * @brief  六个自由度的激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        bool m_states[6]{false, false, false, false, false, false};
        /**
         * @brief  六个自由度的位移或旋转值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        double m_values[6]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        /**
         * @brief  幅值曲线
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int m_amplitudeId{0};
    };
    /**
     * @brief  位移/旋转边界条件类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaBoundaryConditionDisplacement: public FITKAbaAbstractBoundaryCondition
    {
        FITKCLASS(Interface, FITKAbaBoundaryConditionDisplacement)
    public:
        /**
         * @brief  构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaBoundaryConditionDisplacement() = default;
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        ~FITKAbaBoundaryConditionDisplacement() = default;
        /**
         * @brief  获取边界条件类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaBoundaryConditionType getBoundaryConditionType() const override;
        /**
         * @brief  在分析步中是否修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  在分析步中的状态发生变化时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void activeStateChanged() override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        int getCsysId() const;
        /**
         * @brief  设置坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setCsysId(int id);
        /**
         * @brief  获取解析场Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        int getAnalyticalFieldId() const;
        /**
         * @brief  设置解析场Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setAnalyticalFieldId(int id);
        /**
         * @brief  获取U1状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getU1State(int stepId = 0) ;
        /**
         * @brief  设置U1状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU1State(bool state, int stepId = 0);
        /**
         * @brief  获取U2状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getU2State(int stepId = 0) ;
        /**
         * @brief  设置U2状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU2State(bool state, int stepId = 0);
        /**
         * @brief  获取U3状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getU3State(int stepId = 0) ;
        /**
         * @brief  设置U3状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU3State(bool state, int stepId = 0);
        /**
         * @brief  获取UR1状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getUR1State(int stepId = 0) ;
        /**
         * @brief  设置UR1状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR1State(bool state, int stepId = 0);
        /**
         * @brief  获取UR2状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getUR2State(int stepId = 0) ;
        /**
         * @brief  设置UR2状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR2State(bool state, int stepId = 0);
        /**
         * @brief  获取UR3状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool getUR3State(int stepId = 0) ;
        /**
         * @brief  设置UR3状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR3State(bool state, int stepId = 0);
        /**
         * @brief  获取U1值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getU1(int stepId = 0) ;
        /**
         * @brief  设置U1值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU1(double value, int stepId = 0);
        /**
         * @brief  获取U2值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getU2(int stepId = 0) ;
        /**
         * @brief  设置U2值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU2(double value, int stepId = 0);
        /**
         * @brief  获取U3值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getU3(int stepId = 0) ;
        /**
         * @brief  设置U3值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setU3(double value, int stepId = 0);
        /**
         * @brief  获取UR1值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getUR1(int stepId = 0) ;
        /**
         * @brief  设置UR1值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR1(double value, int stepId = 0);
        /**
         * @brief  获取UR2值
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getUR2(int stepId = 0) ;
        /**
         * @brief  设置UR2值
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR2(double value, int stepId = 0);
        /**
         * @brief  获取UR3值
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        double getUR3(int stepId = 0) ;
        /**
         * @brief  设置UR3值
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setUR3(double value, int stepId = 0);
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        int getAmplitudeId(int stepId = 0);
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void setAmplitudeId(int id, int stepId = 0);

        /**
        * @brief  获取坐标系对象
        * @return FITKCoordinateSystem*
        * @author libaojun (libaojunqd@foxmail.com)
        * @date   2024-05-28
        */
        FITKCoordinateSystem* getCsysObj();
    private:
        /**
         * @brief  获取分析步中的载荷数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        BCDispalcementRotationData getDataInStep(int stepId);
    private:
        /**
         * @brief  创建分析步中的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        BCDispalcementRotationData m_data;
        /**
         * @brief  解析场Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int m_analyticalFieldId{0};
        /**
         * @brief  坐标系Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int m_csysId{0};
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        QMap<int, BCDispalcementRotationData> m_modifiedDatas{};
    };
}


#endif // FITKABABOUNDARYCONDITIONDISPLACEMENT_H
