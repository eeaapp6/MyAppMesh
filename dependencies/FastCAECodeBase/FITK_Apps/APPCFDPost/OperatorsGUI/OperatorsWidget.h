/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsWidget.h
 * @brief  信息操作器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _OperatorsWidget_H__
#define _OperatorsWidget_H__

#include "OperatorsGUIAPI.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI {
    class MainWindow;
}

namespace Interface {
    class PostGraphObjectManager;
    class FITKCFDPost3DManager;
};

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace OperGUI
{
    /**
     * @brief  渲染操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class OperatorsGUIAPI OperatorsWidget : public EventOper::ParaWidgetInterfaceOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit OperatorsWidget();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        ~OperatorsWidget() = default;
        /**
         * @brief    当前数据对象更改操作器
         * @param[i] objID    数据对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual void CurrentObjectChange(int objID) override;
    private:
        /**
         * @brief  主窗口
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        GUI::MainWindow* _mainWin = nullptr;
        /**
         * @brief  渲染对象管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::PostGraphObjectManager* _postGraphObjectManager = nullptr;
        /**
         * @brief  渲染数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::FITKCFDPost3DManager* _postGraphDataManager = nullptr;
    };

    Register2FITKOperatorRepo(OperatorsWidget, OperatorsWidget);

}  // namespace GUIOper

#endif