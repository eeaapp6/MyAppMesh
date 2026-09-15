/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorChangeGraph.h
 * @brief  前后处理渲染视图切换操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-02-14
 *********************************************************************/
#ifndef _OperatorChangeGraph_H___
#define _OperatorChangeGraph_H___
 

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

class QTreeWidgetItem;

namespace Interface
{
    class FITKAbaAmplitudeManager;
}

namespace ModelOper
{
    /**
     * @brief  前后处理渲染视图切换操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-02-14
     */
    class OperatorChangeGraph : public Core::FITKActionOperator
    {
    public:
        explicit OperatorChangeGraph() = default;
        virtual ~OperatorChangeGraph() = default;
        /**
         * @brief    界面逻辑，生成UI交互
         * @return   true             执行成功
         * @return   false            执行失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-02-14
         */
        virtual bool execGUI() override;
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(OperatorChangePreGraph, OperatorChangeGraph);
    Register2FITKOperatorRepo(OperatorChangePostGraph, OperatorChangeGraph);
}

#endif
