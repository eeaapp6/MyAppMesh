/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGraphAxisActor2D.h
 * @brief  External vtkAxisActor2D with some common functions.
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-07-16
 *********************************************************************/

#ifndef FITKGRAPHAXISACTOR2D_H__
#define FITKGRAPHAXISACTOR2D_H__

#include <vtkAxisActor2D.h>
#include "FITKVTKAlgorithmAPI.h"

class vtkViewport;

class FITKVTKALGORITHMAPI FITKGraphAxisActor2D : public vtkAxisActor2D
{
public:
    vtkTypeMacro(FITKGraphAxisActor2D, vtkAxisActor2D);

    /**
     * @brief  设置单位标签（Title）的独立偏移量
     * @param  offset 偏移量（像素）
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-07-16
     */
    vtkSetMacro(TitleOffset, int);
    vtkGetMacro(TitleOffset, int);
    /**
     * @brief  VTK new
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-07-16
     */
    static FITKGraphAxisActor2D* New();

    virtual void BuildAxis(vtkViewport* viewport) override;
protected:
    FITKGraphAxisActor2D();
    ~FITKGraphAxisActor2D() override;

    // 单位标签的独立偏移量
    int TitleOffset;
};

#endif // !FITKGRAPHAXISACTOR2D_H__
