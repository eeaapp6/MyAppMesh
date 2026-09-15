/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file SARibbonBarHelper.h
 * @brief  SARibbonBar 相关设置
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-10-13
 *
 */
#ifndef _SARIBBON_BAR_HELPER_H___
#define _SARIBBON_BAR_HELPER_H___

#include "GUIFrameAPI.h"
#include <QString>
#include "SARibbonPannelItem.h"

class QWidget;
class QComboBox;
class QAction;
class SARibbonBar;
class SARibbonPannel;
class SARibbonCategory;
namespace GUI
{
    class MainWindow;
    /**
     * @brief SARibbonBar 相关设置
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-10-13
     */
    class GUIFRAMEAPI SARibbonBarHelper
    {
    public:
        /**
         * @brief Construct a new SARibbonBarHelper object
         * @param[i]  mw             主页面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-13
         */
        explicit SARibbonBarHelper(MainWindow* mw);
        /**
         * @brief Destroy the SARibbonBarHelper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-13
         */
        virtual ~SARibbonBarHelper() = default;

        /**
        * @brief 初始化Ribbon页面
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-10-12
        */
        void initRibbonBar();
        /**
          * @brief 添加页面
          * @param[i]  pageText      页面标题
          * @return SARibbonCategory*
          * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
          * @date 2025-03-13
          */
        SARibbonCategory* addPage(const QString& pageText);
        /**
          * @brief 添加面板
          * @param[i]  page          页面
          * @param[i]  text          面板标题
          * @return SARibbonPannel*
          * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
          * @date 2025-03-13
          */
        SARibbonPannel* addPanel(SARibbonCategory* page, const QString& text);
        /**
         * @brief action追加到panel
         * @param[i]  panel         面板
         * @param[i]  action         按钮
         * @param[i]  actionType     按钮大小
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date 2025-03-14
         */
        void panelAddAction(SARibbonPannel* pannel, QAction* action, SARibbonPannelItem::RowProportion actionType = SARibbonPannelItem::Large);
        /**
          * @brief 移除页面
          * @param[i]  page          页面
          * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
          * @date 2025-03-13
          */
        void removePage(SARibbonCategory* page);

    private:
        /**
         * @brief 创建action
         * @param[i]  text           字符提示
         * @param[i]  name           对象名称
         * @param[i]  icon           图标
         * @param[i]  parent         父对象
         * @return QAction*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-12
         */
        static QAction* CreateAction(const QString& text, const QString & name,
            const QString& icon, QWidget* parent = nullptr);
        /**
        * @brief 初始化开始菜单
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-10-12
        */
        void initRibbonBarStartButton();

        /**
         * @brief 初始化文件页
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-12
         */
        void initRibbonBarFile();
        /**
         * @brief 初始化分析页
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-12
         */
        void initRibbonBarAnalysis();
        void initRibbonBarMesh();
        /**
         * @brief  初始化工具页
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-05
         */
        void initRibbonBarTools();
        /**
         * @brief    初始化求解页
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-19
         */
        void initRibbonBarSolver();
        void initRibbonBarPost();

        void initRibbonBarGeometry();
        void initRibbonBarProp();
        /**
         * @brief    初始化帮助页
         * @return   void
         * @author   libaojun
         * @date     2025-10-14
         */
        void initRibbonBarHelp();
        /**
         * @brief    下拉框关联操作器
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-17
         */
        void comboBoxConnectOper();
        /**
         * @brief    设置按钮样式
         * @param[i] action 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-23
         */
        void setPushButtonStyle(QAction* action);
    private:
        /**
         * @brief 主界面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-13
         */
        MainWindow* _mainWindow{};
        /**
         * @brief ribbon栏
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-13
         */
        SARibbonBar* _ribbonBar{};
    };
}


#endif
