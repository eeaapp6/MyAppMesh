/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossResultRequest.h
 * @brief  Radioss 结果请求
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-09
 *********************************************************************/
#ifndef __FITKRADIOSSRESULTREQUEST_H__
#define __FITKRADIOSSRESULTREQUEST_H__

#include "FITKRadiossDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Radioss
{

    /**
     * @brief  Radioss 求解设置
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-05
     */
    class FITKRadiossDataAPI FITKRadiossResultRequest : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
        FITKCLASS(Radioss, FITKRadiossResultRequest);
    public:
        /**
         * @brief  节点动画输出类型 /ANIM/VECT/
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        enum NodalOutputAnimationsType
        {
            Nodal_Displacement,                    ///< 位移
            Nodal_Velocity,                        ///< 速度
            Nodal_Acceleration,                    ///< 加速度
            Nodal_ContactForces,                   ///< 加速度
            Nodal_RigidBodyForcesMoments,          ///< 刚体的力和力矩
            Nodal_InternalForces,                  ///< 内部力
            Nodal_ContactPressure,                 ///< 接触压力
            Nodal_StressAllIntegrationPoints,      ///< 所有积分点的应力GPS
            Nodal_StrainAllIntegrationPoints,      ///< 所有积分点的应变GPS
        };
        Q_ENUM(NodalOutputAnimationsType);
        /**
         * @brief  单元动画输出类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        enum ElementalOutputAnimationsType
        {
            Elemental_SpecificEnergyDensity,       ///< 特殊能量密度 （包含砖块元素数据）六面体
            Elemental_PlasticStrain,               ///< 塑性应变 （包含砖块元素数据）六面体
            Elemental_HourglassEnergy,             ///< 沙漏能量 (发动机舱)
            Elemental_VonMisesStress,              ///< 等效应力
            Elemental_Pressure,                    ///< 压力
            Elemental_Density,                     ///< 密度
            Elemental_BeamPlasticStrain,           ///< 塑性应变 梁
            Elemental_BeamVonMisesStress,          ///< 等效应力 梁
            Elemental_BeamFORC,                    ///< 梁的力与力矩
            Elemental_TrussFORC,                   ///< Truss的力与力矩
            Elemental_SpringFORC,                  ///< Spring的力与力矩
        };
        Q_ENUM(ElementalOutputAnimationsType);
        /**
         * @brief  Brick动画输出类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        enum BrickOutputAnimationsType
        {
            Elemental_BrickElementStress,          ///< 体应力
            Elemental_BrickElementStrain,          ///< 体应变
            Elemental_BrickElementTensor,          ///< 体张量
            Elemental_BrickElementEPSP,            ///< 体塑性应变
            Elemental_BrickDamageOfAll,            ///< 实体的损伤
        };
        Q_ENUM(BrickOutputAnimationsType);
        /**
         * @brief  Shell动画输出类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        enum ShellOutputAnimationsType
        {
            Elemental_ShellElementStress,          ///< 壳应力
            Elemental_ShellElementStrain,          ///< 壳应变
            Elemental_ShellElementEPSDOT,          ///< 壳应变率张量
            Elemental_ShellDamageOfAll,            ///< 壳的损伤
            Elemental_ShellPlasticStrain,          ///< 壳的塑性应变
            Elemental_ShellFLDFailure,             ///< 壳的FLD损伤
        };
        Q_ENUM(ShellOutputAnimationsType);

    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        explicit FITKRadiossResultRequest();
        
        /**
         * @brief 析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        virtual ~FITKRadiossResultRequest();
        /**
         * @brief    设置动画开始时间
         * @param[i] time
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setStartTime(const double time);
        /**
         * @brief    获取动画开始时间
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getStartTime() const;

        /**
         * @brief    设置动画频率
         * @param[i] f
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setFrequency(const double f);
        /**
         * @brief    获取动画频率
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getFrequency() const;
        /**
         * @brief    设置时间历程频率
         * @param[i] f
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setHisFrequency(const double f);
        /**
         * @brief    获取时间历程频率
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getHisFrequency() const;
        /**
         * @brief    设置节点数据动画
         * @param[i] anim 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        void setNodalOutputAnim(QList<NodalOutputAnimationsType> anim);
        /**
         * @brief    获取节点数据动画
         * @return   QList<NodalOutputAnimationsType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        QList<NodalOutputAnimationsType> getNodalOutputAnim();
        /**
         * @brief    设置单元数据动画
         * @param[i] anim 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        void setElementalOutputAnim(QList<ElementalOutputAnimationsType> anim);
        /**
         * @brief    获取单元数据动画
         * @return   QList<ElementalOutputAnimationsType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        QList<ElementalOutputAnimationsType> getElementalOutputAnim();
        /**
         * @brief    设置Brick数据动画
         * @param[i] anim
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        void setBrickOutputAnim(QList<BrickOutputAnimationsType> anim);
        /**
         * @brief    获取Brick数据动画
         * @return   QList<ElementalOutputAnimationsType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        QList<BrickOutputAnimationsType> getBrickOutputAnim();
        /**
         * @brief    设置Shell数据动画
         * @param[i] anim
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        void setShellOutputAnim(QList<ShellOutputAnimationsType> anim);
        /**
         * @brief    获取Shell数据动画
         * @return   QList<ElementalOutputAnimationsType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        QList<ShellOutputAnimationsType> getShellOutputAnim();
        /**
         * @brief    清理参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void clearParam();

    private:
        /**
         * @brief  动画开始时间
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        double _startTimeAnims{ 0.0 };
        /**
         * @brief  动画频率
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        double _frequencyAnims{ 0.0005 };
        /**
         * @brief  时间历程频率
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        double _frequencyHistory{ 0.0005 };
        /**
         * @brief  节点动画类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        QList<NodalOutputAnimationsType> _nodalAnims{};
        /**
         * @brief  单元动画类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        QList<ElementalOutputAnimationsType> _elementalAnims{};
        /**
         * @brief  Brick动画类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        QList<BrickOutputAnimationsType> _brickAnims{};
        /**
         * @brief  Shell动画类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-09
         */
        QList<ShellOutputAnimationsType> _shellAnims{};

    };


} // namespace Radioss

#endif // __FITKRADIOSSSOLVERSETTINGS_H__