/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_INFLOW__H___
#define _FITK_PHENGLEI_INFLOW__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
	class FITKPHengLEIAbstractInFlowType;
	/**
	 * @brief  来流参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIInFlow : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIInFlow object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIInFlow();
		/**
		 * @brief Destroy the FITKPHengLEIInFlow object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIInFlow();
		/**
		 * @brief 获取来流参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getInFlowAdditionalData();
		/**
		 * @brief 设置来流参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setInFlowAdditionalData(Core::FITKParameter* data);
		/**
		 * @brief 设置来流类型
		 * @params inFLowType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setInFlowType(FITKPHengLEIAbstractInFlowType* inFLowType);
		/**
		 * @brief 获取来流类型
		 * @return  FITKPHengLEIAbstractInFlowType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIAbstractInFlowType* getInFlowType();
	private:
		QString _name{};
		/**
		 * @brief 来流参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalInFlow{};
		/**
		 * @brief 来流类型
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIAbstractInFlowType* _inFlowType{};
	};
}
#endif
