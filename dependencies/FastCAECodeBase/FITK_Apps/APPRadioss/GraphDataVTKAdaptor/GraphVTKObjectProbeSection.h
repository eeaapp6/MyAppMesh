/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectProbeSection.h
 * @brief  探测段可视化对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-12-04
 *********************************************************************/
#ifndef _GraphVTKObjectProbeSection_H__
#define _GraphVTKObjectProbeSection_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObjectModelBase.h"

class vtkUnstructuredGrid;
class FITKGraphActor;
namespace Radioss
{
    class FITKProbeSection;
}
namespace Exchange
{
    /**
     * @brief  探测段可视化对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-12-04
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectProbeSection : public GraphVTKObjectModelBase
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectProbeSection, GraphVTKObjectModelBase);
        FITKCLASS(Exchange, GraphVTKObjectProbeSection);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        GraphVTKObjectProbeSection(Radioss::FITKProbeSection* dataObj);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        virtual ~GraphVTKObjectProbeSection();
        /**
         * @brief  更新场颜色
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        void updateModelColor(QColor color) override;
        /**
         * @brief    获取数据可见
         * @return   bool 是否可见
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-23
         */
        virtual bool getVisibility() override;

        /**
         * @brief  更新可见
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-23
         */
        virtual void updateVisibility() override;
        /**
         * @brief    显示隐藏
         * @param[i] visibility
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-25
         */
        virtual void setVisible(bool visibility) override;
        /**
         * @brief    更新
         * @param[i] forceUpdate
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-29
         */
        virtual void update(bool forceUpdate = false) override;
    private:
        /**
         * @brief    初始化
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        void init();
        /**
         * @brief    生成渲染数据
         * @return   bool               是否成功
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        bool generateDataSet();
        /**
         * @brief    生成渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        void generateGraph();

    private:
        /**
         * @brief  渲染数据
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        vtkUnstructuredGrid* _grid = nullptr;
        /**
         * @brief  演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        FITKGraphActor* _actor{ nullptr };
    };
}

#endif // !_GraphVTKObjectProbeAcceleration_H__
