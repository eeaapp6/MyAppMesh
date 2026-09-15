/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorNodeGroup.h
 * @brief  Radioss 节点集合 (/GRNOD/type) 读写适配器声明
 * @author  libaojun
 * @date   2025-08-20
 * @details
 * 负责在 Radioss 关键字块 /GRNOD/NODE 与内部 FITKModelSet 数据对象之间
 * 的数据交换（读取与写出）。通过工厂注册实现按需动态创建。
 *********************************************************************/
#ifndef _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__
#define _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"


namespace Radioss
{
    class FITKRadiossNodeGroup;
    /**
     * @class  FITKRadiossAdaptorNodeGroup
     * @brief  节点集合适配器
     * @author libaojun
     * @date   2025-08-20
     * @details
     *  - adaptR(): 解析输入流中 /GRNOD/type 关键字段, 构建或填充内部模型集合
     *  - adaptW(): 将内部模型集合序列化为 Radioss 关键字格式
     *  - getAdaptorClass(): 返回用于日志/调试的适配器类别名称
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorNodeGroup : public FITKRadiossAbstractAdaptor
    {
    public:
        /** @brief 默认构造 (无额外初始化) */
        explicit FITKRadiossAdaptorNodeGroup() =default;
        /** @brief 默认析构 (无资源需释放) */
        virtual ~FITKRadiossAdaptorNodeGroup() =default;

        /**
         * @brief  获取适配器类别标识
         * @return 适配器类别名称
         * @author  libaojun
         * @date   2025-08-20
         */
        QString getAdaptorClass() override;

        /**
         * @brief  从 Radioss 输入读取 /GRNOD/type 数据
         * @return true 读取并解析成功; false 读取/语义校验失败
         * @author  libaojun
         * @date   2025-08-20
         */
        bool adaptR() override;

        /**
         * @brief  将内部节点集合写出为 /GRNOD/type 关键字块
         * @return true 写出成功; false 数据不完整或输出失败
         * @author  libaojun
         * @date   2025-08-20
         */
        bool adaptW() override;

    };


    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossNodeGroup, FITKRadiossAdaptorNodeGroup);
    /**
     * @brief  注册 Radioss 关键字与适配器映射 (/GRNOD/type)
     */
    RegRadiossRadAdaptor(/GRNOD/NODE, FITKRadiossAdaptorNodeGroup);
    RegRadiossRadAdaptor(/GRNOD/PART, FITKRadiossAdaptorNodeGroup, 1);
    RegRadiossRadAdaptor(/GRNOD/SURF, FITKRadiossAdaptorNodeGroup, 2);
    RegRadiossRadAdaptor(/GRNOD/GRNOD, FITKRadiossAdaptorNodeGroup, 3);
}


#endif ///< _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__