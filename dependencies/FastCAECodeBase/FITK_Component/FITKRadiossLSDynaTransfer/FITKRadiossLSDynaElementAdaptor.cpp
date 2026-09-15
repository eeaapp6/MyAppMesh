/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaElementAdaptor.h"
#include "FITKRadiossLSDynaLineParser.h"
#include "FITKRadiossLSDynaReader.h"
#include "FITKRadiossLSDynaWriter.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementLine.h"
#include <QStringList>

namespace Radioss
{
    FITKRadiossLSDynaElementAdaptor::FITKRadiossLSDynaElementAdaptor()
    {
    }

    QString FITKRadiossLSDynaElementAdaptor::getAdaptorClass()
    {
        return "Radioss::FITKRadiossElementAdaptor";
    }

    bool FITKRadiossLSDynaElementAdaptor::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 读取单元
        return readElementBlock();
    }

    bool FITKRadiossLSDynaElementAdaptor::adaptW()
    {
        if (_caseData == nullptr || _writer == nullptr) return false;
        // 获取单元ID
        getElementIDs();
        // 写出单元
        return writeElementBlock();
    }

    bool FITKRadiossLSDynaElementAdaptor::readElementBlock()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossMeshTopo * meshTopo = meshModel->getMeshTopo();
        if (meshTopo == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr)  return false;
        int count = partMgr->getDataCount();

        _reader->backLine();
        QString line = _reader->readLine();
        // 根据关键字读取
        QString currentKeyword = FITKRadiossLSDynaLineParser::parseKeyword(line);
        if (currentKeyword.contains("ELEMENT_BEAM", Qt::CaseInsensitive))
        {
            if (!readBeamElements()) return false;
        }
        else if (currentKeyword.contains("ELEMENT_SHELL", Qt::CaseInsensitive))
        {
            if (!readShellElements()) return false;
        }
        else if (currentKeyword.contains("ELEMENT_SOLID", Qt::CaseInsensitive))
        {
            if (!readSolidElements()) return false;
        }
        else if (currentKeyword.contains("ELEMENT_DISCRETE", Qt::CaseInsensitive))
        {
            if (!readDiscreteElements()) return false;
        }
        // 更新网格拓扑关系
        meshTopo->updateAllMeshTopo();
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::readBeamElements()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;

        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();

            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;

            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }

            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            if (fields.size() < 5)
            {
                _reader->consoleMessage(2, QString("Invalid BEAM element values: %1").arg(line));
                continue;
            }
            //读取数据
            QString eidStr = fields[0];
            QString pidStr = fields[1];
            QString n1Str = fields[2];
            QString n2Str = fields[3];
            QString n3Str = fields[4];

            bool ok1 = false, ok2 = false, ok3 = false, ok4 = false, ok5 = false;
            int eid = eidStr.toInt(&ok1);
            int pid = pidStr.toInt(&ok2);
            int n1 = n1Str.toInt(&ok3);
            int n2 = n2Str.toInt(&ok4);
            int n3 = n3Str.toInt(&ok5);

            if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5)
            {
                _reader->consoleMessage(2, QString("Invalid BEAM element values: %1").arg(line));
                continue;
            }
            // 获取部件
            FITKRadiossPart* part = getOrCreatePart(pid);
            if (part == nullptr)
            {
                _reader->consoleMessage(2, QString("Failed to create Part for PID: %1").arg(pid));
                continue;
            }
            //创建单元
            Interface::FITKAbstractElementLine* element = dynamic_cast<Interface::FITKAbstractElementLine*>(
                Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::Line2));
            if (element == nullptr) continue;
            // 设置数据
            element->setEleID(eid);
            element->setNodeID(QList<int>() << n1 << n2);
            element->setDirectionNodeID(n3);
            part->addElement(element);
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::readShellElements()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;

        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();

            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;

            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }


            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            if (fields.size() == 5)
            {
                _reader->backLine();
                return readSH3NElements();
            }
            if (fields.size() < 6)
            {
                _reader->consoleMessage(2, QString("Invalid SHELL element values: %1").arg(line));
                continue;
            }
            // 读取数据
            QString eidStr = fields[0];
            QString pidStr = fields[1];
            QString n1Str = fields[2];
            QString n2Str = fields[3];
            QString n3Str = fields[4];
            QString n4Str = fields[5];

            bool ok1 = false, ok2 = false, ok3 = false, ok4 = false, ok5 = false, ok6 = false;
            int eid = eidStr.toInt(&ok1);
            int pid = pidStr.toInt(&ok2);
            int n1 = n1Str.toInt(&ok3);
            int n2 = n2Str.toInt(&ok4);
            int n3 = n3Str.toInt(&ok5);
            int n4 = n4Str.toInt(&ok6);

            if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5 || !ok6)
            {
                _reader->consoleMessage(2, QString("Invalid SHELL element values: %1").arg(line));
                continue;
            }
            // 获取部件
            FITKRadiossPart* part = getOrCreatePart(pid);
            if (part == nullptr)
            {
                _reader->consoleMessage(2, QString("Failed to create Part for PID: %1").arg(pid));
                continue;
            }
            // 创建单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::Quad4);
            if (element == nullptr) continue;
            //设置数据
            element->setEleID(eid);
            element->setNodeID(QList<int>() << n1 << n2 << n3 << n4);
            part->addElement(element);
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::readSH3NElements()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;

        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();

            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;

            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }

            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            if (fields.size() < 5)
            {
                _reader->consoleMessage(2, QString("Invalid SHELL element values: %1").arg(line));
                continue;
            }
            // 读取数据
            QString eidStr = fields[0];
            QString pidStr = fields[1];
            QString n1Str = fields[2];
            QString n2Str = fields[3];
            QString n3Str = fields[4];

            bool ok1 = false, ok2 = false, ok3 = false, ok4 = false, ok5 = false;
            int eid = eidStr.toInt(&ok1);
            int pid = pidStr.toInt(&ok2);
            int n1 = n1Str.toInt(&ok3);
            int n2 = n2Str.toInt(&ok4);
            int n3 = n3Str.toInt(&ok5);

            if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5)
            {
                _reader->consoleMessage(2, QString("Invalid SHELL element values: %1").arg(line));
                continue;
            }
            // 获取部件
            FITKRadiossPart* part = getOrCreatePart(pid);
            if (part == nullptr)
            {
                _reader->consoleMessage(2, QString("Failed to create Part for PID: %1").arg(pid));
                continue;
            }
            // 创建单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::Tri3);
            if (element == nullptr) continue;
            // 设置数据
            element->setEleID(eid);
            element->setNodeID(QList<int>() << n1 << n2 << n3);
            part->addElement(eid,element);
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::readDiscreteElements()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;

        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();

            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;

            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }

            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            if (fields.size() < 4)
            {
                _reader->consoleMessage(2, QString("Invalid DISCRETE element values: %1").arg(line));
                continue;
            }
            // 读取数据
            QString eidStr = fields[0];
            QString pidStr = fields[1];
            QString n1Str = fields[2];
            QString n2Str = fields[3];

            bool ok1 = false, ok2 = false, ok3 = false, ok4 = false;
            int eid = eidStr.toInt(&ok1);
            int pid = pidStr.toInt(&ok2);
            int n1 = n1Str.toInt(&ok3);
            int n2 = n2Str.toInt(&ok4);

            if (!ok1 || !ok2 || !ok3 || !ok4)
            {
                _reader->consoleMessage(2, QString("Invalid DISCRETE element values: %1").arg(line));
                continue;
            }
            // 获取部件
            FITKRadiossPart* part = getOrCreatePart(pid);
            if (part == nullptr)
            {
                _reader->consoleMessage(2, QString("Failed to create Part for PID: %1").arg(pid));
                continue;
            }
            // 创建单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::Line2);
            if (element == nullptr) continue;
            // 设置数据
            element->setEleID(eid);
            element->setNodeID(QList<int>() << n1 << n2);
            part->addElement(element);
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::readSolidElements()
    {
        if (_reader == nullptr || _caseData == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;

        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();

            if (line.trimmed().isEmpty() || line.startsWith('$')) continue;

            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }

            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            bool ok1 = false, ok2 = false;
            int eid = fields[0].toInt(&ok1);
            int pid = fields[1].toInt(&ok2);

            if (!ok1 || !ok2)
            {
                _reader->consoleMessage(2, QString("Invalid SOLID element values: %1").arg(line));
                continue;
            }
            // 获取部件
            FITKRadiossPart* part = getOrCreatePart(pid);
            if (part == nullptr)
            {
                _reader->consoleMessage(2, QString("Failed to create Part for PID: %1").arg(pid));
                continue;
            }
            //读取节点
            QList<int> nodeIds;
            for (int i = 2; i < fields.size() && i < 12; ++i)
            {
                bool okNode = false;
                int nodeId = fields[i].toInt(&okNode);
                if (!okNode)
                {
                    _reader->consoleMessage(2, QString("Invalid node ID in SOLID element: %1").arg(line));
                    nodeIds.clear();
                    break;
                }
                nodeIds.append(nodeId);
            }

            if (nodeIds.size() < 4)
            {
                _reader->consoleMessage(2, QString("Invalid SOLID element node line (unsupported node count %1): %2").arg(nodeIds.size()).arg(line));
                continue;
            }
            // 判断类型
            Interface::FITKModelEnum::FITKEleType eleType;
            if (nodeIds.size() == 4)
                eleType = Interface::FITKModelEnum::Tet4;
            else if (nodeIds.size() == 8)
                eleType = Interface::FITKModelEnum::Hex8;
            else
                eleType = Interface::FITKModelEnum::Tet10;
            // 创建单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(eleType);
            if (element == nullptr) continue;
            //设置数据
            element->setEleID(eid);
            element->setNodeID(nodeIds);
            part->addElement(element);
        }
        return true;
    }

    void FITKRadiossLSDynaElementAdaptor::getElementIDs()
    {
        _eleTypeIds.clear();
        _discreteEleIds.clear();
        //获取网格数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return;
        // 部件管理器
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return;
        // 遍历部件
        int partCount = partMgr->getDataCount();
        for (int i = 0; i < partCount; ++i)
        {
            FITKRadiossPart* part = partMgr->getDataByIndex(i);
            int elementCount = part->getElementCount();
            //遍历单元
            for (int j = 0; j < elementCount; ++j)
            {
                Interface::FITKAbstractElement* element = part->getElementAt(j);
                if (element == nullptr) continue;
                Interface::FITKModelEnum::FITKEleType eleType = element->getEleType();
                int eleId = element->getEleID();
                if (eleType == Interface::FITKModelEnum::Line2)
                {
                    Interface::FITKAbstractElementLine* lineElement = dynamic_cast<Interface::FITKAbstractElementLine*>(element);
                    if (lineElement && lineElement->getDirectionNodeID() == 0)
                    {
                        _discreteEleIds.append(eleId);
                        continue;
                    }
                }
                if (!_eleTypeIds.contains(eleType))
                    _eleTypeIds.insert(eleType, QList<int>());
                _eleTypeIds[eleType].append(eleId);
            }
        }
    }

    bool FITKRadiossLSDynaElementAdaptor::writeElementBlock()
    {
        if (_writer == nullptr) return false;

        bool ok = true;

        ok &= writeBeamElements();
        ok &= writeShellElements();
        ok &= writeSH3NElements();
        ok &= writeDiscreteElements();
        ok &= writeHex8Elements();
        ok &= writeTet4Elements();
        ok &= writeSolidElements();

        return ok;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeBeamElements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Line2];
        if (eleIDs.empty()) return true;
        // 写出单元
        *stream << "*ELEMENT_BEAM" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Line2) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() < 2) continue;

            Interface::FITKAbstractElementLine* lineElement = dynamic_cast<Interface::FITKAbstractElementLine*>(element);
            if (!lineElement) continue;
            int dirNodeId = lineElement->getDirectionNodeID();

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(dirNodeId, 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeShellElements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Quad4];
        if (eleIDs.empty()) return true;
        //写出数据
        *stream << "*ELEMENT_SHELL" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Quad4) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 4) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeSH3NElements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Tri3];
        if (eleIDs.empty()) return true;
        // 写出单元
        *stream << "*ELEMENT_SHELL" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Tri3) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 3) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeSolidElements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Tet10];
        if (eleIDs.empty()) return true;
        // 写出单元
        *stream << "*ELEMENT_SOLID" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Tet10) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();

            if (nodeIds.size() != 10) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[4], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[5], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[6], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[7], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[8], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[9], 10, 10, QChar(' '))
                << endl;
        }

        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeDiscreteElements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        if (_discreteEleIds.isEmpty()) return true;
        // 写出单元
        *stream << "*ELEMENT_DISCRETE" << endl;
        for (int eleId : _discreteEleIds)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Line2) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 2) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeHex8Elements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Hex8];
        if (eleIDs.isEmpty()) return true;
        //写出单元
        *stream << "*ELEMENT_SOLID" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Hex8) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 8) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[4], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[5], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[6], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[7], 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    bool FITKRadiossLSDynaElementAdaptor::writeTet4Elements()
    {
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        // 获取数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;
        // 获取单元ID
        QList<int> eleIDs = _eleTypeIds[Interface::FITKModelEnum::Tet4];
        if (eleIDs.isEmpty()) return true;
        // 写出单元
        *stream << "*ELEMENT_SOLID" << endl;
        for (int eleId : eleIDs)
        {
            QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result = partMgr->getElementByID(eleId);
            FITKRadiossPart* part = result.first;
            Interface::FITKAbstractElement* element = result.second;
            if (part == nullptr || element == nullptr) continue;
            if (element->getEleType() != Interface::FITKModelEnum::FITKEleType::Tet4) continue;
            int partId = part->getDataObjectID();
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 4) continue;

            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(partId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '))
                << endl;
        }
        return true;
    }

    FITKRadiossPart* FITKRadiossLSDynaElementAdaptor::getOrCreatePart(int pid)
    {
        if (_caseData == nullptr) return nullptr;
        // 获取网格数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr) return nullptr;
        // 部件管理器
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return nullptr;
        Core::FITKAbstractDataIDAlter dataIDAlter;
        FITKRadiossPart* part = partMgr->getDataByID(pid);
        if (part == nullptr)
        {
            part = new FITKRadiossPart();
            dataIDAlter.modifyDataID(part, pid);
            part->setDataObjectName(QString("AutoCreated_Part_%1").arg(pid));
            partMgr->appendDataObj(part);
        }
        return part;
    }
}
