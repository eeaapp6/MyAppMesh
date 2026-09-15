/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PlotMonitorWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKResidualDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKResidualFileReaderManager.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Component/FITKPlotWindow/FITKXYPlotWindow.h"
#include "FITK_Component/FITKPlotWindow/FITKPlotWindowsInterface.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "FITK_Component/FITKPlotWindow/FITKXYCurveDrawManager.h"
#include "FITK_Component/FITKPlotWindow/FITKBarChartWindow.h"
#include "FITK_Component/FITKPlotWindow/FITKBarChartProperty.h"
#include "FITK_Component/FITKPlotWindow/FITKXYPlotProperty.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include <QToolBox>
#include <QReadLocker>
#include <QFile>
#include <QDebug>

namespace GUI
{
	PlotMonitorWidget::PlotMonitorWidget()
	{
		_physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
		_readerManager = new Interface::FITKResidualFileReaderManager(this);

		connect(_readerManager, &Interface::FITKResidualFileReaderManager::dataUpdated,
			this, &PlotMonitorWidget::onDataUpdated);
		connect(_readerManager, &Interface::FITKResidualFileReaderManager::monitoringStateChanged,
			this, &PlotMonitorWidget::onMonitoringStateChanged);
		connect(_readerManager, &Interface::FITKResidualFileReaderManager::errorOccurred,
			this, &PlotMonitorWidget::onMonitoringError);

		init();
	}

	PlotMonitorWidget::~PlotMonitorWidget()
	{
		stopMonitoring();

		for (Plot::FITKXYCurveDrawProp* curve : _curveLists)
		{
			if (curve) {
				delete curve;
			}
		}
		_curveLists.clear();
	}

	void PlotMonitorWidget::init()
	{
		Plot::FITKPlotWindowsInterface* PlotWindows = FITKAPP->getComponents()
			->getComponentTByName<Plot::FITKPlotWindowsInterface>("FITKPlotWindowsInterface");
		_residualWidget = dynamic_cast<Plot::FITKXYPlotWindow*>(PlotWindows->getWidget(0));
		GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
		if (mainWindow == nullptr)return;
		GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
		if (!renderWidget) return;
		Comp::FITKMdiArea* mdiArea = renderWidget->getMdiArea();
		if (!mdiArea) return;
		mdiArea->addSubWidget(_residualWidget, "Res Monitor");
		mdiArea->setLayoutType(Comp::FITKVportsLayoutType::Tab);

		_residualWidget->getPlotProperty()->setTitle("Res Monitor");
		//设置XY轴名称
		_residualWidget->getPlotProperty()->setAxisName("iter", "Residual[-]");
		//将y轴设为指数轴
		_residualWidget->getPlotProperty()->setAxisScaleEngine(0);
		//将图例挪动到窗口右侧
		_residualWidget->getPlotProperty()->setLegendPlace(1);
		_residualWidget->getPlotProperty()->setPlotGrid(true);
	}

	void PlotMonitorWidget::update(QString filePath)
	{
		startMonitoring(filePath);
	}	void PlotMonitorWidget::createResidualCurves()
	{
		// 使用Qt读锁进行RAII管理，直接获取表头引用
		QReadLocker locker(&_readerManager->getReadWriteLock());
		auto& headers = _readerManager->getHeadersUnsafe();
		if (headers.isEmpty()) return;

		// 清理现有曲线
		for (Plot::FITKXYCurveDrawProp* curve : _curveLists)
		{
			if (curve)
			{
				_residualWidget->getCurveManager()->removeGraphObj(curve);
				delete curve;
			}
		}
		_curveLists.clear();

		// 创建新曲线（跳过时间列）
		for (int i = 0; i < headers.size(); ++i)
		{
			const QString& header = headers[i];
			if (header.toLower() == "iter") continue;

			Plot::FITKXYCurveDrawProp* curve = new Plot::FITKXYCurveDrawProp();
			curve->setColor(QColor::fromHsl((i * 60) % 360, 200, 120));
			_curveLists.append(curve);
		}
		// 读锁在此处自动释放
	}

	void PlotMonitorWidget::startMonitoring(const QString& filePath)
	{
		_currentFilePath = filePath;

		if (_readerManager)
		{
			bool success = _readerManager->startMonitoring(_currentFilePath, 3000);
			if (!success)
			{
				qWarning() << "启动残差文件监控失败:" << _currentFilePath;
			}
		}
		else {
			return;
		}
	}

	void PlotMonitorWidget::stopMonitoring()
	{
		// 使用线程管理器停止监控
		if (_readerManager) {
			_readerManager->stopMonitoring();
		}

		_currentFilePath.clear();
	}

	void PlotMonitorWidget::onDataUpdated()
	{
		if (!_readerManager) return;
		if (_curveLists.isEmpty()) createResidualCurves();
		// 使用Qt读锁进行RAII管理，直接获取数据引用
		QReadLocker locker(&_readerManager->getReadWriteLock());
		auto& headers = _readerManager->getHeadersUnsafe();
		if (headers.isEmpty()) return;

		bool headersChanged = (_cachedHeaders != headers);
		if (headersChanged)
		{
			_cachedHeaders = headers;  // 这里需要拷贝用于缓存比较
			createResidualCurves();
		}

		// 查找时间列索引
		int timeIndex = -1;
		QString timeColumnName;
		for (int i = 0; i < headers.size(); ++i)
		{
			const QString& header = headers[i];
			if (header.toLower() == "iter")
			{
				timeIndex = i;
				timeColumnName = header;
				break;
			}
		}

		// 处理时间数据
		bool hasTimeData = false;
		if (timeIndex >= 0)
		{
			// 使用引用访问时间数据，避免拷贝
			auto& timeDataRef = _readerManager->getColumnDataUnsafe(timeColumnName);
			hasTimeData = !timeDataRef.isEmpty();

			// 只在数据变化时才更新缓存
			if (hasTimeData && (_cachedTimeData.size() != timeDataRef.size() || headersChanged))
			{
				_cachedTimeData = timeDataRef;  // 拷贝到缓存用于后续使用
			}
		}

		// 更新曲线数据
		int curveIndex = 0;
		for (int i = 0; i < headers.size(); ++i)
		{
			const QString& header = headers[i];
			if (header.toLower() == "iter") continue; // 跳过时间列

			if (curveIndex >= _curveLists.size()) break;            // 使用引用访问Y轴数据，避免拷贝
			auto& yDataRef = _readerManager->getColumnDataUnsafe(header);
			if (yDataRef.isEmpty())
			{
				curveIndex++;
				continue;
			}
			Plot::FITKXYCurveDrawProp* curve = _curveLists[curveIndex];
			if (curve)
			{
				if (hasTimeData && _cachedTimeData.size() == yDataRef.size())
				{
					curve->setData(header, _cachedTimeData, yDataRef);
				}
			}
			curveIndex++;
		}

		// 更新图表显示
		for (int i = 0; i < _curveLists.size(); ++i)
		{
			_residualWidget->getCurveManager()->appendGraphObj(_curveLists[i]);
		}

		_residualWidget->updataCanvas();
	}

	void PlotMonitorWidget::onMonitoringStateChanged(bool isRunning)
	{
		qDebug() << "监控状态变化:" << (isRunning ? "已启动" : "已停止");
	}

	void PlotMonitorWidget::onMonitoringError(const QString& errorMessage)
	{
		qCritical() << "监控错误:" << errorMessage;
	}
}

