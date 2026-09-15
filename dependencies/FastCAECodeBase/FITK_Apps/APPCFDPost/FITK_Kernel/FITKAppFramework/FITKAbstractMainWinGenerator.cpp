/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMainWinGenerator.h"
#include "FITKGlobalData.h"
#include "FITKAppFramework.h"
#include "FITKAppSettings.h"
#include <QFile>
#include <QApplication>
#include <QTranslator>
#include <QAbstractNativeEventFilter>
#include <QDebug>

#ifdef Q_OS_WIN
#include "windows.h"


class WinMaxFix : public QAbstractNativeEventFilter 
{
public:
    bool nativeEventFilter(const QByteArray& type, void* message, long* result) override
    {
        if (type != "windows_generic_MSG") return false;
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_GETMINMAXINFO) {
            auto* mmi = reinterpret_cast<MINMAXINFO*>(msg->lParam);
            HMONITOR mon = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi{ sizeof(MONITORINFO) };
            if (GetMonitorInfo(mon, &mi)) {
                RECT rcWork = mi.rcWork;
                RECT rcMonitor = mi.rcMonitor;
                mmi->ptMaxPosition.x = qAbs(rcWork.left - rcMonitor.left);
                mmi->ptMaxPosition.y = qAbs(rcWork.top - rcMonitor.top);
                mmi->ptMaxSize.x = qAbs(rcWork.right - rcWork.left);
                mmi->ptMaxSize.y = qAbs(rcWork.bottom - rcWork.top);
                *result = 0;
                return true;
            }
        }
        return false;
    }
};


#endif



namespace AppFrame
{

    FITKAbstractMainwindowGenerator::FITKAbstractMainwindowGenerator()
    {
#ifdef Q_OS_WIN
        FITKAPP->installNativeEventFilter(new WinMaxFix);
#endif
    }

    void FITKAbstractMainwindowGenerator::EnableHighDPIScaling()
    {
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);
    }

    bool FITKAbstractMainwindowGenerator::loadQssStyle()
    {
        //打开qss文件
        QString styleFile = this->getQssStyle();
        if (styleFile.isEmpty()) return false;
        QFile qssFile(styleFile);
        if (!qssFile.open(QFile::ReadOnly)) return false;
        //读取qss
        const QString q = qssFile.readAll();
        qssFile.close();
        //加载qss
        qApp->setStyleSheet(q);
        return true;
    }

    bool FITKAbstractMainwindowGenerator::loadTranslatorQmFiles()
    {
        FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings != nullptr)
        {
            bool loadZHCN = settings ->isLoadZHCN();
            if (!loadZHCN) return false;
        }

        //获取全部翻译文件
        QStringList qmFiles = this->getTranslatorQmFiles();
        for (QString qmfile : qmFiles)
        {
            //遍历加载翻译文件
            if(qmfile.isEmpty()) continue;
            QTranslator* translator = new QTranslator(this);
            bool ok = translator->load(qmfile);
            //加载失败则报错，释放translator
            //加载翻译
            if (ok)
            {
                qApp->installTranslator(translator);
            }
            
            
            if (!ok)
            {
                qDebug() << QString("Failed to load lang file: %1").arg(qmfile);
                delete translator;
                continue;
            }
            
        }
        return true;
    }

    QWidget* FITKAbstractMainwindowGenerator::genMainWindow()
    {
        //虚函数错误值
        return nullptr;
    }
        
    void FITKAbstractMainwindowGenerator::showMaximize(bool max)
    {
        //最大化
        _showMaximize = max;
    }

    bool FITKAbstractMainwindowGenerator::showMaximize()
    {
        return _showMaximize;
    }

    void FITKAbstractMainwindowGenerator::showWelcomePage()
    {

    }

    bool FITKAbstractMainwindowGenerator::showPreSettingPage()
    {
        return true;
    }

   
    QString FITKAbstractMainwindowGenerator::getQssStyle()
    {
        return QString();
    }

    QStringList FITKAbstractMainwindowGenerator::getTranslatorQmFiles()
    {
        return QStringList();
    }


}
