/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaHistoryOutput.h
 * @brief  历程输出
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-22
 * 
 */
#ifndef FITKABAHISTORYOUTPUT_H
#define FITKABAHISTORYOUTPUT_H
#include "FITKAbaAbstractOutput.h"


namespace Interface {
    /**
     * @brief  历程输出类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-22
     */
    class FITKInterfaceStructuralAPI FITKAbaHistoryOutput: public FITKAbaAbstractOutput
    {
        FITKCLASS(Interface, FITKAbaHistoryOutput)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaHistoryOutput();
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaHistoryOutput() = default;
        /**
         * @brief  获取输出类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        FITKAbaOutputType getOutputType() const override;
        /**
         * @brief  根据分析步类型和作用区域获取变量列表树形结构
         * @param[in]
         * @return 字符串二维数组，[[parent,child],[parent, child, grandchild]]
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-23
         */
        static QVector<QVector<QString>> getAllVariablesWithParents(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据分析步类型和作用区域获取All变量列表
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-23
         */
        static QStringList getAllVariables(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据分析步类型和作用区域获取Preselected变量列表
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-23
         */
        static QStringList getPreselectedVariables(FITKAbaAbstractStep::FITKAbaStepType stepType, 
            FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  根据变量名称获取变量描述
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-23
         */
        static QString getVariableDescription(const QString& var);
    };
}


#endif // FITKABAHISTORYOUTPUT_H
