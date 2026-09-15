/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ToolBarEvent.h
 * @brief  事件工具条
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _ToolBarEvent_H
#define _ToolBarEvent_H

#include "GUIWidgetAPI.h"
#include "ToolBarBase.h"
#include <QComboBox>

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    class GUIComboBox;
    class GUIWidgetAPI ToolBarEvent : public ToolBarBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent 父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        ToolBarEvent(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        ~ToolBarEvent();
        /**
         * @brief    根据当前渲染对象修改颜色映射
         * @param[i] objID    渲染对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual void updateCurrentGraphObj(int objID) override;
    private:
        /**
         * @brief  极值按钮
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-14
         */
        QAction* _extremumAction = nullptr;
    };
}
#endif