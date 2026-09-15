/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaLoadPressure.h
 * @brief  压强载荷
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-08
 * 
 */
#ifndef FITKABALOADPRESSURE_H
#define FITKABALOADPRESSURE_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractLoad.h"
#include "FITKAbaStepRelatedDataObject.h"

namespace Interface {
    /**
     * @brief  压强载荷数据，在个分析步中可编辑
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class PressureData
    {
    public:
        /**
         * @brief  获取压强值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        double getPreesure() const;
        /**
         * @brief  设置压强值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setPressure(double value);
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int getAmplitudeId() const;
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        void setAmplitudeId(int id);
        /**
         * @brief  获取压强方向
         * @param[out]  dir 压强方向
         * @return 
         * @author libaojun 
         * @date   2024-07-28
         * details 该参数一般情况下不会使用，防备特殊需求
         */
        void getDirection(double* dir) const;
        /**
         * @brief  设置压强方向
         * @param[in]  dir 压强方向
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-28
         * @details 该参数一般情况下不会使用，防备特殊需求
         */
        void setDirection(const double dirx, const double diry, const double dirz);
    private:
        /**
         * @brief  幅值曲线Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        int m_m_AmplitudeId{0};
        /**
         * @brief  压强值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-08
         */
        double m_value{0.0};
        /**
         * @brief  压强方向，默认Z轴正方向,一般情况下不会使用，防备特殊需求
         * @author libaojun
         * @date   2025-07-28
         */
        double m_direction[3]= {0.0, 0.0, 1.0}; // 默认方向为Z轴正方向
    };
    /**
     * @brief  压强载荷类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-08
     */
    class FITKInterfaceStructuralAPI FITKAbaLoadPressure: public FITKAbaAbstractLoad
    {
        FITKCLASS(Interface, FITKAbaLoadPressure)
    public:
        FITKAbaLoadPressure() = default;
        ~FITKAbaLoadPressure() = default;
        FITKAbaLoadType getLoadType() const override;
        bool copy(Core::FITKAbstractDataObject* obj) override;
        bool isModifiedInStep(int stepId) const override;
        void activeStateChanged() override;
        /**
         * @brief  获取压强
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        double getPressure(int stepId = 0);
        /**
         * @brief  设置压强
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        void setPressure(double value, int stepId = 0);
        /**
         * @brief  获取幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        int getAmplitudeId(int stepId = 0) override;
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-10
         */
        void setAmplitudeId(int id, int stepId = 0) override;
        /**
         * @brief  获取压强方向
         * @param[out]  dir 压强方向
         * @return 
         * @author libaojun 
         * @date   2024-07-28
         * details 该参数一般情况下不会使用，防备特殊需求
         */
        void getDirection(double* dir, int stepId = 0) ;
        /**
         * @brief  设置压强方向
         * @param[in]  dir 压强方向
         * @return 
         * @author libaojun 
         * @details 该参数一般情况下不会使用，防备特殊需求
         * @date   2024-07-28
         */
        void setDirection(const double dirx, const double diry, const double dirz, int stepId = 0);
    private:
        /**
         * @brief  获取分析步对应的数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        PressureData getDataInStep(int stepId);
    private:
        /**
         * @brief  创建数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        PressureData m_data;
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        QMap<int, PressureData> m_modifiedDatas{};
    };
}


#endif // FITKABALOADPRESSURE_H
