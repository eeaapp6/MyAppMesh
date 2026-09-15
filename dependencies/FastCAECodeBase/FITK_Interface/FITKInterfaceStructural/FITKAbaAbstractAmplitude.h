/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractAmplitude.h
 * @brief  幅值曲线基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-03
 * 
 */
#ifndef FITKABAABSTRACTAMPLITUDE_H
#define FITKABAABSTRACTAMPLITUDE_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"


namespace Interface {
    
    /**
     * @brief  幅值曲线基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractAmplitude: public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaAbstractAmplitude);
    public:
        /**
         * @brief  幅值曲线类型枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum FITKAbaAmplitudeType
        {
            Tabular,
            Periodic,
        };
        Q_ENUM(FITKAbaAmplitudeType);
        /**
         * @brief  时间跨度类型枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum TimeSpan
        {
            StepTime,
            TotalTime
        };
        Q_ENUM(TimeSpan);
        /**
         * @brief  赋值曲线平滑类型，包括求解器默认和指定值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum SmoothingType
        {
            UseSolverDefault,
            Specify
        };
        Q_ENUM(SmoothingType);
        /**
         * @brief  基线修正
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum BaselineCorrection
        {
            None,
            SingleInterval,
            MultipleIntervals
        };
        Q_ENUM(BaselineCorrection);


    public:


        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractAmplitude() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractAmplitude() = default;
        /**
         * @brief  获取幅值曲线类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        virtual FITKAbaAmplitudeType getAmplitudeType()const = 0;
    };
}


#endif // FITKABAABSTRACTAMPLITUDE_H
