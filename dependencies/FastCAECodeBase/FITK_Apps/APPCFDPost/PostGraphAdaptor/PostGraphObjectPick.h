/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _PostGraphObjectPick_H
#define _PostGraphObjectPick_H

#include "PostGraphAdaptorAPI.h"
#include "PostGraphObjectBase.h"

class vtkDataSet;
class vtkPolyData;
class vtkUnstructuredGrid;
class vtkDataSetMapper;

namespace Interface
{
    class PostGraphAdaptorAPI PostGraphObjectPick :public PostGraphObjectBase
    {
    public:
        PostGraphObjectPick(FITKAbstractCFDPostData* postData, Comp::FITKGraph3DWindowVTK* graph3DWidget);
        ~PostGraphObjectPick();
        void setPickeData(vtkUnstructuredGrid* grid);

        void setPickedType(int type);

        void setVisible(bool visibility);

        void setColor(QColor color);

        void update();
        /**
         * @brief    渲染层级
         * @return   int       渲染层级
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        virtual int getRenderLayer();
    private:
        /**
         * @brief  演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-22
         */
        vtkActor* _actor = nullptr;

        vtkUnstructuredGrid* _pickGrid = nullptr;

        vtkUnstructuredGrid* _grid = nullptr;

        vtkDataSetMapper* _mapper = nullptr;
    };
}

#endif
