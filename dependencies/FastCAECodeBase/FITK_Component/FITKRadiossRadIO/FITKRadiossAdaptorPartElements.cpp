/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPartElements.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementLine.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    FITKRadiossAdaptorPartElements::FITKRadiossAdaptorPartElements(FITKRadiossRadReader *reader, FITKRadiossRadWriter *writer, FITKRadiossPart *part)
        : _reader(reader),_writer(writer), _part(part)
    {
    }

    bool FITKRadiossAdaptorPartElements::readElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;

        //回退一行
        _reader->backLine(); 
        bool ok = false;
        // 读取元素数据
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/SPRING"))
            {
                //弹簧单元
                ok = this->readSpringElements();
                break;
            }
            else if (line.startsWith("/SHELL"))
            {
                //四边形单元
                ok = this->readShellElements();
                break;
            }
            else if (line.startsWith("/SH3N"))
            {
                //三角形单元
                ok = this->readSH3NElements();
                break;
            }
            else if (line.startsWith("/BRICK") )
            {
                //六面体单元
                ok = this->readBrickElements();
                break;
            }
            else if (line.startsWith("/TETRA10") )
            {
                //10节点高阶4面体单元
                ok = this->readTETRA10Elements();
                break;
            }
            else if (line.startsWith("/TETRA4"))
            {
                //4面体单元
                ok = this->readTETRA4Elements();
                break;
            }
            else if (line.startsWith("/BEAM"))
            {
                //梁单元
                ok = this->readBEAMElements();
                break;
            }
            else if (line.startsWith("/SPHCEL"))
            {
                // 粒子单元
                ok = this->readSPHCELElements();
                break;
            }
            else 
            {
                //未知单元类型不处理
                _reader->consoleMessage(2, QString("Unknown element type: %1").arg(line));
                ok = false;
                break;
            }
        }
        if (!ok)
        {
            _reader->consoleMessage(3, "Read Element Error");
            return false;
        }
        _reader->backLine();
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        bool ok = true;
        int elementCount = _part->getElementCount();
        for (int i = 0; i < elementCount; ++i)
        {
            Interface::FITKAbstractElement* element = _part->getElementAt(i);
            if (element == nullptr) continue;
            Interface::FITKModelEnum::FITKEleType eleType = element->getEleType();
            int eleId = element->getEleID();
            if (!_eleTypeIds.contains(eleType))
                _eleTypeIds.insert(eleType, QList<int>());
            _eleTypeIds[eleType].append(eleId);
        }
        //写出弹簧单元
        ok &= this->writeSpringElements();
        //写出三角形单元
        ok &= this->writeSH3NElements();
        //写出四边形单元
        ok &= this->writeShellElements();
        //写出六面体单元
        ok &= this->writeBrickElements();
        //写出高阶4面体单元
        ok &= this->writeTETRA10Elements();
        //写出4面体单元
        ok &= this->writeTETRA4Elements();
        //写出梁单元
        ok &= this->writeBEAMElements();
        //写出粒子单元
        ok &= this->writeSPHCELElements();

        if (!ok) _writer->consoleMessage(3, QString("Write Element Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        return ok;
    }

    bool FITKRadiossAdaptorPartElements::writeSpringElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取弹簧单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Line2];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr|| infoMapper == nullptr ) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# SPRING elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/SPRING+/partID
        *stream << QString("/SPRING/%1").arg(partID) << endl;
        //弹簧单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 2) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '));
            //暂不处理非对称弹簧及新接头类型
            *stream << endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeSH3NElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取三角形单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Tri3];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# SH3N elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/SH3N+/partID，暂不处理unitId
        *stream << QString("/SH3N/%1").arg(partID);
        *stream << endl;
        //三角形单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 3) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '));
            //暂不处理偏斜角度及厚度信息
            *stream << endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeShellElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取四边形单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Quad4];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# SHELL elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/SHELL+/partID，暂不处理unitId
        *stream << QString("/SHELL/%1").arg(partID);
        *stream << endl;
        //四边形单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 4) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '));
            //暂不处理偏斜角度及厚度信息
            *stream << endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeBrickElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取六面体单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Hex8];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# BRICK elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/BRICK+/partID
        *stream << QString("/BRICK/%1").arg(partID) << endl;
        //六面体单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 8) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
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
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeTETRA10Elements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取10节点4面体单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Tet10];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# TETRA10 elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/TETRA10+/partID
        *stream << QString("/TETRA10/%1").arg(partID) << endl;
        //4面体单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 10) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' ')) << endl;
            *stream << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[4], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[5], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[6], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[7], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[8], 10, 10, QChar(' '))
                    << QString("%1").arg(nodeIds[9], 10, 10, QChar(' '))<< endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeTETRA4Elements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取4面体单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Tet4];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# TETRA4 elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/SH3N+/partID，暂不处理unitId
        *stream << QString("/TETRA4/%1").arg(partID);
        *stream << endl;
        //4面体单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            if (nodeIds.size() != 4) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[2], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[3], 10, 10, QChar(' '));
            //暂不处理偏斜角度及厚度信息
            *stream << endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeBEAMElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取梁单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Line2];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# BEAM elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/BEAM+/partID
        *stream << QString("/BEAM/%1").arg(partID);
        *stream << endl;
        //梁单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElementLine* element = dynamic_cast<Interface::FITKAbstractElementLine*>(_part->getElementByID(eleId));
            if (element == nullptr) continue;
            QList<int> nodeIds = element->getAllNodes();
            int dirNodeID = element->getDirectionNodeID();
            if (nodeIds.size() != 2) continue;
            *stream << QString("%1").arg(eleId, 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[0], 10, 10, QChar(' '))
                << QString("%1").arg(nodeIds[1], 10, 10, QChar(' '))
                << QString("%1").arg(dirNodeID, 10, 10, QChar(' '));
            *stream << endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::writeSPHCELElements()
    {
        if (_writer == nullptr || _part == nullptr) return false;
        //获取粒子单元列表
        QList<int> eleIds = _eleTypeIds[Interface::FITKModelEnum::Vertex1];
        if (eleIds.empty()) return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        int partID = infoMapper->getRadIDByPartID(_part->getDataObjectID());
        QString partTitle = _part->getDataObjectName();
        *stream << QString("# SPHCEL elements of /PART/%1/%2:").arg(partID).arg(partTitle) << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        //关键字/SPHCEL+/partID
        *stream << QString("/SPHCEL/%1").arg(partID);
        *stream << endl;
        //粒子单元数量
        int eleCount = eleIds.size();
        for (int i = 0; i < eleCount; ++i)
        {
            int eleId = eleIds[i];
            Interface::FITKAbstractElement* element = _part->getElementByID(eleId);
            if (element == nullptr) continue;
            int nodeId = element->getNodeID(0);
            if (nodeId<1) continue;
            *stream << QString("%1").arg(nodeId, 10, 10, QChar(' '))<< endl;
        }
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readSpringElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleSpring = line.simplified().split(" ");
            if (eleSpring.size() != 3)
            {
                _reader->consoleMessage(2, QString("Invalid Spring element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleSpring[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < 3; ++i) {
                int radID_node = eleSpring[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }

            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Line2);

            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);

            //暂不处理非对称弹簧及新接头类型
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readShellElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleShell = line.simplified().split(" ");
            if (eleShell.size() != 5)
            {
                _reader->consoleMessage(2, QString("Invalid SHELL element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleShell[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < 5; ++i){
                int radID_node = eleShell[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }

            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Quad4);

            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);

            //暂不处理偏斜角度及厚度信息
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readSH3NElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleSH3N = line.simplified().split(" ");
            if (eleSH3N.size() != 4)
            {
                _reader->consoleMessage(2, QString("Invalid SH3N element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleSH3N[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < eleSH3N.size(); ++i) {
                int radID_node = eleSH3N[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }

            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Tri3);

            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);

            //暂不处理偏斜角度及厚度信息
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readBrickElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if(line.startsWith("/")) 
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleBrick = line.simplified().split(" ");
            if (eleBrick.size() < 9)
            {
                _reader->consoleMessage(2, QString("Invalid BRICK element line: %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleBrick[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < eleBrick.size(); ++i) {
                int radID_node = eleBrick[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }
            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                    createElement(Interface::FITKModelEnum::Hex8);
            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readTETRA10Elements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleRadID = line.simplified().split(" ");
            if(eleRadID.size()!=1) {
                _reader->consoleMessage(2, QString("Invalid TETRA10 element line: %1").arg(line));
                continue;
            }
            line = _reader->readLine();
            QStringList eleBrick = line.simplified().split(" ");
            if (eleBrick.size() != 10)
            {
                _reader->consoleMessage(2, QString("Invalid TETRA10 element line: %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleRadID[0].toInt();
            QList<int> nodeIds;
            for (int i = 0; i < eleBrick.size(); ++i) {
                int radID_node = eleBrick[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }
            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Tet10);
            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readTETRA4Elements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleSH3N = line.simplified().split(" ");
            if (eleSH3N.size() != 5)
            {
                _reader->consoleMessage(2, QString("Invalid TETRA4 element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleSH3N[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < eleSH3N.size(); ++i) {
                int radID_node = eleSH3N[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }

            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElement *element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Tet4);

            int elementId = _part->addElement(element);
            element->setNodeID(nodeIds);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);
        }
        return true;
    }

    bool FITKRadiossAdaptorPartElements::readBEAMElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleBEAM = line.simplified().split(" ");
            if (eleBEAM.size() != 4)
            {
                _reader->consoleMessage(2, QString("Invalid BEAM element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleBEAM[0].toInt();
            QList<int> nodeIds;
            for (int i = 1; i < eleBEAM.size(); ++i) {
                int radID_node = eleBEAM[i].toInt();
                nodeIds.append(infoMapper->getNodeIDByRadID(radID_node));
            }
            if (nodeIds.size() < 3) continue;
            //新建单元，添加单元id及节点id信息
            Interface::FITKAbstractElementLine *element = dynamic_cast<Interface::FITKAbstractElementLine*>(Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Line2));

            int elementId = _part->addElement(element);
            element->setNodeID(QList<int>() << nodeIds[0] << nodeIds[1]);
            element->setDirectionNodeID(nodeIds[2]);
            //添加映射
            infoMapper->insertEleIDMap(radId, elementId);
        }
        return true;
    }
    bool FITKRadiossAdaptorPartElements::readSPHCELElements()
    {
        if (_reader == nullptr || _part == nullptr) return false;
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!infoMapper)return false;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //跳过无效行
            QStringList eleBEAM = line.simplified().split(" ");
            if (eleBEAM.size() != 1)
            {
                _reader->consoleMessage(2, QString("Invalid SPHCEL element line : %1").arg(line));
                continue;
            }
            //单元节点id
            const int radId = eleBEAM[0].toInt();
            int nodeId = infoMapper->getNodeIDByRadID(radId);


            //新建单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::
                createElement(Interface::FITKModelEnum::Vertex1);
            
            _part->addElement(element);
            element->setNodeID(0,nodeId);
            //添加映射
            //infoMapper->insertEleIDMap(radId, elementId);
        }
        return true;
    }
}