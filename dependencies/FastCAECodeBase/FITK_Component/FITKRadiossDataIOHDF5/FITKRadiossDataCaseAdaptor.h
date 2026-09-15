/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataCaseAdaptor.h
 * @brief  Radioss数据HDF5适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef FITKRadiossDataCaseAdaptor_H
#define FITKRadiossDataCaseAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace H5
{
    class Group;
}
namespace Radioss
{
    class FITKRadiossCase;
}
namespace Interface
{
    /**
     * @brief  RadiossData适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-29
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataCaseAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataCaseAdaptor() = default;
        ~FITKRadiossDataCaseAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:
        bool readGeomPart(H5::Group& g);
        bool writeGeomPart(H5::Group& g);

        bool readPart(H5::Group& g);
        bool writePart(H5::Group& g);

        bool readNode(H5::Group& g);
        bool writeNode(H5::Group& g);

        bool readNodeSet(H5::Group& g);
        bool writeNodeSet(H5::Group& g);

        bool readSubSet(H5::Group& g);
        bool writeSubSet(H5::Group& g);

        bool readSurface(H5::Group& g);
        bool writeSurface(H5::Group& g);

        bool readMaterials(H5::Group& g);
        bool writeMaterials(H5::Group& g);

        bool readProperty(H5::Group& g);
        bool writeProperty(H5::Group& g);

        bool readCurve(H5::Group& g);
        bool writeCurve(H5::Group& g);

        bool readFailureModel(H5::Group& g);
        bool writeFailureModel(H5::Group& g);

        bool readEquationOfState(H5::Group& g);
        bool writeEquationOfState(H5::Group& g);

        bool readSolution(H5::Group& g);
        bool writeSolution(H5::Group& g);
    private:
        /**
         * @brief  算例数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        Radioss::FITKRadiossCase* _caseData{};

    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossCase, FITKRadiossDataCaseAdaptor)
}

#endif 

