/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   RenderWidget.h
 * @brief  渲染区子面板
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-06-11
 */
#ifndef __RENDERWIDGET_H__
#define __RENDERWIDGET_H__

#include "GUIFrameAPI.h"
#include <QWidget>

namespace Core
{
    class FITKAbstractGraph3DWidget;
}

namespace GUI
{
    /**
     * @brief  渲染区子面板类
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-06-11
     */
    class GUIFRAMEAPI RenderWidget : public QWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        RenderWidget(QWidget* parent = nullptr);
        /**
         * @brief  析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        ~RenderWidget() = default;

        Core::FITKAbstractGraph3DWidget* getGraph3DWidget() const;
    private:
        /**
         * @brief  3D图形管理
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-12
         */
        Core::FITKAbstractGraph3DWidget* m_Graph3DWidget{};

    };
}
#endif // !__RENDERWIDGET_H__
