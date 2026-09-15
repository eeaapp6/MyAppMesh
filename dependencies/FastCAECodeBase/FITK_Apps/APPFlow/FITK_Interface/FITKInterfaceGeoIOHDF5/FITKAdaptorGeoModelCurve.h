/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelCurve.h
 * @brief  几何曲线适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELCURVE_H
#define FITKADAPTORGEOMODELCURVE_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  无限直线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelLine : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelLine() = default;
        virtual ~FITKAdaptorGeoModelLine() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  有限线段适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSegment : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelSegment() = default;
        virtual ~FITKAdaptorGeoModelSegment() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  多段线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelWire : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelWire() = default;
        virtual ~FITKAdaptorGeoModelWire() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCircle : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelCircle() = default;
        virtual ~FITKAdaptorGeoModelCircle() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };

    /**
     * @brief  圆弧适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCircleArc : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelCircleArc() = default;
        virtual ~FITKAdaptorGeoModelCircleArc() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  椭圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelEllipse : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelEllipse() = default;
        virtual ~FITKAdaptorGeoModelEllipse() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  椭圆弧适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelEllipseArc : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelEllipseArc() = default;
        virtual ~FITKAdaptorGeoModelEllipseArc() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  双曲线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelHyperbola : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelHyperbola() = default;
        virtual ~FITKAdaptorGeoModelHyperbola() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  双曲线弧适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelHyperbolaArc : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelHyperbolaArc() = default;
        virtual ~FITKAdaptorGeoModelHyperbolaArc() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  抛物线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelParabola : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelParabola() = default;
        virtual ~FITKAdaptorGeoModelParabola() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  抛物弧线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelParabolaArc : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelParabolaArc() = default;
        virtual ~FITKAdaptorGeoModelParabolaArc() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  偏移曲线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOffsetCurve : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelOffsetCurve() = default;
        virtual ~FITKAdaptorGeoModelOffsetCurve() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  通过控制点创建贝塞尔曲线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBeizeByControlPoints : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelBeizeByControlPoints() = default;
        virtual ~FITKAdaptorGeoModelBeizeByControlPoints() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  通过线上的点创建B样条曲线
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBsplineByThroughPoints : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelBsplineByThroughPoints() = default;
        virtual ~FITKAdaptorGeoModelBsplineByThroughPoints() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  点到线或面最短距离的连线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-19
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelNearestPointLine : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelNearestPointLine() = default;
        virtual ~FITKAdaptorGeoModelNearestPointLine() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  曲线采样适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-19
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSampleCurve : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelSampleCurve() = default;
        virtual ~FITKAdaptorGeoModelSampleCurve() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  构造线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-23
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelConstruction : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoModelConstruction() = default;
        virtual ~FITKAdaptorGeoModelConstruction() = default;

    protected:

        virtual bool writeParameters(H5::Group& g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODELCURVE_H
