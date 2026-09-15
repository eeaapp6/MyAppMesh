/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKPHengLEILinuxDriver
 * @brief   PHengLEI Linux程序驱动器
 * @author  WangKai (wang_starry@outlook.com)
 * @date    2024-08-22
 *
 */
#ifndef _FITKPHengLEILinuxDriver_H__
#define _FITKPHengLEILinuxDriver_H__


#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"
#include "FITKPHengLEILinuxShellDriver.h"

namespace AppFrame
{
	class FITKProgramInputInfo;
}

namespace PHengLEIDriver
{
	/**
	 * @brief   PHengLEI Linux程序驱动器
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKPHengLEILinuxDriver : public Interface::FITKPHengLEILinuxShellDriver
	{
		Q_OBJECT
	public:
		explicit FITKPHengLEILinuxDriver() = default;
		virtual ~FITKPHengLEILinuxDriver() = default;
		/**
		 * @brief   获取程序类型
		 * @return  1
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2024-08-22
		 */
		int getProgramType() override;
		/**
		 * @brief   获取程序名称
		 * @return  PHengLEI
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2024-08-22
		 */
		QString getProgramName() override;
	};
	Register2FITKProgramDriverFactory(2, FITKPHengLEILinuxDriver, FITKPHengLEILinuxDriver)
}

#endif // FITKOFFOAMRUNDRIVER_H
