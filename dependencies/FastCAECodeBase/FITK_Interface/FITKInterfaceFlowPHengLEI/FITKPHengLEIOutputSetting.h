/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_OUTPUTSETTING__H___
#define _FITK_PHENGLEI_OUTPUTSETTING__H___
 
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
	 * @brief  PHengLEI输出参数类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIOutputSetting : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIOutputSetting object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIOutputSetting();
		/**
		 * @brief Destroy the FITKPHengLEIOutputSetting object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIOutputSetting();
		/**
		 * @brief 获取基本流动参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getBasicFlowData();
		/**
		 * @brief 设置基本流动参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setBasicFlowData(Core::FITKParameter* data);
		/**
		 * @brief 获取Viscosity相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getViscosityData();
		/**
		 * @brief 设置Viscosity相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setViscosityData(Core::FITKParameter* data);
		/**
		 * @brief 获取Vorticity相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getVorticityData();
		/**
		 * @brief 设置Vorticity相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setVorticityData(Core::FITKParameter* data);
		/**
		 * @brief 获取Derived相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getDerivedFlowData();
		/**
		 * @brief 设置Derived相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setDerivedFlowData(Core::FITKParameter* data);
		/**
		 * @brief 获取TurbulenceModel相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getTurbulenceModelData();
		/**
		 * @brief 设置TurbulenceModel相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setTurbulenceModelData(Core::FITKParameter* data);
		/**
		 * @brief 获取NonEquilibrium相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getNonEquilibriumData();
		/**
		 * @brief 设置NonEquilibrium相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setNonEquilibriumData(Core::FITKParameter* data);
		/**
		 * @brief 获取Dimensional相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getDimensionalData();
		/**
		 * @brief 设置Dimensional相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setDimensionalData(Core::FITKParameter* data);
		/**
		 * @brief 获取Dimensionless相关参数
		Core::FITKParameter* getDimensionlessData();
		 * @return
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getDimensionlessData();
		/**
		 * @brief 设置Dimensionless相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setDimensionlessData(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief 基本流动参数
		 */
		Core::FITKParameter* _basicFlowData{};
		/**
		 * @brief 粘度参数
		 */
		Core::FITKParameter* _viscosityData{};
		/**
		 * @brief 涡度参数
		 */
		Core::FITKParameter* _vorticityData{};
		/**
		 * @brief 派生流动变量
		 */
		Core::FITKParameter* _derivedFlowData{};
		/**
		 * @brief 湍流模型参数
		 */
		Core::FITKParameter* _turbulenceModelData{};
		/**
		 * @brief 非平衡参数
		 */
		Core::FITKParameter* _nonEquilibriumData{};
		/**
		 * @brief 有量纲参数
		 */
		Core::FITKParameter* _dimensionalData{};
		/**
		 * @brief 无量纲数
		 */
		Core::FITKParameter* _dimensionlessData{};
	};
}
#endif
