/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEICentralControl.h"

namespace Interface
{
	// 构造函数，初始化枚举类型的默认值
	FITKPHengLEICentralControl::FITKPHengLEICentralControl()
		: gridDimension(two_dimensional), gridType(Unstruct) {}

	// 设置网格维度
	void FITKPHengLEICentralControl::setGridDimension(GridDimension dimension) {
		gridDimension = dimension;
	}

	// 获取网格维度
	GridDimension FITKPHengLEICentralControl::getGridDimension() const {
		return gridDimension;
	}

	// 设置网格类型
	void FITKPHengLEICentralControl::setGridType(GridType type) {
		gridType = type;
	}

	// 获取网格类型
	GridType FITKPHengLEICentralControl::getGridType() const {
		return gridType;
	}
}