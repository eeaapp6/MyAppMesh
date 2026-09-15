/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKPHengLEIWriteCFDCompressibleSIMPLE.h
 * @brief CompressibleSIMPLE参数文件写出
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-12
 *
 */
#ifndef _FITKPHENGLEI_WRITE_CFD_CompressibleSIMPLE_H___
#define _FITKPHENGLEI_WRITE_CFD_CompressibleSIMPLE_H___

#include "FITKPHengLEIAbsParaWriter.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QString>

namespace Interface
{
	/**
	 * @brief CompressibleSIMPLE参数文件写出
	 * @author LiBaojun (libaojunqd@foxmail.com)
	 * @date 2024-09-12
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIWriteCFDCompressibleSIMPLE : public FITKPHengLEIAbsParaWriter
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIParmWriterGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		explicit  FITKPHengLEIWriteCFDCompressibleSIMPLE() = default;
		/**
		 * @brief Destroy the FITKPHengLEIParmWriterGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		virtual ~FITKPHengLEIWriteCFDCompressibleSIMPLE() = default;
		/**
		 * @brief 执行写出
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		bool writeParamFile() override;

		void consoleMessage(int level, const QString& str) override;

		void run() override;
	};

}


#endif
