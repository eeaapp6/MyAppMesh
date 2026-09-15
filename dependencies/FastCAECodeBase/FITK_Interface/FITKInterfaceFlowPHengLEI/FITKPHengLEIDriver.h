/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKPHengLEIDriver
 * @brief   PHengLEI求解器驱动类
 * @author  WangKai (wang_starry@outlook.com)
 * @date    2024-08-22
 *
 */
#ifndef FITKAFLRRUNDRIVER_H
#define FITKAFLRRUNDRIVER_H


#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"
#include "FITKPHengLEIExecProgramDriver.h"

namespace AppFrame
{
    class FITKProgramInputInfo;
}

namespace PHengLEIDriver
{
	/**
	 * @brief   PHengLEI求解器驱动类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
    class FITKPHengLEIDriver : public Interface::FITKPHengLEIExecProgramDriver
    {
        Q_OBJECT
    public:
        explicit FITKPHengLEIDriver() = default;
        virtual ~FITKPHengLEIDriver() = default;
        /**
         * @brief   获取程序类型
         * @return  1
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        int getProgramType() override;
        /**
         * @brief   获取程序名称
         * @return  “AFLR”
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        QString getProgramName() override;
	private slots:
		void threadFinishedSlot();
    };
    Register2FITKProgramDriverFactory(1, FITKPHengLEIDriver, FITKPHengLEIDriver)
}

#endif // FITKOFFOAMRUNDRIVER_H
