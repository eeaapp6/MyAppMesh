/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperFillFaceManager.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperFace.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIDialog/FillFaceInfoDialog.h"

namespace ModelOper
{
    void OperFillFaceManager::showDialog()
    {
        initDialog<GUI::FillFaceInfoDialog>();
    }

    bool OperFillFaceManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        QList<Interface::VirtualShape> vShapes;
        // 保留历史参数。
        QList<Interface::VirtualShape> lastVShapes;
        bool flag = argValue("VShapes", vShapes);
        if (!flag) return false;

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (geoData == nullptr) return false;

        //QList<int> faceIDs;
        //for (int i = 0; i < m_tempVShapes.size(); i++)
        //{
        //    //数据获取
        //    FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i]->CmdId);
        //    if (comm == nullptr) return false;
        //
        //    OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i]->VirtualTopoId);
        //    if (vOCCShapeEdge == nullptr) return false;
        //
        //
        //
        //    // 创建或获取历史线操作数据。
        //    Interface::FITKAbsVirtualTopo* vTopo = dynamic_cast<Interface::FITKAbsVirtualTopo*>(geoData->getDataByID(m_tempVShapes[i]->VirtualTopoId));
        //    if (vTopo == nullptr) return;
        //    // 向上查找所选边对应体数据。
        //    int newEdgeId = vTopo->getDataObjectID();
        //    Interface::FITKVirtualTopoMapper* vMapper = new Interface::FITKVirtualTopoMapper;
        //    vMapper->mapTopo(vTopo, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        //
        //    bool hasSameSolid = true;
        //
        //    // 获取查询结果。
        //    int nRet = vMapper->length();
        //    for (int i = 0; i < nRet; i++)
        //    {
        //        Interface::FITKAbsVirtualTopo* vFace = vMapper->virtualTopo(i);
        //        int newFaceId = vFace->getDataObjectID();
        //        faceIDs.append(newFaceId);
        //    }
        //}
        //qDebug() << faceIDs;

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelOperFace* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperFace*>(geoData->getDataByID(histId));
        if (cmdHist) lastVShapes = cmdHist->getVShapes();
        Interface::FITKAbsGeoModelOperFace* operFace = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperFace>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTFillGapsFace) : cmdHist;
        if (operFace == nullptr) return false;
        operFace->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType::GBTFillGaps);
        operFace->set(vShapes);

        // 设置参数生成数据。
        if (!operFace->update())
        {
            if (!cmdHist)
            {
                delete operFace;
                operFace = nullptr;
            }
            else
            {
                operFace->set(lastVShapes);
            }
            return false;
        }

        // 移除命令引用并添加新命令引用。
        operFace->clearReference();
        for (int i = 0; i < vShapes.size(); i++)
        {
            operFace->addReferenceCmdID(vShapes[i].CmdId);
        }

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operFace->setDataObjectName(geoData->checkName(QString("FillGap-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operFace);
        }

        // 保存数据ID。
        setArgs("Id", operFace->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
} // namespace ModelOper
