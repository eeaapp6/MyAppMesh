/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKPHengLEIWriteBoundary.h
 * @brief boundary参数文件写出
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-12
 *
 */
#ifndef _FITKPHENGLEI_WRITE_BOUNDARYCONDITION_H___
#define _FITKPHENGLEI_WRITE_BOUNDARYCONDITION_H___

#include "FITKPHengLEIAbsParaWriter.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QString>

namespace Interface
{
	class FITKPHengLEIBoundaryManager;
	/**
	 * @brief boundary参数文件写出
	 * @author LiBaojun (libaojunqd@foxmail.com)
	 * @date 2024-09-12
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIWriteBoundary : public FITKPHengLEIAbsParaWriter
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIParmWriterGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		explicit  FITKPHengLEIWriteBoundary() = default;
		/**
		 * @brief Destroy the FITKPHengLEIParmWriterGrid object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		virtual ~FITKPHengLEIWriteBoundary() = default;
		/**
		 * @brief 执行写出
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-12
		 */
		bool writeParamFile() override;

		void consoleMessage(int level, const QString& str) override;

		void run() override;
	private:
		void writeBoundaryFarfield(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryWall(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryInFlow(QTextStream* stream, int meshBoundaryID);

		void writeBoundarySYMMETRY(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryPressureInlet(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryPressureOutlet(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryMassFlowInlet(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryMassFlowOutlet(QTextStream* stream, int meshBoundaryID);

		void writeBoundaryMassFlowOutFlow(QTextStream* stream, int meshBoundaryID);
	private:
		FITKPHengLEIBoundaryManager* _boundaryManager{};

	};


}


#endif
