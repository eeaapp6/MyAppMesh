/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file PreGraph.h
 * @brief 前处理模型显示
 * @author shenzhuowei (2931275006@qq.com)
 * @date 2024-12-19
 * 
 */
#ifndef __PRE_GRAPHH_WIDGET_H___
#define __PRE_GRAPHH_WIDGET_H___

#include "GUIWidgetAPI.h"
#include "GraphArea.h"

namespace GUI
{
    
    /**
     * @brief 前处理模型显示
     * @author shenzhuowei (2931275006@qq.com)
     * @date 2024-12-19
     */
    class GUIWIDGETAPI PreGraphWidget : public GraphArea
    {
        Q_OBJECT
    public:
        /**
         * @brief Create
         * @param parent 父对象
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        explicit PreGraphWidget(QWidget* parent);
        /**
         * @brief Destroy
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        virtual ~PreGraphWidget() = default;

        virtual GraphAreaType getGraphAreaType() override;
 
    private:
    
    };
}

#endif
