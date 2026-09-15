/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file    ControlPanel.h
 * @brief   控制面板类声明
 * @author  LiBaojun (libaojunqd@foxmail.com)
 * @date    2024-10-12
 * 
 */
#ifndef _CONTROLPANEL__H___
#define _CONTROLPANEL__H___
 
#include "GUIWidgetAPI.h"
#include <QDockWidget>

namespace Comp
{
    class FITKTabWidget;
}

namespace GUI
{
    class TreeWidgetAssembly;
    class TreeWidgetGroup;
    class TreeWidgetProperty;
    class TreeWidgetSolution;
    class ResultTreeWidget;
    class ParamWidget;
    /**
     * @brief 控制面板类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-10-12
     */
    class GUIWIDGETAPI ControlPanel : public QDockWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief     控制面板对象
         * @param[i]  parent         父对象
         * @author    LiBaojun (libaojunqd@foxmail.com)
         * @date      2024-10-12
         */
        explicit ControlPanel(QWidget *parent = nullptr);
        /**
         * @brief   Destroy the Control Panel object
         * @author  LiBaojun (libaojunqd@foxmail.com)
         * @date    2024-10-12
         */
        virtual ~ControlPanel();
        /**
         * @brief   获取装配树对象
         * @return  TreeWidgetAssembly* 
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-23
         */
        TreeWidgetAssembly* getAssemblyTree();
        /**
        * @brief   获取分组树对象
        * @return  TreeWidgetAssembly*
        * @author  lilongyuan(lilongyuan@diso.cn)
        * @date    2025-09-23
        */
        TreeWidgetGroup* getGroupTree();
        /**
        * @brief   获取属性树对象
        * @return  TreeWidgetAssembly*
        * @author  lilongyuan(lilongyuan@diso.cn)
        * @date    2025-09-23
        */
        TreeWidgetProperty* getPropertyTree();
        /**
        * @brief   获取解决方案树对象
        * @return  TreeWidgetAssembly*
        * @author  lilongyuan(lilongyuan@diso.cn)
        * @date    2025-09-23
        */
        TreeWidgetSolution* getSolutionTree();
        /**
         * @brief    获取结果树对象
         * @return   ResultTreeWidget *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        ResultTreeWidget* getResultTree();
        /**
         * @brief     设置显示的页
         * @param[i]  name 页名
         * @author    wangning (2185896382@qq.com)
         * @date      2025-05-06
         */
        void setPage(const QString& name);

    private slots:
        ;
        /**
         * @brief    tab页发生变化槽函数
         * @param[i] index                    索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-14
         */
        void slot_TabCurrentChanged(int index);
    private: 
        
        /**
         * @brief  结果树
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        ResultTreeWidget* _resultTreeWidget{};

        Comp::FITKTabWidget* _tab{};

        /**
         * @brief   装配树
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-23
         */
        TreeWidgetAssembly* _treeAssembly{};
        /**
         * @brief   分组树
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-23
         */
        TreeWidgetGroup* _treeGroup{};
        /**
         * @brief   属性树
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-16
         */
        TreeWidgetProperty* _treeProperty{};
        /**
         * @brief   解决方案树
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-16
         */
        TreeWidgetSolution* _treeSolution{};
    };
}
#endif