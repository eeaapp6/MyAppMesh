/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperDeleteFloatingEdgeManager.h"

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
#include "GUIDialog/DeleteFloatingEdgeDialog.h"

namespace ModelOper
{
    /** 删除悬浮边 */
/*@{*/
    void OperDeleteFloatingEdgeManager::showDialog()
    {
        initDialog<GUI::DeleteFloatingEdgeDialog>();
    }

    bool OperDeleteFloatingEdgeManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        QList<Interface::VirtualShape> vShapes;
        // 保留历史参数。
        QList<Interface::VirtualShape> lastVShapes;

        bool flag = true;
        flag &= argValue("VShapes", vShapes);
        if (!flag) return false;

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge*>(geoData->getDataByID(histId));
        if (cmdHist) lastVShapes = cmdHist->getVShapes();

        Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge* operFaceDeleteFloatingEdge = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTDeleteFloatingEdge) : cmdHist;

        if (operFaceDeleteFloatingEdge == nullptr) return false;
        operFaceDeleteFloatingEdge->setType(Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge::GeoFaceOperType::GBTDeleteFloatingEdge);
        operFaceDeleteFloatingEdge->set(vShapes);

        // 设置参数生成数据。
        if (!operFaceDeleteFloatingEdge->update())
        {
            if (!cmdHist)
            {
                delete operFaceDeleteFloatingEdge;
                operFaceDeleteFloatingEdge = nullptr;
            }
            else
            {
                cmdHist->set(lastVShapes);
            }
            return false;
        }

        // 移除命令引用并添加新命令引用。
        operFaceDeleteFloatingEdge->clearReference();
        for (int i = 0; i < vShapes.size(); i++)
        {
            operFaceDeleteFloatingEdge->addReferenceCmdID(vShapes[i].CmdId);
        }

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operFaceDeleteFloatingEdge->setDataObjectName(geoData->checkName(QString("FloatingFace-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operFaceDeleteFloatingEdge);
        }

        // 保存数据ID。
        setArgs("Id", operFaceDeleteFloatingEdge->getDataObjectID());

        // 结束创建模型。
        finalize();
        return true;
    }
    /*@*/
} // namespace ModelOper
