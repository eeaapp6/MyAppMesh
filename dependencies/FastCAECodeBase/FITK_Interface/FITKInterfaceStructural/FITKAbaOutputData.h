/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaOutputData.h
 * @brief  输出数据，包含输出中需要定义的基本字段
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-28
 * 
 */
#ifndef FITKABAOUTPUTDATA_H
#define FITKABAOUTPUTDATA_H
#include "FITKAbaOutputVariables.h"


namespace Interface {
    /**
     * @brief  输出数据类，在后续分析步中可编辑
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class AbaOutputData
    {
    public:
        /**
         * @brief  获取输出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputFrequency getOutputFrequency() const;
        /**
         * @brief  设置输出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputFrequency(FITKAbaOutputVariable::OutputFrequency value);
        /**
         * @brief  获取输出定时类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputTiming getOutputTiming() const;
        /**
         * @brief  设置输出定时类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputTiming(FITKAbaOutputVariable::OutputTiming value);
        /**
         * @brief  获取输出变量选择模式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputVariablesSelectMode getSelectMode() const;
        /**
         * @brief  设置输出变量选择模式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSelectMode(FITKAbaOutputVariable::OutputVariablesSelectMode mode);
        /**
         * @brief  获取每N个增量步输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getEveryNIncrements() const;
        /**
         * @brief  设置每N个增量步输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setEvertyNIncrements(int value);
        /**
         * @brief  获取输出间隔数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getIntervals() const;
        /**
         * @brief  设置输出间隔数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setIntervals(int value);
        /**
         * @brief  获取每X时间单位输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getXUnitsOfTime() const;
        /**
         * @brief  设置每X个时间单位输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setXUnitsOfTime(double value);
        /**
         * @brief  获取时间点Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getTimePointsId() const;
        /**
         * @brief  设置时间点Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTimePointsId(int id);
        /**
         * @brief  获取模态列表，用于模态、屈曲分析步场输出频率按照指定模态进行输出的情况
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        QList<int> getModeList() const;
        /**
         * @brief  设置模态列表，用于模态、屈曲分析步场输出频率按照指定模态进行输出的情况
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        void setModeList(const QList<int> modes);
        /**
         * @brief  获取已选择的变量列表
         * @param[in]
         * @return 已选择的变量列表
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> getSelectedVariables() const;
        /**
         * @brief  设置已选择的变量列表
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSelectedVariables(const QList<QString>& vars);
        /**
         * @brief  增加一个变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void appendVariable(const QString& var);
        /**
         * @brief  移除一个变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void removeVariable(const QString& var);
        /**
         * @brief  清空所有变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void clearVariables();
    private:
        /**
         * @brief  输出频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputFrequency m_frequency{ FITKAbaOutputVariable::OutputFrequency::EvenlySpacedTimeIntervals};
        /**
         * @brief  输出定时
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputTiming m_timing{ FITKAbaOutputVariable::OutputTiming::OutputAtApproximateTimes};
        /**
         * @brief  变量选择模式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputVariablesSelectMode m_selectMode{ FITKAbaOutputVariable::OutputVariablesSelectMode::PreselectedDefaults};
        /**
         * @brief  每N个增量步输出
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int m_n{1};
        /**
         * @brief  输出间隔
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int m_intervals{20};
        /**
         * @brief  每X单位时间输出
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_x{0.0};
        /**
         * @brief  模态阶次列表
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        QList<int> m_modeList{};
        /**
         * @brief  时间点Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int m_timePointsId{0};
        /**
         * @brief  已选择的变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> m_Variables{};
    };
}

#endif // FITKABAOUTPUTDATA_H
