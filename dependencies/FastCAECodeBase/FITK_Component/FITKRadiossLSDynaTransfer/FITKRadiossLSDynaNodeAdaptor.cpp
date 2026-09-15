/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaNodeAdaptor.h"
#include "FITKRadiossLSDynaReader.h"
#include "FITKRadiossLSDynaWriter.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfaceModel/FITKNodeList.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include <QStringList>

namespace Radioss
{
    FITKRadiossLSDynaNodeAdaptor::FITKRadiossLSDynaNodeAdaptor()
    {
    }

    QString FITKRadiossLSDynaNodeAdaptor::getAdaptorClass()
    {
        return "Radioss::FITKRadiossNodes";
    }

    bool FITKRadiossLSDynaNodeAdaptor::adaptR()
    {
        if (_caseData == nullptr) return false;
        m_nodeData = _caseData->getMeshModel()->getNodes();
        if (m_nodeData == nullptr || _reader == nullptr) return false;
        // 读取节点数据
        return readNodeBlock();
    }

    bool FITKRadiossLSDynaNodeAdaptor::adaptW()
    {
        m_nodeData = dynamic_cast<FITKRadiossNodes*>(_dataObj);
        if (m_nodeData == nullptr || _writer == nullptr) return false;
        // 写出节点数据
        return writeNodeBlock();
    }

    bool FITKRadiossLSDynaNodeAdaptor::readNodeBlock()
    {
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            // 跳过空行
            if (line.trimmed().isEmpty()) continue;
            // 跳过无效行
            if (line.startsWith('$')) continue;
            // 读到关键字行退出
            if (line.startsWith('*'))
            {
                _reader->backLine();
                break;
            }
            // 分割数据
            QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if (fields.size() < 1)
            {
                _reader->consoleMessage(3, QString("Invalid Node line (no node ID): %1").arg(line));
                continue;
            }

            QString nidStr = fields[0];
            bool ok1 = false;
            int nid = nidStr.toInt(&ok1);
            if (!ok1)
            {
                _reader->consoleMessage(3, QString("Invalid Node ID: %1").arg(nidStr));
                continue;
            }
            // 空值处理（未实现，后续修改）
            double x = 0.0, y = 0.0, z = 0.0;
            if (fields.size() >= 2)
            {
                x = fields[1].toDouble();
            }
            if (fields.size() >= 3)
            {
                y = fields[2].toDouble();
            }
            if (fields.size() >= 4)
            {
                z = fields[3].toDouble();
            }
            // 设置数据
            m_nodeData->addNode(nid, x, y, z);
        }
        return true;
    }

    bool FITKRadiossLSDynaNodeAdaptor::writeNodeBlock()
    {
        QTextStream* stream = _writer->getStream(); 
        if (stream == nullptr) return false;
        // 写出
        *stream << "*NODE" << endl;
        // 节点数量
        int nodeCount = m_nodeData->getNodeCount();
        for (int i = 0; i < nodeCount; ++i)
        {
            Core::FITKNode* node = m_nodeData->getNodeAt(i);
            int nodeId = node->getNodeID();
            double x = node->x();
            double y = node->y();
            double z = node->z();

            *stream << QString("%1").arg(nodeId, 10, 10, QChar(' '))
                << QString("%1").arg(x, 20, 'g', 10, QChar(' '))
                << QString("%1").arg(y, 20, 'g', 10, QChar(' '))
                << QString("%1").arg(z, 20, 'g', 10, QChar(' '))
                << endl;
        }
        return true;
    }

}
