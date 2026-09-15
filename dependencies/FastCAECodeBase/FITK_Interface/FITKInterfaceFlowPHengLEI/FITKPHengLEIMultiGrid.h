/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_MultiGrid__H___
#define _FITK_PHENGLEI_MultiGrid__H___
 
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
	 * @brief  多重网格类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIMultiGrid : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIMultiGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIMultiGrid();
		/**
		 * @brief Destroy the FITKPHengLEIMultiGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIMultiGrid();
		/**
		 * @brief 获取多重网格参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalMultiGridData();
		/**
		 * @brief 设置多重网格参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalMultiGridData(Core::FITKParameter* data);

		void setEnableMultiGrid(bool enable);

		bool getEnableMultiGrid();

	private:
		QString _name{};
		/**
		 * @brief 多重网格参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalMultiGrid{};

		bool _isEnableMultiGrid{ true };
	};
}
#endif
