/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaPredefinedFieldTemperature.h
 * @brief  预定义温度场
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAPREDEFINEDFIELDTEMPERATURE_H
#define FITKABAPREDEFINEDFIELDTEMPERATURE_H
#include "FITKAbaAbstractPredefinedField.h"


namespace Interface {

    /**
     * @brief  温度场数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class PredefinedTemperatureData
    {
    public:
        double getMagnitude() const;
        void setMagnitude(double mag);
        int getAmplitudeId() const;
        void setAmplitudeId(int id);
    private:
        double m_Magnitude{0.0};
        int m_amplitudeId{0};
    };
    /**
     * @brief  预定义温度场类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaPredefinedFieldTemperature:
        public FITKAbaAbstractPredefinedField
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaPredefinedFieldTemperature);
    public:
        /**
         * @brief  温度场分布类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        enum TemperatureDistribution
        {
            DirectSpecification
        };
        Q_ENUM(TemperatureDistribution);
    public:
        FITKAbaPredefinedFieldTemperature() = default;
        ~FITKAbaPredefinedFieldTemperature() = default;
        FITKAbaPredefinedFiledType getPreDefinedType() const override;
        bool isModifiedInStep(int stepId) const override ;
        void activeStateChanged() override;
        bool isValidInStep(int stepId) override;
        bool isValidInCreatedStep() override;
        TemperatureDistribution getDistribution() const;
        void setDistribution(TemperatureDistribution dis);
        double getMagnitude(int stepId = 0);
        void setMagnitude(double mag, int stepId = 0);
        int getAmplitudeId(int stepID = 0);
        void setAmplitudeId(int id, int stepId = 0);
        /**
         * @brief  预定义场在分析步中的状态与其它数据不同，在此重新实现。
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        FITKAbaStepEnum::StateInStep getStateInStep(int stepId) override;
        void setStateInStep(FITKAbaStepEnum::StateInStep state, int stepId = 0);

        /**
         * @brief       拷贝数据。[重写]
         * @param[in]   obj：被拷贝的对象
         * @return      是否拷贝成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-17
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;

    private:
        PredefinedTemperatureData& getDataInStep(int stepId);
    private:
        TemperatureDistribution m_distribution{TemperatureDistribution::DirectSpecification};
        PredefinedTemperatureData m_data;
        QMap<int, PredefinedTemperatureData> m_modifiedDatas{};
        QMap<int, FITKAbaStepEnum::StateInStep> m_States{};
    };

}

#endif // FITKABAPREDEFINEDFIELDTEMPERATURE_H
