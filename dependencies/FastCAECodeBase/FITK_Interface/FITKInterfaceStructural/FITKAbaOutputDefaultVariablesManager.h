/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaOutputDefaultVariablesManager.h
 * @brief  输出变量管理，
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-22
 * 
 */
#ifndef FITKABAOUTPUTDEFAULTVARIABLESMANAGER_H
#define FITKABAOUTPUTDEFAULTVARIABLESMANAGER_H
#include "FITKAbaOutputVariables.h"


namespace Interface {
    class FITKAbaOutputDefaultVariablesManager
    {
    public:
        /**
         * @brief  单例模式，或取实例
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        static FITKAbaOutputDefaultVariablesManager* getInstance();
        /**
         * @brief  根据分析步类型及作用区域获取PreSelected场变量列表
         * @param[in]  stepType分析步类型
         * @param[in]  domain作用区域
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QStringList getPreSelectedDefaultFeildVaraiables(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据分析步类型及作用区域获取All场变量列表（当前已支持的）
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QStringList getAllDefaultFieldVaraibles(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据分析步类型及作用区域获取PreSelected历史变量列表
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QStringList getPreSelectedDefaultHistoryVariables(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据分析步类型及作用区域获取All历史变量列表
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QStringList getAllDefaultHistoryVariables(FITKAbaAbstractStep::FITKAbaStepType stepType,
            FITKAbaOutputVariable::OutputDomain domain);
    private:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaOutputDefaultVariablesManager();
        /**
         * @brief  阻止拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaOutputDefaultVariablesManager(const FITKAbaOutputDefaultVariablesManager&) = delete;
        /**
         * @brief  阻止赋值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaOutputDefaultVariablesManager& operator=(const FITKAbaOutputDefaultVariablesManager&) = delete;
        /**
         * @brief  初始化变量容器
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void init();
        /**
         * @brief  初始化预选择场变量容器
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void initPreField();
        /**
         * @brief  初始化预选择历史变量容器
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void initPreHistory();
        /**
         * @brief  初始化All场变量容器
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void initAllField();
        /**
         * @brief  初始化All历史变量容器
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void initAllHistory();
    private:
        /**
         * @brief  预选择场变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QMap<FITKAbaAbstractStep::FITKAbaStepType, QMap<FITKAbaOutputVariable::OutputDomain, QList<FITKAbaOutputVariable::OutputVariable>>> m_preselectedFieldVariables{};
        /**
         * @brief  All场变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QMap<FITKAbaAbstractStep::FITKAbaStepType, QMap<FITKAbaOutputVariable::OutputDomain, QList<FITKAbaOutputVariable::OutputVariable>>> m_allFieldVariables{};
        /**
         * @brief  All历史变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QMap<FITKAbaAbstractStep::FITKAbaStepType, QMap<FITKAbaOutputVariable::OutputDomain, QList<FITKAbaOutputVariable::OutputVariable>>> m_allHistoryVariables{};
        /**
         * @brief  预选择历史变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QMap<FITKAbaAbstractStep::FITKAbaStepType, QMap<FITKAbaOutputVariable::OutputDomain, QList<FITKAbaOutputVariable::OutputVariable>>> m_preselectedHistoryVariables{};

    };
}


#endif // FITKABAOUTPUTDEFAULTVARIABLESMANAGER_H
