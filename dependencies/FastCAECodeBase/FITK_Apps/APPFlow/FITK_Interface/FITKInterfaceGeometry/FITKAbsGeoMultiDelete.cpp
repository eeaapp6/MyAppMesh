/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoMultiDelete.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoMultiDelete::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiDelete;
    }

    bool FITKAbsGeoMultiDelete::update()
    {
        // 返回是否设置移除状态成功。
        return redo();
    }

    bool FITKAbsGeoMultiDelete::undo()
    {
        for (const int & id : m_deletedCmdIds)
        {
            // 撤销将数据还原为常规模式。
            FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
            if (!cmd)
            {
                return false;
            }

            // 设置状态为常规并移除一次引用计数。
            cmd->setCommandStatus(FITKGeoEnum::FGSNormal);
            cmd->enableReferenceList(true);
        }

        enableReferenceList(false);

        return true;//cmd->redo();
    }

    bool FITKAbsGeoMultiDelete::redo()
    {
        for (const int & id : m_deletedCmdIds)
        {
            // 重新将数据置为移除状态。
            FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
            if (!cmd)
            {
                return false;
            }

            // 设置状态为移除并添加一次引用计数。
            cmd->setCommandStatus(FITKGeoEnum::FGSDeleted);

            // 添加引用。
            addReferenceCmdID(id);
            cmd->enableReferenceList(false);   
        }

        enableReferenceList(true);

        return true;// cmd->undo();
    }

    void FITKAbsGeoMultiDelete::setDeleteCommandIDs(QList<int> ids)
    {
        // 保存被移除命令ID。
        m_deletedCmdIds = ids;
    }

    QList<int> FITKAbsGeoMultiDelete::getDeleteCommandIDs()
    {
        return m_deletedCmdIds;
    }
}