/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPropBeam.h
 * @brief  梁属性数据声明 描述梁在扭转、弯曲、膜内或轴向变形下的性能。
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-23
 *********************************************************************/
#ifndef FITK_PROP_BEAM_H
#define FITK_PROP_BEAM_H

#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss
{
    /**
     * @brief   梁单元属性值类
     * @details 该类用于描述Radioss中梁单元的属性值信息，继承自FITKPropValueAbstract
     *          对应Radioss关键字/PROP/TYPE3（BEAM），用于定义梁单元的各种属性参数
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-10-23
     */
    class FITKRadiossDataAPI FITKPropValueBeam : public FITKPropValueAbstract,
        public FITKAbstractRadiossData
    {
    public:
        explicit FITKPropValueBeam() = default;
        virtual ~FITKPropValueBeam() = default;
        /**
         * @brief    设置小应变选项
         * @param[i] smallStrainOption 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-23
         */
        void setSmallStrainOption(int smallStrainOption);
        /**
         * @brief    获取小应变选项
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-23
         */
        int getSmallStrainOption();
        /**
         * @brief    设置梁膜阻尼
         * @param[i] beamMembraneDamping 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-23
         */
        void setBeamMembraneDamping(double beamMembraneDamping);
        double getBeamMembraneDamping();

        void setBeamFlexuralDamping(double beamFlexuralDamping);
        double getBeamFlexuralDamping();

        void setCrossSection(double crossSection);
        double getCrossSection();

        void setAreaMomentInertiaIyy(double areaMomentInertiaIyy);
        double getAreaMomentInertiaIyy();

        void setAreaMomentInertiaIzz(double areaMomentInertiaIzz);
        double getAreaMomentInertiaIzz();

        void setAreaMomentInertiaIxx(double areaMomentInertiaIxx);
        double getAreaMomentInertiaIxx();

        void setNeglectsShear(bool isNeglects);
        bool isNeglectsShear();

        /**
        * @brief    设置节点1，节点2的旋转自由度
        * @param[i] index：0-2分别代表X、Y、Z方向
        * @author   wangning
        * @date     2025-10-24
        */
        void setRotationDOF1(int index, bool is);
        bool isRotationDOF1(int index);
        void setRotationDOF2(int index, bool is);
        bool isRotationDOF2(int index);

    private:
        /**
         * @brief  小应变选项：0 1（开始从0开始的小应变公式） 2 3 4（全几何非线性）
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        int _smallStrainOption{ 0 };//Ismstr
        /**
         * @brief  梁膜阻尼
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _beamMembraneDamping{ 0.0 };//dm
        /**
         * @brief  梁弯曲阻尼
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _beamFlexuralDamping{ 0.01 };//df
        /**
         * @brief  横截面
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _crossSection{ 0.0 };//Area
        /**
         * @brief  截面惯性矩Iyy
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _areaMomentInertiaIyy{ 0.0 };
        /**
         * @brief  截面惯性矩Izz
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _areaMomentInertiaIzz{ 0.0 };
        /**
         * @brief  截面惯性矩Ixx
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        double _areaMomentInertiaIxx{ 0.0 };
        /**
         * @brief  梁公式是否忽略剪贴
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        bool _isNeglectsShear{ false };//Ishear
        // @brief  节点1旋转自由度(1代表自由，0代表限制)
        bool _rotationDOF1[3]{ true, true, true };
        // @brief  节点2旋转自由度(1代表自由，0代表限制)
        bool _rotationDOF2[3]{ true, true, true };
    };

    /**
     * @brief   梁单元属性类
     * @details 该类用于描述Radioss中梁单元的属性信息，继承自FITKPropAbstract和FITKAbstractRadiossData
     *          对应Radioss关键字/PROP/TYPE13（SPR_BEAM），用于定义梁单元的各种属性参数
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-10-23
     */
    class FITKRadiossDataAPI FITKPropBeam : public FITKPropAbstract,
        public FITKAbstractRadiossData 
    {
        FITKCLASS(Radioss, FITKPropBeam); //!< FITK类标识
        RadiossKeyWord(FITKPropBeam,/PROP/TYPE3) //!< Radioss关键字标识
    public:
        explicit FITKPropBeam();
        virtual ~FITKPropBeam();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();
        /**
         * @brief    获取属性
         * @return   FITKPropValueBeam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-23
         */
        FITKPropValueBeam* getValue();

    private:
        /**
         * @brief  属性值
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        Core::FITKAbstractObjectContainer<FITKPropValueBeam> _value{}; //!< 属性值
    };





}

#endif