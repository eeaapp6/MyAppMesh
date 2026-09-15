/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorNode.h
 * @brief  节点读取适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-11
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORNODE_H__
#define __FITKRADIOSSADAPTORNODE_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossNodes;
    /**
     * @brief  节点读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-11
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorNode : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorNode() = default;
        ~FITKRadiossAdaptorNode() = default;
        /**
         * @brief
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptW() override;
    private:
        /**
         * @brief    
         * @param[i] nodes 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-10
         */
        bool readNodes(FITKRadiossNodes* nodes);
    private:

    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossNodes, FITKRadiossAdaptorNode);
    RegRadiossRadAdaptor(/NODE, FITKRadiossAdaptorNode);
}
#endif //!__FITKRADIOSSADAPTORNODE_H__
