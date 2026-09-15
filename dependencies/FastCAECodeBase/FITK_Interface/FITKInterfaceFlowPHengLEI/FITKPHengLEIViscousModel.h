/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_VISCOUSMODEL__H___
#define _FITK_PHENGLEI_VISCOUSMODEL__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
	class FITKPHengLEIViscousType;
	/**
	 * @brief  粘性模型类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIViscousModel : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIViscousModel object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIViscousModel();
		/**
		 * @brief Destroy the FITKPHengLEIViscousModel object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIViscousModel();
		/**
		 * @brief 获取粘性模型参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalVicousModelData();
		/**
		 * @brief 设置粘性模型参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalVicousModelData(Core::FITKParameter* data);
		/**
		 * @brief 设置粘性模型类型
		 * @params viscousType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setViscousType(FITKPHengLEIViscousType* viscousType);
		/**
		 * @brief 获取来流类型
		 * @return  FITKPHengLEIAbstractInFlowType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIViscousType* getViscousType();
		/**
		 * @brief   获取当前湍流类型
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		FITKPHengLEIAbsSolver::PHengLEIViscousType getViscousEnumType();
		/**
		 * @brief   设置当前湍流类型
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		void setViscousEnumType(FITKPHengLEIAbsSolver::PHengLEIViscousType type);
	private:
		QString _name{};
		/**
		 * @brief 粘性模型参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalViscous{};
		/**
		 * @brief 粘流模型类型
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		FITKPHengLEIViscousType* _viscousType{};
		/**
		 * @brief
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		FITKPHengLEIAbsSolver::PHengLEIViscousType _type{};
	};
}
#endif