/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperFaceHidden.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Operator
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIDialog/FaceHiddenDialog.h"

namespace ModelOper
{
    bool OperFaceHidden::execGUI()
    {
        // 先退出草图模式。
        exitSketchMode();

        // 显示对话框。
        GUI::FaceHiddenDialog* dlg = GUI::FaceHiddenDialog::New(m_MainWindow, this);
        dlg->show();

        //switch (m_OperType)
        //{
        //case Create:
        //{
        //    break;
        //}
        //case Edit:
        //{
        //    break;
        //}
        //case Delete:
        //{
        //    break;
        //}
        //case Show:
        //{
        //    break;
        //}
        //case Hide:
        //{
        //    break;
        //}
        //default:
        //    return false;
        //}

        return false;
    }

    bool OperFaceHidden::execProfession()
    {
        // 此操作器无需其他业务逻辑。
        return true;
    }

    void OperFaceHidden::finalize(bool updateCommand)
    {
        // 此操作器无需更新几何命令。
        Q_UNUSED(updateCommand);
    }
} // namespace ModelOper