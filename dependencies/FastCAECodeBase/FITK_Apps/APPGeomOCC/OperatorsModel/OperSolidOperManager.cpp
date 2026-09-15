/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSolidOperManager.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"

// GUI
#include "GUIDialog/ChamferInfoDialog.h"
#include "GUIDialog/FilletInfoDialog.h"
#include "GUIDialog/DefeatureInfoDialog.h"

namespace ModelOper
{
    // 倒角。
    //@{
    void OperChamferManager::showDialog()
    {
        initDialog<GUI::ChamferInfoDialog>();
    }

    bool OperChamferManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        int inputCmdId = -1;
        QList<Interface::VirtualShape> inputEdgeTopos;
        double dist = 0.;
        bool flag = argValue("InputCmdId", inputCmdId);
        flag &= argValue("InputEdgeIndice", inputEdgeTopos);
        flag &= argValue("InputDistance", dist);
        if (!flag)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 保留历史参数。
        int lastInputCmdId = 0;
        QList<Interface::VirtualShape> lastEdgeTopos;
        double lastDist = 0.;

        // 创建或获取历史倒角数据。
        Interface::FITKAbsGeoModelChamferSolid* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelChamferSolid*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastInputCmdId = cmdHist->getInputCmdId();
            lastEdgeTopos = cmdHist->getEdgeTopos();
            lastDist = cmdHist->getDistance();
        }

        Interface::FITKAbsGeoModelChamferSolid* chamferCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelChamferSolid>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTChamferSolid) : cmdHist;

        // 设置参数生成数据。
        chamferCmd->setInputCmdId(inputCmdId);
        chamferCmd->setEdgeTopos(inputEdgeTopos);
        chamferCmd->setDistance(dist);
        flag = chamferCmd->update();
        if (!flag)
        {
            // 还原历史参数。
            if (cmdHist)
            {
                cmdHist->setInputCmdId(lastInputCmdId);
                cmdHist->setEdgeTopos(lastEdgeTopos);
                cmdHist->setDistance(lastDist);
            }
            // 析构创建的数据对象。
            else
            {
                delete chamferCmd;
                chamferCmd = nullptr;
            }

            return false;
        }

        // 移除命令引用并添加新命令引用。
        chamferCmd->clearReference();
        chamferCmd->addReferenceCmdID(inputCmdId);

        // 添加至数据仓库。
        if (!cmdHist)
        {
            chamferCmd->setDataObjectName(geoData->checkName(QString("Chamfer-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(chamferCmd);
        }

        // 暂存数据ID。
        setArgs("Id", chamferCmd->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
    //@}

    // 倒圆。
    //@{
    void OperFilletManager::showDialog()
    {
        initDialog<GUI::FilletInfoDialog>();
    }

    bool OperFilletManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        int inputCmdId = -1;
        QList<Interface::VirtualShape> inputEdgeTopos;
        double radius = 0.;
        bool flag = argValue("InputCmdId", inputCmdId);
        flag &= argValue("InputEdgeIndice", inputEdgeTopos);
        flag &= argValue("InputRadius", radius);
        if (!flag)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 保留历史参数。
        int lastInputCmdId = 0;
        QList<Interface::VirtualShape> lastEdgeTopos;
        double lastRadius = 0.;

        // 创建或获取历史倒角数据。
        Interface::FITKAbsGeoModelFilletSolid* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelFilletSolid*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastInputCmdId = cmdHist->getInputCmdId();
            lastEdgeTopos = cmdHist->getEdgeTopos();
            lastRadius = cmdHist->getRadius();
        }

        Interface::FITKAbsGeoModelFilletSolid* filletCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelFilletSolid>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTFilletSolid) : cmdHist;

        // 设置参数生成数据。
        filletCmd->setInputCmdId(inputCmdId);
        filletCmd->setEdgeTopos(inputEdgeTopos);
        filletCmd->setRadius(radius);
        flag = filletCmd->update();
        if (!flag)
        {
            // 还原历史参数。
            if (cmdHist)
            {
                cmdHist->setInputCmdId(lastInputCmdId);
                cmdHist->setEdgeTopos(lastEdgeTopos);
                cmdHist->setRadius(lastRadius);
            }
            // 析构创建的数据对象。
            else
            {
                delete filletCmd;
                filletCmd = nullptr;
            }

            return false;
        }

        // 移除命令引用并添加新命令引用。
        filletCmd->clearReference();
        filletCmd->addReferenceCmdID(inputCmdId);

        // 添加至数据仓库。
        if (!cmdHist)
        {
            filletCmd->setDataObjectName(geoData->checkName(QString("Fillet-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(filletCmd);
        }

        // 暂存数据ID。
        setArgs("Id", filletCmd->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
    //@}

    // 移除特征。
    //@{
    void OperDefeatureManager::showDialog()
    {
        initDialog<GUI::DefeatureInfoDialog>();
    }

    bool OperDefeatureManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        int inputCmdId = -1;
        QList<Interface::VirtualShape> inputFaceTopos;
        Interface::FITKGeoEnum::FITKGeometryComType geoCmdType;
        bool flag = argValue("InputCmdId", inputCmdId);
        flag &= argValue("InputFaceIndice", inputFaceTopos);
        flag &= argValue("InputGeoCmdType",geoCmdType);
        if (!flag)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 保留历史参数。
        int lastInputCmdId = 0;
        QList<Interface::VirtualShape> lastFaceTopos;

        // 创建或获取历史倒角数据。
        Interface::FITKAbsGeoModelDefeatureSolid* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelDefeatureSolid*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastInputCmdId = cmdHist->getInputCmdId();
            lastFaceTopos = cmdHist->getFaceTopos();
        }

        Interface::FITKAbsGeoModelDefeatureSolid* defCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelDefeatureSolid>(geoCmdType) : cmdHist;

        // 设置参数生成数据。
        defCmd->setInputCmdId(inputCmdId);
        defCmd->setFaceTopos(inputFaceTopos);
        flag = defCmd->update();
        if (!flag)
        {
            // 还原历史参数。
            if (cmdHist)
            {
                cmdHist->setInputCmdId(lastInputCmdId);
                cmdHist->setFaceTopos(lastFaceTopos);
            }
            // 析构创建的数据对象。
            else
            {
                delete defCmd;
                defCmd = nullptr;
            }

            return false;
        }

        // 移除命令引用并添加新命令引用。
        defCmd->clearReference();
        defCmd->addReferenceCmdID(inputCmdId);

        // 添加至数据仓库。
        if (!cmdHist)
        {
            defCmd->setDataObjectName(geoData->checkName(QString("Defeature-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(defCmd);
        }

        // 暂存数据ID。
        setArgs("Id", defCmd->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
    //@}

} // namespace ModelOper
