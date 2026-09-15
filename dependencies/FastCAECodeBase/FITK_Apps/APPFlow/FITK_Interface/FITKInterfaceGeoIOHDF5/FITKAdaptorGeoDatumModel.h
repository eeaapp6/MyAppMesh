/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoDatum.h
 * @brief  基准元素适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-05-27
 *********************************************************************/
#ifndef FITKADAPTORGEODATUMMODEL_H
#define FITKADAPTORGEODATUMMODEL_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  基准点适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoDatumPoint : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoDatumPoint() = default;
        virtual ~FITKAdaptorGeoDatumPoint() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  基准线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoDatumLine : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoDatumLine() = default;
        virtual ~FITKAdaptorGeoDatumLine() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  平面相交基准线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelReferenceLineByIntersectPlanes : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelReferenceLineByIntersectPlanes() = default;
        virtual ~FITKAdaptorModelReferenceLineByIntersectPlanes() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  平旋转基准线适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelReferenceLineByRotateLine : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelReferenceLineByRotateLine() = default;
        virtual ~FITKAdaptorModelReferenceLineByRotateLine() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  基准面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoDatumPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoDatumPlane() = default;
        virtual ~FITKAdaptorGeoDatumPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  偏移参考面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoReferenceOffsetPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoReferenceOffsetPlane() = default;
        virtual ~FITKAdaptorGeoReferenceOffsetPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  三点参考面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoReferenceThreePointsPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoReferenceThreePointsPlane() = default;
        virtual ~FITKAdaptorGeoReferenceThreePointsPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  方程（Ax+By+Cz+D=0）创建参考面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoReferenceEquationPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoReferenceEquationPlane() = default;
        virtual ~FITKAdaptorGeoReferenceEquationPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  点和法线创建参考面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoReferencePointAndDirectionPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoReferencePointAndDirectionPlane() = default;
        virtual ~FITKAdaptorGeoReferencePointAndDirectionPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  旋转方式创建基准面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelReferenceRotateFromPlanePlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelReferenceRotateFromPlanePlane() = default;
        virtual ~FITKAdaptorModelReferenceRotateFromPlanePlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEODATUMMODEL_H
