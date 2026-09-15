/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaTimePoints.h
 * @brief  输出时间点
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABATIMEPOINTS_H
#define FITKABATIMEPOINTS_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QVector>
#include <array>


namespace Interface {
    /**
     * @brief  输出时间点类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaTimePoints: public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKAbaTimePoints)
    public:
        /**
         * @brief  默认构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaTimePoints() = default;
        /**
         * @brief  默认析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ~FITKAbaTimePoints() = default;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取是否spedify using delimiters
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool getSpecifyUsingDelimiters() const;
        /**
         * @brief  设置是否使用spefify using delimiters
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSpecifyUsingDelimiters(bool state);
        /**
         * @brief  获取数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getNumberOfRows();
        /**
         * @brief  设置数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setNumberOfRows(int rows);
        /**
         * @brief  获取第index行时间点
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getPoint(int index);
        /**
         * @brief  设置第index行时间点
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setPoint(double value, int index);
        /**
         * @brief  在第index行插入时间点value
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void insertPoint(double value, int index);
        /**
         * @brief  获取第index行start参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getStart(int index);
        /**
         * @brief  获取第index行end参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getEnd(int index);
        /**
         * @brief  获取第index行increment参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getIncrement(int index);
        /**
         * @brief  设置第index行start参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setStart(double value, int index);
        /**
         * @brief  设置第index行end参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setEnd(double value, int index);
        /**
         * @brief  设置第index行increment参数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setIncrement(double value, int index);
        /**
         * @brief  在第index行位置插入数据行
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void insertRow(int index);
        /**
         * @brief  移除第index行数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void removeRow(int index);
        /**
         * @brief  清空数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void clearData();
    private:
        /**
         * @brief  是否specify using delimiters
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool m_specifyUsingDelimiters{false};
        /**
         * @brief  时间点数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QVector<double> m_points{};
        /**
         * @brief  当specify using delimiters为true时的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QVector<std::array<double, 3>> m_datas{};
    };
}


#endif // FITKABATIMEPOINTS_H
