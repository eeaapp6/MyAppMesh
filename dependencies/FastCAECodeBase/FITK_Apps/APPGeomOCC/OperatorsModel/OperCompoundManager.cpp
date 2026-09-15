/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperCompoundManager.h"

//Geomerty
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperCompound.h"

//GUI
#include "GUIDialog/AppendCompoundDialog.h"
#include "GUIDialog/RemoveCompoundDialog.h"


namespace ModelOper
{
    /** 复合体添加对象 */
    /*@{*/
    void OperAppendCompoundManager::showDialog()
    {
        initDialog<GUI::AppendCompoundDialog>();
    }

    bool OperAppendCompoundManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        Interface::VirtualShape compoundVShape;
        QList<Interface::VirtualShape> vShapes;
        // 保留历史参数。
        Interface::VirtualShape lastCompoundVShape;
        QList<Interface::VirtualShape> lastVShapes;

        bool flag = true;
        flag &= argValue("CompoundVShape", compoundVShape);
        flag &= argValue("VShapes", vShapes);
        if (!flag) return false;

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelOperCompound* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperCompound*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastCompoundVShape = cmdHist->getInputCmdId();
            vShapes = cmdHist->getShapeCmdIds();
        }
        Interface::FITKAbsGeoModelOperCompound* operCompound = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperCompound>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTCompoundAppendShape) : cmdHist;

        if (operCompound == nullptr) return false;
        operCompound->setInputCmdId(compoundVShape);
        operCompound->setShapeCmdIds(vShapes);
        // 设置参数生成数据。
        if (!operCompound->update())
        {
            if (!cmdHist)
            {
                delete operCompound;
                operCompound = nullptr;
            }
            else
            {
                operCompound->setInputCmdId(lastCompoundVShape);
                operCompound->setShapeCmdIds(lastVShapes);
            }
            return false;
        }

        // 移除命令引用并添加新命令引用。
        operCompound->clearReference();
        operCompound->addReferenceCmdID(compoundVShape.CmdId);
        for (int i = 0; i < vShapes.size(); i++)
        {
            operCompound->addReferenceCmdID(vShapes[i].CmdId);
        }

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operCompound->setDataObjectName(geoData->checkName(QString("AppendCompound-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operCompound);
        }

        // 暂存数据ID。
        setArgs("Id", operCompound->getDataObjectID());
        // 结束操作。
        finalize();

        return true;
    }
    /*@*/

    /** 复合体移除对象 */
    /*@{*/
    void OperRemoveCompoundManager::showDialog()
    {
        initDialog<GUI::RemoveCompoundDialog>();
    }

    bool OperRemoveCompoundManager::execProfession()
    {
        // 获取历史数据ID。
        int histId = -1;
        argValue("Id", histId);

        // 获取输入参数。
        Interface::VirtualShape compoundVShape;
        QList<Interface::VirtualShape> vShapes;
        // 保留历史参数。
        Interface::VirtualShape lastCompoundVShape;
        QList<Interface::VirtualShape> lastVShapes;

        bool flag = true;
        flag &= argValue("CompoundVShape", compoundVShape);
        flag &= argValue("VShapes", vShapes);
        if (!flag) return false;

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelOperCompound* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelOperCompound*>(geoData->getDataByID(histId));
        if (cmdHist)
        {
            lastCompoundVShape = cmdHist->getInputCmdId();
            vShapes = cmdHist->getShapeCmdIds();
        }
        Interface::FITKAbsGeoModelOperCompound* operCompound = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperCompound>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTCompoundRemoveShape) : cmdHist;

        if (operCompound == nullptr) return false;
        operCompound->setInputCmdId(compoundVShape);
        operCompound->setShapeCmdIds(vShapes);

        // 设置参数生成数据。
        if (!operCompound->update())
        {
            if (!cmdHist)
            {
                delete operCompound;
                operCompound = nullptr;
            }
            else
            {
                operCompound->setInputCmdId(lastCompoundVShape);
                operCompound->setShapeCmdIds(lastVShapes);
            }
            return false;
        }

        // 移除命令引用并添加新命令引用。
        operCompound->clearReference();
        operCompound->addReferenceCmdID(compoundVShape.CmdId);
        for (int i = 0; i < vShapes.size(); i++)
        {
            operCompound->addReferenceCmdID(vShapes[i].CmdId);
        }
        // 添加至数据仓库。
        if (!cmdHist)
        {
            operCompound->setDataObjectName(geoData->checkName(QString("RemoveCompound-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operCompound);
        }

        // 暂存数据ID。
        setArgs("Id", operCompound->getDataObjectID());

        // 结束操作。
        finalize();

        return true;
    }
    /*@*/

} // namespace ModelOper
