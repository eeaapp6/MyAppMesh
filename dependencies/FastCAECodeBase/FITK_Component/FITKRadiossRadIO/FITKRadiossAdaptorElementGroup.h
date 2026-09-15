/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorElementGroup.h
 * @brief  Radioss 元素集合读写适配器声明
 * @author  wangning (2185896382@qq.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef _FITKRADIOSS_ADAPTOR_ELEMENT_GROUP_H__
#define _FITKRADIOSS_ADAPTOR_ELEMENT_GROUP_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"


namespace Radioss
{
    class FITKRadiossElementGroup;
    /**
     * @class  FITKRadiossAdaptorElementGroup
     * @brief  元素集合适配器
     * @author  wangning (2185896382@qq.com)
     * @date   2025-09-28
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorElementGroup : public FITKRadiossAbstractAdaptor
    {
    public:
        /** @brief 默认构造 (无额外初始化) */
        explicit FITKRadiossAdaptorElementGroup() = default;
        /** @brief 默认析构 (无资源需释放) */
        virtual ~FITKRadiossAdaptorElementGroup() = default;

        /**
         * @brief  获取适配器类别标识
         * @return 适配器类别名称
         * @author  wangning (2185896382@qq.com)
         * @date   2025-09-28
         */
        QString getAdaptorClass() override;

        /**
         * @brief  从 Radioss 输入读取数据
         * @return true 读取并解析成功; false 读取/语义校验失败
         * @author  wangning (2185896382@qq.com)
         * @date   2025-09-28
         */
        bool adaptR() override;

        /**
         * @brief  将内部节点集合写出为元素关键字块
         * @return true 写出成功; false 数据不完整或输出失败
         * @author  wangning (2185896382@qq.com)
         * @date   2025-09-28
         */
        bool adaptW() override;

    private:
        /**
         * @brief  读取参数
         * @author  wangning (2185896382@qq.com)
         * @date   2025-09-28
         */
        bool readListParam(QList<int>& idList);

    };


    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossElementGroup, FITKRadiossAdaptorElementGroup);
    /**
     * @brief  注册 Radioss 关键字与适配器映射
     */
    RegRadiossRadAdaptor(/GRSHEL/SHEL, FITKRadiossAdaptorElementGroup );
    RegRadiossRadAdaptor(/GRSHEL/PART, FITKRadiossAdaptorElementGroup, 1);
    RegRadiossRadAdaptor(/GRSHEL/GRSHEL, FITKRadiossAdaptorElementGroup, 2);
    RegRadiossRadAdaptor(/GRSHEL/SUBSET, FITKRadiossAdaptorElementGroup, 3);
    RegRadiossRadAdaptor(/GRSHEL/BOX, FITKRadiossAdaptorElementGroup, 4);
    RegRadiossRadAdaptor(/GRSHEL/GEN_INCR, FITKRadiossAdaptorElementGroup, 5);
    RegRadiossRadAdaptor(/GRSHEL/GENE, FITKRadiossAdaptorElementGroup, 6);
    RegRadiossRadAdaptor(/GRSHEL/MAT, FITKRadiossAdaptorElementGroup, 7);
    RegRadiossRadAdaptor(/GRSHEL/PROP, FITKRadiossAdaptorElementGroup, 8);
    RegRadiossRadAdaptor(/GRSHEL/SUBMODEL, FITKRadiossAdaptorElementGroup, 9);

    //RegRadiossRadAdaptor(/GRQUAD/     , FITKRadiossAdaptorElementGroup, 10);

    RegRadiossRadAdaptor(/GRSH3N/SH3N   , FITKRadiossAdaptorElementGroup, 20);
    RegRadiossRadAdaptor(/GRSH3N/PART, FITKRadiossAdaptorElementGroup, 21);
    RegRadiossRadAdaptor(/GRSH3N/GRSHEL, FITKRadiossAdaptorElementGroup, 22);
    RegRadiossRadAdaptor(/GRSH3N/SUBSET, FITKRadiossAdaptorElementGroup, 23);
    RegRadiossRadAdaptor(/GRSH3N/BOX, FITKRadiossAdaptorElementGroup, 24);
    RegRadiossRadAdaptor(/GRSH3N/GEN_INCR, FITKRadiossAdaptorElementGroup, 25);
    RegRadiossRadAdaptor(/GRSH3N/GENE, FITKRadiossAdaptorElementGroup, 26);
    RegRadiossRadAdaptor(/GRSH3N/MAT, FITKRadiossAdaptorElementGroup, 27);
    RegRadiossRadAdaptor(/GRSH3N/PROP, FITKRadiossAdaptorElementGroup, 28);
    RegRadiossRadAdaptor(/GRSH3N/SUBMODEL, FITKRadiossAdaptorElementGroup, 29);

    //RegRadiossRadAdaptor(/GRBRIC/     , FITKRadiossAdaptorElementGroup, 30);
    RegRadiossRadAdaptor(/GRBRIC/BRIC, FITKRadiossAdaptorElementGroup, 30);
    RegRadiossRadAdaptor(/GRBRIC/PART, FITKRadiossAdaptorElementGroup, 31);
    RegRadiossRadAdaptor(/GRBRIC/GRSHEL, FITKRadiossAdaptorElementGroup, 32);
    RegRadiossRadAdaptor(/GRBRIC/SUBSET, FITKRadiossAdaptorElementGroup, 33);
    RegRadiossRadAdaptor(/GRBRIC/BOX, FITKRadiossAdaptorElementGroup, 34);
    RegRadiossRadAdaptor(/GRBRIC/GEN_INCR, FITKRadiossAdaptorElementGroup, 35);
    RegRadiossRadAdaptor(/GRBRIC/GENE, FITKRadiossAdaptorElementGroup, 36);
    RegRadiossRadAdaptor(/GRBRIC/MAT, FITKRadiossAdaptorElementGroup, 37);
    RegRadiossRadAdaptor(/GRBRIC/PROP, FITKRadiossAdaptorElementGroup, 38);
    RegRadiossRadAdaptor(/GRBRIC/SUBMODEL, FITKRadiossAdaptorElementGroup, 39);



    //RegRadiossRadAdaptor(/GRTRIA/     , FITKRadiossAdaptorElementGroup, 40);
}


#endif ///< _FITKRADIOSS_ADAPTOR_NODE_GROUP_H__