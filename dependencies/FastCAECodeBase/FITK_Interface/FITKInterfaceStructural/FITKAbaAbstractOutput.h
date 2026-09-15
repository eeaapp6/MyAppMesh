/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractOutput.h
 * @brief  输出基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-22
 * 
 */
#ifndef FITKABAABSTRACTOUTPUT_H
#define FITKABAABSTRACTOUTPUT_H
#include "FITKAbaStepRelatedDataObject.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKBoundaryLocation.h"
#include "FITKAbaOutputData.h"
#include "FITKAbaOutputVariables.h"


namespace Interface {
    
    /**
     * @brief  输出基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-22
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractOutput:public FITKBoundaryLocation,
        public FITKAbaStepRelatedDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  输出类型枚举，场输出、历程输出
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum FITKAbaOutputType
        {
            Field,
            History
        };
        Q_ENUM(FITKAbaOutputType);
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractOutput() = default;
        /**
         * @brief  获取输出类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        virtual FITKAbaOutputType getOutputType() const = 0;
        /**
         * @brief  判断数据是否在分析步中修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  激活状态发生改变时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void activeStateChanged() override;
        /**
         * @brief  是否成功获取分析步管理器
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool hasGetStepManager() override;
        /**
         * @brief  在后续分析步中是否有效
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool isValidInStep(int stepId /* = 0 */) override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取作用区域
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputDomain getDomain() const;
        /**
         * @brief  设置作用区域
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setDomain(FITKAbaOutputVariable::OutputDomain domain);
        /**
         * @brief  获取钢筋的输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool getOutputForRebar() const;
        /**
         * @brief  设置钢筋的输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputForRebar(bool state);
        /**
         * @brief  获取梁、壳、复合层截面点输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputAtSectionPoints getOutputAtSectionPoints() const;
        /**
         * @brief  设置梁、壳、复合层截面点输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputAtSectionPoints(FITKAbaOutputVariable::OutputAtSectionPoints value);
        /**
         * @brief  获取指定的梁、壳、复合层截面点
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getSpecify() const;
        /**
         * @brief  设置指定的梁、壳、复合层截面点
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSpecify(int value);
        /**
         * @brief  获取分析步对应数据中的输出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputFrequency getOutputFrequency(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的输出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputFrequency(FITKAbaOutputVariable::OutputFrequency value, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的定时类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputTiming getOutputTiming(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的定时类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setOutputTiming(FITKAbaOutputVariable::OutputTiming value, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的输出变量选择模式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputVariablesSelectMode getSelectMode(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的输出变量选择模式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSelectMode(FITKAbaOutputVariable::OutputVariablesSelectMode mode, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的每N个增量步输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getEveryNIncrements(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的每N个增量步输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setEvertyNIncrements(int value, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的输出间隔数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getIntervals(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的输出间隔数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setIntervals(int value, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的每X单位时间输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getXUnitsOfTime(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的每X单位时间输出
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setXUnitsOfTime(double value, int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的时间点Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int getTimePointsId(int stepId = 0) ;
        /**
         * @brief  设置分析步对应数据中的时间点Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTimePointsId(int id, int stepId = 0);
        /**
         * @brief  获取模态列表，用于模态、屈曲分析步场输出频率按照指定模态进行输出的情况
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        QList<int> getModeList(int stepId = 0);
        /**
         * @brief  设置模态列表，用于模态、屈曲分析步场输出频率按照指定模态进行输出的情况
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        void setModeList(const QList<int> modes, int stepId = 0);
        /**
         * @brief  获取Interaction Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        int getInteractionId() const;
        /**
         * @brief  设置Interaction Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        void setInteractionId(int id);
        /**
         * @brief  分析步对应数据中的已选择的所有变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> getSelectedVariables(int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的已选择的所有节点变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> getSelectedNodalVariables(int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的已选择的所有单元变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> getSelectedElementVariables(int stepId = 0);
        /**
         * @brief  获取分析步对应数据中的所有接触变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QList<QString> getSelectedContactVaraiables(int stepId = 0);
        /**
         * @brief  获取分析步对应数据中energy关键字相关变量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        QList<QString> getSelectedEnergyVariables(int stepId = 0);
        /**
         * @brief  获取分析步相关数据中Incrementation关键字相关变量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        QList<QString> getSelectedIncrementationVariables(int stepId = 0);
        /**
         * @brief  设置分析步对应数据中的所有选择的变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setSelectedVariables(const QList<QString>& vars, int stepId = 0);
        /**
         * @brief  分析步对应数据增加变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void appendVariable(const QString& var, int stepId = 0);
        /**
         * @brief  分析不对应数据移除变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void removeVariable(const QString& var, int stepId = 0);
        /**
         * @brief  分析步对应数据清空所有选择变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void clearVariables(int stepId = 0);
        /**
         * @brief  获取输出的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool getActiveState() const;
        /**
         * @brief  设置输出的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setActiveState(bool stat);
    protected:
        /**
            * @brief  激活状态
            * @author LiJin (1003680421@qq.com)
            * @date   2024-04-22
            */
        bool m_activeState{ true };
    private:
        /**
         * @brief  获取分析步对应的数据
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        AbaOutputData& getDataInStep(int stepId);
        /**
         * @brief  当选择区域或选择模式发生变化时，调整选择变量
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void modifySelectedVariables(AbaOutputData& data, int stepId);
        /**
         * @brief  选择变量发生变化时，自动调整选择模式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-07
         */
        void modifySlectedMode( int stepId = 0);
    
     protected:
        /**
         * @brief  创建数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        AbaOutputData m_data;
        /**
         * @brief  后续分析步调整的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        QMap<int, AbaOutputData> m_modifiedDatas{};
        /**
         * @brief  作用区域
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputDomain m_domain{ FITKAbaOutputVariable::OutputDomain::WholeModel};
        /**
         * @brief  相互作用Id，当场输出Domain为Interaction时，用于保存其Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-24
         */
        int m_interactionId{0};
        /**
         * @brief  是否包含钢筋单元的输出
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        bool m_outputForRebar{false};
        /**
         * @brief  梁、壳、复合层截面点输出类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaOutputVariable::OutputAtSectionPoints m_outputAtSecPts{ FITKAbaOutputVariable::OutputAtSectionPoints::UseDefaults};
        /**
         * @brief  指定的梁、壳、复合层截面点输出
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        int m_specify{0};
    };
}


#endif // FITKABAABSTRACTOUTPUT_H
