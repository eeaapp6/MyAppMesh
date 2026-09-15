/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAmplitudePeriodic.h
 * @brief  周期型幅值曲线
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-03
 * 
 */
#ifndef FITKABAAMPLITUDEPERIODIC_H
#define FITKABAAMPLITUDEPERIODIC_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractAmplitude.h"
#include <QVector>
#include <QPair>


namespace Interface {
    /**
     * @brief  周期型幅值曲线数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class AmplitudePeriodicData
    {
    public:
        /**
         * @brief  获取时间间隔类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::TimeSpan getTimeSpan() const;
        /**
         * @brief  设置时间间隔类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTimeSpan(FITKAbaAbstractAmplitude::TimeSpan type);
        /**
         * @brief  获取圆频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getCircularFrequency() const;
        /**
         * @brief  设置圆频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCircularFrequency(double value);
        /**
         * @brief  获取开始时间
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getStartingTime() const;
        /**
         * @brief  设置开始时间
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setStartingTime(double time);
        /**
         * @brief  设置初始幅值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getInitialAmplitude() const;
        /**
         * @brief  设置初始幅值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setInitialAmplitude(double value);
        /**
         * @brief  获取数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getNumberOfRows() const;
        /**
         * @brief  设置数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setNumberOfRows(int num);
        /**
         * @brief  插入数据行
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void insertRow(int index);
        /**
         * @brief  移除数据行
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void removeRow(int index);
        /**
         * @brief  获取A值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getA(int index) const;
        /**
         * @brief  设置A值
         * @param[in]  value 值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setA(double value, int index);
        /**
         * @brief  获取B值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getB(int index) const;
        /**
         * @brief  设置B值
         * @param[in]  value 值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setB(double value, int index);

    private:
        /**
         * @brief  时间间隔类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::TimeSpan m_timeSpan{ FITKAbaAbstractAmplitude::TimeSpan::StepTime};
        /**
         * @brief  圆频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_circularFrequency{0.0};
        /**
         * @brief  开始时间
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_startingTime{0.0};
        /**
         * @brief  初始幅值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_initialAmplitude{0.0};
        /**
         * @brief  幅值曲线多行数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        QVector<QPair<double, double>> m_data{};
    };
    /**
     * @brief  周期型幅值曲线类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class FITKInterfaceStructuralAPI FITKAbaAmplitudePeriodic: public FITKAbaAbstractAmplitude
    {
        FITKCLASS(Interface, FITKAbaAmplitudePeriodic)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAmplitudePeriodic() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAmplitudePeriodic() = default;
        /**
         * @brief  获取幅值曲线类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAmplitudeType getAmplitudeType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取时间间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        TimeSpan getTimeSpan() const;
        /**
         * @brief  设置时间间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTimeSpan(TimeSpan type);
        /**
         * @brief  获取圆频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getCircularFrequency() const;
        /**
         * @brief  设置圆频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setCircularFrequency(double value);
        /**
         * @brief  获取开始时间
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getStartingTime() const;
        /**
         * @brief  设置开始时间
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setStartingTime(double time);
        /**
         * @brief  获取初始幅值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getInitialAmplitude() const;
        /**
         * @brief  设置初始幅值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setInitialAmplitude(double value);
        /**
         * @brief  获取数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getNumberOfRows() const;
        /**
         * @brief  设置数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setNumberOfRows(int num);
        /**
         * @brief  插入数据行
         * @param[in]  index 要插入的行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void insertRow(int index);
        /**
         * @brief  移除数据行
         * @param[in]  index 要插入的行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void removeRow(int index);
        /**
         * @brief  获取A值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getA(int index) const;
        /**
         * @brief  设置A值
         * @param[in]  value 值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setA(double value, int index);
        /**
         * @brief  获取B值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getB(int index) const;
        /**
         * @brief  设置B值
         * @param[in]  value 值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setB(double value, int index);
    private:
        /**
         * @brief  周期型幅值曲线数据对象
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        AmplitudePeriodicData m_data;
    };
}


#endif // FITKABAAMPLITUDEPERIODIC_H
