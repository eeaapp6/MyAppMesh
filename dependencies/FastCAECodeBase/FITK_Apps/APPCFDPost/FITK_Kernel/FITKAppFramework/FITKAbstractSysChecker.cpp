/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractSysChecker.h"
#include <QStorageInfo>
#include <QSysInfo>
#include <QOperatingSystemVersion>
#include <QProcess>
#include <QThread>

namespace AppFrame
{
    /**
     * @brief 析构函数
     */
    FITKAbstractSysChecker::~FITKAbstractSysChecker()
    {
    }

    /**
     * @brief 进行检查，返回错误信息
     * @return QStringList 错误信息
     */
    QStringList FITKAbstractSysChecker::check()
    {
        // 返回空的错误值
        return QStringList();
    }

    void FITKAbstractSysChecker::appendError(QStringList& errors, const QString& error)
    {
        if (error.isEmpty()) return;
        errors.append(error);
    }

 
    /**
     * @brief 检查系统硬盘容量是否满足最小要求
     * @param minDiskSpaceMB 最小硬盘容量，单位MB
     * @return QString 若不满足最小资源则返回当前资源信息，若满足则返回空
     */
    QString FITKAbstractSysChecker::checkDiskSpace(qint64 minDiskSpaceMB)
    {
        // 遍历所有挂载的存储卷
        for (const QStorageInfo& storage : QStorageInfo::mountedVolumes())
        {
            // 检查存储卷是否有效、已准备好且不是只读的
            if (storage.isValid() && storage.isReady() && !storage.isReadOnly())
            {
#ifdef Q_OS_LINUX
                // 跳过伪文件设备。
                QByteArray fst = storage.fileSystemType();
                if (fst == "tmpfs" || fst == "devtmpfs" || fst == "ramfs")
                {
                    continue;
                }

                // 跳过系统启动设备。
                QString rPath = storage.rootPath();
                if (rPath.startsWith("/boot/efi"))
                {
                    continue;
                }
#endif

                // 获取可用的磁盘空间（单位：MB）
                qint64 freeSpaceMB = storage.bytesAvailable() / (1024 * 1024);
                // 如果可用空间小于最小要求，返回错误信息
                if (freeSpaceMB < minDiskSpaceMB)
                {
                    return QString("Insufficient disk space. Available: %1 MB, Required: %2 MB")
                        .arg(freeSpaceMB).arg(minDiskSpaceMB);
                }
            }
        }
        // 如果满足要求，返回空字符串
        return QString();
    }

    /**
     * @brief 检查系统内存容量是否满足最小要求
     * @param minMemoryMB 最小内存容量，单位MB
     * @return QString 若不满足最小资源则返回当前资源信息，若满足则返回空
     */
    QString FITKAbstractSysChecker::checkMemory(qint64 minMemoryMB)
    {
        qint64 freeMemoryMB = 0;

        // 在 Windows 上使用 wmic 命令获取可用内存
    #ifdef Q_OS_WIN
        // 在 Windows 上使用 powershell 命令获取可用内存
        QProcess process;
        process.start("powershell", QStringList() << "-Command" << "Get-WmiObject -Class Win32_OperatingSystem | Select-Object -ExpandProperty FreePhysicalMemory");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        if (output.isEmpty()) return QString();

        QStringList lines = output.split("\n");
        for (const QString& line : lines)
        {
            if (!line.trimmed().isEmpty())
            {
                freeMemoryMB = line.trimmed().toLongLong() / 1024;
                break;
            }
        }
    #elif defined(Q_OS_LINUX)
        // 在 Linux 上读取 /proc/meminfo 文件获取可用内存
        QProcess process;
        process.start("grep MemAvailable /proc/meminfo");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        QStringList lines = output.split("\n");
        for (const QString& line : lines) {
            if (line.startsWith("MemAvailable:")) {
                freeMemoryMB = line.split(QRegExp("\\s+")).at(1).toLongLong() / 1024;
                break;
            }
        }
    #endif


        // 如果可用内存小于最小要求，返回错误信息
        if (freeMemoryMB < 1) return QString();

        if (freeMemoryMB < minMemoryMB)
        {
            return QString("Insufficient memory. Available: %1 MB, Required: %2 MB")
                .arg(freeMemoryMB).arg(minMemoryMB);
        }
        // 如果满足要求，返回空字符串
        return QString();
    }

    /**
     * @brief 检查系统线程数是否满足最小要求
     * @param minThreadCount 最小线程数
     * @return QString 若不满足最小资源则返回当前资源信息，若满足则返回空
     */
    QString FITKAbstractSysChecker::checkThreadCount(int minThreadCount)
    {
        // 获取系统的理想线程数
        int nThread = QThread::idealThreadCount();
        // 如果线程数小于最小要求，返回错误信息
        if (nThread < minThreadCount)
        {
            return QString("Insufficient CPU thread count. Available: %1, Required: %2")
                .arg(nThread).arg(minThreadCount);
        }
        // 如果满足要求，返回空字符串
        return QString();
    }
    QString FITKAbstractSysChecker::checkWindowsVersion(int minMajorVersion)
    {
    #ifdef Q_OS_WIN
        // 获取当前Windows版本信息
        QOperatingSystemVersion currentVersion = QOperatingSystemVersion::current();
        // 如果当前版本小于最小要求，返回错误信息
        if (currentVersion.majorVersion() < minMajorVersion)
        {
            return QString("Insufficient Windows version. Current: %1, Required: %2")
                .arg(currentVersion.majorVersion()).arg(minMajorVersion);
        }
        // 如果满足要求，返回空字符串
        return QString();
    #else
        // 如果不是Windows系统，直接返回错误提示
        return QString("This system is not Windows.");
    #endif
    }
}