/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRAMProportionBar.h"
#include "ui_FITKRAMProportionBar.h"

#include <QSysInfo>
#include <QProcessEnvironment>
#include <QFrame>
#include <QShowEvent>
#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTimer>
#include <cmath>

#ifdef Q_OS_WINDOWS
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dxgi.lib")
#include <Windows.h>
#include <Psapi.h>
#include <dxgi.h>
#include <wrl.h>
#elif defined(Q_OS_LINUX)
#include <iostream>
#include <fstream>
#include <sstream>
#include <QApplication>
#endif

#define MemoryTimerInterval 1000

namespace GUI
{
     FITKRAMProportionBar::FITKRAMProportionBar(QWidget *parent) : 
      FITKWidget(parent), _ui(new Ui::FITKRAMProportionBar())
    {
        _ui->setupUi(this);
       // setWindowFlags(Qt::FramelessWindowHint);
        _timerMem = new QTimer(this);
        //初始化
        this->init();
    }

    FITKRAMProportionBar::~FITKRAMProportionBar()
    {
        if (_ui) delete _ui;
        if( _timerMem ) delete _timerMem;
    }

    void FITKRAMProportionBar::init()
    {
        // Set the fixed size.
        setMaximumWidth(900);
        //初始化
        _timerMem->setInterval(MemoryTimerInterval);
        connect(_timerMem, &QTimer::timeout, this, &FITKRAMProportionBar::slot_memoryTimerOut);
        _timerMem->start();
    }

#ifdef Q_OS_WIN
    void FITKRAMProportionBar::getSystemMemoryKBs(double& max, double& cost, double& currProcess)
    {
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        max = statex.ullTotalPhys;
        double avi = statex.ullAvailPhys;
        cost = max - avi;

        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        currProcess = pmc.WorkingSetSize;

        max /= 1024.;
        cost /= 1024.;
        currProcess /= 1024.;
    }

#elif defined(Q_OS_LINUX)
    void FITKRAMProportionBar::getSystemMemoryKBs(double& max, double& cost, double& currProcess)
    {
        std::ifstream memInfoFile("/proc/meminfo");
        std::string line;
        std::string key;
        unsigned long value;

        long long freeMem = 0, buffers = 0, cached = 0;

        // Read system memory info from file.
        while (std::getline(memInfoFile, line))
        {
            std::stringstream ss(line);
            ss >> key >> value;
            if (key == "MemTotal:")
            {
                max = value;
            }
            else if (key == "MemFree:")
            {
                freeMem = value;
            }
            else if (key == "Buffers:")
            {
                buffers = value;
            }
            else if (key == "Cached:")
            {
                cached = value;
            }
        }

        cost = max - (freeMem + buffers + cached);

        // Read process memory info from file.
        std::ifstream statusFile("/proc/" + std::to_string(QApplication::applicationPid()) + "/status");
        while (std::getline(statusFile, line))
        {
            if (line.find("VmRSS") == 0)
            {
                std::stringstream ss(line);
                std::string key;
                ss >> key >> currProcess;
                break;
            }
        }
    }

#endif // Q_OS_WIN

    void FITKRAMProportionBar::memUnitTransKBs(double kbs, double & val, QString & unit)
    {
        //内存单位转换
        QStringList units{ "KB", "MB", "GB", "TB", "PB" };
        int unitLevel = 0;
        val = kbs;
        while (val >= 1024. && unitLevel < 4)
        {
            val /= 1024.;
            unitLevel++;
        }

        unit = units[unitLevel];
    }

    void FITKRAMProportionBar::showMemBar(bool isShow)
    {
        _ui->progressBar_Mem->setVisible(isShow);
    }
    void FITKRAMProportionBar::showRAMLabel(bool isShow)
    {
        _ui->label_RAM->setVisible(isShow);
    }

    void FITKRAMProportionBar::slot_memoryTimerOut()
    {
        //获取当前系统内存
        double max = 0, cost = 0, currCost = 0;
        getSystemMemoryKBs(max, cost, currCost);
        cost = qMin(cost, max);
        //转换内存单位
        QString unitMax, unitCost, unitCurrCost;
        double maxTrans = 0., costTrans = 0., currCostTrans = 0.;
        memUnitTransKBs(max, maxTrans, unitMax);
        memUnitTransKBs(cost, costTrans, unitCost);
        memUnitTransKBs(currCost, currCostTrans, unitCurrCost);
        //获取系统当前总运行内存占用比
        double per = cost * 100. / max;
        // Get the computer name.
        QString deviceName = QSysInfo::machineHostName();
        // Get the user name.
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        //获取计算机的用户名称
        QString userName;
#ifdef Q_OS_WIN
        userName = env.value("USERNAME");
#elif defined(Q_OS_LINUX)
        userName = env.value("USER");
#endif
        //获取当前程序数据
        _ui->label_RAM->setText(QString("%1 %2 %3").arg(tr("Program RAM:")).arg(QString::number(currCostTrans, 'f', 2)).arg(unitCurrCost));
        //获取内存占用数据
        QString text = QString("%1(%2) : ").arg(deviceName).arg(userName);
        text += tr("%1 %2 / %3 %4 %p%")
            .arg(QString::number(costTrans, 'f', 2)).arg(unitCost)
            .arg(QString::number(maxTrans, 'f', 2)).arg(unitMax);
        _ui->progressBar_Mem->setFormat(text);
        _ui->progressBar_Mem->setValue(std::ceil(per));

        QColor colorText, colorBar;
        if (per < 75.)
        {
            colorText = Qt::black;
            colorBar = QColor(200, 255, 200);
        }
        else if (per >= 75. && per < 90.)
        {
            colorText = Qt::white;
            colorBar = QColor(255, 150, 50);
        }
        else if (per >= 90.)
        {
            colorText = Qt::white;
            colorBar = QColor(255, 100, 100);
        }

        _ui->progressBar_Mem->setStyleSheet(QString
        ("QProgressBar{color:rgb(%1,%2,%3);border:1px solid gray;;border-radius:1px;}"
            "QProgressBar::chunk{background-color:rgb(%4,%5,%6);}")
            .arg(colorText.red()).arg(colorText.green()).arg(colorText.blue())
            .arg(colorBar.red()).arg(colorBar.green()).arg(colorBar.blue()));
    }
}


