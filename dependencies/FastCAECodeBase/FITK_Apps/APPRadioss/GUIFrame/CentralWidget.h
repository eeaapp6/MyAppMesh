/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file CentralWidget.h
 * @brief 中心窗口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-24
 * 
 */
#ifndef _CENTRAL_Widget_H__
#define _CENTRAL_Widget_H__

#include "GUIFrameAPI.h"
#include <QWidget>

class QToolBar;
namespace Ui
{
    class CentralWidget;
}

namespace GUI
{
    class ControlPanel;
    class GraphArea;
    class GraphMdiArea;
    
    /**
     * @brief 中心窗口，核心是 MDI 容器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-24
     */
    class GUIFRAMEAPI CentralWidget : public QWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Central Widget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        explicit CentralWidget(QWidget* parent);
        /**
         * @brief Destroy the Central Widget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        virtual ~CentralWidget();
        /**
         * @brief 获取绘图区域
         * @return GraphMdiArea* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-31
         */
        GraphMdiArea* getGraphMidArea();
        /**
         * @brief    设置当前的渲染窗口为前处理窗口
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-14
         */
        void setCurGraphWidgetToPre();
        /**
         * @brief    设置当前的渲染窗口为后处理窗口
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-02-14
         */
        void setCurGraphWidgetToPost();
        /**
         * @brief    设置当前的渲染窗口为指定id的窗口
         * @param[i] QWidget
         * @return   void void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        void setCurGraphWidget(QWidget* subWidget);
        /**
         * @brief    添加后处理窗口id
         * @param[i] id 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        void addPostWidgetId(int id);
    private:
        /**
         * @brief UI对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        Ui::CentralWidget* _ui{};
        
        /**
         * @brief MDI区域对象
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        GraphMdiArea* _graphArea{};

        /**
         * @brief  前处理窗口id
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-14
         */
        int _preWidgetId = -1;
        /**
         * @brief  后处理窗口id
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-14
         */
        QList<int> _postWidgetId = {};
    };
}

#endif
