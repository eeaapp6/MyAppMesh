/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataTimeHistoryRWALLAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRWALL.h"

namespace Interface
{
    bool FITKRadiossDataTimeHistoryRWALLAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKTimeHistoryRWALL*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 读取变量组类型
        int intValue = 0;
        if (h5tools->readGroupint(*_h5Group, "VarlableGroupType", &intValue))
        {
            _data->setVarlableGroupType(static_cast<Radioss::FITKTimeHistoryRWALL::THRWALLVariableGroup>(intValue));
        }
        // 读取对象ID列表
        std::string strValue{};
        if (h5tools->readGroupStr(*_h5Group, "ObjIDs", strValue))
        {
            QString objIDsStr = QString::fromStdString(strValue);
            QStringList objIDList = objIDsStr.split(";", Qt::SkipEmptyParts);
            QList<int> objIDs{};
            for (const QString& idStr : objIDList)
            {
                bool ok = false;
                int id = idStr.trimmed().toInt(&ok);
                if (ok)
                {
                    objIDs.append(id);
                }
            }
            _data->setObjIDs(objIDs);
        }
        // 读取自定义变量
        if (h5tools->readGroupStr(*_h5Group, "CustomizeVariable", strValue))
        {
            QString customizeVarStr = QString::fromStdString(strValue);
            QStringList customizeVarList = customizeVarStr.split(";", Qt::SkipEmptyParts);
            QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> customizeVars;

            Core::FITKEnumTransfer<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> varTransfer;
            bool isValid = false;

            for (const QString& varStr : customizeVarList)
            {
                Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable varType =
                    varTransfer.fromString(varStr.trimmed(), isValid);
                if (isValid)
                {
                    customizeVars.append(varType);
                }
            }
            _data->setCustomizeVariable(customizeVars);
        }

        return true;
    }

    bool FITKRadiossDataTimeHistoryRWALLAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKTimeHistoryRWALL*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 写入变量组类型
        h5tools->writeGroupAttrInt(*_h5Group, "VarlableGroupType", static_cast<int>(_data->getVarlableGroupType()));
        // 写入对象ID列表
        QList<int> objIDs = _data->getObjIDs();
        int count = objIDs.size();
        QStringList objIDStrs;
        for (int id : objIDs)
        {
            objIDStrs.append(QString::number(id));
        }
        h5tools->writeGroupAttrStr(*_h5Group, "ObjIDs", objIDStrs.join(";").toStdString());
        // 写入自定义变量
        QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> customizeVars = _data->getCustomizeVariable();
        QStringList customizeVarStrs{};
        Core::FITKEnumTransfer<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> varTransfer;
        bool isValid = false;
        for (const Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable& var : customizeVars)
        {
            QString varStr = varTransfer.toString(var, isValid);
            if (isValid)
            {
                customizeVarStrs.append(varStr);
            }
        }
        h5tools->writeGroupAttrStr(*_h5Group, "CustomizeVariable", customizeVarStrs.join(";").toStdString());
        QString type = Radioss::FITKTimeHistoryRWALL::GetFITKTimeHistoryRWALLRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "TimeHistoryType", type.toStdString());
        return true;
    }
}