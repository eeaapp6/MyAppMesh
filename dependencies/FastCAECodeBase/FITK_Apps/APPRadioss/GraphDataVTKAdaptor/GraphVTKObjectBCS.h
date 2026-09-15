/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectBCS.h
 * @brief  边界渲染对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-10-27
 *********************************************************************/
#ifndef GRAPHVTKOBJECTBOUNDARY_H
#define GRAPHVTKOBJECTBOUNDARY_H

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObjectBCBase.h"

#include <QVector>

namespace Radioss
{
    class FITKRadiossBCS;
}

class vtkArrowSource;
class FITKGraphActor;
class vtkGlyph3D;
class vtkPolyData;
class vtkDoubleArray;
class vtkDistanceToCamera;

namespace Exchange
{
    /**
     * @brief  边界渲染对象：采用两种箭头分别表示 T 和 W
     *         箭头颜色：T - 蓝色，W - 黄色，显示状态依据各自的状态布尔值
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-10-27
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectBCS : public GraphVTKObjectBCBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief     构造函数
         * @param[in] bcData  边界条件数据对象
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        GraphVTKObjectBCS(Radioss::FITKRadiossBCS* bcData);
        /**
         * @brief     析构函数
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        virtual ~GraphVTKObjectBCS();
        /**
         * @brief     更新
         * @param[in] forceUpdate 是否强制刷新
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        void update(bool forceUpdate = false) override;
    protected:
        /**
         * @brief     初始化内部对象
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        void init();
        /**
         * @brief     初始化演员（Actor）
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        void initActor();
        /**
         * @brief     更新渲染数据
         * @return    是否成功更新数据
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        bool updateGraphData();
        /**
         * @brief     更新箭头方向及显示状态（依据 U 与 UR 状态）
         * @author    ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date      2025-10-27
         */
        void updateDirection();

    protected:
        // --- T组成员（采用蓝色） ---
        QVector<FITKGraphActor*> _actorT = QVector<FITKGraphActor*>(3, nullptr);
        QVector<vtkPolyData*> _polyDataT = QVector<vtkPolyData*>(3, nullptr);
        QVector<vtkGlyph3D*> _glyphT = QVector<vtkGlyph3D*>(3, nullptr);
        QVector<vtkDoubleArray*> _normalArrayT = QVector<vtkDoubleArray*>(3, nullptr);
        QVector<vtkDistanceToCamera*> _disCameraT = QVector<vtkDistanceToCamera*>(3, nullptr);

        // --- W组成员（采用黄色） ---
        QVector<FITKGraphActor*> _actorW = QVector<FITKGraphActor*>(3, nullptr);
        QVector<vtkPolyData*> _polyDataW = QVector<vtkPolyData*>(3, nullptr);
        QVector<vtkGlyph3D*> _glyphW = QVector<vtkGlyph3D*>(3, nullptr);
        QVector<vtkDoubleArray*> _normalArrayW = QVector<vtkDoubleArray*>(3, nullptr);
        QVector<vtkDistanceToCamera*> _disCameraW = QVector<vtkDistanceToCamera*>(3, nullptr);
    };
}

#endif // GRAPHVTKOBJECTBOUNDARY_H
