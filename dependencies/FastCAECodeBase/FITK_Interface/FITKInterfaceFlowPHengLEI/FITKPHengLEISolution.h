/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_Solution__H___
#define _FITK_PHENGLEI_Solution__H___
 
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
	 * @brief  Solution参数类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEISolution : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEISolution object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEISolution();
		/**
		 * @brief Destroy the FITKPHengLEISolution object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEISolution();
		/**
		 * @brief 获取MomentumEquation相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalMomentumEquationData();
		/**
		 * @brief 设置收敛相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalMomentumEquationData(Core::FITKParameter* data);
		/**
		 * @brief 获取TurbulenceEquation相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalTurbulenceEquationData();
		/**
		 * @brief 设置TurbulenceEquation相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalTurbulenceEquationData(Core::FITKParameter* data);
		/**
		 * @brief 获取EnergyEquation相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalEnergyEquationData();
		/**
		 * @brief 设置EnergyEquation相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalEnergyEquationData(Core::FITKParameter* data);
		/**
		 * @brief 获取SpeciesEquation相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalSpeciesEquationData();
		/**
		 * @brief 设置SpeciesEquation相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalSpeciesEquationData(Core::FITKParameter* data);
		/**
		 * @brief   获取Relaxation相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* getAdditionalRelaxationData();
		/**
		 * @brief   设置Relaxation相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setAdditionalRelaxationData(Core::FITKParameter* data);
		/**
		 * @brief   获取PressurePossionEquation参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* getAdditionalPressurePossionEquation();
		/**
		 * @brief   设置PressurePossionEquation参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setAdditionalPressurePossionEquation(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief MomentumEquationPara相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalMomentumEquationPara{};
		/**
		 * @brief TurbulenceEquationPara相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalTurbulenceEquationPara{};
		/**
		 * @brief EnergyEquationPara相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalEnergyEquationPara{};
		/**
		 * @brief  SpeciesEquationPara相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* _additionalSpeciesEquationPara{};
		/**
		 * @brief   Relaxation相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* _additionalRelaxationPara{};
		/**
		 * @brief   PressurePossionEquation相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* _additionalPressurePossionEquationPara{};
	};
}
#endif
