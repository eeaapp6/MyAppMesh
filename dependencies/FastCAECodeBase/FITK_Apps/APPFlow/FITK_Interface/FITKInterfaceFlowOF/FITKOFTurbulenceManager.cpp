/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbulenceManager.h"
#include "FITKOFPhysicsData.h"
#include "FITKOFTurbKEpsilon.h"
#include "FITKOFTurbKKLOmega.h"
#include "FITKOFTurbKOmega.h"
#include "FITKOFTurbKOmegaSST.h"
#include "FITKOFTurbKOmegaSSTSAS.h"
#include "FITKOFTurbRealizableKEpsilon.h"
#include "FITKOFTurbRNGKEpsilon.h"
#include "FITKOFTurbSpalartAllmaras.h"
#include "FITKOFTurbKOmegaSSTDDES.h"
#include "FITKOFTurbKOmegaSSTIDDES.h"
#include "FITKOFTurbKOmegaSSTDES.h"
#include "FITKOFTurbSpalartAllmarasDES.h"
#include "FITKOFTurbSpalartAllmarasDDES.h"
#include "FITKOFTurbSpalartAllmarasIDDES.h"
#include "FITKOFTurbSmagorinsky.h"
#include "FITKOFTurbWALE.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    FITKOFTurbulenceManager::FITKOFTurbulenceManager()
    {
        this->initTurbulences();
    }

    FITKOFTurbulenceManager::~FITKOFTurbulenceManager()
    {

    }

    QStringList FITKOFTurbulenceManager::filterTurbence(FITKOFSolverTypeEnum::FITKOFTurbulenceType tbType, FITKOFSolverTypeEnum::FITKOFSolverType stype)
    {
        QStringList models;
        for (QList<FITKOFTurbulenceInfo>::iterator iter = _turbulences.begin(); iter != _turbulences.end(); ++iter)
        {
            if (iter->_turbType == tbType && iter->_enableSolverType.contains(stype))
                models.append(iter->_turbulenceName);
        }
        return models;
    }

    FITKOFTurbulenceInfo FITKOFTurbulenceManager::getTurbenceInfoByTurbulenceName(QString turbulenceName)
    {
        for (QList<FITKOFTurbulenceInfo>::iterator iter = _turbulences.begin(); iter != _turbulences.end(); ++iter)
        {
            if (iter->_turbulenceName == turbulenceName)
                return *iter;
        }
        return FITKOFTurbulenceInfo();
    }

    void FITKOFTurbulenceManager::initTurbulences()
    {
        //注册k-Epsilon
        //FITKOFTurbulenceInfo infoKEpsilon;
        //infoKEpsilon._turbulenceName = "k-Epsilon";
        //infoKEpsilon._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER << FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION;
        //infoKEpsilon._turbType = FITKOFSolverTypeEnum::RANS;
        //infoKEpsilon._createFuns = [&] {return new FITKOFTurbKEpsilon; };
        //_turbulences.append(infoKEpsilon);

        ////注册RNG k-Epsilon
        //FITKOFTurbulenceInfo infoRNGKEpsilon;
        //infoRNGKEpsilon._turbulenceName = "RNG k-Epsilon";
        //infoRNGKEpsilon._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoRNGKEpsilon._turbType = FITKOFSolverTypeEnum::RANS;
        //infoRNGKEpsilon._createFuns = [&] {return new FITKOFTurbRNGKEpsilon; };
        //_turbulences.append(infoRNGKEpsilon);

        ////注册Realizable k-Epsilon
        //FITKOFTurbulenceInfo infoRealizableKEpsilon;
        //infoRealizableKEpsilon._turbulenceName = "Realizable k-Epsilon";
        //infoRealizableKEpsilon._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoRealizableKEpsilon._turbType = FITKOFSolverTypeEnum::RANS;
        //infoRealizableKEpsilon._createFuns = [&] {return new FITKOFTurbRealizableKEpsilon; };
        //_turbulences.append(infoRealizableKEpsilon);

        ////注册Spalart-Allmaras
        //FITKOFTurbulenceInfo infoSA;
        //infoSA._turbulenceName = "Spalart-Allmaras";
        //infoSA._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoSA._turbType = FITKOFSolverTypeEnum::RANS;
        //infoSA._createFuns = [&] {return new FITKOFTurbSpalartAllmaras; };
        //_turbulences.append(infoSA);

        ////注册k-Omega
        //FITKOFTurbulenceInfo infoKOmega;
        //infoKOmega._turbulenceName = "k-Omega";
        //infoKOmega._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKOmega._turbType = FITKOFSolverTypeEnum::RANS;
        //infoKOmega._createFuns = [&] {return new FITKOFTurbKOmega; };
        //_turbulences.append(infoKOmega);

        //注册k-Omega SST
        FITKOFTurbulenceInfo infoKOmegaS;
        infoKOmegaS._turbulenceName = "k-Omega SST";
        infoKOmegaS._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER << FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION;
        infoKOmegaS._turbType = FITKOFSolverTypeEnum::RANS;
        infoKOmegaS._createFuns = [&] {return new FITKOFTurbKOmegaSST; };
        _turbulences.append(infoKOmegaS);

        //注册k-Omega SST SAS
        //FITKOFTurbulenceInfo infoKOmegaSSAS;
        //infoKOmegaSSAS._turbulenceName = "k-Omega SST SAS";
        //infoKOmegaSSAS._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKOmegaSSAS._turbType = FITKOFSolverTypeEnum::RANS;
        //infoKOmegaSSAS._createFuns = [&] {return new FITKOFTurbKOmegaSSTSAS; };
        //_turbulences.append(infoKOmegaSSAS);

        ////注册k-kl-Omega
        //FITKOFTurbulenceInfo infoKKLOmega;
        //infoKKLOmega._turbulenceName = "k-kl-Omega";
        //infoKKLOmega._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKKLOmega._turbType = FITKOFSolverTypeEnum::RANS;
        //infoKKLOmega._createFuns = [&] {return new FITKOFTurbKKLOmega; };
        //_turbulences.append(infoKKLOmega);

        ////注册k-Omega SST DDES
        //FITKOFTurbulenceInfo infoKOmegaSDDES;
        //infoKOmegaSDDES._turbulenceName = "k-Omega SST DDES";
        //infoKOmegaSDDES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKOmegaSDDES._turbType = FITKOFSolverTypeEnum::LES;
        //infoKOmegaSDDES._createFuns = [&] {return new FITKOFTurbKOmegaSSTDDES; };
        //_turbulences.append(infoKOmegaSDDES);

        ////注册k-Omega SST DES
        //FITKOFTurbulenceInfo infoKOmegaSDES;
        //infoKOmegaSDES._turbulenceName = "k-Omega SST DES";
        //infoKOmegaSDES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKOmegaSDES._turbType = FITKOFSolverTypeEnum::LES;
        //infoKOmegaSDES._createFuns = [&] {return new FITKOFTurbKOmegaSSTDES; };
        //_turbulences.append(infoKOmegaSDES);

        ////注册k-Omega SST IDDES
        //FITKOFTurbulenceInfo infoKOmegaSIDDES;
        //infoKOmegaSIDDES._turbulenceName = "k-Omega SST IDDES";
        //infoKOmegaSIDDES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoKOmegaSIDDES._turbType = FITKOFSolverTypeEnum::LES;
        //infoKOmegaSIDDES._createFuns = [&] {return new FITKOFTurbKOmegaSSTIDDES; };
        //_turbulences.append(infoKOmegaSIDDES);

        ////注册Spalart-Allmaras DES
        //FITKOFTurbulenceInfo infoSADES;
        //infoSADES._turbulenceName = "Spalart-Allmaras DES";
        //infoSADES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoSADES._turbType = FITKOFSolverTypeEnum::LES;
        //infoSADES._createFuns = [&] {return new FITKOFTurbSpalartAllmarasDES; };
        //_turbulences.append(infoSADES);

        ////注册Spalart-Allmaras DDES
        //FITKOFTurbulenceInfo infoSADDES;
        //infoSADDES._turbulenceName = "Spalart-Allmaras DDES";
        //infoSADDES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoSADDES._turbType = FITKOFSolverTypeEnum::LES;
        //infoSADDES._createFuns = [&] {return new FITKOFTurbSpalartAllmarasDDES; };
        //_turbulences.append(infoSADDES);

        ////注册Spalart-Allmaras IDDES
        //FITKOFTurbulenceInfo infoSAIDDES;
        //infoSAIDDES._turbulenceName = "Spalart-Allmaras IDDES";
        //infoSAIDDES._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoSAIDDES._turbType = FITKOFSolverTypeEnum::LES;
        //infoSAIDDES._createFuns = [&] {return new FITKOFTurbSpalartAllmarasIDDES; };
        //_turbulences.append(infoSAIDDES);

        ////注册Smagorinsky
        //FITKOFTurbulenceInfo infoSmagorinsky;
        //infoSmagorinsky._turbulenceName = "Smagorinsky";
        //infoSmagorinsky._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoSmagorinsky._turbType = FITKOFSolverTypeEnum::LES;
        //infoSmagorinsky._createFuns = [&] {return new FITKOFTurbSmagorinsky; };
        //_turbulences.append(infoSmagorinsky);

        ////注册WALE
        //FITKOFTurbulenceInfo infoWALE;
        //infoWALE._turbulenceName = "WALE";
        //infoWALE._enableSolverType << FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE << FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
        //infoWALE._turbType = FITKOFSolverTypeEnum::LES;
        //infoWALE._createFuns = [&] {return new FITKOFTurbWALE; };
        //_turbulences.append(infoWALE);
    }
}




