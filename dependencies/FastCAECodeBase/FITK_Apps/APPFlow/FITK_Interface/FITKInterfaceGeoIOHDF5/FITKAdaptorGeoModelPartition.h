/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelPartition.h
 * @brief  模型分块适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-24
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELPARTITION_H
#define FITKADAPTORGEOMODELPARTITION_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  边分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionEdge : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionEdge() = default;
        virtual ~FITKAdaptorGeoModelPartitionEdge() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  参数边分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionEdgeWithParameter : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionEdgeWithParameter() = default;
        virtual ~FITKAdaptorGeoModelPartitionEdgeWithParameter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
         
    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用点边分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionEdgeWithPoint : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionEdgeWithPoint() = default;
        virtual ~FITKAdaptorGeoModelPartitionEdgeWithPoint() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用基准面边分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionEdgeWithDatumPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionEdgeWithDatumPlane() = default;
        virtual ~FITKAdaptorGeoModelPartitionEdgeWithDatumPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };

    /**
     * @brief  面分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFace() = default;
        virtual ~FITKAdaptorGeoModelPartitionFace() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用草图进行面分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithSketch : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithSketch() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithSketch() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private: 
        // 添加需要的私有成员
    };
    /**
     * @brief  使用两点进行面分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithTwoPoints : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithTwoPoints() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithTwoPoints() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用基准面分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithDatumPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithDatumPlane() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithDatumPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用曲线路径分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithCurvedPath : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithCurvedPath() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithCurvedPath() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用扩展面分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithExtendFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithExtendFace() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithExtendFace() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用相交面分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithIntersectFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithIntersectFace() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithIntersectFace() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用投影边分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionFaceWithProjectEdges : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionFaceWithProjectEdges() = default;
        virtual ~FITKAdaptorGeoModelPartitionFaceWithProjectEdges() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  体分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionSolid() = default;
        virtual ~FITKAdaptorGeoModelPartitionSolid() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用扩展面进行实体分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionSolidWithExtendFace : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionSolidWithExtendFace() = default;
        virtual ~FITKAdaptorGeoModelPartitionSolidWithExtendFace() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用草绘进行实体分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionSolidWithSketchPlanar : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionSolidWithSketchPlanar() = default;
        virtual ~FITKAdaptorGeoModelPartitionSolidWithSketchPlanar() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用扫掠边进行实体分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionSolidWithSweepEdge : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionSolidWithSweepEdge() = default;
        virtual ~FITKAdaptorGeoModelPartitionSolidWithSweepEdge() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  使用平面进行实体分块适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-24
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelPartitionSolidWithPlane : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelPartitionSolidWithPlane() = default;
        virtual ~FITKAdaptorGeoModelPartitionSolidWithPlane() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELPARTITION_H
