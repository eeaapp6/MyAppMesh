/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file PHengLEIDiscretisationWidget.h
 * @brief 离散参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _PHengLEIDiscretisationWidget_H
#define _PHengLEIDiscretisationWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class PHengLEIDiscretisationWidget;
}

namespace Core {
	class FITKParameter;
}

namespace Interface {
    class FITKPHengLEIDiscretisation;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief 离散参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI PHengLEIDiscretisationWidget :public GUICalculateWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Solution Widget object
         * @param[i]  oper           操作器对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        PHengLEIDiscretisationWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Solution Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~PHengLEIDiscretisationWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	public slots:
		void VenkatCoeffWidgetChanged();
    protected:
        /**
         * @brief 显示事件重写
         * @param[i]  event          事件对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-16
         */
        //void showEvent(QShowEvent *event);
        ///**
        // * @brief 界面大小变化事件重写
        // * @param[i]  event          事件对象
        // * @author BaGuijun (baguijun@163.com)
        // * @date 2024-08-16
        // */
        //void resizeEvent(QResizeEvent *event);
    private:
        void updateSpatial();
        void updateTemporal();
        void updateFlow();
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::PHengLEIDiscretisationWidget* _ui = nullptr;
        /**
         * @brief  离散参数对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-23
         */
        Interface::FITKPHengLEIDiscretisation* _solValue = nullptr;

		QList<QMetaObject::Connection> _senders = {};
    };
}
#endif
