/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKResidualDataManager.h"
#include <QDebug>

namespace Interface
{

FITKResidualDataManager::FITKResidualDataManager(QObject *parent) : QObject(parent)
{
}

void FITKResidualDataManager::onNewDataReceived(const DataTable& newDataChunk)
{   
    {
        QWriteLocker writeLocker(&m_readWriteLock);  // 写入时独占锁

        // 如果是第一次接收数据，则设置表头并初始化列
        if (m_currentData.headers.isEmpty() && !newDataChunk.headers.isEmpty()) 
		{
            m_currentData.headers = newDataChunk.headers;
            for (const QString& header : m_currentData.headers) {
                // 确保 m_currentData.columns 中存在所有表头对应的空 QVector
                if (!m_currentData.columns.contains(header)) {
                    m_currentData.columns[header] = QVector<double>();
                }
            }
        }

        if (m_currentData.headers != newDataChunk.headers && !m_currentData.headers.isEmpty() && !newDataChunk.headers.isEmpty()) 
        {
            qWarning() << "FITKResidualDataManager: Header mismatch detected. Current:" << m_currentData.headers
                       << "New:" << newDataChunk.headers << ". Consider clearing data or handling this case.";
        }

        for (const QString& header : m_currentData.headers) 
        {
            if (newDataChunk.columns.contains(header)) 
            {
                const QVector<double>& newColumnData = newDataChunk.columns.value(header);
                if (!newColumnData.isEmpty())
                { // 只追加非空数据
                     m_currentData.columns[header].append(newColumnData);
                }
            }    
        }
    } // 写锁在此处自动释放

    emit dataUpdated();
}

DataTable FITKResidualDataManager::getCurrentData() const
{
    QReadLocker readLocker(&m_readWriteLock);  // 读取时使用读锁
    return m_currentData; // 返回数据的副本
}

QStringList FITKResidualDataManager::getHeaders() const
{
    QReadLocker readLocker(&m_readWriteLock);  // 读取时使用读锁
    return m_currentData.headers; // 返回表头的副本
}

QVector<double> FITKResidualDataManager::getColumnData(const QString& columnName) const
{
    QReadLocker readLocker(&m_readWriteLock);  // 读取时使用读锁
    if (m_currentData.columns.contains(columnName)) 
	{
        return m_currentData.columns.value(columnName); 
    }
    qWarning() << "FITKResidualDataManager: Column" << columnName << "not found.";
    return QVector<double>(); 
}

QVector<double>& FITKResidualDataManager::getColumnDataUnsafe(const QString& columnName)
{
    static QVector<double> emptyVector;
    if (m_currentData.columns.contains(columnName)) 
    {
        return m_currentData.columns[columnName];
    }
    return emptyVector;
}

void FITKResidualDataManager::clearData()
{
    {
        QWriteLocker writeLocker(&m_readWriteLock);  // 清理时使用写锁
        m_currentData.clear(); 
        qDebug() << "FITKResidualDataManager: All data cleared.";
    } // 写锁在此处自动释放
    
    emit dataUpdated(); 
}

} // namespace Interface