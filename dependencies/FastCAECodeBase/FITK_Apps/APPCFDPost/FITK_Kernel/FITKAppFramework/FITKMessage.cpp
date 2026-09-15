/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMessage.h"
#include "FITKAppFramework.h"
#include "FITKSignalTransfer.h"

void outputMessage(int type, const QString & m)
{
    //获取信号转接器
    auto signalTrans = FITKAPP->getSignalTransfer();
    if (signalTrans == nullptr) return;
    //发送信息
    emit signalTrans->outputMessageSig(type, m);
}

namespace AppFrame
{
    void FITKMessageNormal(const QString& message)
    {
        //正常信息
        outputMessage(1, message);
    }

    void FITKMessageWarning(const QString& message)
    {
        //警告信息
        outputMessage(2, message);
    }

    void FITKMessageError(const QString& message)
    {
        //错误信息
        outputMessage(3, message);
    }

    void FITKMessageInfo(const QString& message)
    {
        outputMessage(4, message);
    }

}



