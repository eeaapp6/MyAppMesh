/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelTransform.h
 * @brief  几何特征变换适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELTRANSFORM_H
#define FITKADAPTORGEOMODELTRANSFORM_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  平移变换适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelTrans : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelTrans() = default;
        virtual ~FITKAdaptorGeoModelTrans() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  通过两点平移适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelTransformByTwoPoints : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelTransformByTwoPoints() = default;
        virtual ~FITKAdaptorGeoModelTransformByTwoPoints() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  通过方向和距离平移适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelTransformByDirAndDis : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelTransformByDirAndDis() = default;
        virtual ~FITKAdaptorGeoModelTransformByDirAndDis() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  旋转变换适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRotate : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRotate() = default;
        virtual ~FITKAdaptorGeoModelRotate() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };

    /**
     * @brief  缩放变换适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelScale : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelScale() = default;
        virtual ~FITKAdaptorGeoModelScale() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  镜像适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelMirror : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelMirror() = default;
        virtual ~FITKAdaptorGeoModelMirror() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  矩形阵列适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRectangularPattern : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRectangularPattern() = default;
        virtual ~FITKAdaptorGeoModelRectangularPattern() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  环形阵列适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCircularPattern : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelCircularPattern() = default;
        virtual ~FITKAdaptorGeoModelCircularPattern() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODELTRANSFORM_H
