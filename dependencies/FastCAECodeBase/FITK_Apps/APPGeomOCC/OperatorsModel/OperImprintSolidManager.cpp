/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperImprintSolidManager.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperImprintSolid.h"
// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIDialog/ImprintSolidDialog.h"

namespace ModelOper
{
    void OperImprintSolidManager::showDialog()
    {
        initDialog<GUI::ImprintSolidDialog>();
    }

    bool OperImprintSolidManager::execProfession()
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
        Interface::FITKAbsGeoModelOperImprintSolid* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperImprintSolid*>(geoData->getDataByID(histId));
        if (cmdHist) lastVShapes = cmdHist->getVShapes();

        Interface::FITKAbsGeoModelOperImprintSolid* operImprintSolid = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperImprintSolid>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTImprintSolid) : cmdHist;

        if (operImprintSolid == nullptr) return false;
        operImprintSolid->setType(Interface::FITKAbsGeoModelOperImprintSolid::GBTImprintSolid);
        operImprintSolid->clear();
        operImprintSolid->add(vShapes);
        // 设置参数生成数据。
        if (!operImprintSolid->update())
        {
            if (!cmdHist)
            {
                delete operImprintSolid;
                operImprintSolid = nullptr;
            }
            else
            {
                operImprintSolid->clear();
                operImprintSolid->add(lastVShapes);
            }
            return false;
        }

        // 移除命令引用并添加新命令引用。
        operImprintSolid->clearReference();
        for (int i = 0; i < vShapes.size(); i++)
        {
            operImprintSolid->addReferenceCmdID(vShapes[i].CmdId);
        }

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operImprintSolid->setDataObjectName(geoData->checkName(QString("ImprintSolid-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operImprintSolid);
        }

        // 暂存数据ID。
        setArgs("Id", operImprintSolid->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
} // namespace ModelOper
