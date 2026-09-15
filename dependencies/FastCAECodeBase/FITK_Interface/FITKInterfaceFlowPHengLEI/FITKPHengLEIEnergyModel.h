/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_EnergyModel__H___
#define _FITK_PHENGLEI_EnergyModel__H___
 
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
	 * @brief  通用控制参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIEnergyModel : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIEnergyModel object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIEnergyModel();
		/**
		 * @brief Destroy the FITKPHengLEIEnergyModel object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIEnergyModel();
		/**
		 * @brief 获取参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalData();
		/**
		 * @brief 设置参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalData(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief 参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalEnergyModel{};
	};
}
#endif
