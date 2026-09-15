/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphOperator.h"

namespace Comp
{
    FITKGraphOperator::FITKGraphOperator()
    {

    }

    FITKGraphOperator::~FITKGraphOperator()
    {

    }
   
    void FITKGraphOperator::setGraph3DWindow(FITKGraph3DWindowVTK* w)
    {
        //设置三维渲染窗口
        m_graph3DWindow = w;
    }

    Comp::FITKGraph3DWindowVTK* FITKGraphOperator::getGraph3DWindow()
    {
        //返回渲染窗口
        return m_graph3DWindow;
    }

}