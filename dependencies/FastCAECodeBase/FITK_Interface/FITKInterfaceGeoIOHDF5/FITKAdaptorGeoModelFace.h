/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelFace.h
 * @brief  几何面适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-30
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELFACE_H
#define FITKADAPTORGEOMODELFACE_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  桥接曲面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBridgeSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelBridgeSurface() = default;
        virtual ~FITKAdaptorGeoModelBridgeSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  体的面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSolidSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelSolidSurface() = default;
        virtual ~FITKAdaptorGeoModelSolidSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  多截面扫略适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelMultiSectionSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelMultiSectionSurface() = default;
        virtual ~FITKAdaptorGeoModelMultiSectionSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  扫略曲面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSweepSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelSweepSurface() = default;
        virtual ~FITKAdaptorGeoModelSweepSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  旋转曲面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRevolSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRevolSurface() = default;
        virtual ~FITKAdaptorGeoModelRevolSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  拉伸曲面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelExtrudeSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelExtrudeSurface() = default;
        virtual ~FITKAdaptorGeoModelExtrudeSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  偏移曲面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOffsetSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelOffsetSurface() = default;
        virtual ~FITKAdaptorGeoModelOffsetSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  基础几何面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelClosedWireSurface : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelClosedWireSurface() = default;
        virtual ~FITKAdaptorGeoModelClosedWireSurface() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  填补孔洞适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOperFaceFillHoles : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelOperFaceFillHoles() = default;
        virtual ~FITKAdaptorGeoModelOperFaceFillHoles() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  填补缝隙适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOperFaceFillGaps : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelOperFaceFillGaps() = default;
        virtual ~FITKAdaptorGeoModelOperFaceFillGaps() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief   删除悬浮边适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOperFaceDeleteFloatingEdge : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelOperFaceDeleteFloatingEdge() = default;
        virtual ~FITKAdaptorGeoModelOperFaceDeleteFloatingEdge() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };


    /**
     * @brief  覆盖面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-23
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCoverEdges : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelCoverEdges() = default;
        virtual ~FITKAdaptorGeoModelCoverEdges() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  缝合面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-23
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoStitch : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoStitch() = default;
        virtual ~FITKAdaptorGeoStitch() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  替换面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelReplaceFaces : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelReplaceFaces() = default;
        virtual ~FITKAdaptorModelReplaceFaces() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  移除面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelRemoveFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelRemoveFace() = default;
        virtual ~FITKAdaptorModelRemoveFace() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  偏移面适配器
     * @author ChengHaotian (yeguangbaozi@foxmail.com)
     * @date   2026-07-06
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelOffsetFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelOffsetFace() = default;
        virtual ~FITKAdaptorGeoModelOffsetFace() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODELFACE_H
