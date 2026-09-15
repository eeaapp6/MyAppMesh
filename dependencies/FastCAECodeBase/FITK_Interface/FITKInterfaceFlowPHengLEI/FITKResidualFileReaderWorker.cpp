/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKResidualFileReaderWorker.h"
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <limits.h>
#include <QRegularExpression>
#include <QMetaType>

namespace Interface
{

static const int s_dataTableTypeId = qRegisterMetaType<DataTable>("DataTable");

FITKResidualFileReaderWorker::FITKResidualFileReaderWorker(const QString& filePath, int intervalMs)
    :m_fileName(filePath),
      m_intervalMs(intervalMs),
      m_lastPos(0),
      m_running(false),
      m_headerPased(false) 
{

}

FITKResidualFileReaderWorker::~FITKResidualFileReaderWorker()
{
    stopReading();
}

void FITKResidualFileReaderWorker::stopReading()
{
    QMutexLocker locker(&m_mutex);
    if (!m_running) return; 
    
    m_running = false;
    emit stopSignal();
}

void FITKResidualFileReaderWorker::resetState()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_headerPased = false;
    m_lastPos = 0;
    m_table.clear();
}


void FITKResidualFileReaderWorker::run()
{
    {
        QMutexLocker locker(&m_mutex);
        m_running = true;
    }
    
    while(true)
	{
        {
            QMutexLocker locker(&m_mutex);
            if(!m_running) break; 
        }

        QFile file(m_fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
		{
            qWarning() << "无法打开文件" << m_fileName;
        } else 
		{
            file.seek(m_lastPos);
            QTextStream in(&file);
            
            // 清空上一轮的数据，但保留头部信息（只清空数据，不影响读取位置）
            for(auto it = m_table.columns.begin(); it != m_table.columns.end(); ++it) {
                it.value().clear();
            }
            
            // 解析Tecplot格式的残差文件头部信息（仅在第一次时解析）
            if(!m_headerPased) {
                // 保存当前位置
                qint64 originalPos = m_lastPos;
                
                // 从文件开头解析头部信息
                file.seek(0);
                QTextStream headerStream(&file);
                
                bool inVariableSection = false;
                QStringList variableNames;
                qint64 dataStartPos = 0;
                
                while(!headerStream.atEnd()){
                    qint64 lineStartPos = headerStream.pos();
                    QString line = headerStream.readLine().trimmed();
                    
                    // 跳过空行
                    if(line.isEmpty()) continue;
                    
                    // 跳过标题行
                    if(line.startsWith("Title=")) continue;
                    
                    // 检查是否进入Variables段
                    if(line.startsWith("Variables=")) {
                        inVariableSection = true;
                        continue;
                    }
                    
                    // 如果在Variables段中，收集变量名
                    if(inVariableSection) {
                        // 检查是否是数据行（不以引号开头）
                        if(!line.startsWith("\"")) {
                            // 遇到数据行，说明Variables段结束
                            inVariableSection = false;
                            dataStartPos = lineStartPos; // 记录数据开始位置
                            break;
                        } else {
                            // 解析变量名（去掉引号）
                            QString varName = line;
                            if(varName.startsWith("\"") && varName.endsWith("\"")) {
                                varName = varName.mid(1, varName.length() - 2);
                                variableNames.append(varName);
                            }
                        }
                    }
                }
                
                // 设置头部信息
                if(!variableNames.isEmpty()) {
                    m_table.headers = variableNames;
                    m_table.columns.clear();
                    for(const QString& h : variableNames){
                        m_table.columns[h] = QVector<double>();
                    }
                    m_headerPased = true;
                    // 如果是第一次解析，从数据开始位置读取；否则保持原来的位置
                    if(m_lastPos == 0) {
                        m_lastPos = dataStartPos;
                    }
                    qDebug() << "Tecplot残差文件头部解析成功:" << variableNames;
                } else {
                    // 如果没有找到Variables段，尝试从第一行数据推断
                    file.seek(0);
                    QTextStream fallbackStream(&file);
                    while(!fallbackStream.atEnd()) {
                        qint64 lineStartPos = fallbackStream.pos();
                        QString line = fallbackStream.readLine().trimmed();
                        if(!line.isEmpty() && !line.startsWith("Title=") && !line.startsWith("Variables=") && !line.startsWith("\"")) {
                            QStringList dataFields = line.split(QRegularExpression("[\\s\\t]+"), Qt::SkipEmptyParts);
                            if(dataFields.size() >= 2) {
                                // 使用默认的列名
                                m_table.headers.clear();
                                m_table.headers.append("iter");
                                m_table.headers.append("averageRes");
                                m_table.headers.append("maxRes");
                                m_table.headers.append("maxResCoorX");
                                m_table.headers.append("maxResCoorY");
                                m_table.headers.append("maxResCoorZ");
                                m_table.headers.append("maxResVariable");
                                m_table.headers.append("WallTime");
                                
                                m_table.columns.clear();
                                for(const QString& h : m_table.headers){
                                    m_table.columns[h] = QVector<double>();
                                }
                                m_headerPased = true;
                                // 如果是第一次解析，从数据开始位置读取
                                if(m_lastPos == 0) {
                                    m_lastPos = lineStartPos;
                                }
                                break;
                            }
                        }
                    }
                }
            }

            if (!m_headerPased || m_table.headers.isEmpty()) {
                file.close();
                QThread::msleep(m_intervalMs);
                continue;
            }
            
            // 确保列数据结构已初始化
            if (m_table.columns.isEmpty() && !m_table.headers.isEmpty()) {
                for(const QString& h : m_table.headers){
                    m_table.columns[h] = QVector<double>();
                }
            }
            
            // 从上次读取位置继续读取数据（增量读取）
            file.seek(m_lastPos);
            in.seek(m_lastPos);
            
            bool hasNewData = false;
            while(!in.atEnd()){
                qint64 lineStartPos = in.pos(); // 记录当前行开始位置
                QString line = in.readLine().trimmed();
                
                // 跳过空行、标题行、Variables行和变量定义行
                if(line.isEmpty() || line.startsWith("Title=") || 
                   line.startsWith("Variables=") || line.startsWith("\"")) {
                    continue;
                }

                // 使用空格分割数据字段
                QStringList fields = line.split(QRegularExpression("[\\s\\t]+"), Qt::SkipEmptyParts);
                
                // 检查字段数量是否匹配
                if(fields.size() != m_table.headers.size()) {
                    qWarning() << "数据字段数量不匹配，期望" << m_table.headers.size() << "个，实际" << fields.size() << "个";
                    continue;
                }

                // 解析数据并存入对应列
                for (int i = 0; i < fields.size(); ++i) {
                    const QString& key = m_table.headers[i];
                    bool ok;
                    double v;
                    
                    // 处理特殊值
                    if (fields[i].toUpper() == "N/A" || fields[i].toUpper() == "NAN") {
                        v = std::numeric_limits<double>::quiet_NaN();
                        ok = true;
                    } else {
                        // 解析科学计数法或普通数字
                        v = fields[i].toDouble(&ok);
                        if (!ok) {
                            v = std::numeric_limits<double>::quiet_NaN();
                        }
                    }
                    
                    // 存入对应列
                    if (m_table.columns.contains(key)) {
                        m_table.columns[key].append(v);
                    }
                }
                hasNewData = true;
            }

            // 更新读取位置到文件末尾
            m_lastPos = file.pos();
            file.close();

            // 只有在读取到新数据时才发送信号
            if(hasNewData && !m_table.columns.isEmpty()) {
                bool actualDataAdded = false;
                for (const auto& colData : qAsConst(m_table.columns)) {
                    if (!colData.isEmpty()) {
                        actualDataAdded = true;
                        break;
                    }
                }
                
                if(actualDataAdded) {
                    emit newData(m_table);
                }
            }
        }
        QThread::msleep(m_intervalMs);
    }
    QThread::currentThread()->quit();
}

} // namespace Interface

