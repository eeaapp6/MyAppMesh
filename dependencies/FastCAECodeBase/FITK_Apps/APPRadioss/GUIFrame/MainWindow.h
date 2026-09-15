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
#include "SARibbonMainWindow.h"


class QAction;
class QProgressBar;
class QToolBar;

namespace Ui
{
    class MainWindow;
}


namespace GUI
{
    class SARibbonBarHelper;
    class ActionEventHandler;
    class AIAssistantDockWidget;
    class CentralWidget;
    class MessageTabWidget;
    class ControlPanel;
    class ModelLayerPanel;

    /**
     * @brief  主窗口界面类
     * @author libaojun
     * @date   2024-06-12
     */
    class GUIFRAMEAPI MainWindow : public SARibbonMainWindow, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief  构造函数
         * @author libaojun
         * @date   2024-06-12
         */
        explicit MainWindow(QWidget *parent = nullptr);
        /**
         * @brief  析构函数
         * @author libaojun
         * @date   2024-06-12
         */
        virtual ~MainWindow();

        /**
         * @brief  获取界面的action消息处理器
         * @return 消息处理器
         * @author libaojun
         * @data   2024-06-12
         */
        ActionEventHandler* getActionEventHandle() const;
        /**
         * @brief 从框架中获取主界面对象
         * @return MainWindow*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        static MainWindow* GetMainWindowFromFramework();
        /**
         * @brief 获取中心窗口
         * @return CentralWidget*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        CentralWidget* getCentralWidget();
        /**
         * @brief 设置进度值
         * @param[i]  value         进度值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-15
         */
        void setProgressValue(int value);
        /**
         * @brief 获取控制面板
         * @return ControlPanel*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-05
         */
        ControlPanel* getControlPanel();
        /**
         * @brief 获取图层面板
         * @return ModelLayerPanel*
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2025-02-10
         */
        ModelLayerPanel* getModelLayerPanel();
        /**
         * @brief 获取当前属性窗口
         * @return QWidget*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-05
         */
        //QWidget* getCurrentPropWidget();
        /**
         * @brief 获取Ribbon工具条
         * @return SARibbonBarHelper*
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date 2025-03-13
         */
        SARibbonBarHelper* getRibbonBarHelper() const;

    private:
        /**
         * @brief 创建或显示 AI 助手停靠窗口
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-03
         */
        void showAIAssistantDock();

    private:
        /**
         * @brief 初始化窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        void initWindow();

        /**
         * @brief    重写关闭事件
         * @param[i] event 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-03-18
         */
        virtual void closeEvent(QCloseEvent* event) override;
    private:
        /**
         * @brief UI对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        Ui::MainWindow* _ui{};
        /**
         * @brief Ribbon菜单栏
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        SARibbonBarHelper* _ribbonBarHelper{};
        /**
         * @brief  action点击事件处理
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        ActionEventHandler* m_ActionHandler{};
        /**
         * @brief 中心窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        CentralWidget* _centralWidget{};
        /**
         * @brief 信息输出窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        MessageTabWidget* _messageTab{};

        ControlPanel* _controlPanel{};

        /**
         * @brief 右侧模型图层dockwidget
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        ModelLayerPanel* _modelLayerPanel{};

        /**
         * @brief 进度条
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-15
         */
        QProgressBar* _progerssBar{};
    };
}

#endif // !__MAINWINDOW_H__
