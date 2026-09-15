/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectPostModel.h
 * @brief  后处理模型可视化对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-10
 *********************************************************************/

#ifndef _GRAPHVTKOBJECTPOSTMODEL_H__
#define _GRAPHVTKOBJECTPOSTMODEL_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObjectPostModelBase.h"

namespace Exchange
{
    /**
     * @brief  后处理模型可视化对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-10
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectPostModel : public GraphVTKObjectPostModelBase
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObjectPostModel, GraphVTKObjectPostModelBase);
        FITKCLASS(Exchange, GraphVTKObjectPostModel);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        GraphVTKObjectPostModel(Interface::FITKAbstractStructuralPostVTK* postData);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        virtual ~GraphVTKObjectPostModel();
        /**
         * @brief    获取色带演员
         * @return   vtkScalarBarActor *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        vtkScalarBarActor* getScalarBarActor() override;
    private:

    };
}
#endif // !_GRAPHVTKOBJECTPOSTMODEL_H__
