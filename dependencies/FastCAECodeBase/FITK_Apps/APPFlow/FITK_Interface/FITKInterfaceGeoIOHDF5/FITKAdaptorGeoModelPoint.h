/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelPoint.h
 * @brief  点适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELPOINT_H
#define FITKADAPTORGEOMODELPOINT_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  坐标点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoPoint : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoPoint() = default;
        virtual ~FITKAdaptorGeoPoint() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
         
    private:
        // 添加需要的私有成员
    };

    /**
     * @brief  曲线端点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoCurveEnd : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoCurveEnd() = default;
        virtual ~FITKAdaptorGeoCurveEnd() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  距离曲线的端点指定距离适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoCurveEndDistance : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoCurveEndDistance() = default;
        virtual ~FITKAdaptorGeoCurveEndDistance() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  指定曲线比例创建点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoCurveRadio : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoCurveRadio() = default;
        virtual ~FITKAdaptorGeoCurveRadio() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  点在线上的投影适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoPointProjectionOnCurve : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoPointProjectionOnCurve() = default;
        virtual ~FITKAdaptorGeoPointProjectionOnCurve() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  两线交点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoTwoCurveIntersection : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoTwoCurveIntersection() = default;
        virtual ~FITKAdaptorGeoTwoCurveIntersection() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  点在面上的投影适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoPointProjectionOnSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoPointProjectionOnSurface() = default;
        virtual ~FITKAdaptorGeoPointProjectionOnSurface() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  线和面的交点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoCurveIntersectionSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoCurveIntersectionSurface() = default;
        virtual ~FITKAdaptorGeoCurveIntersectionSurface() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  在线、面上交互生成点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-23
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoInteractionPoint : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoInteractionPoint() = default;
        virtual ~FITKAdaptorGeoInteractionPoint() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  提取球面或者圆弧的圆心适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-23
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoExtractCenter : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoExtractCenter() = default;
        virtual ~FITKAdaptorGeoExtractCenter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELPOINT_H
