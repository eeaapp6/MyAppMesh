/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaFieldOutputManager.h
 * @brief  场输出管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAFIELDOUTPUTMANAGER_H
#define FITKABAFIELDOUTPUTMANAGER_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbaFieldOutput.h"


namespace Interface {
    class FITKAbaAbstractStep;
    /**
     * @brief  场输出管理器
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaFieldOutputManager: public Core::FITKAbstractDataObject
    , public Core::FITKAbstractDataManager<FITKAbaFieldOutput>
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaFieldOutputManager() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaFieldOutputManager() = default;
        /**
         * @brief  删除在分析步中创建的场输出
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void deleteStepFieldOutputs(int stepId);
        /**
         * @brief  创建分析步时，为分析步创建默认的场输出。如果前序分析步存在可用于该分析步的场输出
         * 则不创建
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void createDefaultFieldOutput(FITKAbaAbstractStep* step);
        /**
         * @brief  分析步替换时，对数据进行处理
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        void replaceStep(int oldStep, int newStep);
    };
}


#endif // FITKABAFIELDOUTPUTMANAGER_H
