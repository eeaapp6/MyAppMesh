/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIDialogManager.h
 * @brief  对话框管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-22
 *********************************************************************/
#ifndef __GUIDIALOGMANAGER_H__
#define __GUIDIALOGMANAGER_H__

#include "GUIDialogAPI.h"
#include <QList>
#include <QHash>
#include <QMessageBox>

namespace GUI
{
    class GUIDialogBase;
    class GUIDialogManagerDeletor;
    /**
     * @brief 对话框管理器，对话框的全生命周期管理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-06-09
     */
    class GUIDialogAPI GUIDialogManager
    {
    public:
        /**
         * @brief 获取Instance的值
         * @return GUIDialogManager* 实例
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        static GUIDialogManager *getInstance()
        {
            if (_instance == nullptr)
                _instance = new GUIDialogManager;
            return _instance;
        }
        /**
         * @brief  处理对话框创建时的逻辑
         * @param  dlg 对话框
         * @param  uniqueFlag 该对话框类型的唯一标记（用于判断是否已经有同类型对话框打开）
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-05-17
         */
        void dialogCreated(GUIDialogBase *dlg, QString uniqueFlag = QString());
        /**
         * @brief 对话框被关闭，对象被释放
         * @param  dlg         被销毁的对话框
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        void dialogDestoried(GUIDialogBase *dlg);
        /**
         * @brief 获取对话框对象指针
         * @tparam T 对话框类型，GUI::GUIDialogBase的子类
         * @return QList<T*>  对象列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        template <typename T>
        QList<T *> getDialog()
        {
            QList<T *> ds;
            for (auto d : _dialogList)
            {
                T *dt = dynamic_cast<T *>(d);
                if (dt != nullptr)
                    ds.append(dt);
            }
            return ds;
        }
    private:
        /**
         * @brief 单例对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        static GUIDialogManager *_instance;
        /**
         * @brief 构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        GUIDialogManager() {}
        /**
         * @brief 析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        ~GUIDialogManager() = default;
        /**
         * @brief 获取全部对话框对象
         * @return QList<GUIDialogBase*>  全部对话框列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        QList<GUIDialogBase *> getAllDialogs();

    private:
        /**
         * @brief 在生命周期内的对话框列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-06-09
         */
        QList<GUIDialogBase *> _dialogList{};
        /**
         * @brief  只能打开一次的对话框
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-05-17
         */
        QHash<QString, GUIDialogBase *> _dialogHash{};
    };

} // namespace GUI

#endif