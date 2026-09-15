/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectLoadBase.h
 * @brief  载荷渲染对象基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-11
 *********************************************************************/
#ifndef _GraphVTKObjectLoadBase_H
#define _GraphVTKObjectLoadBase_H

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObject3D.h"

namespace AbaqusData {
    class FITKDataCase;
}

namespace Interface {
    class FITKAbaAbstractLoad;
    class FITKModelSet;
    class FITKBoundaryLocation;
    class FITKAbaAssembly;
    class FITKAbstractAssInstance;
}

class vtkPolyData;

namespace Exchange
{
    /**
     * @brief  载荷渲染对象基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-11
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectLoadBase :public GraphVTKObject3D
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] loadData                  载荷数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        GraphVTKObjectLoadBase(Interface::FITKAbaAbstractLoad* loadData);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        virtual ~GraphVTKObjectLoadBase();
    protected:
        /**
         * @brief    获取载荷数据点信息
         * @param[i] dataset                   vtk数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void generatePolyData(vtkPolyData* ploydata);
        /**
         * @brief    通过集合与装配数据生成polyData
         * @param[i] set                       集合
         * @param[i] instanceId                装配
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void generateSetPolyData(Interface::FITKModelSet* set, int instanceId);
        /**
         * @brief    实例集合点生成
         * @param[i] set                        集合
         * @param[i] instance                   实例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void generatePointsWithInstanceSet(Interface::FITKModelSet* set, Interface::FITKAbstractAssInstance* instance);
        /**
         * @brief    装配集合点生成
         * @param[i] set                        集合
         * @param[i] assembly                   装配
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void generatePointsWithAssemblySet(Interface::FITKModelSet* set, Interface::FITKAbaAssembly* assembly);
    protected:
        /**
         * @brief  集合ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        int _setID = -1;
    };
}

#endif