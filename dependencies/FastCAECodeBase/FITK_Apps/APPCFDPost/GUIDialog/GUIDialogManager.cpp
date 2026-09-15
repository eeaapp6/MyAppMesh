/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#include "GUIDialogManager.h"
#include "GUIDialogBase.h"

namespace GUI
{
    // GUIDialogManager的单例实例初始化为nullptr
    GUIDialogManager *GUIDialogManager::_instance = nullptr;

    // 当对话框创建时调用此函数，将其添加到管理列表中
    void GUIDialogManager::dialogCreated(GUIDialogBase *dlg, QString uniqueFlag)
    {
        if (_dialogList.contains(dlg))
            return;
        _dialogList.append(dlg);

        if (uniqueFlag.isEmpty()) return;
        if (!_dialogHash.contains(uniqueFlag)) {
            _dialogHash.insert(uniqueFlag, nullptr);
        }
        auto dialog = _dialogHash.value(uniqueFlag, nullptr);

        if (dialog != nullptr) {
            dialog->setAttribute(Qt::WA_DeleteOnClose, false);
            dialog->close();
            delete dialog;
            dialog = nullptr;
        }
        _dialogHash[uniqueFlag] = dlg;
    }

    // 当对话框销毁时调用此函数，将其从管理列表中移除
    void GUIDialogManager::dialogDestoried(GUIDialogBase *dlg)
    {
        if (!_dialogList.contains(dlg))
            return;
        _dialogList.removeOne(dlg);
        // 删除对话框的时候将hash中的value设置nullptr
        auto it = _dialogHash.constBegin();
        while (it != _dialogHash.constEnd()) {
            if (it.value() == dlg) {
                _dialogHash[it.key()] = nullptr;
            }
            ++it;
        }
    }

    // 获取当前所有存活的对话框列表
    QList<GUIDialogBase *> GUIDialogManager::getAllDialogs()
    {
        return _dialogList;
    }
} // namespace GUI
