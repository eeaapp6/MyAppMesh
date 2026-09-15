/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKResidualDataManager_H
#define FITKResidualDataManager_H

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QObject>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include "FITKResidualFileReaderWorker.h"

// 前向声明
namespace Interface
{
    class FITKResidualDataManager;
}


namespace Interface
{
    class FITKFlowPHengLEIAPI FITKResidualDataManager : public QObject
    {
        Q_OBJECT
    public:
        explicit FITKResidualDataManager(QObject *parent = nullptr);

    public slots:
		/**
		 * @brief   新数据接收槽函数
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-11
		 */
        void onNewDataReceived(const DataTable& newDataChunk);

	signals:
		/**
		 * @brief   数据更新信号
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-11
		 */
        void dataUpdated();
    public:
		/**
		 * @brief   获取当前数据
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-10
		 */
        DataTable getCurrentData() const;        
		/**
		 * @brief   获取物理量数据标签
		 * @return  
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-10
		 */
		QStringList getHeaders() const;
		/**
		 * @brief   获取对应物理量标签的数据
		 * @param   columnName
		 * @return  
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-10
		 */
        QVector<double> getColumnData(const QString& columnName) const;

        /**
         * @brief 获取读写锁的引用（供外部使用QReadLocker/QWriteLocker）
         * @return 读写锁引用
         * @author WangKai (wang_starry@outlook.com)  
         * @date 2025-06-12
         * @note 使用示例：
         * QReadLocker locker(&manager->getReadWriteLock());
         * const auto& headers = manager->getHeadersUnsafe();
         * const auto& data = manager->getColumnDataUnsafe("pressure");
         */
        QReadWriteLock& getReadWriteLock() const { return m_readWriteLock; }
        
        /**
         * @brief 获取表头（无锁版本，需要调用者自己管理锁）
         * @return 表头列表的常量引用
         * @warning 调用前必须获取读锁或写锁
         */
        QStringList& getHeadersUnsafe() { return m_currentData.headers; }
          /**
         * @brief 获取列数据（无锁版本，需要调用者自己管理锁）
         * @param columnName 列名
         * @return 列数据的引用
         * @warning 调用前必须获取读锁或写锁
         */
        QVector<double>& getColumnDataUnsafe(const QString& columnName);
        
        /**
         * @brief 获取完整数据表（无锁版本，需要调用者自己管理锁）
         * @return 数据表的常量引用
         * @warning 调用前必须获取读锁或写锁
         */
        DataTable& getDataTableUnsafe() { return m_currentData; }
		/**
		 * @brief   清理数据
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-10
		 */
        void clearData();
    private:
		/**
		 * @brief   获取累计的数据
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-10
		 */
        DataTable m_currentData;		/**
		 * @brief   用于保护 m_currentData 的线程安全（读写锁）
		 * 允许多个线程同时读取，写入时独占
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-06-12
		 */
        mutable QReadWriteLock m_readWriteLock;
    };
}

#endif // FITKResidualDataManager_H