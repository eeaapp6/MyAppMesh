/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruPostMessageText.h"


namespace Interface
{
    FITKStruPostMessageText::FITKStruPostMessageText()
    {

    }

    FITKStruPostMessageText::~FITKStruPostMessageText()
    {

    }

    void FITKStruPostMessageText::setText(const QString & text)
    {
        _text = text;
        emit this->sigUpdate();
    }

    QString FITKStruPostMessageText::getText()
    {
        return _text;
    }

    QColor FITKStruPostMessageText::getColor()
    {
        return _color;
    }

    void FITKStruPostMessageText::setColor(QColor & color)
    {
        _color = color;
        emit this->sigUpdate();
    }

    void FITKStruPostMessageText::setPos(double * pos)
    {
        _position[0] = pos[0];
        _position[1] = pos[1];
        emit this->sigUpdate();
    }

    void FITKStruPostMessageText::getPos(double * pos)
    {
        pos[0] = _position[0];
        pos[1] = _position[1];
    }

    bool FITKStruPostMessageText::getIsEnable()
    {
        return _isEnable;
    }

    void FITKStruPostMessageText::setIsEnable(bool isEnable)
    {
        _isEnable = isEnable;
        emit this->sigUpdate();
    }
    int FITKStruPostMessageText::getTextSize()
    {
        return _size;
    }

    void FITKStruPostMessageText::setTextSize(int size)
    {
        _size = size;
        emit this->sigUpdate();
    }
}
