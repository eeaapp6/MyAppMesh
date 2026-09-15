/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ToolBarFilter.h
 * @brief  过滤器工具条
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _ToolBarFilter_H
#define _ToolBarFilter_H

#include "GUIWidgetAPI.h"
#include "ToolBarBase.h"
#include <QComboBox>

class QAction;

namespace GUI
{
    class GUIWidgetAPI ToolBarFilter : public ToolBarBase
    {
        Q_OBJECT;
    public:
        ToolBarFilter(QWidget* parent = nullptr);
        ~ToolBarFilter();
        /**
         * @brief    根据当前渲染对象修改颜色映射
         * @param[i] objID    渲染对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual void updateCurrentGraphObj(int objID) override;
    private:
        void updateIsoSurf();
        void updateVector();
        void updateClip();
        void updateSlice();
        void updateStreamLine();
        void updateThreshold();
        void updateDeform();
    private:
        QAction* _isoSurfAction = nullptr;
        QAction* _vectorfAction = nullptr;
        QAction* _clipAction = nullptr;
        QAction* _sliceAction = nullptr;
        QAction* _streamlineSurfAction = nullptr;
        QAction* _thresholdAction = nullptr;
        QAction* _deformAction = nullptr;
    };
}

#endif