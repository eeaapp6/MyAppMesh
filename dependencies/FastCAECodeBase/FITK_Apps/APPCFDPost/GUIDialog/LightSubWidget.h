/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   LightSubWidget.h
 * @brief  灯光界面子界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-17
 *********************************************************************/
#ifndef _LightSubWidget_H
#define _LightSubWidget_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIWidgetBase.h"

namespace Ui {
    class LightSubWidget;
}

namespace Interface {
    class FITKCFDPostLight;
    class PostGraphWidgetLine;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief  灯光界面子界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-17
     */
    class GUIDialogAPI LightSubWidget : public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] currentDataID     数据id 
         * @param[i] parent            父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        LightSubWidget(int currentDataID, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        ~LightSubWidget();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void init();
        /**
         * @brief    设置界面标题
         * @param[i] title      标题
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void setWidgetTitle(QString title);
    protected:
        /**
         * @brief    显示事件
         * @param[i] event    事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void showEvent(QShowEvent *event);
        /**
         * @brief    关闭事件
         * @param[i] event   事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void closeEvent(QCloseEvent *event);
    private slots:
        ;
        /**
         * @brief    移除事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void on_pushButton_Remove_clicked();
        /**
         * @brief    灯光类型切换事件
         * @param[i] index    类型索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void on_comboBox_Type_currentIndexChanged(int index);
        /**
         * @brief    颜色选择事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void on_pushButton_SelectColor_clicked();
        /**
         * @brief    数据修改事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void slotValueChange();
        /**
         * @brief    VTK交互界面更改事件
         * @param[i] startPoint 
         * @param[i] endPoint 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void slotVTKLineWidgetValueChange(double* startPoint, double* endPoint);
    private:
        /**
         * @brief    将数据填充至界面
         * @param[i] data         数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void setDataToWidget(Interface::FITKCFDPostLight* data);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        Ui::LightSubWidget* _ui = nullptr;

        //Interface::PostGraphWidgetLine* _lineWidget = nullptr;
    };
}
#endif
