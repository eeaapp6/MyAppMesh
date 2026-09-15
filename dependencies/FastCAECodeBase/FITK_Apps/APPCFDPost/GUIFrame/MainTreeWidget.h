/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _MainTreeWidget_H
#define _MainTreeWidget_H

#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace GUI
{
    class TreeWidget;

    class GUIFRAMEAPI MainTreeWidget : public Core::FITKWidget
    {
        Q_OBJECT;
    public:
        MainTreeWidget(QWidget* parent = nullptr);
        ~MainTreeWidget();
        /**
         * @brief    获取树界面对象
         * @return   TreeWidget*   树界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        TreeWidget* getTreeWidget();
    private:
        /**
         * @brief  树界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-04
         */
        TreeWidget* _treeWidget = nullptr;
    };
}

#endif