/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOCCFeature.h
 * @brief  occ几何特征类.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-25
 */
#ifndef FITKOCCFEATURE_H
#define FITKOCCFEATURE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoFeature.h"

namespace OCC {
    /**
         * @brief  occ几何特征类.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
    class FITKGeoCompOCCAPI FITKOCCStiffener :
        public Interface::FITKAbsGeoStiffener, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCStiffener();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCStiffener() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;

    private:
        /**
         * @brief       根据提供的线与形状获取交点，并获取最远交点间的参数值。
         * @param[in]   shape：形状
         * @param[in]   edge：线
         * @param[out]  u1：起始参数[引用]
         * @param[out]  u2：结束参数[引用]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-09
         */
        bool getUByCrossPoint(TopoDS_Shape & shape, TopoDS_Shape & edge, double & u1, double & u2);

    };

}
#endif // FITKOCCFEATURE_H
