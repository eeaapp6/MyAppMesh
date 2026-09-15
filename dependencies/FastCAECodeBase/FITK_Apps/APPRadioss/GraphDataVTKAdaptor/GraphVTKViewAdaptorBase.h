/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorBase.h
 * @brief  渲染适配器基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef __GraphVTKViewAdaptorBase_H__
#define __GraphVTKViewAdaptorBase_H__

#include "FITK_Kernel/FITKAdaptor/FITKAbstractViewAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"
#include "GraphDataVTKAdaptorAPI.h"

namespace AbaqusData {
    class FITKDataCase;
}

namespace Exchange{
    class GraphVTKObject3D;
}

namespace Exchange
{
    /**
     * @brief  渲染适配器基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-11
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorBase : public Adaptor::FITKAbstractViewAdaptor
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorBase);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        explicit GraphVTKViewAdaptorBase() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        virtual ~GraphVTKViewAdaptorBase() = default;
        /**
         * @brief    设置数据对象
         * @param[i] data                      数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data) override;
        /**
         * @brief    设置额外参数
         * @param[i] details               参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void setDetails(QVariant details);
        /**
         * @brief    获取渲染数据
         * @return   Exchange::GraphVTKObject3D*             渲染数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        Exchange::GraphVTKObject3D* getOutputData();
        /**
         * @brief    更新
         * @return   bool                                    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        virtual bool update();

    protected:
        /**
         * @brief  额外参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        QVariant _details;
        /**
         * @brief  渲染对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        Exchange::GraphVTKObject3D* _outputData = nullptr;
        /**
         * @brief  是否需要更新
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        bool _needUpdate = false;
    };
}
#endif 
