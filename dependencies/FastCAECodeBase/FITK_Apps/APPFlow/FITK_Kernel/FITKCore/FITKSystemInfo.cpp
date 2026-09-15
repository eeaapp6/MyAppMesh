/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKSystemInfo.h"
#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <intrin.h>
#endif // Q_OS_WIN

namespace Core
{
    FITKSystemInfo::FITKSystemInfo()
    {
        // 检测操作系统类型
        this->detectOSInfo();
        // 检测系统架构类型
        this->detectArchInfo();
        
        switch (m_osType)
        {
            case FITKOperatingSystem::FITK_OS_WINDOWS:// Windows平台检测CPU和GPU类型
                this->detectInfoWin();
                break;
            default:
                break;
        }
    }

    FITKOperatingSystem FITKSystemInfo::getOperatingSystem() const
    {
        return m_osType;
    }

    FITKArchitecture FITKSystemInfo::getArchitecture() const
    {
        return m_archType;
    }

    CPUType FITKSystemInfo::getCPUType() const
    {
        return m_cpuType;
    }

    GPUType FITKSystemInfo::getGPUType() const
    {
        return m_gpuType;
    }

    void FITKSystemInfo::detectOSInfo()
    {
        // 检测操作系统类型
    #ifdef Q_OS_WIN
        m_osType = FITKOperatingSystem::FITK_OS_WINDOWS;
    #elif Q_OS_MACOS
        m_osType = FITKOperatingSystem::FITK_OS_MACOS;
    #elif Q_OS_LINUX
        m_osType = FITKOperatingSystem::FITK_OS_LINUX;
    #else
        m_osType = FITKOperatingSystem::FITK_OS_UNKNOWN;
    #endif
    }
    void FITKSystemInfo::detectArchInfo()
    {
        // 检测系统架构类型
    #if defined(__x86_64__) || defined(_M_X64)
            m_archType = FITKArchitecture::FITK_ARCH_X64;
    #elif defined(__i386__) || defined(_M_IX86)
            m_archType = FITKArchitecture::FITK_ARCH_X86;
    #elif defined(__aarch64__) || defined(_M_ARM64)
            m_archType = FITKArchitecture::FITK_ARCH_ARM64; 
    #elif defined(__arm__) || defined(_M_ARM)
            m_archType = FITKArchitecture::FITK_ARCH_ARM;
    #else
            m_archType = FITKArchitecture::FITK_ARCH_UNKNOWN;
    #endif
    }

    void FITKSystemInfo::detectInfoWin()
    {
        this->detectCPUInfoWin();
        this->detectGPUInfoWin();
    }

    void FITKSystemInfo::detectGPUInfoWin()
    {
#if defined(Q_OS_WIN)
          // 获取当前可执行程序所在路径
        QString exePath = QCoreApplication::applicationDirPath();
        QString glewInfoPath = QDir(exePath).filePath("glewinfo.exe");
        //拷贝文件
        QFile::copy("://OpenGL/glew/glew32.dll", QDir(exePath).filePath("glew32.dll"));
        QFile::copy("://OpenGL/glew/glewinfo.exe", glewInfoPath);

        // 保存当前工作目录
        QString currentPath = QDir::currentPath();
        // 设置工作目录为可执行文件所在路径
        QDir::setCurrent(exePath);
        // 删除 glewinfo.txt 文件
        QFile::remove(QDir(exePath).filePath("glewinfo.txt"));

        // 启动 glewinfo.exe 并获取输出
        QProcess process;
        process.setProgram(glewInfoPath);
//        process.setArguments({ "-version", "5.2" });
        process.start();
        if (!process.waitForFinished())
        {
            // 恢复工作目录
            QDir::setCurrent(currentPath);
            qDebug() << QString("Failed to start glewinfo.exe");
            return;
        }
        // 恢复工作目录
        QDir::setCurrent(currentPath);

        // 读取 glewinfo.txt 文件的第六行和第七行
        QFile file(QDir(exePath).filePath("glewinfo.txt"));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // 恢复工作目录
            QDir::setCurrent(currentPath);
            qDebug() << QString("Failed to open glewinfo.txt");
            return;
        }
        
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(!line.startsWith("Running on "))continue;
            if (line.contains("NVIDIA"))
                m_gpuType = GPUType::FITK_GPU_NVIDIA;
            else if (line.contains("AMD"))
                m_gpuType = GPUType::FITK_GPU_AMD;
            else if (line.contains("Intel"))
                m_gpuType = GPUType::FITK_GPU_INTEL;
            break;        
        }
        
        file.close(); 
        file.remove();
        
#endif  // Windows平台检测GPU类型
    }
    void FITKSystemInfo::detectCPUInfoWin()
    {
 #if defined(Q_OS_WIN)
        int cpuInfo[4] = { -1 };
        __cpuid(cpuInfo, 0);

        // Vendor ID is in EBX, EDX, ECX
        char vendor[13];
        memset(vendor, 0, sizeof(vendor));
        *reinterpret_cast<int*>(vendor) = cpuInfo[1]; // EBX
        *reinterpret_cast<int*>(vendor + 4) = cpuInfo[3]; // EDX
        *reinterpret_cast<int*>(vendor + 8) = cpuInfo[2]; // ECX

        QString vendorStr = QString::fromLatin1(vendor);

        if (vendorStr == "GenuineIntel") {
            m_cpuType = CPUType::FITK_CPU_INTEL;
        }
        else if (vendorStr == "AuthenticAMD") {
            m_cpuType = CPUType::FITK_CPU_AMD;
        }
        else {
            m_cpuType = CPUType::FITK_CPU_UNKNOWN;
        }
    #endif
    }
}
