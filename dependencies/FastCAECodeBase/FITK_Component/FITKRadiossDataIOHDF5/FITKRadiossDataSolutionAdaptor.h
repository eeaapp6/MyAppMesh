/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataSolutionAdaptor.h
 * @brief  求解数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-04
 *********************************************************************/
#ifndef FITKRadiossDataSolutionAdaptor_H
#define FITKRadiossDataSolutionAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace H5
{
    class Group;
}
namespace Radioss
{
    class FITKRadiossSolution;
}
namespace Interface
{
    /**
     * @brief  求解数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-04
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataSolutionAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataSolutionAdaptor() = default;
        ~FITKRadiossDataSolutionAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

        bool readInteraction(H5::Group& g);
        bool writeInteraction(H5::Group& g);

        bool readInitialField(H5::Group& g);
        bool writeInitialField(H5::Group& g);

        bool readGravity(H5::Group& g);
        bool writeGravity(H5::Group& g);

        bool readRigidWall(H5::Group& g);
        bool writeRigidWall(H5::Group& g);

        bool readSolverSetting(H5::Group& g);
        bool writeSolverSetting(H5::Group& g);

        bool readResultRequest(H5::Group& g);
        bool writeResultRequest(H5::Group& g);

        bool readBCS(H5::Group& g);
        bool writeBCS(H5::Group& g);

        bool readConnection(H5::Group& g);
        bool writeConnection(H5::Group& g);

        bool readProbe(H5::Group& g);
        bool writeProbe(H5::Group& g);
        bool readTimeHistory(H5::Group& g);
        bool writeTimeHistory(H5::Group& g);
    private:
        /**
         * @brief  求解数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-04
         */
        Radioss::FITKRadiossSolution* _data{};

    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossSolution, FITKRadiossDataSolutionAdaptor)
}

#endif 

