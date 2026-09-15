/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaCaseAdaptor.h"
#include "FITKRadiossLSDynaWriter.h"
#include "FITKRadiossLSDynaNodeAdaptor.h"
#include "FITKRadiossLSDynaPartAdaptor.h"
#include "FITKRadiossLSDynaElementAdaptor.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"

namespace Radioss
{
    QString FITKRadiossLSDynaCaseAdaptor::getAdaptorClass()
    {
        return "Radioss::FITKRadiossCase";
    }

    bool FITKRadiossLSDynaCaseAdaptor::adaptR()
    {
        return true;
    }

    bool FITKRadiossLSDynaCaseAdaptor::adaptW()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_writer == nullptr || _caseData == nullptr) return false;
        //获取文件流
        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;
        bool ok = true;
        // 写出
        *stream << "*KEYWORD" << endl;
        // 写出节点数据
        ok = writeNodeData();
        // 写出部件数据
        ok &= writePartData();
        // 写出单元数据
        ok &= writeElementData();
        *stream << "*END" << endl;
        return ok;
    }

    bool FITKRadiossLSDynaCaseAdaptor::writeNodeData()
    {
        // 获取网格数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr)  return false;
        // 获取节点
        FITKRadiossNodes* nodes = meshModel->getNodes();
        if (nodes == nullptr) return false;
        // 获取适配器
        FITKRadiossLSDynaNodeAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<FITKRadiossLSDynaNodeAdaptor>("k", nodes);
        if (adaptor == nullptr)
        {
            AppFrame::FITKMessageError("Failed to create node adaptor.");
            return false;
        }
        adaptor->setFileWriter(_writer);
        adaptor->setDataObject(nodes);
        bool ok = adaptor->adaptW();
        if (!ok)
        {
            AppFrame::FITKMessageError("Node adaptor write failed.");
            return false;
        }
        return true;
    }

    bool FITKRadiossLSDynaCaseAdaptor::writePartData()
    {
        // 获取网格数据
        FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (meshModel == nullptr)  return false;
        // 获取部件
        FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr)  return false;
        int count = partMgr->getDataCount();
        for(int i = 0; i < count; i++)
        {
            FITKRadiossPart* part = partMgr->getDataByIndex(i);
            if (part == nullptr) continue;
            // 获取适配器
            FITKRadiossLSDynaPartAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<FITKRadiossLSDynaPartAdaptor>("k", part);
            if (adaptor == nullptr)
            {
                AppFrame::FITKMessageError("Failed to create node adaptor.");
                return false;
            }
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(part);
            bool ok = adaptor->adaptW();
            if (!ok)
            {
                AppFrame::FITKMessageError("Node adaptor write failed.");
                return false;
            }
        }

        return true;
    }
    bool FITKRadiossLSDynaCaseAdaptor::writeElementData()
    {
        // 获取适配器
        FITKRadiossLSDynaElementAdaptor adaptor;
        adaptor.setCaseData(_caseData);
        adaptor.setFileWriter(_writer);
        bool ok = adaptor.adaptW();
        if (!ok)
        {
            AppFrame::FITKMessageError("Element adaptor write failed.");
            return false;
        }
        return ok;
    }
}
