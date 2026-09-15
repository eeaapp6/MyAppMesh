/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKResidualFileReaderWorker_H
#define FITKResidualFileReaderWorker_H

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QMutex>
#include <QMap>

namespace Interface
{
    struct DataTable {
        QStringList headers;                      // 列名列表
        QMap<QString,QVector<double>> columns;    // 列名 -> 数据向量

        void clear() 
		{
            headers.clear();
            columns.clear();
        }
    };

    class FITKFlowPHengLEIAPI FITKResidualFileReaderWorker : public QObject
    {
        Q_OBJECT    
    public:
        explicit FITKResidualFileReaderWorker(const QString& filePath, int intervalMs = 1000);
        ~FITKResidualFileReaderWorker();
        void stopReading(); // 停止读取并退出线程
		void run();
        
        /**
         * @brief 重置工作状态（用于外部线程管理）
         * @author WangKai (wang_starry@outlook.com)
         * @date 2025-06-09
         */
        void resetState();
        
    signals:
        void newData(const DataTable& data);
        void stopSignal(); // 用于通知线程退出的信号
    private:
        QString m_fileName{};
        int m_intervalMs{0};
        int m_lastPos{0};
        bool m_running{false};
        QMutex m_mutex;

        bool m_headerPased = false;
        DataTable m_table;

    };
}

#endif // FITKResidualFileReaderWorker_H
