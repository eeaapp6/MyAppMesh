/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGUIDialog.h"
#include "FITKGUIRepo.h"
#include <QLineEdit>
#include <QEvent>
#include <QDebug>

namespace Core
{

    FITKDialog::FITKDialog(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
        :QDialog(parent,f)
    {
        this->installEventFilter(this);
    }
 

    /**
    * @brief Destroy the FITKDialog::FITKDialog object
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-03-02
    */
    FITKDialog::~FITKDialog()
    {
        this->removeEventFilter(this);
    }

    void FITKDialog::hideEvent(QHideEvent *event)
    {
        _dlgLocation = this->pos();
        _locationUpdated = true;
        QDialog::hideEvent(event);
    }

    void FITKDialog::showEvent(QShowEvent *event)
    {
        if (_locationUpdated)
            this->move(_dlgLocation);
        QDialog::showEvent(event);
    }

    void FITKDialog::moveEvent(QMoveEvent *event)
    {
        _dlgLocation = this->pos();
        _locationUpdated = true;
        QDialog::moveEvent(event);
    }

    bool FITKDialog::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched == nullptr || event == nullptr) return false;
        if (watched == this)
        {
            //设置激活对象
            if (QEvent::WindowActivate == event->type())
            {
                FITKGUIRepo::getInstance()->setActiveDialog(this->getGUIObjectID());
            }
        }
 
       return QDialog::eventFilter(watched, event);
    }

    FITKDialogRevokeable::~FITKDialogRevokeable()
    {
    }

    void FITKDialogRevokeable::undo()
    {
        RevokeableValue rv = getPreviousOne();
        this->setObjectValue(rv._guiObj, rv._value);
    }

    void FITKDialogRevokeable::redo()
    {
        RevokeableValue rv = getNextOne();
        this->setObjectValue(rv._guiObj, rv._value);
    }

    void FITKDialogRevokeable::initRevokeable()
    {
        QList<QLineEdit*> les = this->findChildren<QLineEdit*>();
        for (QLineEdit* le : les)
        {
            if(le == nullptr) continue;
            connect(le, SIGNAL(editingFinished()), this, SLOT(recoredRevokeableValue()));
            _revokeableValues.append(RevokeableValue(le, QVariant(le->text())));

        }
    }

    void FITKDialogRevokeable::appendValue(QObject* obj, const QVariant & va)
    {
        if (!this->coudleBeAppend(obj, va)) return;

        RevokeableValue v = _revokeableValues.last();
        //添加的内容与最后一个相同则不追加
        if (v._guiObj == obj && v._value == va) return;
        //追加对象
        _revokeableValues.append(RevokeableValue(obj, va));
        _currentIndex = _revokeableValues.size() - 1;
    }

    Core::RevokeableValue FITKDialogRevokeable::getPreviousOne()
    {
        const int index = _currentIndex - 1;
        if (index < 0 || index >= _revokeableValues.size()) 
            return Core::RevokeableValue(nullptr, QVariant());
        Core::RevokeableValue v = _revokeableValues[index];
        _currentIndex--;
        return v;
    }

    Core::RevokeableValue FITKDialogRevokeable::getNextOne()
    {
        const int index = _currentIndex + 1;

        if (index < 0 || index >= _revokeableValues.size()) 
            return Core::RevokeableValue(nullptr,QVariant());
        Core::RevokeableValue v = _revokeableValues[index];
        _currentIndex++;
        return v;
    }

    void FITKDialogRevokeable::recoredRevokeableValue()
    {
        QObject* sender = this->sender();
        if (sender == nullptr) return;
        QVariant va;
        QLineEdit* ledit = dynamic_cast<QLineEdit*>(sender);
        if (ledit)
        {
            va.setValue(ledit->text());
        }

        if (!va.isValid()) return;
        this->appendValue(sender, va);
    }

    bool FITKDialogRevokeable::coudleBeAppend(QObject* obj, const QVariant& va)
    {
        if (obj == nullptr) return false;
        const int n = _revokeableValues.size();
        for (int i = n-1; i>-1;i-- )
        {
            RevokeableValue v = _revokeableValues[i];
            if(v._guiObj != obj) continue;
            //与最后一个不一致则可以追加
            return v._value != va;
        }
        return true;
    }

    void FITKDialogRevokeable::setObjectValue(QObject* obj, const QVariant& va)
    {
        if (obj == nullptr || !va.isValid()) return;

        QLineEdit* le = dynamic_cast<QLineEdit*>(obj);
        if (le)
        {
            le->setText(va.toString());
            le->selectAll();
            qDebug() << obj;
            qDebug() << va;
            qDebug() << " ";
        }
    }

}
  