/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKResidualFileReaderManager_H
#define FITKResidualFileReaderManager_H

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKResidualFileReaderWorker.h"
#include "FITKResidualDataManager.h"
#include <QObject>
#include <QThread>
#include <QString>
#include <QMutex>

// 前向声明
namespace Interface
{
}

namespace Interface
{
    /**
     * @brief 残差文件读取线程管理器负责管理 FITKResidualFileReaderWorker 的线程生命周期
     * @author WangKai (wang_starry@outlook.com)
     * @date 2025-06-09
     */
    class FITKFlowPHengLEIAPI FITKResidualFileReaderManager : public QObject
    {
        Q_OBJECT

    public:
        explicit FITKResidualFileReaderManager(QObject *parent = nullptr);
        ~FITKResidualFileReaderManager();

        /**
         * @brief 开始监控残差文件
         * @param filePath 残差文件路径
         * @param intervalMs 监控间隔（毫秒）
         * @return 是否成功启动
         */
        bool startMonitoring(const QString& filePath, int intervalMs = 1000);

        /**
         * @brief 停止监控
         * return
         */
        void stopMonitoring();

        /**
         * @brief 检查是否正在监控
         * @return 是否正在监控
         */
        bool isMonitoring() const;
		/**
         * @brief 获取当前监控的文件路径
         * @return 文件路径
         */
        QString getCurrentFilePath() const;
        
        /**
         * @brief 获取数据表头
         * @return 表头列表
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-09
         */
        QStringList getHeaders() const;        
        /**
         * @brief 获取列数据（拷贝版本）
         * @param columnName 列名
         * @return 列数据
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-09
         */        QVector<double> getColumnData(const QString& columnName) const;

        /**
         * @brief 获取读写锁引用（用于外部管理锁）
         * @return 读写锁引用
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-12
         * @note 使用示例：
         * QReadLocker locker(&manager->getReadWriteLock());
         * const auto& headers = manager->getHeadersUnsafe();
         */
        QReadWriteLock& getReadWriteLock() const;
          /**
         * @brief 获取表头（无锁版本，需要调用者自己管理锁）
         * @return 表头列表的引用
         * @warning 调用前必须获取读锁或写锁
         */
        QStringList& getHeadersUnsafe();
        
        /**
         * @brief 获取列数据（无锁版本，需要调用者自己管理锁）  
         * @param columnName 列名
         * @return 列数据的引用
         * @warning 调用前必须获取读锁或写锁
         */
        QVector<double>& getColumnDataUnsafe(const QString& columnName);

        /**
         * @brief 清空数据
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-09
         */
        void clearData();

        /**
         * @brief 获取数据管理器（用于直接访问）
         * @return 数据管理器指针
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-11
         */
        FITKResidualDataManager* getDataManager();
    signals:
        /**
         * @brief 数据更新信号（替代原来的newData信号）
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-09
         */
        void dataUpdated();
        /**
         * @brief 监控状态变化信号
         * @param isRunning 是否正在运行
         */
        void monitoringStateChanged(bool isRunning);
        /**
         * @brief 错误信号
         * @param errorMessage 错误消息
         */
        void errorOccurred(const QString& errorMessage);    
	private slots:
        /**
         * @brief 线程完成槽函数
         */
        void onThreadFinished();

        /**
         * @brief 转发新数据到数据管理器
         * @param data 数据表
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-11
         */
        void onNewDataReceived(const DataTable& data);
    private:
        /**
         * @brief 清理资源
         */
        void cleanup();
        
    private:
        /**
         * @brief   工作线程和工作对象
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        QThread* m_workerThread{nullptr};
        /**
         * @brief   工作对象指针
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        FITKResidualFileReaderWorker* m_worker{nullptr};
        /**
         * @brief   当前文件路径
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        QString m_currentFilePath{};
        
        /**
         * @brief   是否正在监控
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        bool m_isMonitoring{false};

        /**
         * @brief   互斥锁用于保护共享资源
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        mutable QMutex m_mutex{};

        /**
         * @brief   数据管理器，用于存储和处理读取到的数据
         * @author  WangKai (wang_starry@outlook.com)
         * date    2025-06-09
         */
        FITKResidualDataManager* m_dataManager{nullptr};

        // 线程等待超时（毫秒）
        static constexpr int THREAD_WAIT_TIMEOUT = 5000;
        // 线程强制终止等待时间（毫秒）
        static constexpr int THREAD_TERMINATE_TIMEOUT = 2000; 
    };

} // namespace Interface

#endif // FITKResidualFileReaderManager_H
