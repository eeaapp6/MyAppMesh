/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_GENERAL__H___
#define _FITK_PHENGLEI_GENERAL__H___
 
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
	class FITKFlowPHengLEIAPI FITKPHengLEIGeneral : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIGeneral object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIGeneral();
		/**
		 * @brief Destroy the FITKPHengLEIGeneral object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIGeneral();
		/**
		 * @brief 获取通用控制参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAxisRotateData();
		/**
		 * @brief 设置通用控制参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAxisRotateData(Core::FITKParameter* data);
		/**
		 * @brief 获取通用控制参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getTransientData();
		/**
		 * @brief 设置通用控制参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setTransientData(Core::FITKParameter* data);

		void setMeshScale(Core::FITKParameter* data);

		Core::FITKParameter* getMeshScale();

		void initGenral();

	private:
		QString _name{};
		/**
		 * @brief 通用控制参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _axisRotateData{};

		Core::FITKParameter* _transientData{};

		Core::FITKParameter* _meshScale{};
	};
}
#endif
