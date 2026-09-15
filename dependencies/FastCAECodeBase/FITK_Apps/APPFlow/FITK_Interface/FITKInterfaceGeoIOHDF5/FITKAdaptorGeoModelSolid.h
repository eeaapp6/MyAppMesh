/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelSolid.h
 * @brief  几何体适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-30
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELSOLID_H
#define FITKADAPTORGEOMODELSOLID_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  倒角适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelChamferSolid : public FITKAdaptorGeoCommand
    {
    public:
        
        explicit FITKAdaptorGeoModelChamferSolid() = default;
        virtual ~FITKAdaptorGeoModelChamferSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  删除倒角适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRemoveChamferSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRemoveChamferSolid() = default;
        virtual ~FITKAdaptorGeoModelRemoveChamferSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  倒圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelFilletSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelFilletSolid() = default;
        virtual ~FITKAdaptorGeoModelFilletSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  删除倒圆适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRemoveFilletSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRemoveFilletSolid() = default;
        virtual ~FITKAdaptorGeoModelRemoveFilletSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  特征移除适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelDefeatureSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelDefeatureSolid() = default;
        virtual ~FITKAdaptorGeoModelDefeatureSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  填补孔洞适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelFillHoleSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelFillHoleSolid() = default;
        virtual ~FITKAdaptorGeoModelFillHoleSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  封闭曲面形成的体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelClosedSurfaceSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelClosedSurfaceSolid() = default;
        virtual ~FITKAdaptorGeoModelClosedSurfaceSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  拉伸体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelExtrudeSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelExtrudeSolid() = default;
        virtual ~FITKAdaptorGeoModelExtrudeSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  旋转体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelRevolSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelRevolSolid() = default;
        virtual ~FITKAdaptorGeoModelRevolSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  扫略体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSweepSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelSweepSolid() = default;
        virtual ~FITKAdaptorGeoModelSweepSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  多截面扫略体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-30
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelMultiSectionSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelMultiSectionSolid() = default;
        virtual ~FITKAdaptorGeoModelMultiSectionSolid() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
     /**
      * @brief  肋特征适配器
      * @author guqingtao (15598887859@163.com)
      * @date   2025-04-30
      */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelStiffener : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelStiffener() = default;
        virtual ~FITKAdaptorGeoModelStiffener() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  圆孔适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCircularHole : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelCircularHole() = default;
        virtual ~FITKAdaptorGeoModelCircularHole() = default;
    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODELSOLID_H
