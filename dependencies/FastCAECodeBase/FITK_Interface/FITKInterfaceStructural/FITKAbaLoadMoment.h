/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaLoadMoment.h
 * @brief  扭矩载荷类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABALOADMOMENT_H
#define FITKABALOADMOMENT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractLoad.h"
#include "FITKAbaLoadConcentratedForce.h"
#include <QMap>


namespace Interface {
    class FITKCoordinateSystem;
    /**
     * @brief  扭矩载荷类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaLoadMoment:public FITKAbaAbstractLoad
    {
        FITKCLASS(Interface, FITKAbaLoadMoment)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaLoadMoment() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaLoadMoment() = default;
        /**
         * @brief  获取载荷类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaLoadType getLoadType() const override;
        /**
         * @brief  数据是否在分析步中被修改
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
         * @brief  获取CM1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getCM1(int stepId = 0) ;
        /**
         * @brief  设置CM1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setCM1(double value, int stepId = 0);
        /**
         * @brief  获取CM2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getCM2(int stepId = 0) ;
        /**
         * @brief  设置CM2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setCM2(double value, int stepId = 0);
        /**
         * @brief  获取CM3
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getCM3(int stepId = 0) ;
        /**
         * @brief  设置CM3
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setCM3(double value, int stepId = 0);
        /**
         * @brief  获取幅值曲线id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int getAmplitudeId(int stepId = 0) override;
        /**
         * @brief  设置幅值曲线id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setAmplitudeId(int id, int stepId = 0) override;
        /**
         * @brief  获取解析场id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int getAnalyticalFieldId() const;
        /**
         * @brief  设置解析场id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setAnalyticalFieldId(int id);
        /**
         * @brief  获取坐标系id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int getCsysId() const;
        /**
         * @brief  设置坐标系id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
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
         * @brief  获取分析步数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ConcentratedLoadData getDataInStep(int stepId);
    private:
        /**
         * @brief  创建分析步中的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        ConcentratedLoadData m_data;
        /**
         * @brief  解析场id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int m_analyticalFieldId{0};
        /**
         * @brief  坐标系id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int m_csysId{0};
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        QMap<int, ConcentratedLoadData> m_modifiedDatas{};
    };
}


#endif // FITKABALOADMOMENT_H
