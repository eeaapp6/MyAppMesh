/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataResultRequestAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"

namespace Interface
{
    bool FITKRadiossDataResultRequestAdaptor::adaptR()
    {
        _resultRequest = dynamic_cast<Radioss::FITKRadiossResultRequest*>(_dataObj);
        if (_reader == nullptr || _resultRequest == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double doubleValue = 0.0;
        std::string strValue;
        // 读取基本参数
        if (!h5tools->readGroupDouble(*_h5Group, "StartTime", &doubleValue)) return false;
        _resultRequest->setStartTime(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Frequency", &doubleValue)) return false;
        _resultRequest->setFrequency(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "FrequencyHistory", &doubleValue)) return false;
        _resultRequest->setHisFrequency(doubleValue);
        // 读取节点动画类型
        if (h5tools->readGroupStr(*_h5Group, "NodalAnimations", strValue))
        {
            QString nodalAnimsStr = QString::fromStdString(strValue);
            QStringList nodalAnimList = nodalAnimsStr.split(";", Qt::SkipEmptyParts);
            QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalAnims;

            Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalTransfer;
            bool isValid = false;

            for (const QString& animStr : nodalAnimList)
            {
                Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType animType =
                    nodalTransfer.fromString(animStr.trimmed(), isValid);
                if (isValid)
                {
                    nodalAnims.append(animType);
                }
            }
            _resultRequest->setNodalOutputAnim(nodalAnims);
        }
        // 读取单元动画类型
        if (h5tools->readGroupStr(*_h5Group, "ElementalAnimations", strValue))
        {
            QString elementalAnimsStr = QString::fromStdString(strValue);
            QStringList elementalAnimList = elementalAnimsStr.split(";", Qt::SkipEmptyParts);
            QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalAnims;

            Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalTransfer;
            bool isValid = false;

            for (const QString& animStr : elementalAnimList)
            {
                Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType animType =
                    elementalTransfer.fromString(animStr.trimmed(), isValid);
                if (isValid)
                {
                    elementalAnims.append(animType);
                }
            }
            _resultRequest->setElementalOutputAnim(elementalAnims);
        }

        // 读取Brick动画类型
        if (h5tools->readGroupStr(*_h5Group, "BrickAnimations", strValue))
        {
            QString brickAnimsStr = QString::fromStdString(strValue);
            QStringList brickAnimList = brickAnimsStr.split(";", Qt::SkipEmptyParts);
            QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickAnims;

            Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickTransfer;
            bool isValid = false;

            for (const QString& animStr : brickAnimList)
            {
                Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType animType =
                    brickTransfer.fromString(animStr.trimmed(), isValid);
                if (isValid)
                {
                    brickAnims.append(animType);
                }
            }
            _resultRequest->setBrickOutputAnim(brickAnims);
        }
        // 读取Shell动画类型
        if (h5tools->readGroupStr(*_h5Group, "ShellAnimations", strValue))
        {
            QString shellAnimsStr = QString::fromStdString(strValue);
            QStringList shellAnimList = shellAnimsStr.split(";", Qt::SkipEmptyParts);
            QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellAnims;

            Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellTransfer;
            bool isValid = false;

            for (const QString& animStr : shellAnimList)
            {
                Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType animType =
                    shellTransfer.fromString(animStr.trimmed(), isValid);
                if (isValid)
                {
                    shellAnims.append(animType);
                }
            }
            _resultRequest->setShellOutputAnim(shellAnims);
        }
        return true;
    }

    bool FITKRadiossDataResultRequestAdaptor::adaptW()
    {
        _resultRequest = dynamic_cast<Radioss::FITKRadiossResultRequest*>(_dataObj);
        if (_writer == nullptr || _resultRequest == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //获取数据
        // 写入基本参数
        h5tools->writeGroupAttrDouble(*_h5Group, "StartTime", _resultRequest->getStartTime());
        h5tools->writeGroupAttrDouble(*_h5Group, "Frequency", _resultRequest->getFrequency());
        h5tools->writeGroupAttrDouble(*_h5Group, "FrequencyHistory", _resultRequest->getHisFrequency());

        // 写入节点动画类型
        QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalAnims =
            _resultRequest->getNodalOutputAnim();
        QStringList nodalAnimStrs;
        Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalTransfer;
        bool isValid = false;
        for (const Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType& anim : nodalAnims)
        {
            QString animStr = nodalTransfer.toString(anim, isValid);
            if (isValid)
            {
                nodalAnimStrs.append(animStr);
            }
        }
        h5tools->writeGroupAttrStr(*_h5Group, "NodalAnimations", nodalAnimStrs.join(";").toStdString());
        // 写入单元动画类型
        QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalAnims =
            _resultRequest->getElementalOutputAnim();
        QStringList elementalAnimStrs;
        Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalTransfer;
        isValid = false;
        for (const auto& anim : elementalAnims)
        {
            QString animStr = elementalTransfer.toString(anim, isValid);
            if (isValid)
            {
                elementalAnimStrs.append(animStr);
            }
        }
        h5tools->writeGroupAttrStr(*_h5Group, "ElementalAnimations", elementalAnimStrs.join(";").toStdString());
        // 写入Brick动画类型
        QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickAnims =
            _resultRequest->getBrickOutputAnim();
        QStringList brickAnimStrs;
        Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickTransfer;
        isValid = false;
        for (const auto& anim : brickAnims)
        {
            QString animStr = brickTransfer.toString(anim, isValid);
            if (isValid)
            {
                brickAnimStrs.append(animStr);
            }
        }
        h5tools->writeGroupAttrStr(*_h5Group, "BrickAnimations", brickAnimStrs.join(";").toStdString());
        // 写入Shell动画类型
        QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellAnims =
            _resultRequest->getShellOutputAnim();
        QStringList shellAnimStrs;
        Core::FITKEnumTransfer<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellTransfer;
        isValid = false;
        for (const auto& anim : shellAnims)
        {
            QString animStr = shellTransfer.toString(anim, isValid);
            if (isValid)
            {
                shellAnimStrs.append(animStr);
            }
        }
        h5tools->writeGroupAttrStr(*_h5Group, "ShellAnimations", shellAnimStrs.join(";").toStdString());
        return true;
    }
}
