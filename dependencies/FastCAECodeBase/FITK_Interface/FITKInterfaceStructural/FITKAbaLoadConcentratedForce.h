/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaLoadConcentratedForce.h
 * @brief  集中力
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-03
 * 
 */
#ifndef FITKABALOADCONCENTRATEDFORCE_H
#define FITKABALOADCONCENTRATEDFORCE_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractLoad.h"
#include <QMap>


namespace Interface {
    class FITKCoordinateSystem;
    /**
     * @brief  集中力数据类，包含在各分析步中均可编辑的数据
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
class ConcentratedLoadData
    {
    public:
        /**
         * @brief  获取X方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getValue1() const;
        /**
         * @brief  获取Y方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getValue2() const;
        /**
         * @brief  获取Z方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getValue3() const;
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getAmplitudeId() const;
        /**
         * @brief  设置X方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setValue1(double value);
        /**
         * @brief  设置Y方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setValue2(double value);
        /**
         * @brief  设置Z方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setValue3(double value);
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAmplitudeId(int id);
        /**
         * @brief  重写==运算符，判断两个数据对象是否相等
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool operator==(const ConcentratedLoadData) const;

    private:
        /**
         * @brief  X方向集中载荷大小
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_value1{0};
        /**
         * @brief  Y方向集中载荷大小
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_value2{0};
        /**
         * @brief  Z方向集中载荷大小
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_value3{0};
        /**
         * @brief  幅值曲线Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_AmplitudeId{0};
    };
    /**
     * @brief  集中力载荷类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class FITKInterfaceStructuralAPI FITKAbaLoadConcentratedForce: public FITKAbaAbstractLoad
    {
        FITKCLASS(Interface, FITKAbaLoadConcentratedForce)
    public:
        /**
         * @brief  构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaLoadConcentratedForce();
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        ~FITKAbaLoadConcentratedForce() = default;
        /**
         * @brief  获取载荷类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaLoadType getLoadType() const override;
        /**
         * @brief  在分析步中是否被修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  在分析步中激活状态发生变化时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void activeStateChanged() override;
        /**
         * @brief  拷贝数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取X方向集中力大小
         * @param[in]  stepId 分析步Id，默认为0时，返回载荷创建所在分析步的数据
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getCF1(int stepId = 0);
        /**
         * @brief  获取Y方向集中力大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getCF2(int stepId = 0);
        /**
         * @brief  获取Z方向集中力大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getCF3(int stepId = 0);
        /**
         * @brief  获取载荷随节点旋转状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getFlowNodalRotation() const;
        /**
         * @brief  获取解析场Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getAnalyticalFieldId() const;
        /**
         * @brief  获取坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getCsysId() const;
        /**
         * @brief  获取坐标系对象
         * @return FITKCoordinateSystem*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date   2024-05-28
         */
        FITKCoordinateSystem* getCsysObj();

        /**
         * @brief  获取幅值曲线Id
         * @param[in]  stepId 分析步Id，该数据在不同分析步中可能会调整
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getAmplitudeId(int stepId = 0) override;
        /**
         * @brief  设置X方向数值
         * @param[in]  stepId 分析步Id，该数据在不同分析步中可能会调整
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCF1(double value,int stepId = 0);
        /**
         * @brief  设置Y方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCF2(double value,int stepId = 0);
        /**
         * @brief  设置Z方向数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCF3(double value,int stepId = 0);
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAmplitudeId(int id,int stepId = 0) override;
        /**
         * @brief  设置解析场Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAnalyticalFieldId(int id);
        /**
         * @brief  设置坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCsysId(int id);
        /**
         * @brief  设置载荷是否随节点旋转
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setFlowNodalRotation(bool stat);
    private:
        /**
         * @brief  获取分析步stepId中的数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        ConcentratedLoadData getDataInStep(int stepId);
    private:
        /**
         * @brief  是否随节点旋转
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_flowNodalRotation{false};
        /**
         * @brief  解析场Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_analyticalFieldId{0};
        /**
         * @brief  坐标系Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_csysId{0};
        /**
         * @brief  创建的载荷数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        ConcentratedLoadData m_data;
        /**
         * @brief  后续分析步修改的载荷数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        QMap<int, ConcentratedLoadData> m_modifiedData{};
    };
}


#endif // FITKABALOADCONCENTRATEDFORCE_H
