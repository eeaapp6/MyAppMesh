/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaFieldOutput.h
 * @brief  场输出
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAFIELDOUTPUT_H
#define FITKABAFIELDOUTPUT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKAbaStepRelatedDataObject.h"
#include <QList>
#include "FITKAbaAbstractOutput.h"

namespace Interface {

    /**
     * @brief  场变量输出类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaFieldOutput: public FITKAbaAbstractOutput
    {
        FITKCLASS(Interface, FITKAbaFieldOutput)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaFieldOutput() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaFieldOutput() = default;
        /**
         * @brief  根据分析步类型和作用区域获取All状态下变量列表树形结构
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
        /**
         * @brief  获取输出类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaOutputType getOutputType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取是否包括可用的局部坐标方向
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool getIncludeLocalCoordDirWhenAvailable() const;
        /**
         * @brief  设置是否包括可用的局部坐标方向
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setIncludeLocalCoordDirWhenAvailable(bool value);
        /**
         * @brief  仅限于外部
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool getExteriorOnly() const;
        /**
         * @brief  设置仅限于外部
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setExteriorOnly(bool state);
    private:
        /**
         * @brief  是否包含可用的局部坐标系方向
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool m_includeLocalCoordDirWhenAvailable{true};
        /**
         * @brief  是否仅限于外部
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool m_exteriorOnly{false};
    };
}


#endif // FITKABAFIELDOUTPUT_H
