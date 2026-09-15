/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_Calculation__H___
#define _FITK_PHENGLEI_Calculation__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{ 
	/**
	 * @brief  网格相关参数类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEICalculation : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEICalculation object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEICalculation();
		/**
		 * @brief Destroy the FITKPHengLEICalculation object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEICalculation();
		/**
		 * @brief 获取收敛相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalConvergenceData();
		/**
		 * @brief 设置收敛相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalConvergenceData(Core::FITKParameter* data);
		/**
		 * @brief 获取SavingIntervals相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalSavingIntervalsData();
		/**
		 * @brief 设置SavingIntervals相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalSavingIntervalsData(Core::FITKParameter* data);
		/**
		 * @brief 获取Parallel相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalParallelData();
		/**
		 * @brief 设置Parallel相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalParallelData(Core::FITKParameter* data);
		/**
		 * @brief 获取SolverControl相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalSolverControlData();
		/**
		 * @brief 设置SolverControl相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalSolverControlData(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief ConvergencePara相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalConvergencePara{};
		/**
		 * @brief SavingIntervals相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalSavingIntervals{};
		/**
		 * @brief Parallel相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalParallel{};
		/**
		 * @brief   求解控制相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* _additionalSolverControl{};
	};
}
#endif
