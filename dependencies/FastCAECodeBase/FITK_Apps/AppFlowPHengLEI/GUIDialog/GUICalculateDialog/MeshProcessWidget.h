/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file MeshProcessWidget.h
 * @brief 求解器传输界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _MeshProcessWidget_H
#define _MeshProcessWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"
#include <qobjectdefs.h>

namespace Ui {
    class MeshProcessWidget;
}

namespace Interface {
	class FITKPHengLEIMeshProcess;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief 求解器传输界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI MeshProcessWidget :public GUICalculateWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Transport Widget object
         * @param[i]  oper           操作器
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        MeshProcessWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~MeshProcessWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	private slots:
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::MeshProcessWidget* _ui = nullptr;
        /**
         * @brief  通用控制参数数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEIMeshProcess * _meshProcess = nullptr;

		QList<QMetaObject::Connection> _senders = {};
    };
}
#endif
