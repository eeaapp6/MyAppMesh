/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ABSTRACT_GUI_DIALOG_H___
#define _FITK_ABSTRACT_GUI_DIALOG_H___

#include "FITKAbstractGUIObject.h"
#include <QDialog>
#include <QVariant>

namespace Core
{
    /**
 * @brief GUI Dialog 抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 */
    class FITKCoreAPI FITKDialog : public QDialog, public FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 使用全部基类构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        FITKDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
   
        virtual ~FITKDialog() = 0;
        /**
         * @brief 对话框隐藏事件
         * @param[i]  event          隐藏事件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        virtual void hideEvent(QHideEvent *event) override;
        /**
         * @brief 对话框显示事件
         * @param[i]  event          显示事件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        virtual void showEvent(QShowEvent *event) override;
        /**
         * @brief 对话框移动事件
         * @param[i]  event          移动事件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        virtual void moveEvent(QMoveEvent *event) override;
        /**
         * @brief 事件处理器
         * @param[i]  watched       事件发出对象
         * @param[i]  event          事件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        virtual bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        /**
         * @brief 对话框位置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        QPoint _dlgLocation;
        /**
         * @brief 是否使用对话框的位置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-25
         */
        bool _locationUpdated{ false };
    };

    struct RevokeableValue 
    {
        RevokeableValue(QObject* obj, QVariant va) { _guiObj = obj; _value = va; }

        QObject* _guiObj{ nullptr };
        QVariant _value{};
    };

    class FITKCoreAPI FITKDialogRevokeable : public FITKDialog
    {
        Q_OBJECT
    public:
        using FITKDialog::FITKDialog;
        virtual ~FITKDialogRevokeable() = 0;

        void undo();
        void redo();

    protected:
        void initRevokeable();

        void appendValue(QObject* obj, const QVariant & va);
        
        RevokeableValue getPreviousOne();
        RevokeableValue getNextOne();


    protected slots:
        void recoredRevokeableValue();

    private:
        bool coudleBeAppend(QObject* obj, const QVariant& va);

        void setObjectValue(QObject* obj, const QVariant& va);
    
    protected:
        int _currentIndex{ -1 };
        QList<RevokeableValue> _revokeableValues{};



    };

}


#endif
