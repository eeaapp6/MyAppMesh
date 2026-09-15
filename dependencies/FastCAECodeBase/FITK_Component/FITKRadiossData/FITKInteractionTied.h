/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKInteractionTied.h
 * @brief  动力学绑定类定义 inter/type2
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-09-12
 * 
 */
#ifndef FITK_INTERACTION_TIED_H_Radioss
#define FITK_INTERACTION_TIED_H_Radioss

#include "FITKAbstractInteraction.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss
{
    /**
     * @brief 动力学绑定默认类定义 /DEFAULT/INTER/TYPE2
     * @author wangning (2185896382@qq.com)
     * @date 2025-09-18
     */
    class FITKRadiossDataAPI FITKInteractionKinematicTiedDefault : public FITKInteractionDefaultAbstract
    {
        Q_OBJECT
            FITKCLASS(Radioss, FITKInteractionKinematicTiedDefault)
            RadiossKeyWord(FITKInteractionKinematicTiedDefault, /DEFAULT/INTER/TYPE2)
    public:
        explicit FITKInteractionKinematicTiedDefault() = default;
        virtual ~FITKInteractionKinematicTiedDefault() = default;

        virtual DefaultInteractionType getDefaultInteractionType() override;

        /**
         * @brief 设置/获取 忽略标志
         * @param flag 忽略标志
         */
        void setIgnoreFlag(int flag);
        int getIgnoreFlag() const;
        /**
         * @brief 设置/获取 点焊公式标志
         * @param flag 点焊公式标志
         */
        void setSpotWeldFormulationFlag(int flag);
        int getSpotWeldFormulationFlag() const;
        /**
         * @brief 设置/获取 搜索公式标志
         * @param flag 搜索公式标志
         */
        void setSearchFormulationFlag(int flag);
        int getSearchFormulationFlag() const;
        /**
         * @brief 设置/获取 节点删除阈值/标志
         * @param flag 节点删除标志
         */
        void setNodeDeletionFlag(int flag);
        int getNodeDeletionFlag() const;
        /**
         * @brief 设置/获取 接口刚度定义标志
         * @param flag 接口刚度定义标志
         */
        void setIstfFlagDefault(int flag);
        int getIstfFlagDefault() const;

    private:
        int m_ignoreFlag{ 0 };
        int m_spotWeldFormulationFlag{ 0 };
        int m_searchFormulationFlag{ 2 };
        int m_nodeDeletionFlag{ 1000 };

        //只有m_spotWeldFormulationFlag=25，27，28时有效,m_IstfFlag_Default只用于TiedDefault，Tie中使用TiedOptionalData.Istf
        int m_IstfFlag_Default{ 0 };
    };

    //只用于/INTER/TYPE2的可选参数
    struct TiedOptionalData
    {
        /********* m_spotWeldFormulationFlag=20，21，22时，添加以下参数 *******/
        int Rupt = 0;              //Failure model 
        int Ifilter = 0;           //filter flag
        int fct_IDsr = 0;           //fct_IDsr is the input variable coefficient（Stress factor versus stress rate function）曲线ID
        int fct_IDsn = 0;           //fct_IDsn is the input stress-displacement functions(Max normal stress versus normal relative displacement function)曲线ID
		int fct_IDst = 0;           //fct_IDst is the input stress-displacement functions(Max tangential stress versus tangential relative displacementfunction)曲线ID
        int Isym = 0;               //非对称破裂标志
        double Max_N_Dist = 1e20;   //最大法线相对位移
        double Max_T_Dist = 1e20;   //最大切向相对位移
        double Fscale_stress = 1.0; //应力比例因子
        double Fscale_str_rate =1.0;//应力速率比例因子
        double Fscale_dist = 1.0;   //距离比例因子
        double Alpha = 1.0;         //应力滤波器alpha值
        double Area = 0.0;          //当从次要节点侧计算的面积为空或次要节点仅连接到一维元素时使用的表面面积

        /********* m_spotWeldFormulationFlag=25，27，28时，添加以下参数 *******/
        double Stfac=1.0;           //刚度系数
        double Visc=0.05;           //临界阻尼系数对界面刚度的影响
        int Istf=0;                 //接口刚度定义标志
     };
    /**
     * @brief 动力学绑定类定义 inter/type2
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKInteractionKinematicTied : public FITKAbstractInteraction
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKInteractionKinematicTied)
        RadiossKeyWord(FITKInteractionKinematicTied, /INTER/TYPE2)
    public:
        explicit FITKInteractionKinematicTied();
        virtual ~FITKInteractionKinematicTied();
        /**
         * @brief 获取相互作用类型 
         * @return InteractionType 
         * @author LiBaojun
         * @date 2025-09-12
         */
        virtual InteractionType getInteractionType() override;

        /**
        * @brief 获取其它必须属性值
        * @return FITKInteractionKinematicTiedDefault* 属性值指针
        */
        FITKInteractionKinematicTiedDefault* getValue();

        /**
        * @brief 获取可选属性值
        * @return FITKMaterialPlasTabData 
        */
        TiedOptionalData& getOptionalData();

        /**
         * @brief 设置层级（Level）
         * @param level 层级
         */
        void setLevel(int level);
        /**
         * @brief 获取层级（Level）
         * @return 层级
         */
        int getLevel() const;

        /**
         * @brief 设置搜索距离
         * @param dist 搜索距离
         */
        void setSearchDistance(double dist);
        /**
         * @brief 获取搜索距离
         * @return 搜索距离
         */
        double getSearchDistance() const;

    private:    
        Core::FITKAbstractObjectContainer<FITKInteractionKinematicTiedDefault> _value{}; //!< 其它必须属性值
        TiedOptionalData _optionalData; //!< 可选数据
        int m_level{0};
        double m_searchDistance{0.0};

    };

} // namespace Radioss

#endif // FITK_INTERACTION_KINEMATIC_TIED_H_Radioss