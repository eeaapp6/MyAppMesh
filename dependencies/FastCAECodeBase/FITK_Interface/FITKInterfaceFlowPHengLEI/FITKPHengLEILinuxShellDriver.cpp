/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEILinuxShellDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"
#include <QFileSystemWatcher>
#include <QDebug>



namespace Interface
{
	FITKPHengLEILinuxShellDriver::FITKPHengLEILinuxShellDriver()
	{
		if (_commandRunner != nullptr) return;
		// 初始化命令运行器并连接输出信号
		_commandRunner = new AppFrame::FITKLinuxCommandRunner();
		connect(_commandRunner, &AppFrame::FITKAbstractCommandRunner::commandOutput, this, &FITKPHengLEILinuxShellDriver::sendOutputMessage);
	}

	FITKPHengLEILinuxShellDriver::~FITKPHengLEILinuxShellDriver()
	{
		if (!_commandRunner || _processId == -1) return;
		if (_commandRunner->isProcessRunning(_processId))
		{
			_commandRunner->killProcess(_processId);
		}
		// 断开信号连接
		disconnect(_commandRunner);
		delete _commandRunner;
		_commandRunner = nullptr;
	}

	void FITKPHengLEILinuxShellDriver::sendOutputMessage(const QString &str)
	{
		qDebug() << "100101 :" << str;
		// 发送消息到日志系统
		sendMessage(100101, str);
	}

	void FITKPHengLEILinuxShellDriver::threadFinishedSlot()
	{
		// 发出执行完成信号
		emit sig_Finish();
	}

	void FITKPHengLEILinuxShellDriver::setShellCommand(const QString & command)
	{
		_shellCommand = command;
	}

	QString FITKPHengLEILinuxShellDriver::getShellCommand() const
	{
		return _shellCommand;
	}
	void FITKPHengLEILinuxShellDriver::start()
	{
		if (_commandRunner == nullptr || _shellCommand.isEmpty())
		{
			return;
		}

		// 执行命令并监听完成信号
		connect(_commandRunner, SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SLOT(threadFinishedSlot()), Qt::UniqueConnection);

		// 通过 FITKAbstractCommandRunner 的接口设置并执行命令
		_commandRunner->setExecuteCommand(_shellCommand);
		_commandRunner->push2ThreadPool();
	}
	void FITKPHengLEILinuxShellDriver::stop()
	{
		if (_commandRunner == nullptr) return;

		// 从 Runner 获取当前正在运行的进程ID
		_processId = _commandRunner->getID();

		if (_processId > 0)
		{
			_commandRunner->killProcess(_processId);
		}
	}
	bool FITKPHengLEILinuxShellDriver::isProcessRunning() const
	{
		if (_commandRunner == nullptr) return false;

		qint64 currentPid = _commandRunner->getID();
		if (currentPid <= 0) return false;

		return _commandRunner->isProcessRunning(currentPid);
	}



	
}


