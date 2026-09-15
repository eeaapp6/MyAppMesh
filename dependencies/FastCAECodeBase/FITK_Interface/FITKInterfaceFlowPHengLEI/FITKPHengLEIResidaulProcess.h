/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_ResidaulProcess__H___
#define _FITK_PHENGLEI_ResidaulProcess__H___
 
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"

#include <QString>
#include <QStringList>
#include <QVector>

namespace Interface
{
	class FITKFlowPHengLEIAPI FITKPHengLEIResidaulProcess : public Core::FITKAbstractNDataObject
	{
	public:
	
		FITKPHengLEIResidaulProcess();

		~FITKPHengLEIResidaulProcess();

		void storeData(QString fileName);

		QVector<double> getData(const QString &name);

		void setName(const QString &dataname);

		QStringList getDataName();
		void SetEnableAverageRes(bool data);
		void SetEnableMaxRes(bool data);
		void SetEnableMaxResCoorX(bool data);
		void SetEnableMaxResCoorY(bool data);
		void SetEnableMaxResCoorZ(bool data);

	private:
		QString _name{};
		QStringList _dataname{};
		QVector<double> _iter{};
		QVector<double> _averageRes{};
		QVector<double> _maxRes{};
		QVector<double> _maxResCoorX{};
		QVector<double> _maxResCoorY{};
		QVector<double> _maxResCoorZ{};
		QVector<double> _maxResVariable{};
		QVector<double> _WallTime{};
		bool _isAverageRes{false};
		bool _isMaxRes{false};
		bool _isMaxResCoorX{false};
		bool _isMaxResCoorY{false};
		bool _isMaxResCoorZ{false};
	};
}
#endif
