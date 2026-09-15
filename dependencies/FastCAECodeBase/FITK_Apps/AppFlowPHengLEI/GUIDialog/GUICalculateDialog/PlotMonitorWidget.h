/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file PlotMonitorWidget.h
 * @brief 求解器传输界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 *
 */
#ifndef _PlotMonitorWidget_H
#define _PlotMonitorWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"
#include <QFileSystemWatcher>

namespace Plot {
	class FITKXYPlotWindow;
	class FITKPlotWindowsInterface;
	class FITKXYCurveDrawProp;
}

namespace Interface {
	class FITKPHengLEIData;
	class FITKResidualFileReaderManager;
	class FITKResidualDataManager;
	struct DataTable;
}

namespace EventOper {
	class ParaWidgetInterfaceOperator;
}

namespace GUI
{
	/**
	 * @brief 求解器传输界面
	 * @author BaGuijun (baguijun@163.com)
	 * @date 2024-08-14
	 */
	class GUICalculateDialogAPI PlotMonitorWidget :public QWidget
	{
		Q_OBJECT;
	public:
		/**
		 * @brief Construct a new Transport Widget object
		 * @param[i]  oper           操作器
		 * @param[i]  parent         父对象
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-08-14
		 */
		PlotMonitorWidget();
		/**
		 * @brief Destroy the Transport Widget object
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-08-14
		 */
		~PlotMonitorWidget();
		/**
		 * @brief 初始化
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-08-14
		 */
		void init();
		/**
		 * @brief 创建残差曲线
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-12-06
		 */
		void createResidualCurves();
		/**
		 * @brief 开始监控文件
		 * @param[i] filePath 要监控的文件路径
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-12-06
		 */
		void startMonitoring(const QString& filePath);
		/**
		 * @brief 停止监控文件
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-12-06
		 */
		void stopMonitoring();
	private slots:
		/**
		 * @brief   更新残差曲线
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		void update(QString filePath);
		/**
		 * @brief   数据驱动曲线更新
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		void onDataUpdated(); // 数据更新槽函数
		/**
		 * @brief   处理监控状态变化
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-09
		 */
		void onMonitoringStateChanged(bool isRunning);
		/**
		 * @brief   处理监控错误
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-09
		 */
		void onMonitoringError(const QString& errorMessage);
	private:
		/**
		 * @brief   物理场数据
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		Interface::FITKPHengLEIData* _physicsData{};
		/**
		 * @brief   曲线界面
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		Plot::FITKXYPlotWindow* _residualWidget{};
		/**
		 * @brief   曲线
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		QList<Plot::FITKXYCurveDrawProp*> _curveLists;
		/**
		 * @brief   当前的监控文件路径
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		QString _currentFilePath;
		/**
		 * @brief   残差文件读取线程管理器
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-09
		 */
		Interface::FITKResidualFileReaderManager* _readerManager{};
		/**
		 * @brief   缓存时间数据
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		QVector<double> _cachedTimeData;
		/**
		 * @brief   缓存列头
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-06
		 */
		QStringList _cachedHeaders;
	};
}
#endif
