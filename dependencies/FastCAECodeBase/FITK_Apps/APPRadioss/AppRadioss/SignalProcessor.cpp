/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SignalProcessor.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "GUIFrame/MainWindow.h"

void SignalProcessor::on_sendProgramDriverMessageSig(AppFrame::FITKAbstractProgramerDriver* driver, int messageType, const QString& message)
{
    //显示信息
    AppFrame::FITKMessageInfo(message);
}

void SignalProcessor::on_sendProcessSig(QObject* sender, int process)
{
    //获取主页面
    GUI::MainWindow* mw = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
    if (mw == nullptr) return;
    //设置进度值
    mw->setProgressValue(process);
}

