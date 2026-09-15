/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaStepFrequency.h
 * @brief  模态分析步
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-26
 * 
 */
#ifndef FITKABASTEPFREQUENCY_H
#define FITKABASTEPFREQUENCY_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractStep.h"
namespace Interface {
    /**
     * @brief  模态分析步类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-26
     */
    class FITKInterfaceStructuralAPI FITKAbaStepFrequency: public FITKAbaAbstractStep
    {
        FITKCLASS(Interface, FITKAbaStepFrequency);
    public:
        /**
         * @brief  默认构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaStepFrequency() = default;
        /**
         * @brief  默认析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ~FITKAbaStepFrequency() = default;
        /**
         * @brief  获取分析步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaStepType getStepType() const override;
        /**
         * @brief  是否是线性摄动分析步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool isPerturbation() const override;
        /**
         * @brief  获取分析步时间，模态分析步返回0.0
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getTime() const override;
        /**
         * @brief  获取矩阵求解器类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::Eigensolver getEigenSolver() const;
        /**
         * @brief  设置矩阵求解器类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setEigenSolver(FITKAbaAbstractStep::Eigensolver solver);
        /**
         * @brief  特征值个数请求类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::NumberOfEigenvaluesRequsted getNumberOfEigenvaluesRequstedType() const;
        /**
         * @brief  设置特征值个数请求类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setNumberOfEigenvaluesRequestedType(FITKAbaAbstractStep::NumberOfEigenvaluesRequsted type);
        /**
         * @brief  获取特征值请求个数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        int getNumberOfEigenvaluesRequested() const;
        /**
         * @brief  设置特征值求解请求个数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setNumberOfEigenvaluesRequested(int num);
        /**
         * @brief  获取Acoustic-structual coupling where applicable
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::AcousticStructuralCouplingWhereApplicable getAcousticStructuralCouplingWhereApplicableType() const;
        /**
         * @brief  设置Acoustic-structual coupling where applicable
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAcousticStructuralCouplingWhereApplicableType(FITKAbaAbstractStep::AcousticStructuralCouplingWhereApplicable type);
        /**
         * @brief  获取频率变换状态，（周期/时间**2）
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getFrequencyShift() const;
        /**
         * @brief  设置频率变换状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setFrequencyShift(bool stat);
        /**
         * @brief  设置频率变换值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getFrequencyShiftValue() const;
        /**
         * @brief  设置频率变换值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setFrequencyShiftValue(double value);
        /**
         * @brief  获取是否定义最小频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        bool getMinFreqIntrest() const;
        /**
         * @brief  设置是否定义最小频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMinFreqIntrest(bool stat);
        /**
         * @brief  获取最小频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getMinFreqInterstValue() const;
        /**
         * @brief  设置最小频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMinFreqInterstValue(double value);
        /**
         * @brief  获取是否定义最大频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        bool getMaxFreqInterst() const;
        /**
         * @brief  设置是否定义最大频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMaxFrqInterst(bool stat);
        /**
         * @brief  获取最大频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getMaxFreqInterstValue() const;
        /**
         * @brief  设置最大频率值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMaxFreqInterstValue(double value);
        /**
         * @brief  获取块大小类型，默认或数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        BlockSize getBlockSizeType() const;
        /**
         * @brief  设置块大小类型，默认或数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setBlockSizeType(BlockSize type);
        /**
         * @brief  获取块大小数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getBlockSizeValue() const;
        /**
         * @brief  设置块大小数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setBlockSizeValue(int size);
        /**
         * @brief  获取block lanczos分析步最大个数类型，默认或数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::MaximumNumberOfBlockLanczosStep getMaximumNumberOfBlockLanczosStepType() const;
        /**
         * @brief  设置block lanczos分析不最大个数类型，默认或数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setMaximumNumberOfBlockLanczosStepType(FITKAbaAbstractStep::MaximumNumberOfBlockLanczosStep type);
        /**
         * @brief  获取block lanczos分析步最大数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getMaxNumOfBlockLanczosSteps() const;
        /**
         * @brief  设置block lanczos分析步最大数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setMaxNumOfBlockLanczosSteps(int num);
        /**
         * @brief  获取是否使用基于SIM的线性动力学分析步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getUseSIMBasedLinearDynamicProcedures() const;
        /**
         * @brief  设置是否使用基于SIM的线性动力学分析步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setUseSIMBasedLinearDynamicProcedures(bool stat);
        /**
         * @brief  获取是否开启投影阻尼算子
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getProjectDampingOperators() const;
        /**
         * @brief  设置是否开启投影阻尼算子
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setProjectDampingOperators(bool stat);
        /**
         * @brief  获取是否包含残差模式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getIncludeResidualModes() const;
        /**
         * @brief  设置是否包含残差模式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setIncludeResidualModes(bool stat);
        /**
         * @brief  获取用在每次迭代的向量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getVectorsUsedPerIteration() const;
        /**
         * @brief  设置子空间法求解用在每次迭代的向量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setVectorsUsedPerIteration(int num);
        /**
         * @brief  获取子空间法最大迭代数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int getMaxNumberOfIteration() const;
        /**
         * @brief  设置子空间法最大迭代数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setMaxNumberOfIteration(int num);
        /**
         * @brief  获取AMS方法声域因子
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double getAcousticRangeFactor() const;
        /**
         * @brief  设置AMS方法声域因子
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setAcousticRangeFactor(double value);
        /**
         * @brief  获取矩阵存储方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        MatrixStorage getMatrixStorage() const;
        /**
         * @brief  设置矩阵存储方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setMatrixStorage(MatrixStorage type);
        /**
         * @brief  获取特征向量正规化，by mass 或者 by displacement
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::NormalizeEnginevectors getNormalizeEnginevectors() const;
        /**
         * @brief  设置特征向量正规化
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setNormalizeEnginevectors(FITKAbaAbstractStep::NormalizeEnginevectors type);
    private:
        /**
         * @brief  特征值求解类型，Lanczos、subspace或AMS
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::Eigensolver m_eigenSolver{Eigensolver::Lanczos};
        /**
         * @brief  最大特征值个数请求类型，基于频率范围或者指定数目
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::NumberOfEigenvaluesRequsted m_NumEigenValuesRequested{ FITKAbaAbstractStep::NumberOfEigenvaluesRequsted::AllFrequencyRange};
        /**
         * @brief  最大特征值个数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_numOfEigenvalues{0};
        /**
         * @brief  是否进行频率转换
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_frequencyShift{false};
        /**
         * @brief  频率转换
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_frequencyShiftValue{0.0};
        /**
         * @brief  声固耦合应用类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::AcousticStructuralCouplingWhereApplicable m_AcousticStructuralCoupling{ FITKAbaAbstractStep::AcousticStructuralCouplingWhereApplicable::Include};
        /**
         * @brief  是否设置关注的最小频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_minFreqInterest{false};
        /**
         * @brief  关注的最小频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_minFreqInterstValue{0.0};
        /**
         * @brief  是否设置关注的最高频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_maxFreqInterest{false};
        /**
         * @brief  关注的最高频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_maxFreqInterstValue{0.0};
        /**
         * @brief  块数目类型，默认或者指定数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::BlockSize m_blockSize{ FITKAbaAbstractStep::BlockSize::DefaultBlock};
        /**
         * @brief  块数目
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_blockSizeValue{0};
        /**
         * @brief  block lanczos分析步最大数目类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::MaximumNumberOfBlockLanczosStep m_maxNumbOfBlockLanczosSteps{ FITKAbaAbstractStep::MaximumNumberOfBlockLanczosStep::DefaultMaximum};
        /**
         * @brief  Block lanczos分析步的最大数目
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_maxNumOfLanczosStep{0};
        /**
         * @brief  使用基于SIM的动力学分析步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_useSIMBasedLinearDynamicProcedures{true};
        /**
         * @brief  投影阻尼算子
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_projectDampingOperators{true};
        /**
         * @brief  包含残差模式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_includeResidualModes{false};
        /**
         * @brief  子空间法，用在每次迭代的向量数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_VectorsUsedPerIteration{20};
        /**
         * @brief  子空间法，最大迭代数目
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        int m_maxNumOfIter{30};
        /**
         * @brief  AMS声域因子
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        double m_acousticRangeFactor{0.0};
        //other相关参数
        /**
         * @brief  矩阵存储类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::MatrixStorage m_matrixStroage{ FITKAbaAbstractStep::MatrixStorage::Symmetric };
        /**
         * @brief  特征向量正规化
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        FITKAbaAbstractStep::NormalizeEnginevectors m_normalizeEnginevectors{ FITKAbaAbstractStep::NormalizeEnginevectors::Mass };
    };
}
#endif // FITKABASTEPFREQUENCY_H
