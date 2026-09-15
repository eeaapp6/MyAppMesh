/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperNBodyUniteManager.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperNBodyUnite.h"

// GUI
#include "GUIDialog/NBodyUniteInfoDialog.h"

namespace ModelOper
{
    void OperNBodyUniteManager::showDialog()
    {
        initDialog<GUI::NBodyUniteInfoDialog>();
    }

    bool OperNBodyUniteManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        QList<Interface::VirtualShape> inputSolidTopos;
        bool flag = argValue("InputSolidIndice", inputSolidTopos);
        if (!flag)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 保留历史参数。
        QList<Interface::VirtualShape> lastSolidTopos;

        // 创建或获取历史布尔合并数据。
        Interface::FITKAbsGeoOperNBodyUnite* cmdHist = dynamic_cast<Interface::FITKAbsGeoOperNBodyUnite*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastSolidTopos = cmdHist->getShapes();
        }

        Interface::FITKAbsGeoOperNBodyUnite* nbUniteCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoOperNBodyUnite>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTNBodyUnite) : cmdHist;

        // 设置参数生成数据。
        for (Interface::VirtualShape & vSolid : inputSolidTopos)
        {
            nbUniteCmd->addShape(vSolid);
        }

        flag = nbUniteCmd->update();
        if (!flag)
        {
            // 还原历史参数。
            if (cmdHist)
            {
                cmdHist->clearShapes();
                for (Interface::VirtualShape & vSolid : lastSolidTopos)
                {
                    cmdHist->addShape(vSolid);
                }
            }
            // 析构创建的数据对象。
            else
            {
                delete nbUniteCmd;
                nbUniteCmd = nullptr;
            }

            return false;
        }

        // 移除命令引用并添加新命令引用。
        nbUniteCmd->clearReference();

        QList<int> refIds;
        for (Interface::VirtualShape & vSolid : inputSolidTopos)
        {
            if (!refIds.contains(vSolid.CmdId))
            {
                refIds.push_back(vSolid.CmdId);
            }
        }

        for (const int & cmdId : refIds)
        {
            nbUniteCmd->addReferenceCmdID(cmdId);
        }

        // 添加至数据仓库。
        if (!cmdHist)
        {
            nbUniteCmd->setDataObjectName(geoData->checkName(QString("NBodyUnite-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(nbUniteCmd);
        }

        // 暂存数据ID。
        setArgs("Id", nbUniteCmd->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
} // namespace ModelOper
