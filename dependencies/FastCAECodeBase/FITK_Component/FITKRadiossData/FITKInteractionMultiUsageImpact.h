/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITK_INTERACTION_MULTI_USAGE_IMPACT_H
#define FITK_INTERACTION_MULTI_USAGE_IMPACT_H

#include "FITKAbstractInteraction.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss
{
    class FITKRadiossDataAPI FITKInteractionMultiUsageImpactDefaultValue  : public FITKInteractionDefaultAbstract
    {
        Q_OBJECT
            FITKCLASS(Radioss, FITKInteractionMultiUsageImpactDefaultValue)
            RadiossKeyWord(FITKInteractionMultiUsageImpactDefaultValue, /DEFAULT/INTER/TYPE7)
    public:
        explicit FITKInteractionMultiUsageImpactDefaultValue() = default;
        virtual ~FITKInteractionMultiUsageImpactDefaultValue() = default;

        virtual DefaultInteractionType getDefaultInteractionType() override;

        int _Istf{1000}; // 刚度标志 Istf
        int _Igap{1000}; // 间隙单元选项标志 Igap
        int _Ibag{2}; // 气囊通风孔关闭标志 Ibag
        int _Idel{1000}; // 节点退化表面标志 Idel
        int _Irem_gap{ 0 };          //在自碰撞接触的情况下，如果元素大小 <间隙值，则用于停用次要节点的标志。
        int _Irem_i2{ 0 };           //如果在接口类型2中定义了相同的contact对 (节点)，则用于停用辅助节点的标志。
        int _Inacti{ 0 };            //初始穿透情况下的刚度停用标志。
        int _Iform{ 0 };             //摩擦惩罚公式类型。
    };


    //只用于/INTER/TYPE7的可选参数
    struct MultiUsageImpactOptionalData
    {
        /********* _Icurv = 1，2时，添加以下参数 （添加到_Irem_i2后面）*******/
        int node_ID1 = 0;           //第一节点识别              
        int node_ID2 = 0;           //第二个节点标识符 (当Icurv = 1时忽略)

        /********* _Ifric > 0时，添加以下参数 *******/
        double C1 = 0.0;            
        double C2 = 0.0;            
        double C3 = 0.0;            /*摩擦定律系数C1-C6*/
        double C4 = 0.0;            
        double C5 = 0.0;            

        /********* _Ifric > 1时，在C1-C5的基础上额外添加以下参数 *******/
        double C6 = 0.0;            

        /********* _Iadm = 2 时，添加以下参数 *******/
        int NRadm = 0;              //通过90度半径的元素数。
        double Padm = 1.0;          //渗透百分比的标准。
        double Angladm = 0.0;       //角度标准

        /********* _Ithe = 1 时，添加以下参数 *******/
        double Kthe = 0.0;          //热交换系数 (如果fct_IDK = 0，default=0,如果fct_IDK != 0，default=1.0)。
        int fct_IDK = 0;            //具有接触压力的热换热定义的功能标识符。
        double Tint = 0.0;          //接口温度
        int Ithe_form = 0;          //热接触配方标志。
        double AscaleK = 1.0;       //fct_IDK上的绝对比例因子
        double Frad = 0.0;          //辐射因子。
        double Drad = 0.0;          //辐射计算的最大距离。
        double Fheats = 0.0;        //二次摩擦发热系数。
        double Fheatm = 0.0;        //主摩擦发热系数。
    };
    /**
     * @brief 多用途冲击 /inter/type7
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-09-24 
     */
    class FITKRadiossDataAPI FITKInteractionMultiUsageImpact : public FITKAbstractInteraction
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKInteractionMultiUsageImpact)
        RadiossKeyWord(FITKInteractionMultiUsageImpact, /INTER/TYPE7)

    public:
        explicit FITKInteractionMultiUsageImpact();
        virtual ~FITKInteractionMultiUsageImpact();

        virtual InteractionType getInteractionType() override;

        //可选属性值
        MultiUsageImpactOptionalData& getOptionalData();
        // 部分必须参数
        FITKInteractionMultiUsageImpactDefaultValue* getValues();

    public:
        bool _Ithe{ true };           //是否启用热传导或热摩擦
        int _Icurv{ 0 };                //带有曲率的间隙包络线
        int _Iadm{ 0 };                 //计算自适应网格划分的局部曲率标志
        double _Fscale_gap{ 1.0 };      //间隙比例因子 (仅在Igap = 2和3时有效)
        double _GAPmax{ 0.0 };          //最大间隙 (仅在Igap = 2和3时有效)
        double _Fpenmax{ 0.0 };         //初始渗透的最大分数
        int _Itied{ 0 };                //绑定关联点标志
        double _STmin{ 0.0 };           //最小刚度 (仅在Istf = 2、3、4或5时有效)。
        double _STmax{ 1e30 };          //最大刚度 (仅在Istf = 2、3、4或5时有效)。
        double _mesh_size{ 0.4 };       //网格大小的百分比 (仅在Igap = 3时有效)。
        double _dtmin{ 0.0 };           //最小接口时间步长
        double _Stfac{1.0};             //应用于接口主侧的刚度比例因子 (如果为Istf不为1) or 界面刚度 (如果Istf = 1)。
        double _Fric{0.0};              //库仑摩擦 (如果fct_IDF = 0) or 库仑摩擦比例因子 (如果fct_IDF不为0)。 
        double _Gapmin{1.0};            //冲击激活的最小间隙。
        double _Tstart{0.0};            //开始时间
        double _Tstop{0.0};             //暂时停用的时间
        int _Ibc{000};                  //碰撞时边界条件的停用标志。3个位置分别代表碰撞时X、Y、Z边界条件的停用标志。
        double _VisS{0.05};             //临界阻尼系数对界面刚度的影响。
        double _VisF{1.0};              //界面摩擦的临界阻尼系数。
        double _Bumult{0.2};            //排序因子用于加快排序算法，并且与机器有关。
        int _Ifric{0};                  //摩擦配方标志。
        int _Ifiltr{0};                 //摩擦过滤标志。
        double _Xfreq{0.0};             //滤波系数
        int _sens_ID{0};                //用于激活/停用接口的传感器标识符。如果定义了标识符传感器，则接口的激活/去激活基于传感器，而不是基于Tstart或Tstop。
        int _fct_IDf{0};                //带温度函数标识符的摩擦系数。
        double _AscaleF{1.0};           //_fct_IDf的横坐标比例因子
        int _fric_ID{0};                //选定零件对的摩擦定义的摩擦标识符

    private:
        Core::FITKAbstractObjectContainer<FITKInteractionMultiUsageImpactDefaultValue> _defaultValue;
        MultiUsageImpactOptionalData _optionalData; //!< 可选数据
    };

} // namespace Radioss

#endif // FITK_INTERACTION_MULTI_USAGE_IMPACT_H