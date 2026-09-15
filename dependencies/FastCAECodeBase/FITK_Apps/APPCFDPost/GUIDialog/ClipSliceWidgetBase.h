/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ClipSliceWidgetBase.h
 * @brief  切割界面基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-08
 *********************************************************************/
#ifndef _ClipSliceWidgetBase_H
#define _ClipSliceWidgetBase_H

#include "GUIWidget/GUIWidgetBase.h"

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    /**
     * @brief  切割切片界面类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-11
     */
    enum class ClipSlice{
        None,
        Clip,
        Slice,
    };
    /**
     * @brief  切割界面基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-08
     */
    class ClipSliceWidgetBase :public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent 父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        ClipSliceWidgetBase(ClipSlice widgetType,QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        virtual ~ClipSliceWidgetBase();
        /**
         * @brief    将界面数据填充至数据对象中
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        virtual void getDataFromWidget(Interface::FITKAbstractCFDPostData* postData) = 0;
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        virtual void setDataToWidget(Interface::FITKAbstractCFDPostData* postParentData, Interface::FITKAbstractCFDPostData* postData) = 0;
    protected:
        /**
         * @brief  界面类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-11
         */
        ClipSlice _widgetType = ClipSlice::None;
    };
}

#endif
