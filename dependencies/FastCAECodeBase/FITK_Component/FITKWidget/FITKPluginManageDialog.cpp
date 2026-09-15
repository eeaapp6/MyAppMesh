/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPluginManageDialog.h"
#include "ui_FITKPluginManageDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractPlugin.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKPluginManager.h"

#include <QFileDialog>
#include <QDir>
namespace Plugins {

    FITKPluginManageDialog::FITKPluginManageDialog(const QString& pluginPath, QWidget *parent /*=nullptr*/)
        : _pluginsManager(FITKAPP->getPluginsManager()), _pluginPath(pluginPath), FITKDialog(parent)
    {
        _ui = new Ui::PluginManageDialog;
        _ui->setupUi(this);
        _ui->availableList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _ui->selectedList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        init();
    }

    FITKPluginManageDialog::~FITKPluginManageDialog()
    {
        if (_ui != nullptr)
            delete _ui;
    }

    void FITKPluginManageDialog::init()
    {
        //路径标准化
        _pluginPath = QDir::cleanPath(_pluginPath);
        // 获取已加载的插件数量
        int pluginCount = _pluginsManager->getLibraryCount();

        // 根据索引获取已加载的插件，并添加到selectedList中
        for (int i = 0; i < pluginCount; ++i)
        {
            AppFrame::FITKAbstractPlugin* plugin = _pluginsManager->getPlugin(i);
            if (plugin != nullptr)
            {
                QString path = plugin->getLibraryPath();
                QFileInfo fileInfo(path);
                QString name = fileInfo.fileName();
                QListWidgetItem* item = new QListWidgetItem(QIcon("://plugin.png"), name);

                _ui->selectedList->addItem(item);
            }
        }
        //WIN
        QString suffix = ".dll";
        //Linux
#if defined(Q_OS_LINUX)
        suffix = ".so";
#endif
        QDir dir(_pluginPath);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            QString plugpath = fileInfo.absoluteFilePath();
            QString pluginname = fileInfo.fileName();
            if (!pluginname.toLower().endsWith(suffix))
                continue;
            //排除已经装载的对象
            if (_ui->selectedList->findItems(pluginname, Qt::MatchExactly).size() > 0)
                continue;
            QListWidgetItem* item = new QListWidgetItem(QIcon("://plugin.png"), pluginname);
            _ui->availableList->addItem(item);
        }
    }

    void FITKPluginManageDialog::on_addButton_clicked()
    {
        //available 中被选中的对象
        QList<QListWidgetItem*> selectedItems = _ui->availableList->selectedItems();

        for (QListWidgetItem *item : selectedItems)
        {
            if (item == nullptr) continue;
            //从available中移除
            const int row = _ui->availableList->row(item);
            _ui->availableList->takeItem(row);
            //在selected中加入
            _ui->selectedList->addItem(item);

        }
    }

    void FITKPluginManageDialog::on_removeButton_clicked()
    {
        //已经装载 中被选中的对象
        QList<QListWidgetItem*> selectedItems = _ui->selectedList->selectedItems();

        for (QListWidgetItem *item : selectedItems)
        {
            if (item == nullptr) continue;
            //从selected中移除
            const int row = _ui->selectedList->row(item);
            _ui->selectedList->takeItem(row);
            //在available中加入
            _ui->availableList->addItem(item);
        }
    }

    void FITKPluginManageDialog::on_buttonBox_accepted()
    {
        QStringList selectedPlugins;
        for (int i = 0; i < _ui->selectedList->count(); ++i)
        {
            QListWidgetItem* item = _ui->selectedList->item(i);
            if (item == nullptr) continue;
            selectedPlugins << QString("%1/%2").arg(_pluginPath).arg(item->text());;
        }
        // 获取当前已加载的插件路径列表
        QStringList loadedPluginPaths;
        //Lambda函数
        auto updateLoadedPlugins = [&]()
        {
            loadedPluginPaths.clear();
            int pluginCount = _pluginsManager->getLibraryCount();
            for (int i = 0; i < pluginCount; ++i)
            {
                AppFrame::FITKAbstractPlugin* plugin = _pluginsManager->getPlugin(i);
                if (plugin == nullptr) continue;
                loadedPluginPaths << QDir::cleanPath(plugin->getLibraryPath());
            }
        };

        updateLoadedPlugins();
        //首先卸载已经加载，但是不在select中的插件
        for (const QString& installed : loadedPluginPaths)
        {
            if (selectedPlugins.contains(installed)) continue;
            _pluginsManager->uninstallLibByPath(installed);
        }
        //卸载后更新已经装载的插件
        updateLoadedPlugins();
        //加载在select中，但是没有被装载的插件
        for (const QString& selected : selectedPlugins)
        {
            //已将装载的contiune
            if (loadedPluginPaths.contains(selected)) continue;
            if (_pluginsManager->installLibrary(selected))
                loadedPluginPaths.append(selected);
        }
        this->accept();
    }

    void FITKPluginManageDialog::on_buttonBox_rejected()
    {
        this->reject();
    }
} // namespace Plugins