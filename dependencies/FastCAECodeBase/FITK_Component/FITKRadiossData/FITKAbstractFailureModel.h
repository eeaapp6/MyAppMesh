/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractFailureModel.h
 * @brief  失效模型基类声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef FITK_ABSTRACT_FAILUREMODEL_H
#define FITK_ABSTRACT_FAILUREMODEL_H

#include "FITKRadiossDataAPI.h"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
    class FITKRadiossFailureModelManager;
    /**
     * @brief   Radioss失效模型基类
     * @details 此类定义了Radioss失效模型的通用接口。
     *          所有Radioss失效模型类型都需要继承此类。
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-11-10
     */
    class FITKRadiossDataAPI FITKAbstractFailureModel
        : public Core::FITKAbstractNDataObject, public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum FailureModelType
        {
            None = -1,       ///< 无
            ALTER, 
            BIQUAD,     ///< 这种失效模型使用一种简化的非线性、基于应变、带有线性损伤累积的失效准则
            CHANG,
            COCKCROFT,
            CONNECT,
            ECM,
            ENERGY,
            FABRIC,
            FLD,
            GURSON,
            HASHIN,
            HC_DSSE,
            JOHNSON,     ///< 描述了Johnson-Cook失效模型的失效准则。
            LAD_DAMA,
            MULLINS_OR,
            NXT,
            ORTHOBIQUAD,//网页文档没有该类型？
            ORTHSTRAIN,
            PUCK,
            SNCONNECT,
            SPALLING,
            TAB1,
            TBUTCHER,
            TENSSTRAIN,
            USERi,
            VISUAL,
            WIERZBICKI,
            WILKINS,
        }; Q_ENUM(FailureModelType)

    public:
        explicit FITKAbstractFailureModel();
        virtual ~FITKAbstractFailureModel() = 0;
        /**
         * @brief    获取失效模型类型
         * @return   FailureModelType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-10
         */
        virtual FITKAbstractFailureModel::FailureModelType getFailureModelType() = 0;
        /**
        * @brief 获取材料数量
        */
        int getMaterialIDCount();
        /**
         * @brief    添加材料ID,检测其他失效模型中是否有该材料ID，若存在，则移除
         */
        void addMaterialID(int matID, FITKRadiossFailureModelManager*mgr = nullptr);
        /**
         * @brief    获取材料ID
         */
        QList<int> getMaterialIDs();
        /**
         * @brief 移除材料ID
         */
        void removeMaterialID(int matID);
        /**
         * @brief 检查材料ID是否存在,存在返回true
         */
        bool containsMaterialID(int matID);
        /**
         * @brief 清空所有材料ID
         */
        void clearMaterialIDs();

    protected: 
        /**
         * @brief  材料ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        QList<int> m_matIDs{};
    };
    
    /**
     * @brief  Radioss失效模型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-10
     */
    class FITKRadiossDataAPI FITKRadiossFailureModelManager :
        public Core::FITKAbstractDataManager<FITKAbstractFailureModel>
    {
    public:
        explicit FITKRadiossFailureModelManager()= default;

        virtual ~FITKRadiossFailureModelManager() = default;

    };

} // namespace Radioss

#endif // FITK_ABSTRACT_FAILUREMODEL_H
