/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectBCBase.h
 * @brief  边界渲染对象基类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-10-27
 *********************************************************************/
#ifndef _GraphVTKObjectBCBase_H__
#define _GraphVTKObjectBCBase_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObject3D.h"

namespace Radioss
{
    class FITKAbstractBCS;
}
class vtkPolyData;

namespace Exchange {

    /**
     * @brief  边界渲染对象基类
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-10-27
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectBCBase :public GraphVTKObject3D
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObjectBCBase, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectBCBase);
    public:
        /**
         * @brief    构造函数
         * @param[i] loadData                  载荷数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        GraphVTKObjectBCBase(Radioss::FITKAbstractBCS* loadData);
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        virtual ~GraphVTKObjectBCBase();
    protected:
        /**
         * @brief    获取数据点信息
         * @param[i] dataset                   vtk数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        void generatePolyData(vtkPolyData* ploydata);

    protected:
        /**
         * @brief  集合id
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-27
         */
        int _setID = -1;
    };
}

#endif // !_GraphVTKObjectBCBase_H__

