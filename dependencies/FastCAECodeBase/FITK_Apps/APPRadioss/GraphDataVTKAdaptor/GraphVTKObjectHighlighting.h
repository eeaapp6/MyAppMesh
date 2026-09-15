/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectHighlighting.h
 * @brief  高亮可视化对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-17
 *********************************************************************/

#ifndef _GRAPHVTKOBJECTHIGHLIGHTING_H__
#define _GRAPHVTKOBJECTHIGHLIGHTING_H__
#include "GraphVTKObject3D.h"
#include "GraphDataVTKAdaptorAPI.h"

class vtkDataSet;
class FITKGraphActor;
class vtkAppendFilter;
class vtkActorType;

namespace Exchange
{
    /**
     * @brief  高亮可视化对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-17
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectHighlighting : public GraphVTKObject3D
    {
        FITKGraphObjectRegist(GraphVTKObjectHighlighting, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectHighlighting);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        GraphVTKObjectHighlighting(Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        virtual ~GraphVTKObjectHighlighting();
        /**
         * @brief  显示隐藏
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        virtual void setVisible(bool isVisible) override;
        /**
         * @brief    设置模型输入数据对象
         * @param[i] inputData 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setModelInputDataObject(vtkDataSet* inputData);
        void setModelInputConnection(vtkAlgorithmOutput* input, int port = 0);
        void addModelInputDataObject(vtkDataSet* dataSet);
        void addModelInputConnection(vtkAlgorithmOutput* input, int port = 0);
        void removeModelInputDataObject(vtkDataSet* dataSet);
        void removeModelInputConnection(vtkAlgorithmOutput* input, int port = 0);
        void clearModelInputs();
        /**
         * @brief    高亮
         * @param[i] details 
         * @param[i] color 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void highlight(QColor color = QColor()) override;
        /**
         * @brief    取消高亮
         * @param[i] details 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void disHighlight() override;
        /**
         * @brief    设置点显示隐藏
         * @param[i] visibility 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setVertexVisible(bool visibility);
        /**
         * @brief    设置线大小
         * @param[i] size 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-29
         */
        void setVertexSize(double size);
        /**
         * @brief    设置线大小
         * @param[i] width 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setLineWidth(double width);
        /**
         * @brief    设置渲染模式
         * @param[i] rep 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setRepresentation(int rep);
        /**
         * @brief    设置透明度
         * @param[i] trans 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setTransparency(double trans);
        /**
         * @brief    设置颜色
         * @param[i] color 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setColor(QColor color) override;
        /**
         * @brief    设置演员类型
         * @param[i] type     -1: 其他 0: 体 1: 面 2: 线 3: 点
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        void setActorType(int type);

    private:
        FITKGraphActor* m_actorHighlight{ nullptr };      ///< 高亮显示Actor
        vtkAppendFilter* m_appendFilter{ nullptr };          ///< 合并过滤器
    };
}

#endif // !_GRAPHVTKOBJECTHIGHLIGHTING_H__
