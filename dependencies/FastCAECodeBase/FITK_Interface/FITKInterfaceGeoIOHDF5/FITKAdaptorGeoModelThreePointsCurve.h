/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelThreePointsCurve.h
 * @brief  三点线适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELTHREEPOINTSCURVE_H
#define FITKADAPTORGEOMODELTHREEPOINTSCURVE_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  三点圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelThreePointsCircle : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelThreePointsCircle() = default;
        virtual ~FITKAdaptorGeoModelThreePointsCircle() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  三点椭圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelThreePointsEllipse : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelThreePointsEllipse() = default;
        virtual ~FITKAdaptorGeoModelThreePointsEllipse() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  三点双曲线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelThreePointsHyperbolae : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelThreePointsHyperbolae() = default;
        virtual ~FITKAdaptorGeoModelThreePointsHyperbolae() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  三点抛物线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelThreePointsParabola : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelThreePointsParabola() = default;
        virtual ~FITKAdaptorGeoModelThreePointsParabola() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  线在面上的投影适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCurveProjectionOnSurface : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelCurveProjectionOnSurface() = default;
        virtual ~FITKAdaptorGeoModelCurveProjectionOnSurface() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  裁剪曲线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelTrimmedCurve : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelTrimmedCurve() = default;
        virtual ~FITKAdaptorGeoModelTrimmedCurve() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  面和面的交线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSurfaceIntersectionSurface : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelSurfaceIntersectionSurface() = default;
        virtual ~FITKAdaptorGeoModelSurfaceIntersectionSurface() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  桥接曲线
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBridgeCurve : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelBridgeCurve() = default;
        virtual ~FITKAdaptorGeoModelBridgeCurve() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  面的边线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSurfaceEdge : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelSurfaceEdge() = default;
        virtual ~FITKAdaptorGeoModelSurfaceEdge() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  线操作适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOperLine : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelOperLine() = default;
        virtual ~FITKAdaptorGeoModelOperLine() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODELTHREEPOINTSCURVE_H
