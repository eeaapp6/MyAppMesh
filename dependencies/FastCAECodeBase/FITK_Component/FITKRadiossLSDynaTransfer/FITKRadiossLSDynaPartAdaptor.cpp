/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaPartAdaptor.h"
#include "FITKRadiossLSDynaReader.h"
#include "FITKRadiossLSDynaWriter.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include <QStringList>

namespace Radioss
{
    FITKRadiossLSDynaPartAdaptor::FITKRadiossLSDynaPartAdaptor()
    {
    }

    QString FITKRadiossLSDynaPartAdaptor::getAdaptorClass()
    {
        return "Radioss::FITKRadiossPart";
    }

    bool FITKRadiossLSDynaPartAdaptor::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 读取
        return readPartBlock();
    }

    bool FITKRadiossLSDynaPartAdaptor::adaptW()
    {
        m_partData = dynamic_cast<FITKRadiossPart*>(_dataObj);
        if (m_partData == nullptr || _writer == nullptr) return false;
        // 写出
        return writePartBlock();
    }

    bool FITKRadiossLSDynaPartAdaptor::readPartBlock()
    {
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr)  return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr)  return false;
        Core::FITKAbstractDataIDAlter dataIDAlter;

        FITKRadiossPart* currentPart = nullptr;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            // 跳过无效行
            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;
            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }
            // 读取数据
            QString trimmedLine = line.trimmed();
            // 判断是否为数据行
            auto isDataLine = [](const QString& str) -> bool {
                QString s = str.trimmed();
                if (s.isEmpty()) return false;
                for (int i = 0; i < s.length(); ++i)
                {
                    QChar c = s[i];
                    if (!c.isDigit() && !c.isSpace())
                    {
                        return false;
                    }
                }
                return true;
            };
            // title行为部件名称
            if (currentPart == nullptr || !isDataLine(trimmedLine))
            {
                if (currentPart != nullptr)
                {
                    partMgr->appendDataObj(currentPart);
                }

                currentPart = new FITKRadiossPart();
                currentPart->setDataObjectName(trimmedLine);
            }
            else
            {
                // 部件数据
                QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

                if (fields.size() < 3)
                {
                    _reader->consoleMessage(3, QString("Invalid PART data line: %1").arg(line));
                    continue;
                }

                QString pidStr = fields[0];
                QString secidStr = fields[1];
                QString midStr = fields[2];

                bool ok1 = false, ok2 = false, ok3 = false;
                int pid = pidStr.toInt(&ok1);
                int secid = secidStr.toInt(&ok2);
                int mid = midStr.toInt(&ok3);

                if (!ok1 || !ok2 || !ok3)
                {
                    _reader->consoleMessage(3, QString("Invalid PART values: %1").arg(line));
                    continue;
                }
                // 判断部件是否存在（如果文件中单元数据在部件数据前，读取单元时会先创建对应ID的空部件，读部件时再设置数据）
                FITKRadiossPart* existingPart = partMgr->getDataByID(pid);
                if (existingPart != nullptr && existingPart != currentPart)
                {
                    existingPart->setPropertyID(secid);
                    existingPart->setMaterialID(mid);
                    existingPart->setDataObjectName(currentPart->getDataObjectName());
                    delete currentPart;
                    currentPart = existingPart;
                    _reader->consoleMessage(1, QString("Updated existing Part for PID: %1").arg(pid));
                }
                else
                {
                    currentPart->setPropertyID(secid);
                    currentPart->setMaterialID(mid);
                    dataIDAlter.modifyDataID(currentPart, pid);
                }
            }
        }

        if (currentPart != nullptr)
        {
            partMgr->appendDataObj(currentPart);
        }

        return true;
    }

    bool FITKRadiossLSDynaPartAdaptor::writePartBlock()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;

        *stream << "*PART" << endl;
        *stream << m_partData->getDataObjectName() << endl;

        int pid = m_partData->getDataObjectID();
        int secid = m_partData->getPropertyID();
        int mid = m_partData->getMaterialID();

        *stream << QString("%1").arg(pid, 10, 10, QChar(' '))
            << QString("%1").arg(secid, 10, 10, QChar(' '))
            << QString("%1").arg(mid, 10, 10, QChar(' '))
            << endl;

        return true;
    }

}