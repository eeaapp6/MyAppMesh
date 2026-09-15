/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKPHengLEILinuxShellDriver.h
 * @brief 第三方程序驱动抽象类声明
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-06-09
 *
 */

#ifndef  __FITK_PHEENGLEI_LINUX_SHELL_DRIVER_H__
#define  __FITK_PHEENGLEI_LINUX_SHELL_DRIVER_H__

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"
#include <QProcess> 

class FITKAbstractCommandRunner;

namespace Interface
{

	/**
	 * @brief FITKAbstractCommandRunner
	 * @date 2024-08-19
	 */
	//class FITKAbstractCommandRunner;
	/**
	 * @brief 第三方程序驱动抽象类
	 * @author libaojun (libaojunqd@Foxmail.com)
	 * @date 2024-06-09
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEILinuxShellDriver : public AppFrame::FITKAbstractProgramerDriver
	{
		Q_OBJECT
	public:
		/**
		 * @brief Construct a new FITKLinuxProgramerDriver object
		 * @author libaojun (libaojunqd@Foxmail.com)
		 * @date 2024-06-09
		 */
		explicit FITKPHengLEILinuxShellDriver();
		/**
		 * @brief Destroy the FITKLinuxProgramerDriver object
		 * @author libaojun (libaojunqd@Foxmail.com)
		 * @date 2024-06-09
		 */
		virtual ~FITKPHengLEILinuxShellDriver();


		/**
		 * @brief  设置要执行的Shell命令
		 * @param[i]  command       要执行的Shell命令
		 */
		void setShellCommand(const QString & command);

		/**
		 * @brief  获取要执行的Shell命令
		 * @return QString      Shell命令
		 */
		QString getShellCommand() const;

		/**
		 * @brief 启动Shell命令
		 */
		void start() override;

		/**
		 * @brief 停止正在执行的Shell命令
		 */
		void stop() override;

		/**
		 * @brief 判断进程是否正在运行
		 * @return true 进程正在运行
		 * @return false 进程未运行
		 */
		bool isProcessRunning() const;

	private slots:
		/**
		 * @brief   发送Shell命令输出
		 * @author  fulipeng (fulipengqd@yeah.net)
		 * @date    2024-07-18
		 */
		void sendOutputMessage(const QString &str);
	protected slots:
		/**
		 * @brief   线程执行结束
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2024-08-22
		 */
		void threadFinishedSlot();
	protected:
		/**
		 * @brief 要执行的Shell命令
		 */
		QString _shellCommand{};

		/**
		 * @brief 命令执行的工作路径
		 */
		QString _workDir{};
		/**
		 * @brief   进程ID
		 * @author  fulipeng (fulipengqd@yeah.net)
		 * @date    2024-07-18
		 */
		qint64 _processId = -1;
		/**
		 * @brief 终端命令执行 驱动器实例
		 * @author fulipeng (fulipengqd@yeah.net)
		 * @date 2024-07-23
		 */
		AppFrame::FITKAbstractCommandRunner* _commandRunner{};
	};
}

#endif
