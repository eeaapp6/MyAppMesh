/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaLoadBodyForce.h
 * @brief  体力载荷
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABALOADBODYFORCE_H
#define FITKABALOADBODYFORCE_H
#include "FITKAbaAbstractLoad.h"
#include "FITKInterfaceStructuralAPI.h"
#include <QMap>


namespace Interface {
    /**
     * @brief  体力载荷数据类，在分析步中可编辑
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class BodyForceData
    {
    public:
        /**
         * @brief  获取第一个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent1() const;
        /**
         * @brief  设置第一个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent1(double value);
        /**
         * @brief  获取第二个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent2() const;
        /**
         * @brief  设置第二个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent2(double value);
        /**
         * @brief  获取第三个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent3() const;
        /**
         * @brief  设置第三个分量值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent3(double value);
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
         * @brief  三分量值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double m_value[3]{0.0,0.0,0.0};
        /**
         * @brief  幅值曲线Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        int m_amplitudeId{0};
    };
    /**
     * @brief  体力载荷类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaLoadBodyForce: public FITKAbaAbstractLoad
    {
        FITKCLASS(Interface, FITKAbaLoadBodyForce)
    public:
        /**
         * @brief  构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaLoadBodyForce() = default;
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        ~FITKAbaLoadBodyForce() = default;
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
         * @brief  复制数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取第一个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent1(int stepId = 0) ;
        /**
         * @brief  设置第一个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent1(double value,int stepId = 0);
        /**
         * @brief  获取第二个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent2(int stepId = 0) ;
        /**
         * @brief  设置第二个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent2(double value,int stepId = 0);
        /**
         * @brief  获取第三个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        double getComponent3(int stepId = 0) ;
        /**
         * @brief  设置第三个分量值
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setComponent3(double value,int stepId = 0);
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        int getAmplitudeId(int stepId = 0) override;
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setAmplitudeId(int id, int stepId = 0) override;
        /**
         * @brief  获取解析场Id
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        int getAnalyticalFieldId() const;
        /**
         * @brief  设置解析场Id
         * @param[in]  stepId 分析步Id
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setAnalyticalFieldId(int id);
    private:
        /**
         * @brief  获取分析步中的载荷数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        BodyForceData getDataInStep(int stepId);
    private:
        /**
         * @brief  创建时所在分析步中的载荷数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        BodyForceData m_data;
        /**
         * @brief  解析场Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        int m_analyticalFieldId{0};
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        QMap<int, BodyForceData> m_modifiedDatas{};

    };
}


#endif // FITKABALOADBODYFORCE_H
