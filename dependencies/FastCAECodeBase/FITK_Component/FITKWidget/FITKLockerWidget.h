/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief 抽屉组件对象
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-05
 */
#ifndef LOCKERWIDGET_H
#define LOCKERWIDGET_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "FITKWidgetAPI.h"

class QVBoxLayout;

namespace Comp
{
    class FITKDrawerWidget;
    class FITKLockerButton;
    /**
     * @brief 抽屉组件对象
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-05
     */
    class FITKWidgetAPI FITKLockerWidget :public Core::FITKWidget
    {
        Q_OBJECT;
    public:
       /**
        * @brief Construct a new FITKLockerWidget object
        * @param[i]  id             组件ID
        * @param[i]  parent         抽屉控件对象
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-06-05
        */
        FITKLockerWidget(int id, FITKDrawerWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKLockerWidget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        virtual ~FITKLockerWidget();
        /**
         * @brief 设置组件标题
         * @param[i]  text           标题文本
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void setText(const QString& text);
        /**
         * @brief 组件中添加业务界面和空白区域
         * @param[i]  widget         组件界面
         * @param[i]  isStretch      是否添加空白区域：true-是，false-否
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void addWidget(QWidget* widget, bool isStretch = false);
        /**
         * @brief 获取组件ID
         * @return int 组件ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        int getID();
        /**
         * @brief 设置组件的折叠状态
         * @param[i]  fold           true-折叠，false-展开
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void setFoldable(bool fold);
        /**
         * @brief 获取组件的折叠状态
         * @return true 折叠
         * @return false 展开
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        bool getFoldable();
        /**
         * @brief 设置组件在折叠下的图标
         * @param[i]  pixmap         图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void setFoldPixmap(const QPixmap& foldImage);
        /**
         * @brief 设置组件在展开下的图标
         * @param[i]  pixmap         图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void setUnFoldPixmap(const QPixmap& unFoldImage);
    signals:
        ;
        /**
         * @brief 折叠组件信号
         * @param[i]  id             组件ID
         * @param[i]  fold           true-折叠，false-展开
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void foldWidgetSig(int id, bool fold);
    private:
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void initialize();
    private:
        /**
         * @brief 折叠状态
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        bool m_fold;
        /**
         * @brief 组件id
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        int m_id = -1;
        /**
         * @brief 折叠图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        QPixmap m_foldPixmap;
        /**
         * @brief 展开图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        QPixmap m_unfoldPixmap;
        /**
         * @brief 抽屉组件对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        QWidget* m_widget = nullptr;
        /**
         * @brief 垂直布局
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        QVBoxLayout* m_widgetLayout = nullptr;
        /**
         * @brief 按钮组件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        FITKLockerButton* m_button = nullptr;
    };
}
#endif // LOCKERWIDGET_H
