/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelSplitter.h
 * @brief  模型分割适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-05-22
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELSPLITTER_H
#define FITKADAPTORGEOMODELSPLITTER_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  线分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-22
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelCurveSplitter : public FITKAdaptorGeoCommand
    {
    public:
        
        explicit FITKAdaptorGeoModelCurveSplitter() = default;
        virtual ~FITKAdaptorGeoModelCurveSplitter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  面分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-22
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSurfaceSplitter : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelSurfaceSplitter() = default;
        virtual ~FITKAdaptorGeoModelSurfaceSplitter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
    /**
     * @brief  体分割适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-05-22
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSolidSplitter : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorGeoModelSolidSplitter() = default;
        virtual ~FITKAdaptorGeoModelSolidSplitter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELSPLITTER_H
