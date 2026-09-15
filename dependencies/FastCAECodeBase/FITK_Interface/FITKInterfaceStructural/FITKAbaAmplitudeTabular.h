/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAmplitudeTabular.h
 * @brief  表格型幅值曲线
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-03
 * 
 */
#ifndef FITKABAAMPLITUDETABULAR_H
#define FITKABAAMPLITUDETABULAR_H
#include "FITKAbaAbstractAmplitude.h"
#include <QVector>
#include <QPair>


namespace Interface {
    /**
     * @brief  表格型幅值曲线数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class AmplitudeTabularData
    {
    public:
        /**
         * @brief  获取时间间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::TimeSpan getTimeSpan() const;
        /**
         * @brief  设置时间间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTimeSpan(FITKAbaAbstractAmplitude::TimeSpan type);
        /**
         * @brief  获取平滑类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::SmoothingType getSmoothingType() const;
        /**
         * @brief  设置平滑类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setSmoothingType(FITKAbaAbstractAmplitude::SmoothingType type);
        /**
         * @brief  获取平滑值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getSmoothingValue() const;
        /**
         * @brief  设置平滑值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setSmoothingValue(double value);
        /**
         * @brief  获取基线修正类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::BaselineCorrection getBaseLineCorrection() const;
        /**
         * @brief  设置基线修正类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setBaseLineCorrection(FITKAbaAbstractAmplitude::BaselineCorrection type);
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
         * @param[in]  index 要插入的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void insertRow(int index);
        /**
         * @brief  移除数据行
         * @param[in]  index 要移除的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void removeRow(int index);
        /**
         * @brief  获取时间值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getTime(int index) const;
        /**
         * @brief  获取幅值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getAmplitude(int index) const;
        /**
         * @brief  设置时间值
         * @param[in]  value 时间
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTime(double value, int index);
        /**
         * @brief  设置幅值
         * @param[in]  value 幅值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAmplitude(double value, int index);
        /**
         * @brief  获取多区间基线修正行数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int getNumberOfRowsOfMultipleIntervals() const;
        /**
         * @brief  设置多区间基线修正行数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setNumberOfRowsOfMultipleIntervals(int rowCount);
        /**
         * @brief  移除多区间基线修正行
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void removeRowOfMultipleIntervals(int index);
        /**
         * @brief  插入多区间修正行
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void insertRowOfMultipleIntervals(double value, int index);
        /**
         * @brief  获取多区间基线修正间隔
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double getMultipleInterval(int index) const;
        /**
         * @brief  设置多区间基线修正间隔
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setMultipleInterval(double value, int index);

    private:
        /**
         * @brief  时间间隔类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::TimeSpan m_timeSpan{ FITKAbaAbstractAmplitude::TimeSpan::StepTime};
        /**
         * @brief  平滑类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::SmoothingType m_smoothing{ FITKAbaAbstractAmplitude::SmoothingType::UseSolverDefault};
        /**
         * @brief  平滑值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_smoothingValue{0.0};
        /**
         * @brief  基线修正类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractAmplitude::BaselineCorrection m_baseLineCorrection{ FITKAbaAbstractAmplitude::BaselineCorrection::None};
        /**
         * @brief  幅值数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        QVector<QPair<double, double>> m_amplitudeData{};
        /**
         * @brief  多区间基线修正
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        QVector<double> m_correctionMultiIntervals{};

    };
    /**
     * @brief  表格型幅值曲线类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class FITKInterfaceStructuralAPI FITKAbaAmplitudeTabular: public FITKAbaAbstractAmplitude
    {
        FITKCLASS(Interface, FITKAbaAmplitudeTabular)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAmplitudeTabular() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAmplitudeTabular() = default;
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
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取时间间隔类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        TimeSpan getTimeSpan() const;
        /**
         * @brief  设置时间间隔类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTimeSpan(TimeSpan type);
        /**
         * @brief  获取平滑类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        SmoothingType getSmoothingType() const;
        /**
         * @brief  设置平滑类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setSmoothingType(SmoothingType type);
        /**
         * @brief  获取平滑值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getSmoothingValue() const;
        /**
         * @brief  设置平滑值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setSmoothingValue(double value);
        /**
         * @brief  获取基线修正类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        BaselineCorrection getBaseLineCorrection() const;
        /**
         * @brief  设置基线修正类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setBaseLineCorrection(BaselineCorrection type);
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
        void setNmberOfRows(int num);
        /**
         * @brief  插入数据行
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void insertRow(int index);
        /**
         * @brief  移除数据行
         * @param[in]  index 要移除的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void removeRow(int index);
        /**
         * @brief  获取时间值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getTime(int index) const;
        /**
         * @brief  获取幅值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getAmplitude(int index) const;
        /**
         * @brief  设置时间值
         * @param[in]  value 时间
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setTime(double value, int index);
        /**
         * @brief  设置幅值
         * @param[in]  value 幅值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAmplitude(double value, int index);
        /**
         * @brief  获取多区间基线修正行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int getNumberOfRowsOfMultipleIntervals() const;
        /**
         * @brief  设置多区间基线修正行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setNumberOfRowsOfMultipleIntervals(int rowCount);
        /**
         * @brief  移除多区间基线修正行
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void removeRowOfMultipleIntervals(int index);
        /**
         * @brief  插入多区间修正行
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void insertRowOfMultipleIntervals(double value, int index);
        /**
         * @brief  获取多区间基线修正间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double getMultipleInterval(int index) const;
        /**
         * @brief  设置多区间基线修正间隔
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setMultipleInterval(double value, int index);
    private:
        /**
         * @brief  表格型幅值数据对象
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        AmplitudeTabularData m_data;
    };
}


#endif // FITKABAAMPLITUDETABULAR_H
