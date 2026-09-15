/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPlotWindowsInterface.h
 * @brief  图表窗口接口类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-17
 *********************************************************************/
#ifndef _FITK_PLOTWINDOWS_INTERFACE_H___
#define _FITK_PLOTWINDOWS_INTERFACE_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace Plot
{
    class FITKPlotWindowAPI FITKPlotWindowsInterface : public AppFrame::FITKComponentInterface
    {
    public:
        FITKPlotWindowsInterface();
        ~FITKPlotWindowsInterface();

        /**
         * @brief 获取组件名称，不能重复 return "FITKPlotWindowsInterface"
         * @return
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-10
         */
        virtual QString getComponentName() override;

        /**
         * @brief 创建并获取窗口指针
         * @param  indexPort 窗口类型
         * @return 窗口指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-17
         */
        virtual QWidget* getWidget(const int indexPort);
    };
}

#endif
