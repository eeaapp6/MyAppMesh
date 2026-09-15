/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   MainWindow.h
 * @brief  程序主界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-04
 *********************************************************************/
#ifndef  _POST3DMAINWINDOW_H_
#define  _POST3DMAINWINDOW_H_

#include <QMainWindow>
#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"

namespace Ui {
    class MainWindow;
}

namespace Comp {
    class FITKTabWidget;
}

namespace GUI
{
    class MainTreeWidget;
    class ToolBarAnimation;
    class ToolBarColorMapper;
    class ToolBarFilter;
    class ToolBarEvent;
    class RenderWidget;
    class ActionEventHandler;
    class AIAssistantDockWidget;
    class InformationWidget;
    class PropertyWidget;
    class MessageTabWidget;

    /**
     * @brief  主窗口
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-04
     */
    class GUIFRAMEAPI MainWindow : public QMainWindow, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT;
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        void init();
        /**
         * @brief    创建按钮
         * @param[i] actionName    按钮名称 
         * @param[i] iconPath      按钮图标
         * @param[i] actionTitle   按钮标题
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        QAction* createAction(QString actionName, QString iconPath = "", QString actionTitle = "");
        /**
         * @brief    查找action
         * @param[i] actionName    按钮名称 
         * @return   QAction *     按钮对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        QAction* findAction(QString actionName);
        /**
         * @brief    获取渲染窗口界面
         * @return   RenderWidget*  渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        RenderWidget* getRenderWidget();
        /**
         * @brief    获取树界面
         * @return   MainTreeWidget*  树界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        MainTreeWidget* getTreeWidget();
        /**
         * @brief    获取属性界面
         * @return   PropertyWidget*    属性界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        PropertyWidget* getPropertyWidget();
        /**
         * @brief    获取颜色映射工具条
         * @return   ToolBarColorMapper*   颜色映射工具条
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        ToolBarColorMapper* getToolBarColorMapper();
        /**
         * @brief    获取动画工具条
         * @return   ToolBarAnimation*  动画工具条
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        ToolBarAnimation* getToolBarAnimation();
        /**
         * @brief    获取过滤器工具条
         * @return   ToolBarFilter*    过滤器工具条
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        ToolBarFilter* getToolFilter();
        /**
         * @brief    获取事件工具条
         * @return   ToolBarEvent *
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        ToolBarEvent* getToolEvent();
        /**
         * @brief    获取信息界面
         * @return   InformationWidget*  信息界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        InformationWidget* getInfoWidget();
        /**
         * @brief    获取事件界面（位于渲染窗口左侧）
         * @return   Comp::FITKTabWidget* 界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        Comp::FITKTabWidget* getEventWidget();
    signals:
        ;
        void sigMainWindowClose();
    protected:
        /**
         * @brief    主窗口关闭事件重写
         * @param[i] event   事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        void closeEvent(QCloseEvent* event) override;
        /**
         * @brief    显示时间重写
         * @param[i] event                      事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-03
         */
        virtual void showEvent(QShowEvent* event) override;
    private:
        void initCentralWidget();
        /**
         * @brief    初始化action
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        void initAction();
        /**
         * @brief 隐藏或显示 AI 助手停靠窗口
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-03
         */
        void showAIAssistantDock();
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-04
         */
        Ui::MainWindow* _ui = nullptr;
        /**
         * @brief  树界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-04
         */
        MainTreeWidget* _treeWidget = nullptr;
        /**
         * @brief  消息窗口
         * @author guqingtao (15598887859@163.com)
         * @date   2026-06-02
         */
        MessageTabWidget* _msgWidget{};
        /**
         * @brief  渲染界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        RenderWidget* _renderWidget = nullptr;
        /**
         * @brief  信息界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        InformationWidget* _infoWidget = nullptr;
        /**
         * @brief  action点击事件处理
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-04
         */
        ActionEventHandler* _actionHandler{};
        /**
         * @brief  颜色设置工具条
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        ToolBarColorMapper* _colorMapToolBar = nullptr;
        /**
         * @brief  动画工具条
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        ToolBarAnimation* _animationToolBar = nullptr;
        /**
         * @brief  过滤器工具条
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        ToolBarFilter* _filterBar = nullptr;
        /**
         * @brief  事件工具条
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-14
         */
        ToolBarEvent* _filterEvent = nullptr;
        /**
         * @brief  属性界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        PropertyWidget* _propertyWidget = nullptr;
        /**
         * @brief  事件界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        Comp::FITKTabWidget* _eventWidget = nullptr;
        /**
         * @brief 是否已经发送过窗口信息
         * @author BaGuijun (baguijun@163.com)
         * @date 2026-06-03
         */
        bool _isSoftwareInfoSent = false;
        /**
         * @brief  AI 助手界面
         * @author GitHub Copilot
         * @date   2026-06-09
         */
        AIAssistantDockWidget* _aiAssistantWidget = nullptr;
    };
}


#endif
