/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIWidgetBase.h
 * @brief  界面基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _GUIWidgetBase_H_
#define _GUIWidgetBase_H_

#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui {
    class GUIWidgetBase;
}

namespace Interface {
    class FITKCFDPost3DManager;
    class FITKCFDPostLightManager;
    class FITKCFDPostColorLibaryDataManager;
    class PostGraphObjectManager;
}

namespace GUI
{
    class GUIWidgetAPI GUIWidgetBase : public Core::FITKWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent     父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        explicit GUIWidgetBase(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual ~GUIWidgetBase();
        /**
         * @brief    刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void reRender();
        //获取当前数据ID
        int getCurrentID();
    protected:
        /**
         * @brief  后处理数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::FITKCFDPost3DManager* _postDataManager = nullptr;
        /**
         * @brief  后处理渲染对象管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        Interface::PostGraphObjectManager* _postGraphManager = nullptr;
        /**
         * @brief  后处理灯光管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        Interface::FITKCFDPostLightManager* _postLightManager = nullptr;
        /**
         * @brief  后处理色库管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-25
         */
        Interface::FITKCFDPostColorLibaryDataManager* _postColorLibManager = nullptr;
        /**
         * @brief  后处理数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        int _currentDataID = -1;
    };
}

#endif
