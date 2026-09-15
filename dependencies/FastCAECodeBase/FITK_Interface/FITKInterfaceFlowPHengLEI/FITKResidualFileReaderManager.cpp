/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKResidualFileReaderManager.h"
#include "FITKResidualDataManager.h"
#include <QDebug>
#include <QFile>
#include <QMutexLocker>

namespace Interface
{	FITKResidualFileReaderManager::FITKResidualFileReaderManager(QObject *parent)
		: QObject(parent)
	{
		m_dataManager = new FITKResidualDataManager(this);
		
		connect(m_dataManager, &FITKResidualDataManager::dataUpdated,
				this, &FITKResidualFileReaderManager::dataUpdated);
	}

	FITKResidualFileReaderManager::~FITKResidualFileReaderManager()
	{
		stopMonitoring();
	}

	bool FITKResidualFileReaderManager::startMonitoring(const QString& filePath, int intervalMs)
	{
		QMutexLocker locker(&m_mutex);

		if (m_isMonitoring) return true;

		cleanup();

		m_currentFilePath = filePath;

		m_worker = new FITKResidualFileReaderWorker(filePath, intervalMs);
		m_worker->resetState();
		m_workerThread = new QThread(this);
		m_worker->moveToThread(m_workerThread);

		connect(m_workerThread, &QThread::started, m_worker, &FITKResidualFileReaderWorker::run);
		connect(m_worker, &FITKResidualFileReaderWorker::stopSignal, m_workerThread, &QThread::quit);
		connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
		connect(m_workerThread, &QThread::finished, this, &FITKResidualFileReaderManager::onThreadFinished);

		connect(m_worker, &FITKResidualFileReaderWorker::newData,
				this, &FITKResidualFileReaderManager::onNewDataReceived);

		m_workerThread->start();
		m_isMonitoring = true;
		emit monitoringStateChanged(true);
        
		return true;
	}

	void FITKResidualFileReaderManager::stopMonitoring()
	{
		QMutexLocker locker(&m_mutex);
    
		if (!m_isMonitoring) return;

		cleanup();

		m_isMonitoring = false;
		m_currentFilePath.clear();

		emit monitoringStateChanged(false);
	}

	bool FITKResidualFileReaderManager::isMonitoring() const
	{
		QMutexLocker locker(&m_mutex);
		return m_isMonitoring;
	}

	QString FITKResidualFileReaderManager::getCurrentFilePath() const
	{
		QMutexLocker locker(&m_mutex);
		return m_currentFilePath;
	}

	void FITKResidualFileReaderManager::onThreadFinished()
	{
		if (m_workerThread) 
		{
			m_workerThread->deleteLater();
			m_workerThread = nullptr;
		}
		m_worker = nullptr; 
	}	
	
	void FITKResidualFileReaderManager::onNewDataReceived(const DataTable& data)
	{
		// 转发数据到数据管理器进行处理
		if (m_dataManager) {
			m_dataManager->onNewDataReceived(data);
		}
	}

	void FITKResidualFileReaderManager::cleanup()
	{
		if (m_worker) 
		{
			m_worker->stopReading();
		}

		if (m_workerThread && m_workerThread->isRunning()) 
		{
			m_workerThread->quit();
        
			if (!m_workerThread->wait(THREAD_WAIT_TIMEOUT)) 
			{
				m_workerThread->terminate();
			}
		}

		if (m_dataManager) clearData();
	}
	
	QStringList FITKResidualFileReaderManager::getHeaders() const
	{
		return m_dataManager->getHeaders();
	}	
		QVector<double> FITKResidualFileReaderManager::getColumnData(const QString& columnName) const
	{
		return m_dataManager->getColumnData(columnName);
	}

	QReadWriteLock& FITKResidualFileReaderManager::getReadWriteLock() const
	{
		return m_dataManager->getReadWriteLock();
	}
	QStringList& FITKResidualFileReaderManager::getHeadersUnsafe()
	{
		return m_dataManager->getHeadersUnsafe();
	}

	QVector<double>& FITKResidualFileReaderManager::getColumnDataUnsafe(const QString& columnName)
	{
		return m_dataManager->getColumnDataUnsafe(columnName);
	}

	void FITKResidualFileReaderManager::clearData()
	{
		if (m_dataManager) {
			m_dataManager->clearData();
		}
	}

	FITKResidualDataManager* FITKResidualFileReaderManager::getDataManager()
	{
		return m_dataManager;
	}

} // namespace Interface
