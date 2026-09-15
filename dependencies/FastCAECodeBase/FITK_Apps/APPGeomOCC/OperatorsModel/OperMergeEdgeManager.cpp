/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperMergeEdgeManager.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperLine.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIDialog/MergeEdgeDialog.h"

namespace ModelOper
{
    void OperMergeEdgeManager::showDialog()
    {
        initDialog<GUI::MergeEdgeDialog>();
    }

    bool OperMergeEdgeManager::execProfession()
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

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelOperLine* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperLine*>(geoData->getDataByID(histId));
        if (cmdHist) lastVShapes = cmdHist->getVShapes();
        Interface::FITKAbsGeoModelOperLine* operLineCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperLine>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTMergeLine) : cmdHist;
        operLineCmd->setType(Interface::FITKAbsGeoModelOperLine::GeoLineOperType::GBTMerge);
        operLineCmd->set(vShapes);

        // 设置参数生成数据。
        if (!operLineCmd->update())
        {
            if (!cmdHist)
            {
                delete operLineCmd;
                operLineCmd = nullptr;
            }
            else
            {
                operLineCmd->set(lastVShapes);
            }
            return false;
        }

        // 暂时不添加引用
        // 移除命令引用并添加新命令引用。
        //operLineCmd->clearReference();
        //for (int i = 0; i < m_tempVShapes.size(); i++)
        //{
        //    operLineCmd->addReferenceCmdID(m_tempVShapes[i]->CmdId);
        //}

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operLineCmd->setDataObjectName(geoData->checkName(QString("MergeLine-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operLineCmd);
        }

        // 保存数据ID。
        setArgs("Id", operLineCmd->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
} // namespace ModelOper
