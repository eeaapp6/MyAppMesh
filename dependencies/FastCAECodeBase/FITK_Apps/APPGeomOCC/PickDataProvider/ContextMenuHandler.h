/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    ContextMenuHandler.h
 * @brief   三维窗口右键点选菜单信号处理。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-10-21
 *********************************************************************/

#ifndef __CONTEXTMENUHANDLER_H__
#define __CONTEXTMENUHANDLER_H__

#include <QObject>
#include <QPoint>

#include "PickDataProviderAPI.h"

// 前置声明
class QMenu;
class QAction;

class AIS_InteractiveContext;

namespace Render
{
    class FITKGraph3DWindowOCC;
}

namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace GraphEvent
{
    /**
     * @brief   三维窗口右键点选菜单信号处理类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-21
     */
    class PICKDATAPROVIDERAPI ContextMenuHandler : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @brief   获取单例实例。[静态]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        static ContextMenuHandler* GetInstance();

    private:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        ContextMenuHandler();

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        ~ContextMenuHandler();

        /**
         * @brief   初始化。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        void init();

        /**
         * @brief   初始化菜单。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        void initMenu();

        /**
         * @brief   更新可视化对象。
         * @param   id：命令ID
         * @param   refLayer：更新引用数据的层级[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        void updateGraph(int id, int refLayer = 2);

        /**
         * @brief   检查是否可通过右键菜单移除几何。
         * @return  是否可以移除几何
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        bool canDeleteGeometry();

        /**
         * @brief   检查是否可通过右键菜单编辑几何。
         * @param   dataName：可编辑的数据名称[引用]
         * @return  是否可以编辑几何
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        bool canEditGeometry(QString & dataName);

        /**
         * @brief   检查是否可以通过右键菜单隐藏几何。
         * @return  是否可以隐藏几何
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-29
         */
        bool canHideGeometry();

        /**
         * @brief   获取拾取到的第一个模型命令。
         * @param   nPicked：总拾取对象数量[引用]
         * @return  命令数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        Interface::FITKAbsGeoCommand* getFirstPickedEditableCommand(int & nPicked);

    private slots:
        /**
         * @brief   槽函数 - 窗口右键菜单。
         * @param   pos：菜单弹出坐标
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        void slot_windowContextMenu(QPoint pos);

        /**
         * @brief   槽函数 - 删除按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        void slot_actDelete();

        /**
         * @brief   槽函数 - 编辑按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        void slot_actEdit();

        /**
         * @brief   槽函数 - 隐藏模型按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-29
         */
        void slot_actHide();

    private:
        /**
         * @brief   实例。[静态]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        static ContextMenuHandler* s_instance;

        /**
         * @brief   三维可视化窗口。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        Render::FITKGraph3DWindowOCC* m_graphWidget{ nullptr };

        /**
         * @brief   OCC交互上下文。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        AIS_InteractiveContext* m_context{ nullptr };

        /**
         * @brief   右键菜单。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        QMenu* m_menu{ nullptr };

        /**
         * @brief   删除按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-21
         */
        QAction* m_actDelete{ nullptr };

        /**
         * @brief   编辑按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        QAction* m_actEdit{ nullptr };

        /**
         * @brief   隐藏按钮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-29
         */
        QAction* m_actHide{ nullptr };

        /**
         * @brief   重置相机视角至当前选中区域。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        QAction* m_actFitSelection{ nullptr };
    };
}
#endif // __CONTEXTMENUHANDLER_H__