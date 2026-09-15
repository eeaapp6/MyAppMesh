/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIResidaulProcess.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QString>
#include <QStringList>

namespace Interface
{
	FITKPHengLEIResidaulProcess::FITKPHengLEIResidaulProcess()
	{

	}

	FITKPHengLEIResidaulProcess::~FITKPHengLEIResidaulProcess()
	{
	
	}

	void FITKPHengLEIResidaulProcess::storeData(QString filename)
	{
		_iter.clear();
		_averageRes.clear();
		_maxRes.clear();
		_maxResCoorX.clear();
		_maxResCoorY.clear();
		_maxResCoorZ.clear();
		_maxResVariable.clear();
		_WallTime.clear();

		QFile file(filename); // fileName 是你从对话框获取的文件名
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// 处理打开文件失败的情况
			return;
		}

		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			QStringList values = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

			// 确保至少有 8 列数据
			if (values.size() >= 8) {
				_iter.append(values[0].toDouble());
				_averageRes.append(values[1].toDouble());
				_maxRes.append(values[2].toDouble());
				_maxResCoorX.append(values[3].toDouble());
				_maxResCoorY.append(values[4].toDouble());
				_maxResCoorZ.append(values[5].toDouble());
				_maxResVariable.append(values[6].toDouble());
				_WallTime.append(values[7].toDouble());
			}
		}

		file.close();
	}

	QVector<double> FITKPHengLEIResidaulProcess::getData(const QString &name)
	{
		if (name == "iter"){
			return _iter;
		}
		else if(name=="averageRes"){
			return _averageRes;
		}
		else if(name == "maxRes"){
			return _maxRes;
		}
		else if(name == "maxResCoorX"){
			return _maxResCoorX;
		}
		else if(name=="maxResCoorY"){
			return _maxResCoorY;
		}
		else if (name == "maxResCoorZ") {
			return _maxResCoorZ;
		}
		else if (name == "maxResVariable") {
			return _maxResVariable;
		}
		else if (name == "WallTime") {
			return _WallTime;
		}
		return QVector<double>();
	}

	void FITKPHengLEIResidaulProcess::setName(const QString &dataname)
	{
		_dataname.append(dataname);
	}

	QStringList FITKPHengLEIResidaulProcess::getDataName()
	{
		return _dataname;
	}

	void FITKPHengLEIResidaulProcess::SetEnableAverageRes(bool data)
	{
		_isAverageRes = data;
	}

	void FITKPHengLEIResidaulProcess::SetEnableMaxRes(bool data)
	{
		_isMaxRes = data;
	}

	void FITKPHengLEIResidaulProcess::SetEnableMaxResCoorX(bool data)
	{
		_isMaxResCoorX = data;
	}

	void FITKPHengLEIResidaulProcess::SetEnableMaxResCoorY(bool data)
	{
		_isMaxResCoorY = data;
	}

	void FITKPHengLEIResidaulProcess::SetEnableMaxResCoorZ(bool data)
	{
		_isMaxResCoorZ = data;
	}

}
