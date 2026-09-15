/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorSolverSetting.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"

#include <QFileInfo>

namespace Radioss
{

    QString FITKRadiossAdaptorSolverSetting::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorSolverSetting";
    }

    bool FITKRadiossAdaptorSolverSetting::adaptR()
    {
        if (_reader == nullptr) return false;
        
        //回退一行，读取关键字
        _reader->backLine();
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (!line.startsWith("/")) continue;
            bool ok = false;
            //读取打印配置
            if (line.startsWith("/PRINT"))
                ok = readPrint();
            //读取运行设置
            else if (line.startsWith("/RUN"))
                ok = readRun();
            //读取版本
            else if (line.startsWith("/VERS"))
                ok = readVersion();
            //读取监控设置
            else if (line.startsWith("/MON"))
                ok = readMonitoring();
            //读取配置的重启文件信息
            else if (line.startsWith("/RFILE"))
                ok = readRestartFile();
            //读取并行开关
            else if (line.startsWith("/PARITH"))
                ok = readParallel();
            //读取停止控制
            else if (line.startsWith("/NEGVOL"))
                ok = readStopControl();
            else if (line.startsWith("/DT") || line.startsWith("/DT1"))
                ok = readDataTimeStep();
            else if (line.startsWith("/STOP"))
                ok = readConvergenceCriteriaParam();
            //其他关键字if (line.startsWith("/")) 
            else
            {
                _reader->backLine();
                break;
            }
        }
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::adaptW()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_writer == nullptr || _caseData == nullptr) return false;
        bool ok = true;
        //写出输出的打印配置
        ok &= writePrint();
        //写出运行的配置
        ok &= writeRun();
        ok &= writeParallel();
        ok &= writeStopControl();
        //全局时间步设置 该功能已停用
        //ok &= writeGlobalTimeStep();
        ok &= writeNodeTimeParam();
        ok &= writeElemTimeParam();

        ok &= writeConvergenceCriteriaParam();
        return ok;
    }

    bool FITKRadiossAdaptorSolverSetting::readPrint()
    {
        if (_reader == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "PRINT") return false;
        //获取输出的打印频率文件参数
        if (data.size() < 3) return false;
        //获取打印输出每个Nprint循环
        int n_print = data[1].toInt();
        //获取几行就会打印一次标题
        int n_line = data[2].toInt();

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writePrint()
    {
        if (_writer == nullptr) return false;

        //默认写出打印配置
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "/PRINT/-100/50" << endl;
        
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readRun()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "RUN") return false;
        //获取运行参数
        if (data.size() < 3) return false;
        //获取运行名称
        QString modelName = data[1];
        //获取运行数
        int run_number = data[2].toInt();
        //获取重启文件标识
        QString res_letter;
        if (data.size() == 4)
            res_letter = data[3].toUpper();
        //获取运行的最终时间
        QString line = _reader->readLine().trimmed();
        double finalTime = line.toDouble();
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverRunExeTimeParam * runExeTimeParam = solverSetting->getRunExeTimeParam();
        if (!runExeTimeParam) return false;
        //设置运行最终时间
        runExeTimeParam->setFinalTime(finalTime);
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeRun()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverRunExeTimeParam * runExeTimeParam = solverSetting->getRunExeTimeParam();
        if (!runExeTimeParam) return false;
        //获取运行参数
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        QString fileName = QFileInfo(_writer->getFileName()).baseName();
        if (fileName.size() < 5) return false;
        QString modelName = fileName.left(fileName.size() - 5);
        double finalTime = runExeTimeParam->getFinalTime();
        //写出运行参数
        *stream << QString("/RUN/%1/1").arg(modelName) << endl;
        *stream << QString::number(finalTime) << endl;

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readVersion()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "VERS" || data.size() < 2) return false;
        //获取版本信息
        int version = data[1].toInt();
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeVersion()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;
        //默认写出版本信息
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "/VERS/2019" << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readMonitoring()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "MON" || data.size() < 2) return false;
        //获取监控信息
        QString monitoring = data[1];
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeMonitoring()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;
        //默认写出监控信息
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "/MON/ON" << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readRestartFile()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "RFILE" || data.size() < 2) return false;

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeRestartFile()
    {
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readParallel()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "PARITH") return false;
        //获取并行是否开启
        if (data.size() < 2) return false;
        bool isParallel = false;
        if (data[1].toUpper() == "ON")
            isParallel = true;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverComputationControlParam * computationControlParam = solverSetting->getComputationControlParam();
        if (!computationControlParam) return false;

        computationControlParam->setParallelArithmetic(isParallel);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeParallel()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        //获取并行参数
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverComputationControlParam * computationControlParam = solverSetting->getComputationControlParam();
        if (!computationControlParam) return false;
        QString parallel = "OFF";
        if (computationControlParam->getParallelArithmetic())
            parallel = "ON";
        *stream << QString("/PARITH/%1").arg(parallel) << endl;

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readGlobalTimeStep()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "DT") return false;

        if (data.size() < 2) return false;
        //获取类型
        if (data[1].toUpper() != "GLOB") return false;
        if (data.size() < 3) return false;
        bool userEdit = data[2].toInt() == 1;
        //获取参数
        double scaleFactor = 0.9;
        double toleranceGTS = 10e-3;
        int numCycles = 20;
        QString line = _reader->readLine();
        data = line.simplified().split(" ");
        if (data.size() > 0)
            scaleFactor = data[0].toDouble();
        if (userEdit)
        {
            line = _reader->readLine();
            data = line.simplified().split(" ");
            if (data.size() > 0)
                toleranceGTS = data[0].toDouble();
            if (data.size() > 1)
                numCycles = data[1].toInt();
        }

        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverGlobalTimeParam * globalTime = solverSetting->getGlobalTimeParam();
        if (!globalTime) return false;

        globalTime->setEnableAdditionalCards(userEdit);
        globalTime->setScaleFactor(scaleFactor);
        globalTime->setToleranceGTS(toleranceGTS);
        globalTime->setNumCycles(20);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeGlobalTimeStep()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverGlobalTimeParam * globalTime = solverSetting->getGlobalTimeParam();
        if (!globalTime) return false;
        //获取参数
        int userEdit = globalTime->isEnableAdditionalCards() ? 1 : 0;
        double scaleFactor = globalTime->getScaleFactor();
        double toleranceGTS = globalTime->getToleranceGTS();
        int numCycles = globalTime->getNumCycles();
        //写出参数
        *stream << QString("/DT/GLOB/%1").arg(userEdit) << endl;
        *stream << QString("%1").arg(scaleFactor) << endl;
        if (userEdit)
            *stream << QString("%1    %2").arg(toleranceGTS).arg(numCycles) << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readStopControl()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;

        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "NEGVOL") return false;
        //获取并行是否开启
        if (data.size() < 2) return false;
        bool stopComputation = false;
        if (data[1].toUpper() == "STOP")
            stopComputation = true;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverComputationControlParam * computationControlParam = solverSetting->getComputationControlParam();
        if (!computationControlParam) return false;

        computationControlParam->setStopComputationWhenNegativeVolume(stopComputation);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeStopControl()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        //获取并行参数
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverComputationControlParam * computationControlParam = solverSetting->getComputationControlParam();
        if (!computationControlParam) return false;
        QString parallel = "SMSTR";
        if (computationControlParam->getStopComputationWhenNegativeVolume())
            parallel = "STOP";
        *stream << QString("/NEGVOL/%1").arg(parallel) << endl;

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readDataTimeStep()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        //获取关键字
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        QString keyword;
        if (data.size() >= 2)
            keyword = data[1].toUpper();
        //读取DT
        if (data[0].toUpper() == "DT")
        {
            if (keyword == "NODA")
                this->readNodeTimeParam();
            else if (keyword == "GLOB")
                this->readGlobalTimeStep();
        }
        //读取DT1
        else if (data[0].toUpper() == "DT1")
        {
            if (keyword == "BRICK")
                this->readElemTimeParam();
        }
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readNodeTimeParam()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "DT") return false;

        if (data.size() < 2) return false;
        //获取类型
        if (data[1].toUpper() != "NODA") return false;
        FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType type = FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType::NodalTimeStep_SolverDefault;
        if (data.size() > 2)
        {
            if (data[2].toUpper() == "CST")
                type = FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType::NodalTimeStep_CST;
            else if (data[2].toUpper() == "SET")
                type = FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType::NodalTimeStep_SET;
            else if (data[2].toUpper() == "STOP")
                type = FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType::NodalTimeStep_STOP;
        }

        //获取参数
        QString line = _reader->readLine();
        data = line.simplified().split(" ");
        double minTimeStep = 0.0, scaleFactor = 0.0, initAddMassRatio = 0.0;
        if (data.size() > 0)
            scaleFactor = data[0].toDouble();
        if (data.size() > 1)
            minTimeStep = data[1].toDouble();
        if (data.size() > 2)
            initAddMassRatio = data[2].toDouble();

        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverNodalTimeStepControlParam * nodalTimeParam = solverSetting->getNodalTimeStepControlParam();
        if (!nodalTimeParam) return false;
        nodalTimeParam->setTimeStepControlType(type);
        nodalTimeParam->setMinimumTimeStep(minTimeStep);
        nodalTimeParam->setScaleFactor(scaleFactor);
        nodalTimeParam->setInitialAddMassRatio(initAddMassRatio);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeNodeTimeParam()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverNodalTimeStepControlParam * nodalTimeParam = solverSetting->getNodalTimeStepControlParam();
        if (!nodalTimeParam) return false;
        FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType type = nodalTimeParam->getTimeStepControlType();
        double minTimeStep = nodalTimeParam->getMinimumTimeStep(), scaleFactor = nodalTimeParam->getScaleFactor(), initAddMassRatio = nodalTimeParam->getInitialAddMassRatio();

        //写出节点时间步参数
        switch (type)
        {
        case Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_SolverDefault: *stream << "/DT/NODA/" << endl;
            break;
        case Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_CST: *stream << "/DT/NODA/CST" << endl;
            break;
        case Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_SET: *stream << "/DT/NODA/SET" << endl;
            break;
        case Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_STOP: *stream << "/DT/NODA/STOP" << endl;
            break;
        default:
            break;
        }

        *stream << QString("%1  %2  %3").arg(scaleFactor).arg(minTimeStep).arg(initAddMassRatio) << endl;
        //写出高阶四面体的计算步时间改进
        *stream << QString("/DT1TET10") << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readElemTimeParam()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "DT1") return false;

        if (data.size() < 2) return false;
        //获取类型
        int flag = 0;
        if (data[1].toUpper() != "BRICK") return false;
        FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType type = FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType::ElementalTimeStep_SolverDefault;
        if (data.size() > 2)
        {
            if (data[2].toUpper() == "CST")
                type = FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType::ElementalTimeStep_CST;
            else if (data[2].toUpper() == "DEL")
                type = FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType::ElementalTimeStep_DEL;
            else if (data[2].toUpper() == "STOP")
                type = FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType::ElementalTimeStep_STOP;
        }
        if (data.size() > 3)
            flag = data[3].toInt();

        //获取参数
        QString line = _reader->readLine();
        data = line.simplified().split(" ");
        double minTimeStep = 0.0, scaleFactor = 0.0;
        if (data.size() > 0)
            minTimeStep = data[0].toDouble();
        if (data.size() > 1)
            scaleFactor = data[1].toDouble();
        bool numberOfAddCards = false;
        double minAspectRatio = 0.0, minVolumeRatio = 0.0;
        if (type == FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType::ElementalTimeStep_CST && flag > 0)
        {
            numberOfAddCards = true;
            line = _reader->readLine();
            data = line.simplified().split(" ");
            if (data.size() > 0)
                minAspectRatio = data[0].toDouble();
            if (data.size() > 1)
                minVolumeRatio = data[1].toDouble();
        }

        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverElementalTimeStepControlParam * elementalTimeParam = solverSetting->getElementalTimeStepControlParam();
        if (!elementalTimeParam) return false;
        elementalTimeParam->setTimeStepControlType(type);
        elementalTimeParam->setMinimumTimeStep(minTimeStep);
        elementalTimeParam->setScaleFactor(scaleFactor);
        elementalTimeParam->setNumberOfAddCards(numberOfAddCards);
        elementalTimeParam->setMinAspectRatio(minAspectRatio);
        elementalTimeParam->setMinVolumeRatio(minVolumeRatio);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeElemTimeParam()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverElementalTimeStepControlParam * elementalTimeParam = solverSetting->getElementalTimeStepControlParam();
        if (!elementalTimeParam) return false;
        FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType type = elementalTimeParam->getTimeStepControlType();
        double minTimeStep = elementalTimeParam->getMinimumTimeStep(), scaleFactor = elementalTimeParam->getScaleFactor();
        bool numberOfAddCards = elementalTimeParam->getNumberOfAddCards();
        double minAspectRatio = elementalTimeParam->getMinAspectRatio(), minVolumeRatio = elementalTimeParam->getMinVolumeRatio();

        //写出节点时间步参数
        switch (type)
        {
        case Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_SolverDefault: *stream << "/DT1/BRICK/" << endl;
            break;
        case Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_CST:
        {
            if (numberOfAddCards)
                *stream << "/DT1/BRICK/CST/1" << endl;
            else
                *stream << "/DT1/BRICK/CST" << endl;
            break;
        }
        case Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_DEL: *stream << "/DT1/BRICK/DEL" << endl;
            break;
        case Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_STOP: *stream << "/DT1/BRICK/STOP" << endl;
            break;
        default:
            break;
        }


        *stream << QString("%1  %2").arg(minTimeStep).arg(scaleFactor) << endl;
        if (type == Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_CST && numberOfAddCards)
            *stream << QString("%1  %2").arg(minAspectRatio).arg(minVolumeRatio) << endl;
        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::readConvergenceCriteriaParam()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        _reader->backLine();
        QStringList data = getKeywordExtraction(_reader->readLine());
        if (data.isEmpty()) return false;
        if (data[0].toUpper() != "STOP") return false;

        //获取参数
        QString line = _reader->readLine();
        data = line.simplified().split(" ");
        double energyErrorRatio = 0.0, totalMassRatio = 0.0, nodalMassRatio = 0.0;
        bool writeTimeHistoryFile = false, writeAnimationFile = false, energyError = false;
        if (data.size() > 0)
            energyErrorRatio = data[0].toDouble();
        if (data.size() > 1)
            totalMassRatio = data[1].toDouble();
        if (data.size() > 2)
            nodalMassRatio = data[2].toDouble();
        if (data.size() > 3)
            writeTimeHistoryFile = data[3].toInt() > 0 ? true : false;
        if (data.size() > 4)
            writeAnimationFile = data[4].toInt() > 0 ? true : false;
        if (data.size() > 5)
            energyError = data[5].toInt() > 0 ? true : false;

        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverConvergenceCriteriaParam * convergenceCriteriaParam = solverSetting->getConvergenceCriteriaParam();
        if (!convergenceCriteriaParam) return false;
        convergenceCriteriaParam->setEnableState(true);
        convergenceCriteriaParam->setEnergyErrorRatio(energyErrorRatio);
        convergenceCriteriaParam->setTotalMassRatio(totalMassRatio);
        convergenceCriteriaParam->setNodalMassRatio(nodalMassRatio);
        convergenceCriteriaParam->setWriteTimeHistoryFile(writeTimeHistoryFile);
        convergenceCriteriaParam->setWriteAnimationFile(writeAnimationFile);
        convergenceCriteriaParam->setEnergyError(energyError);

        return true;
    }

    bool FITKRadiossAdaptorSolverSetting::writeConvergenceCriteriaParam()
    {
        if (_writer == nullptr || _caseData == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        //获取求解设置数据
        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverConvergenceCriteriaParam * convergenceCriteriaParam = solverSetting->getConvergenceCriteriaParam();
        if (!convergenceCriteriaParam) return false;
        if (!convergenceCriteriaParam->getEnableState()) return true;
        //获取参数数据
        double energyErrorRatio = convergenceCriteriaParam->getEnergyErrorRatio(), totalMassRatio = convergenceCriteriaParam->getTotalMassRatio(), nodalMassRatio = convergenceCriteriaParam->getNodalMassRatio();
        bool writeTimeHistoryFile = convergenceCriteriaParam->getWriteTimeHistoryFile(), writeAnimationFile = convergenceCriteriaParam->getWriteAnimationFile(), energyError = convergenceCriteriaParam->getEnergyError();

        *stream << QString("/STOP") << endl;
        *stream << QString("%1  %2  %3  %4  %5  %6").arg(energyErrorRatio).arg(totalMassRatio).arg(nodalMassRatio).arg(int(writeTimeHistoryFile)).arg(int(writeAnimationFile)).arg(int(energyError)) << endl;

        return true;
    }

    QStringList FITKRadiossAdaptorSolverSetting::getKeywordExtraction(QString line)
    {
        //关键字数据提取
        line.trimmed();
        line.remove(QRegExp("^/+"));   // 移除开头的所有'/'
        line.remove(QRegExp("/+$"));   // 移除结尾的所有'/'
        return line.split('/');
    }
}
