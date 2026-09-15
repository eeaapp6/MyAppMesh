/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModel.h
 * @brief  模型操作适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODEL_H
#define FITKADAPTORGEOMODEL_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

ForwardDeclarNS(Core, FITKAbstractNDataObject);

namespace Interface
{
    /**
     * @brief  模型删除适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelDelete : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelDelete() = default;
        virtual ~FITKAdaptorGeoModelDelete() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  多选模型删除适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelMultiDelete : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelMultiDelete() = default;
        virtual ~FITKAdaptorGeoModelMultiDelete() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  压印体适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoImprintSolid : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoImprintSolid() = default;
        virtual ~FITKAdaptorGeoImprintSolid() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  模型导入适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelImport : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelImport() = default;
        virtual ~FITKAdaptorGeoModelImport() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  模型导出适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelExport : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelExport() = default;
        virtual ~FITKAdaptorGeoModelExport() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  复合体操作适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-29
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCompound : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelCompound() = default;
        virtual ~FITKAdaptorGeoModelCompound() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  拆分平铺装配模式读取几何适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelImportFlattenAssembly : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelImportFlattenAssembly() = default;
        virtual ~FITKAdaptorGeoModelImportFlattenAssembly() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  模型部件命令适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoPart : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoPart() = default;
        virtual ~FITKAdaptorGeoPart() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    private:
        Core::FITKAbstractNDataObject* getCommand(QString enumName);
    };
    /**
     * @brief  从网格生成几何模型适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelFromMesh : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelFromMesh() = default;
        virtual ~FITKAdaptorGeoModelFromMesh() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  修复小边适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelRepairSmallEdges : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelRepairSmallEdges() = default;
        virtual ~FITKAdaptorGeoModelModelRepairSmallEdges() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  修复小面适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelRepairSmallFaces : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelRepairSmallFaces() = default;
        virtual ~FITKAdaptorGeoModelModelRepairSmallFaces() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  拉伸切割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelExtrudeCut : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelExtrudeCut() = default;
        virtual ~FITKAdaptorGeoModelModelExtrudeCut() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  旋转切割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelRevolCut : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelRevolCut() = default;
        virtual ~FITKAdaptorGeoModelModelRevolCut() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  扫略切割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelSweepCut : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelSweepCut() = default;
        virtual ~FITKAdaptorGeoModelModelSweepCut() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
    /**
     * @brief  导出几何部分形状适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelModelExportTopos : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelModelExportTopos() = default;
        virtual ~FITKAdaptorGeoModelModelExportTopos() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;
    };
}

#endif // FITKADAPTORGEOMODEL_H
