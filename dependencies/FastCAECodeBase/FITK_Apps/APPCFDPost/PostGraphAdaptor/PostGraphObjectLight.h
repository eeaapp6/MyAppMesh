/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _PostGraphObjectLight_H
#define _PostGraphObjectLight_H

#include "PostGraphAdaptorAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

class vtkLight;

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Interface
{
    class FITKCFDPostLight;
    /**
     * @brief  后处理渲染对象基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-10
     */
    class PostGraphAdaptorAPI PostGraphObjectLight : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief    构造函数
         * @param[i] postData       后处理数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        explicit PostGraphObjectLight(FITKCFDPostLight* lightData, Comp::FITKGraph3DWindowVTK* graph3DWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        virtual ~PostGraphObjectLight();
        /**
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void update();
        /**
         * @brief    获取当前灯光数据对象
         * @return   FITKCFDPostLight*    灯光数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        FITKCFDPostLight* getLightData();
    private:
        /**
         * @brief  灯光数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        FITKCFDPostLight* _lightData = nullptr;
        /**
         * @brief  渲染界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        Comp::FITKGraph3DWindowVTK* _graph3DWidget = nullptr;
        /**
         * @brief  vtk灯光
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        vtkLight* _VTKLight = nullptr;
    };
}
#endif