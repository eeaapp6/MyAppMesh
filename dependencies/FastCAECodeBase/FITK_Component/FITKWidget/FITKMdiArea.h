/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKMdiArea.h
 * @brief 多视口组件类
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-06
 * 
 */
#ifndef _FITKVports_H
#define _FITKVports_H

#include "FITKWidgetAPI.h"
#include <QMdiArea>
#include <QMutex>

class QMdiSubWindow;

namespace Comp
{
    /**
     * @brief 多视口布局方式
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-06
     */
    enum class FITKVportsLayoutType
    {
        CurrentMax,         ///< 当前窗口最大化
        Cascade,            ///< 层叠显示
        TileHorizontally,   ///< 水平平铺
        TileVertically,     ///< 垂直平铺
        Tab,                ///< Tab页显示
    };
    /**
     * @brief 多视口TabBar的位置
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-06
     */
    enum class FITKVportsTabPos
    {
        Top,                ///< 顶部
        Bottom,             ///< 底部
        Left,               ///< 左侧
        Right,              ///< 右侧
    };
    /**
     * @brief 多视口组件类
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-06
     */
    class FITKWidgetAPI FITKMdiArea :public QMdiArea
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKMdiArea object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        explicit FITKMdiArea(QWidget* parent);
        /**
         * @brief Destroy the FITKMdiArea object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        virtual ~FITKMdiArea();
        /**
         * @brief 添加界面(添加界面后需重新设置布局方式)
         * @param[i]  widgwt         界面
         * @param[i]  title          界面标题
         * @return int 界面在多视口组件中的ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        int addSubWidget(QWidget* widgwt, QString title);
        /**
         * @brief 设置多视口布局方式（需在添加完界面之后）
         * @param[i]  type           布局方式
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void setLayoutType(FITKVportsLayoutType type);
        /**
         * @brief Tab模式下设置tabbar的位置
         * @param[i]  position           tabbar方位
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void setTabPos(FITKVportsTabPos pos);
        /**
         * @brief Tab模式下设置tab bar 是否显示
         * @param[i]  show 是否显示
         * @author libaojun
         * @date 2025-12-31
         */
        void showTabBar(bool show);
        /**
         * @brief 获取界面
         * @param[i]  objectID             界面在多视口中的ID
         * @return QWidget 界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        QWidget* getWidget(int objectID);
        /**
         * @brief 获取界面
         * @param[i]  objectID       界面在多视口中的ID
         * @return QMdiSubWindow* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QMdiSubWindow* getSubWidget(int objectID);
        /**
         * @brief 获取界面
         * @param[i]  objectID             界面在多视口中的ID
         * @return T* 界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        template< class T>
        T* getWidget(int objectID)
        {
            return dynamic_cast<T*>(getWidget(objectID));
        }
        /**
         * @brief 移除界面
         * @param[i]  id             界面在多视口中的ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void removeWidget(int id);
        /**
         * @brief    通过QMdiSubWindow获取QWidget
         * @param[i] subWidget    QMdiSubWindow
         * @return   QWidget*     QWidget
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        QWidget* getWidget(QMdiSubWindow* subWidget);
        /**
         * @brief    通过QWidget获取QMidSubWidget
         * @param[i] widget             QWidget
         * @return   QMdiSubWindow*     QMidSubWidget
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        QMdiSubWindow* getSubWidget(QWidget* widget);
        /**
         * @brief 获取当前界面
         * @return QWidget 界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        QWidget* getCurrentWidget();
        /**
         * @brief 获取当前界面
         * @return QMdiSubWindow* 界面对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QMdiSubWindow* getCurrentSubWidget();
        /**
         * @brief 移除当前界面
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void removeCurrentWidget();
        /**
         * @brief 刷新布局
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void updateLayout();
        /**
         * @brief 设置标签栏是否自动放大
         * @param[i] ex 自动放大
         * @author liobaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setTabBarExpanding(bool ex = true );
    protected:
        /**
         * @brief 大小发生变化事件重写
         * @param[i]  event          事件对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        void resizeEvent(QResizeEvent *event) override;
        /**
         * @brief    事件过滤器重写
         * @param[i] object      事件对象
         * @param[i] event       事件类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        bool eventFilter(QObject *object, QEvent *event) override;
    protected slots:
        ;
        void slotWidgetClose(QMdiSubWindow* subWidget);
        /**
         * @brief    当前窗口最大化槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-05
         */
        virtual void slotCurWinStateMax();
    protected:
        /**
         * @brief 当前窗口最大化布局
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        virtual void currentMaxViewports();
        /**
         * @brief 层叠布局
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        virtual void cascadeViewports();
        /**
         * @brief 水平平铺布局
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        virtual void tileHorizontallyViewports();
        /**
         * @brief 垂直平铺
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        virtual void tileVerticallyViewports();
    protected:
        /**
         * @brief 互斥锁对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        static QMutex _mutex;
        /**
         * @brief 多视口布局方式
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        FITKVportsLayoutType _layoutType = FITKVportsLayoutType::CurrentMax;
        /**
         * @brief 多视口TabBar位置
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        FITKVportsTabPos _tabPos = FITKVportsTabPos::Top;
        /**
         * @brief 视口ID索引
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        int _objectID = 0;
        /**
         * @brief 界面链表
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-06
         */
        QHash<int, QMdiSubWindow*> _widgetHash = {};
    };
}

#endif
