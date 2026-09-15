/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief 自定义抽屉控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-05
 */
#ifndef DRAWERWIDGET_H
#define DRAWERWIDGET_H

#include "FITKWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include <QList>

namespace Ui {
    class FITKDrawerWidget;
}

namespace Core {
    class FITKWidget;
}

namespace Comp 
{
    class FITKLockerWidget;
    /**
     * @brief 自定义抽屉控件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-05
     */
    class FITKWidgetAPI FITKDrawerWidget : public Core::FITKWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKDrawerWidget object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        explicit FITKDrawerWidget(QWidget *parent = nullptr);
        /**
         * @brief Destroy the FITKDrawerWidget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        virtual ~FITKDrawerWidget();
        /**
         * @brief 设置是否折叠所有locker
         * @param[i]  fold           My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        void setAllItemFold(bool fold);
        /**
         * @brief 设置抽屉组件是否全部展开
         * @param[i]  expand         是否展开
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void setItemExpand(bool expand);
        /**
         * @brief 获取抽屉组件是否全部展开
         * @return true 全部展开
         * @return false 全部折叠
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        bool getItemExpand();
        /**
         * @brief 添加新的抽屉组件
         * @param[i]  widget         业务界面
         * @param[i]  title          组件标题
         * @param[i]  isStretch      是否添加空白区域
         * @int                      界面的id
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        virtual int addWidget(QWidget* widget, QString title, bool isStretch = false);
        /**
        * @brief 获取当前的抽屉组件
        * @param id 组件id
        * @return FITKLockerWidget* 抽屉组件
        * @since 1.0.0
        */
        FITKLockerWidget* getLockerWidgetByID(int id);
        /**
         * @brief 设置所有locker的折叠与非折叠的图标
         * @param[i]  foldImage      折叠图标
         * @param[i]  unFoldImage    展开图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        void setLockerWidgetIcon(const QPixmap& foldImage, const QPixmap& unFoldImage);

        /**
         * @brief 展开所有抽屉组件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2026-04-24
         */
        void expandAllWidget();
    protected:
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        virtual void initialize();
    private slots:
        ;
        /**
         * @brief 折叠组件
         * @param[i]  id             组件id
         * @param[i]  fold           是否折叠
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void foldWidget(int id, bool fold);
        /**
         * @brief 移除最底部组件的拉升区域（移除最底部组件的空白区域）
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        void removeLastCompStretch();
    private:
        /**
         * @brief 组件ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        static int m_widgetNum;
       /**
        * @brief 是否全部展开标记符
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-06-05
        */
        bool m_itemExpand =  false ;
        /**
         * @brief 抽屉界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        Ui::FITKDrawerWidget *m_ui = nullptr;
        /**
         * @brief 组件列表
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-05
         */
        QList<FITKLockerWidget*> m_LockerWidgetList = {};
        /**
         * @brief 所有locker是否折叠标记位
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        bool m_allItemFold{ false };
        /**
         * @brief 折叠图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QPixmap m_foldImage;
        /**
         * @brief 展开图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QPixmap m_unFoldImage;
    };
}

#endif // DRAWERWIDGET_H
