/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   MainWindow.h
 * @brief  主窗口界面
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-06-12
 */
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "GUIFrameAPI.h"

#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"

#include <QFont>

namespace Ui {
    class MainWindow;
}

namespace GUI
{
    class MainMenuBase;
    class RenderWidget;
    class ActionEventHandler;
    class TreeWidget;
    class MessageTabWidget;

    /**
     * @brief  主窗口界面类
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-06-12
     */
    class GUIFRAMEAPI MainWindow : public QMainWindow, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief  构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        MainWindow(QWidget *parent = nullptr);
        /**
         * @brief  析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        ~MainWindow();

        void updateGeometryTree();

        /**
         * @brief  获取界面的action消息处理器
         * @return 消息处理器
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-06-12
         */
        ActionEventHandler* getActionEventHandle() const;
        /**
         * @brief  获取渲染子部件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        RenderWidget* getRenderWidget() const;

        /**
         * @brief   获取模型树。
         * @return  模型树
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        TreeWidget* getTreeWidget();

        /**
         * @brief 创建按钮
         * @param[i]  toolTip        提示信息
         * @param[i]  objectName     按钮名称
         * @param[i]  iconPath       按钮图标
         * @param[i]  iconText       图注
         * @return QAction* action对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-05-11
         */
        QAction* createAction(const QString &toolTip, const QString &objectName, const QString &iconPath = "", const QString& iconText = "");

        /**
         * @brief   隐藏或显示所有工具栏与菜单栏。
         * @param   visible：是否可见
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-25
         */
        void setBarVisible(bool visible);

        /**
         * @brief   切换菜单栏与工具栏为草图模式。
         * @param   isOn：是否为草图模式
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-25
         */
        void setSketchMode(bool isOn);

        /**
         * @brief   获取草图按钮。
         * @return  草图按钮
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-25
         */
        QAction* getSketchAction();

        /**
         * @brief   开启或关闭开发菜单。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        void devTest();

    protected:
        /**
         * @brief   主窗口关闭事件。[重写][虚函数]
         * @param   event：窗口关闭事件
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-23
         */
        virtual void closeEvent(QCloseEvent* event) override;

    private:
        /**
         * @brief  初始化
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        void init();
        /**
         * @brief  初始化中间部件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        void initCentralWidget();
        /**
         * @brief 按钮修改
         * @param[i]  action         按钮对象
         * @param[i]  iconPath       图标路径
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-05-11
         */
        bool changeAction(QAction* action, const QString iconPath, const QString& iconText = "");
        /**
         * @brief 隐藏或显示 AI 助手停靠窗口
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-03
         */
        void showAIAssistantDock();

    private:
        QWidget* _currentWidget = nullptr;
        /**
         * @brief tree界面
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-05-30
         */
        TreeWidget* m_TreeWidget{};
        /**
         * @brief   日志窗口。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        MessageTabWidget* m_MsgWidget{};
        /**
         * @brief  渲染部件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        RenderWidget* m_RenderWidget{};
        /**
         * @brief  action点击事件处理
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        ActionEventHandler* m_ActionHandler{};

        /**
         * @brief   菜单栏显示属性。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-25
         */
        bool m_barVisibility = true;

        Ui::MainWindow* m_Ui{};
    };
}

#endif // !__MAINWINDOW_H__
