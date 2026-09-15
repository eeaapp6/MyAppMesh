/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorSolverRequest.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include <QFileInfo>

namespace Radioss
{
    QString FITKRadiossAdaptorSolverRequest::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorSolverRequest";
    }

    bool FITKRadiossAdaptorSolverRequest::adaptR()
    {
        if (_reader == nullptr) return false;
        bool ok = false;
        //回退一行，读取关键字
        _reader->backLine();
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (!line.startsWith("/")) continue;
            //读取动画参数
            if (line.startsWith("/ANIM"))
                ok = readAnimationParam();
            else if (line.startsWith("/TFILE"))
                ok = readHisFrequency();
            //其他关键字if (line.startsWith("/")) 
            else
            {
                _reader->backLine();
                break;
            }
        }
        return ok;
    }

    bool FITKRadiossAdaptorSolverRequest::adaptW()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_writer == nullptr || _caseData == nullptr) return false;
        bool ok = true;

        //写出输出的打印配置
        ok &= writeHisFrequency();
        ok &= writeAnimationTimeStep();
        ok &= writeAnimationVECT();
        ok &= writeAnimationElement();

        return ok;
    }

    bool FITKRadiossAdaptorSolverRequest::readHisFrequency()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "TFILE") return false;
        //获取时间历程频率
        double freqHistory = 0.9;
        QString line = _reader->readLine();
        data = line.simplified().split(" ");
        if (data.size() > 0)
            freqHistory = data[0].toDouble();
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        resultRequest->setHisFrequency(freqHistory);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::writeHisFrequency()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        double freqHistory = resultRequest->getHisFrequency();
        //写出参数
        *stream << QString("/TFILE/0") << endl;
        *stream << QString("%1").arg(freqHistory) << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationParam()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        //获取关键字
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty() || data[0].toUpper() != "ANIM") return false;
        QString keyword;
        if (data.size() >= 2)
            keyword = data[1].toUpper();
        //读取动画时间步
        if (keyword == "DT")
            this->readAnimationTimeStep();
        else if (keyword == "VECT" || keyword == "GPS")
            this->readAnimationVECT();
        else if (keyword == "ELEM" || keyword == "BEAM" || keyword == "SPRING" || keyword == "TRUSS")
            this->readAnimationElement();
        else if (keyword == "BRICK")
            this->readAnimationBrick();
        else if (keyword == "SHELL")
            this->readAnimationShell();
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationTimeStep()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "ANIM") return false;
        //获取时间历程频率
        double startTime = 0.0;
        double freq = 0.9;
        QString line = _reader->readLine();
        data = line.simplified().split(" ");

        if (data.size() > 0)
            startTime = data[0].toDouble();
        if (data.size() > 1)
            freq = data[1].toDouble();
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        resultRequest->setStartTime(startTime);
        resultRequest->setFrequency(freq);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::writeAnimationTimeStep()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        double startTime = resultRequest->getStartTime();
        double frequency = resultRequest->getFrequency();

        //写出动画开始时间和频率
        *stream << "/ANIM/DT" << endl;
        *stream << QString("%1     %2").arg(startTime).arg(frequency) << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationVECT()
    {
        //读取动画节点数据
        auto addType = [&](FITKRadiossResultRequest::NodalOutputAnimationsType type, QList<FITKRadiossResultRequest::NodalOutputAnimationsType>& typeList)
        {
            if (typeList.contains(type)) return;
            typeList.append(type);
        };

        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        //获取关键字
        QStringList keywords = getKeywordExtraction(_reader->readLine());
        if (keywords.isEmpty()) return false;
        if (keywords[0].toUpper() != "ANIM" || keywords.size() < 3) return false;
        //获取请求类型
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::NodalOutputAnimationsType> typeList = resultRequest->getNodalOutputAnim();
        if (keywords[1] == "VECT")
        {
            if (keywords[2] == "VEL")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_Velocity, typeList);
            else if (keywords[2] == "DISP")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_Displacement, typeList);
            else if (keywords[2] == "ACC")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_Acceleration, typeList);
            else if (keywords[2] == "CONT")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_ContactForces, typeList);
            else if (keywords[2] == "FOPT")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_RigidBodyForcesMoments, typeList);
            else if (keywords[2] == "FINT")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_InternalForces, typeList);
            else if (keywords[2] == "PCONT")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_ContactPressure, typeList);
        }
        else if (keywords[1] == "GPS")
        {
            QString kw4;
            if (keywords.size() > 3)
                kw4 = keywords[3];
            if (keywords[2] == "STRESS"  && kw4 == "TENS")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_StressAllIntegrationPoints, typeList);
            else if (keywords[2] == "STRAIN"  && kw4 == "TENS")
                addType(FITKRadiossResultRequest::NodalOutputAnimationsType::Nodal_StrainAllIntegrationPoints, typeList);
        }

        resultRequest->setNodalOutputAnim(typeList);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::writeAnimationVECT()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::NodalOutputAnimationsType> nodalType = resultRequest->getNodalOutputAnim();

        //写出节点动画类型
        for (FITKRadiossResultRequest::NodalOutputAnimationsType type : nodalType)
        {
            switch (type)
            {
            case Radioss::FITKRadiossResultRequest::Nodal_Displacement: *stream << "/ANIM/VECT/DISP" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_Velocity: *stream << "/ANIM/VECT/VEL" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_Acceleration: *stream << "/ANIM/VECT/ACC" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_ContactForces: *stream << "/ANIM/VECT/CONT" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_RigidBodyForcesMoments: *stream << "/ANIM/VECT/FOPT" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_InternalForces: *stream << "/ANIM/VECT/FINT" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_ContactPressure: *stream << "/ANIM/VECT/PCONT" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_StressAllIntegrationPoints: *stream << "/ANIM/GPS/STRESS/TENS" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Nodal_StrainAllIntegrationPoints: *stream << "/ANIM/GPS/STRAIN/TENS" << endl;
                break;
            default:
                break;
            }
        }
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationElement()
    {
        //读取动画单元数据
        auto addType = [&](FITKRadiossResultRequest::ElementalOutputAnimationsType type, QList<FITKRadiossResultRequest::ElementalOutputAnimationsType>& typeList)
        {
            if (typeList.contains(type)) return;
            typeList.append(type);
        };
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        //获取关键字
        QStringList keywords = getKeywordExtraction(_reader->readLine());
        if (keywords.isEmpty()) return false;
        if (keywords[0].toUpper() != "ANIM" || keywords.size() < 3) return false;
        //获取请求类型
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::ElementalOutputAnimationsType> typeList = resultRequest->getElementalOutputAnim();
        if ("ELEM" == keywords[1])
        {
            if (keywords[2] == "ENER")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_SpecificEnergyDensity, typeList);
            else if (keywords[2] == "EPSP")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_PlasticStrain, typeList);
            else if (keywords[2] == "HOURG")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_HourglassEnergy, typeList);
            else if (keywords[2] == "VONM")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_VonMisesStress, typeList);
            else if (keywords[2] == "P")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_Pressure, typeList);
            else if (keywords[2] == "DENS")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_Density, typeList);
        }
        else if ("BEAM" == keywords[1])
        {
            if (keywords[2] == "FORC")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_BeamFORC, typeList);
            else if (keywords[2] == "EPSP")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_BeamPlasticStrain, typeList);
            else if (keywords[2] == "VONM")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_BeamVonMisesStress, typeList);
        }
        else if ("TRUSS" == keywords[1])
        {
            if (keywords[2] == "FORC")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_TrussFORC, typeList);
        }
        else if ("SPRING" == keywords[1])
        {
            if (keywords[2] == "FORC")
                addType(FITKRadiossResultRequest::ElementalOutputAnimationsType::Elemental_SpringFORC, typeList);
        }
        resultRequest->setElementalOutputAnim(typeList);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationBrick()
    {
        //读取动画单元数据
        auto addType = [&](FITKRadiossResultRequest::BrickOutputAnimationsType type, QList<FITKRadiossResultRequest::BrickOutputAnimationsType>& typeList)
        {
            if (typeList.contains(type)) return;
            typeList.append(type);
        };
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        //获取关键字
        QStringList keywords = getKeywordExtraction(_reader->readLine());
        if (keywords.isEmpty()) return false;
        if (keywords[0].toUpper() != "ANIM" || keywords.size() < 3) return false;
        //获取请求类型
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::BrickOutputAnimationsType> typeList = resultRequest->getBrickOutputAnim();
        if ("BRICK" == keywords[1])
        {
            QString kw4;
            if (keywords.size() > 3)
                kw4 = keywords[3];
            if (keywords[2] == "TENS" && kw4 == "STRESS")
                addType(FITKRadiossResultRequest::BrickOutputAnimationsType::Elemental_BrickElementStress, typeList);
            else if (keywords[2] == "TENS" && kw4 == "STRAIN")
                addType(FITKRadiossResultRequest::BrickOutputAnimationsType::Elemental_BrickElementStrain, typeList);
            else if (keywords[2] == "TENS" && kw4 == "DAMA")
                addType(FITKRadiossResultRequest::BrickOutputAnimationsType::Elemental_BrickElementTensor, typeList);
            else if (keywords[2] == "TENS" && kw4 == "EPSP")
                addType(FITKRadiossResultRequest::BrickOutputAnimationsType::Elemental_BrickElementEPSP, typeList);
            else if (keywords[2] == "DAMA")
                addType(FITKRadiossResultRequest::BrickOutputAnimationsType::Elemental_BrickDamageOfAll, typeList);
        }
        resultRequest->setBrickOutputAnim(typeList);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::readAnimationShell()
    {
        //读取动画单元数据
        auto addType = [&](FITKRadiossResultRequest::ShellOutputAnimationsType type, QList<FITKRadiossResultRequest::ShellOutputAnimationsType>& typeList)
        {
            if (typeList.contains(type)) return;
            typeList.append(type);
        };
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        //获取关键字
        QStringList keywords = getKeywordExtraction(_reader->readLine());
        if (keywords.isEmpty()) return false;
        if (keywords[0].toUpper() != "ANIM" || keywords.size() < 3) return false;
        //获取请求类型
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::ShellOutputAnimationsType> typeList = resultRequest->getShellOutputAnim();
        if ("SHELL" == keywords[1])
        {
            QString kw4;
            if (keywords.size() > 3)
                kw4 = keywords[3];
            if (keywords[2] == "TENS" && kw4 == "STRESS")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellElementStress, typeList);
            else if (keywords[2] == "TENS" && kw4 == "STRAIN")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellElementStrain, typeList);
            else if (keywords[2] == "TENS" && kw4 == "EPSDOT")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellElementEPSDOT, typeList);
            else if (keywords[2] == "EPSP")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellPlasticStrain, typeList);
            else if (keywords[2] == "DAMA")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellDamageOfAll, typeList);
            else if (keywords[2] == "FLDF")
                addType(FITKRadiossResultRequest::ShellOutputAnimationsType::Elemental_ShellFLDFailure, typeList);
        }
        resultRequest->setShellOutputAnim(typeList);
        return true;
    }

    bool FITKRadiossAdaptorSolverRequest::writeAnimationElement()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossResultRequest * resultRequest = _caseData->getResultRequest();
        if (!resultRequest) return false;
        QList<FITKRadiossResultRequest::ElementalOutputAnimationsType> elementType = resultRequest->getElementalOutputAnim();
        QList<FITKRadiossResultRequest::BrickOutputAnimationsType> brickType = resultRequest->getBrickOutputAnim();
        QList<FITKRadiossResultRequest::ShellOutputAnimationsType> shellType = resultRequest->getShellOutputAnim();

        //写出单元动画类型
        for (FITKRadiossResultRequest::ElementalOutputAnimationsType type : elementType)
        {
            switch (type)
            {
            case Radioss::FITKRadiossResultRequest::Elemental_SpecificEnergyDensity: *stream << "/ANIM/ELEM/ENER" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_PlasticStrain: *stream << "/ANIM/ELEM/EPSP" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_HourglassEnergy: *stream << "/ANIM/ELEM/HOURG" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_VonMisesStress: *stream << "/ANIM/ELEM/VONM" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_Pressure: *stream << "/ANIM/ELEM/P" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_Density: *stream << "/ANIM/ELEM/DENS" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BeamPlasticStrain: *stream << "/ANIM/BEAM/EPSP" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BeamVonMisesStress: *stream << "/ANIM/BEAM/VONM" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BeamFORC: *stream << "/ANIM/BEAM/FORC" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_TrussFORC: *stream << "/ANIM/TRUSS/FORC" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_SpringFORC: *stream << "/ANIM/SPRING/FORC" << endl;
                break;
            default:
                break;
            }
        }
        //写出Brick单元动画类型
        for (FITKRadiossResultRequest::BrickOutputAnimationsType type : brickType)
        {
            switch (type)
            {
            case Radioss::FITKRadiossResultRequest::Elemental_BrickElementStress: *stream << "/ANIM/BRICK/TENS/STRESS" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BrickElementStrain: *stream << "/ANIM/BRICK/TENS/STRAIN" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BrickElementTensor: *stream << "/ANIM/BRICK/TENS/DAMA" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BrickElementEPSP: *stream << "/ANIM/BRICK/TENS/EPSP" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_BrickDamageOfAll: *stream << "/ANIM/BRICK/DAMA" << endl;
                break;
            default:
                break;
            }
        }
        //写出Shell单元动画类型
        for (FITKRadiossResultRequest::ShellOutputAnimationsType type : shellType)
        {
            switch (type)
            {
            case Radioss::FITKRadiossResultRequest::Elemental_ShellElementStress: *stream << "/ANIM/SHELL/TENS/STRESS/ALL" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_ShellElementStrain: *stream << "/ANIM/SHELL/TENS/STRAIN/ALL" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_ShellElementEPSDOT: *stream << "/ANIM/SHELL/TENS/EPSDOT/ALL" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_ShellPlasticStrain: *stream << "/ANIM/SHELL/EPSP/ALL" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_ShellDamageOfAll: *stream << "/ANIM/SHELL/DAMA" << endl;
                break;
            case Radioss::FITKRadiossResultRequest::Elemental_ShellFLDFailure: *stream << "/ANIM/SHELL/FLDF" << endl;
                break;
            default:
                break;
            }
        }
        return true;
    }

    QStringList FITKRadiossAdaptorSolverRequest::getKeywordExtraction(QString line)
    {
        //关键字数据提取
        line.trimmed();
        line.remove(QRegExp("^/+"));   // 移除开头的所有'/'
        line.remove(QRegExp("/+$"));   // 移除结尾的所有'/'
        return line.split('/');
    }

}
