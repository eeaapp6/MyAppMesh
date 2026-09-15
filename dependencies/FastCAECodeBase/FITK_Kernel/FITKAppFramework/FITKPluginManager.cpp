/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPluginManager.h"
#include <QLibrary>
#include <QFile>
#include "FITKAbstractPlugin.h"
#include "FITKMessage.h"
#include "FITKAppFramework.h"
#include "FITKAppSettings.h"

namespace AppFrame
{
   

    FITKPluginsManager::~FITKPluginsManager()
    {
        while (!_pluginList.isEmpty())
        {
            //不断移除第0个，直到全部移除
            this->uninstallLib(0);
        }
    }

    void FITKPluginsManager::write2Setting()
    {
        FITKAppSettings* s = FITKAPP->getAppSettings();
        if (s == nullptr) return;
        QStringList ps;
        for (FITKAbstractPlugin* p : _pluginList)
        {
            if(p == nullptr) continue;
            ps << p->getLibraryPath();
        }
        s->setPlugins(ps);
    }

    void FITKPluginsManager::loadFromSetting()
    {
        //从配置文件获取
        FITKAppSettings* s = FITKAPP->getAppSettings();
        if (s == nullptr) return;
        QStringList ps = s->getPlugins();
        for (QString p : ps)
        {
            //安装动态库
            this->installLibrary(p);
        }

    }

    bool FITKPluginsManager::installLibrary(const QString& libPath)
    {
        //文件不存在
        if (!QFile::exists(libPath)) return false;

        //动态库已经被加载
        if (this->getPluginByPath(libPath) != nullptr)
        {
            AppFrame::FITKMessageError(QObject::tr("Library already has been installed !"));
            return false;
        }

        //关键字正确性验证
        if (_pluginKey.isEmpty())
        {
            AppFrame::FITKMessageError(QObject::tr("Plugin key is invalid !") );
            return false;
        }
        QLibrary* alib = new QLibrary(libPath);
        bool ok = alib->load();
        //加载动态库失败
        if (!ok)
        {
            AppFrame::FITKMessageError(alib->errorString()) ;
            return false;
        }
        if (!alib->isLoaded()) return false;

        //识别插件
       pluginRecognizeFun rf = (pluginRecognizeFun)alib->resolve(FITKLibraryRecognizeFunName);
       if (rf == nullptr)
       {
           //卸载，并输出错误信息
           FITKMessageError(QString(QObject::tr("Unable to find the C function \"QString %1()\""))
               .arg(FITKLibraryRecognizeFunName));
           alib->unload();
           delete alib;
           return false;

       }
       //关键字识别
       if (_pluginKey != rf())
       {
           //卸载，并输出错误信息
           FITKMessageError(QString(QObject::tr("Unable to match key !")));
           alib->unload();
           delete alib;
           return false;

       }
        //创建插件
        pluginCreateFun f = (pluginCreateFun)alib->resolve(FITKLibraryLoadFunName);
        if (f == nullptr)
        {
            //卸载，并输出错误信息
            FITKMessageError(QString(QObject::tr("Unable to find the C function \"AppFrame::FITKAbstractPlugin* %1(QLibrary*)\""))
                .arg(FITKLibraryLoadFunName));
            alib->unload();
            delete alib;
            return false;
        }
            
        //创建对象
        FITKAbstractPlugin* p = f(alib);
        if (p == nullptr)
        {
            
            FITKMessageError(QString(QObject::tr("Create Plugin Object failed !")));
            alib->unload();
            delete alib;
            return false;
        }
        _pluginList.append(p);
        p->setLibraryPath(libPath);
        //安装
        p->install();
        return true;
    }

    void FITKPluginsManager::uninstallLib(FITKAbstractPlugin* p)
    {
        //执行卸载
        if (p == nullptr) return;
        p->unInstall();
        //库卸载
        QLibrary* lib = p->getLibrary();
        //移除插件
        _pluginList.removeAll(p);
        delete p;
        //必须先移除插件然后再卸载动态库，否则会找不到插件对象的析构函数
        if (lib == nullptr) return;
 //       lib->unload();
        delete lib;
    }

    void FITKPluginsManager::uninstallLib(const int & index)
    {
        //获取插件对象
        AppFrame::FITKAbstractPlugin* mgrPlugin = this->getPlugin(index);
        if (mgrPlugin == nullptr) return;
        this->uninstallLib(mgrPlugin);
    }

    void FITKPluginsManager::uninstallLibByPath(const QString& path)
    {
        if (path.isEmpty()) return;

        for (FITKAbstractPlugin* p : _pluginList)
        {
            if(p == nullptr) continue;
            //判断动态库文件路径
            if (p->getLibraryPath() == path)
            {
                this->uninstallLib(p);
                return;
            }
        }
    }

    void FITKPluginsManager::uninstallLibByName(const QString& name)
    {
        if (name.isEmpty()) return;

        for (FITKAbstractPlugin* p : _pluginList)
        {
            if (p == nullptr) continue;
            //判断插件名称
            if (p->getPluginName() == name)
            {
                this->uninstallLib(p);
                return;
            }
        }
    }

    int FITKPluginsManager::getLibraryCount()
    {
        return _pluginList.count();
    }

    FITKAbstractPlugin* FITKPluginsManager::getPlugin(const int & index)
    {
        if (index < 0 || index >= _pluginList.size()) return nullptr;
        return _pluginList.at(index);
    }

    AppFrame::FITKAbstractPlugin* FITKPluginsManager::getPluginByName(const QString& name)
    {
        if (name.isEmpty()) return nullptr;

        for (FITKAbstractPlugin* p : _pluginList)
        {
            if (p == nullptr) continue;
            //判断插件名称
            if (p->getPluginName() == name)
            {
                return p; 
            }
        }
        return nullptr;
    }

    AppFrame::FITKAbstractPlugin* FITKPluginsManager::getPluginByPath(const QString& path)
    {
        if (path.isEmpty()) return nullptr;

        for (FITKAbstractPlugin* p : _pluginList)
        {
            if (p == nullptr) continue;
            //判断动态库文件路径
            if (p->getLibraryPath() == path)
            {
                return p;
            }
        }
        return nullptr;
    }

    void FITKPluginsManager::setPluginKey(const QString& k)
    {
        _pluginKey = k;
    }

    bool FITKPluginsManager::read(void *filePointer, QStringList &error, int fileType)
    {
        for (FITKAbstractPlugin* p : _pluginList)
        {
            QString err;
            if (p == nullptr) continue;
            //读取插件
            if (!p->read(filePointer, err, fileType))
            {
                error.append(err);
            }
        }
        return error.isEmpty();
    }
    bool FITKPluginsManager::write(void *filePointer, QStringList &error, int fileType)
    {
        for (FITKAbstractPlugin* p : _pluginList)
        {
            QString err;
            if (p == nullptr) continue;
            //写入插件
            if (!p->write(filePointer, err, fileType))
            {
                error.append(err);
            }
        }
        return error.isEmpty();
    }

}