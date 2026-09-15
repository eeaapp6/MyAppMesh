/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  渲染区子面板
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-04
 */
#ifndef __RENDERWIDGET_H__
#define __RENDERWIDGET_H__

#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

namespace Comp {
    class FITKMdiArea;
    class FITKGraph3DWindowVTK;
}

class QVBoxLayout;

namespace GUI
{
    class MainWindow;
    /**
     * @brief  渲染区域子面板类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-04
     */
    class GUIFRAMEAPI RenderWidget : public Core::FITKWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] mainWindow 主窗口对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        RenderWidget(QWidget* parent = nullptr);
        /**
         * @brief  析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        ~RenderWidget() = default;
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void init();
        /**
         * @brief    添加渲染窗口
         * @param[i] title      标题
         * @param[i] index      索引
         * @return   int        视口对应的id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        int addGraphWidget(QString title, int index = 1);
        /**
         * @brief    添加窗口
         * @param[i] widget     界面
         * @param[i] title      标题
         * @return   int        id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        int addWidget(QWidget* widget, QString title);
        /**
         * @brief  获取多视口管理
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        Comp::FITKMdiArea* getMdiArea() const;
        /**
         * @brief    设置视口样式
         * @param[i] type        视口样式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void setVportsType(Comp::FITKVportsLayoutType type);
        /**
         * @brief    获取视口样式
         * @return   Comp::FITKVportsLayoutType    视口样式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        Comp::FITKVportsLayoutType getVportsType();
        /**
         * @brief    获取渲染窗口
         * @param[i] index       索引
         * @return   QWidget*    渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        QWidget* getRenderWidget(int index = 0);
        ///**
        // * @brief    获取当前界面
        // * @return   QWidget*  界面
        // * @author   BaGuijun (baguijun@163.com)
        // * @date     2024-09-18
        // */
        //QWidget* getCurrentWidget();
    private:
        QVBoxLayout* _gridLayout = nullptr;
        /**
         * @brief  多视口管理
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        Comp::FITKMdiArea* _mdiArea = nullptr;
        /**
         * @brief  视口样式
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        Comp::FITKVportsLayoutType _vportsType = Comp::FITKVportsLayoutType::Tab;

        QList<Comp::FITKGraph3DWindowVTK*> _widgetList = {};
    };
}
#endif // !__RENDERWIDGET_H__
