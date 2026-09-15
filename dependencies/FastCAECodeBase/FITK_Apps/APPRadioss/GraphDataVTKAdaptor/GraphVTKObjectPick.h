/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectPick.h
 * @brief  拾取模型渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-26
 *********************************************************************/
#ifndef __GraphVTKObjectPick_H__
#define __GraphVTKObjectPick_H__

#include "GraphVTKObject3D.h"
#include "GraphDataVTKAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkDataSetSurfaceFilter;

class FITKGraphActor;

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Exchange
{
    /**
     * @brief  拾取模型渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-26
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectPick : public GraphVTKObject3D
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectPick, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectPick);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        GraphVTKObjectPick();
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        ~GraphVTKObjectPick();
        /**
         * @brief    设置渲染对象隐藏还是显示
         * @param[i] visibility            隐藏还是显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setVisible(bool visibility) override;
        /**
         * @brief    设置拾取数据对象
         * @param[i] grid           数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedData(vtkDataSet* grid);
        /**
         * @brief    设置拾取类型
         * @param[i] type          类型( 1: Points, 2: Cells, 3: Mixed)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedType(int type);
        /**
         * @brief    设置颜色
         * @param[i] color        颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setColor(QColor color) override;
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void init();
        /**
         * @brief    初始化演员属性
         * @param[i] actor              演员
         * @param[i] details            参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void initActorProperties(vtkProp* actor, QVariant details = QVariant()) override;
    private:
        /**
         * @brief    The actor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        FITKGraphActor* m_fActor{ nullptr };
        /**
         * @brief    The surface filter for high-order element.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkDataSetSurfaceFilter* m_surfaceFilter{ nullptr };
    };
}   // namespace Exchange

#endif // __GraphVTKObjectPick_H__
