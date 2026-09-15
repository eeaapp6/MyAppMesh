/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ABS_GEOOPER_BOOL_H___
#define _FITK_ABS_GEOOPER_BOOL_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    class FITKAbsGeoShapeAgent;

    /**
     * @brief 球体模型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoOperBool :
        public  FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoOperBool);
        Q_OBJECT
    public:
        enum GeoBoolOperType
        {
            GBTNone,
            GBTCommon,
            GBTCut,
            GBTAdd,
        };
        Q_ENUM(GeoBoolOperType)

    public:
        explicit FITKAbsGeoOperBool() = default;
        virtual ~FITKAbsGeoOperBool();


        void setBoolOperType(GeoBoolOperType t);
        GeoBoolOperType getBoolOperType();

        void setGeoShape1(FITKAbsGeoShapeAgent* s1);
        void setGeoShape2(FITKAbsGeoShapeAgent* s2);

        FITKAbsGeoShapeAgent* getGeoShape1();
        FITKAbsGeoShapeAgent* getGeoShape2();

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-01
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape target() const { return m_Target; }
        void setTarget(VirtualShape shape) { m_Target = shape; }

        VirtualShape tool() const { return m_Tool; }
        void setTool(VirtualShape shape) { m_Tool = shape; }

    protected:
        GeoBoolOperType _boolOperType{ GBTNone };

        FITKAbsGeoShapeAgent* _shape1{};
        FITKAbsGeoShapeAgent* _shape2{};

        VirtualShape m_Target{};
        VirtualShape m_Tool{};
    };
}


#endif
