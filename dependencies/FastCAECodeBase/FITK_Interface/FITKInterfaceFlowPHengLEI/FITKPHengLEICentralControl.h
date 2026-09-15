/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#ifndef _FITKPARTION_PHENGLEI_H___
#define _FITKPARTION_PHENGLEI_H___


#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"

namespace Interface
{
	/**
	 * @brief 总控文件
	 * @author LiBaojun (libaojunqd@foxmail.com)
	 * @date 2024-09-11
	 */
	 // 枚举定义网格维度
	enum GridDimension {
		two_dimensional,
		three_dimensional
	};

	// 枚举定义网格类型
	enum GridType {
		Unstruct,
		Struct
	};
	class FITKFlowPHengLEIAPI FITKPHengLEICentralControl : public Core::FITKParameter
		, public Core::FITKAbstractDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIPartion object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-11
		 */
		explicit FITKPHengLEICentralControl();
		/**
		 * @brief Destroy the FITKPHengLEIPartion object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-11
		 */
		virtual ~FITKPHengLEICentralControl() = default;

		/**
		 * @brief 设置网格维度
		 * @param dimension 网格维度
		 */
		void setGridDimension(GridDimension dimension);

		/**
		 * @brief 获取网格维度
		 * @return 当前网格维度
		 */
		GridDimension getGridDimension() const;

		/**
		 * @brief 设置网格类型
		 * @param type 网格类型
		 */
		void setGridType(GridType type);

		/**
		 * @brief 获取网格类型
		 * @return 当前网格类型
		 */
		GridType getGridType() const;

	private:

		GridDimension gridDimension;  // 网格维度
		GridType gridType;            // 网格类型
	};

}  // namespace Interface
#endif
