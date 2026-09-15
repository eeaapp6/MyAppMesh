/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoDelete.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoDelete::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTDelete;
    }

    bool FITKAbsGeoDelete::update()
    {
        // 返回是否设置移除状态成功。
        return redo();
    }

    bool FITKAbsGeoDelete::undo()
    {
        // 撤销将数据还原为常规模式。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_deletedCmdId);
        if (!cmd)
        {
            return false;
        }

        // 设置状态为常规并移除一次引用计数。
        cmd->setCommandStatus(FITKGeoEnum::FGSNormal);
        cmd->enableReferenceList(true);
        enableReferenceList(false);

        return true;//cmd->redo();
    }

    bool FITKAbsGeoDelete::redo()
    {
        // 重新将数据置为移除状态。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_deletedCmdId);
        if (!cmd)
        {
            return false;
        }

        // 设置状态为移除并添加一次引用计数。
        cmd->setCommandStatus(FITKGeoEnum::FGSDeleted);

        // 添加引用。
        addReferenceCmdID(m_deletedCmdId);
        cmd->enableReferenceList(false);
        enableReferenceList(true);

        return true;// cmd->undo();
    }

    void FITKAbsGeoDelete::setDeleteCommandID(int id)
    {
        // 保存被移除命令ID。
        m_deletedCmdId = id;
    }

    int FITKAbsGeoDelete::getDeleteCommandID()
    {
        return m_deletedCmdId;
    }
}