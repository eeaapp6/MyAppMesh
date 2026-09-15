/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKConnectionRBODY.h
 * @brief  刚体连接
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef __FITK_CONNECTION_RBODY_H__
#define __FITK_CONNECTION_RBODY_H__

#include "FITKAbstractConnection.h"

namespace Radioss
{
    ///RBODY 可选数据（当m_Ifail=1需用）
    struct RBODY_OptionData
    {
        double FN = 1e20;    //失效时的法向力。
        double FT = 1e20;    //失效时的切向力。
        int expN = 2;        //法线方向的失效指数参数。
        int expT = 2;        //切线方向的失效指数参数
    };
     /**
      * @brief  刚体连接类
      * @author liuzhonghua (liuzhonghuaszch@163.com)
      * @date   2025-09-24
      */
    class FITKRadiossDataAPI FITKConnectionRBODY : public FITKAbstractConnection
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKConnectionRBODY)
        RadiossKeyWord(FITKConnectionRBODY, /RBODY)

    public:
        enum RbodyIspherType
        {
            Ispher_DEF = 0,       //< 默认惯性：自动Ispher_Auto
            Ispher_Spherical = 1, //< 惯性设为球面。
            Ispher_Auto = 2,      //< 惯性为自动，惯性将被计算并自动纠正
            Ispher_Input = 3,     //< 惯性为输入，不会变化
        };
        Q_ENUM(RbodyIspherType);
        enum GravityCenterComputationType
        {
            ICOG_DEF = 0,                        //< 默认重心计算：ICOG_MainSecondaryNodeCoord
            ICOG_MainSecondaryNodeCoord = 1,     //< 质量和惯性在主节点坐标处添加；重心是通过主节点和次节点坐标计算得出的，主节点被移动到计算出的重心位置。
            ICOG_SecondaryMass = 2,              //< 重心仅通过考虑次级节点质量来计算；主节点被移动到计算出的重心位置，添加的质量和惯性被放置在重心处。
            ICOG_MainNode = 3,                   //< 重心设置在主节点坐标上；附加质量和惯性放置在主节点坐标上；次节点的质量和惯性传递到重心。主节点不移动。
            ICOG_MainNodeIgnoredSecondary = 4,   //< 重心设置在主节点坐标上；附加质量和惯性置于重心上。忽略次节点的质量和惯性。主节点不移动。
        };
        Q_ENUM(GravityCenterComputationType);
    public:
        explicit FITKConnectionRBODY() =default;
        virtual ~FITKConnectionRBODY();
        /**
         * @brief    获取类型
         * @return   ConnectionType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        virtual ConnectionType getType() const override;

        void setMainNodeID(int nodeID);
        int getMainNodeID();

        void setSensorID(int sensorID);
        int getSensorID();

        void setSystemID(int sysID);
        int getSystemID();

        void setInertiaType(RbodyIspherType type);
        RbodyIspherType getInertiaType();

        void setMass(double mass);
        double getMass();

        void setSecondaryNodesGroupID(int nodeSetID);
        int getSecondaryNodesGroupID();

        void setDeactivationRigidWall(bool isDeact);
        bool getDeactivationRigidWall();

        void setGravityCenterComputation(GravityCenterComputationType type);
        GravityCenterComputationType getGravityCenterComputation();

        void setEnvelopeSurface(int surfID);
        int getEnvelopeSurface();

        void setInertiaValue(double* data);
        void getInertiaValue(double* data);

        void setDomainDecomposition(bool isDom);
        bool getDomainDecomposition();

        void setIfail(bool is);
        bool getIfail();

        //获取可选数据
        RBODY_OptionData& getOptionData();

    private:
        /**
         * @brief  主节点ID  RBID/node_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_mainNodeID{ -1 };
        /**
         * @brief  传感器ID  ISENS/sens_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_sensorID{ -1 };
        /**
         * @brief  局部坐标系的ID   NSKEW/Skew_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_systemID{ -1 };
        /**
         * @brief  惯性类型  ISPHER/Ispher
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        RbodyIspherType m_inertiaType{ RbodyIspherType::Ispher_DEF };
        /**
         * @brief  质量  Mass
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        double m_mass{ 0.0 };
        /**
         * @brief  节点集合ID（次要节点集）  Gnod_id/grnd_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_nodeSetID{ -1 };
        /**
         * @brief  是否停用刚性墙 false：从刚性墙上移除改次要节点，true：请勿从刚性墙上移除刚体次节点。  IKREM/Ikrem
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        bool m_isDeactivationRigidWall{ false };
        /**
         * @brief  重心计算类型  ICoG
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        GravityCenterComputationType m_gravityCenter{ ICOG_DEF };
        /**
         * @brief  定义刚性包络面  surf_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_surfID{ -1 };
        /**
         * @brief  惯力：JXX，JYY，JZZ，JXY，JYZ，JXZ
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        double m_inertiaValue[6]{ 0.0 };
        /**
         * @brief  是否考虑领域分解负载均衡。 Ioptoff
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        bool m_isDomainDecomposition{ false };

        //失效标准 Iexpams/Ifail
        bool m_Ifail{ false };

        //可选数据
        RBODY_OptionData _optionData;
    };
}


#endif