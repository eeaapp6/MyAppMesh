/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoPart.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    FITKAbsGeoPart::FITKAbsGeoPart()
    {
        // 创建重做列表。
        m_redoList = new FITKGeoCommandManager;
    }

    FITKAbsGeoPart::~FITKAbsGeoPart()
    {
        // 删除重做列表。
        if (m_redoList)
        {
            delete m_redoList;
            m_redoList = nullptr;
        }
    }

    int FITKAbsGeoPart::reGenerate(int fromDataId)
    {
        // 获取更新数据的源命令索引。
        int index = 0;
        if (fromDataId != -1)
        {
            FITKAbsGeoCommand* cmd = this->getDataByID(fromDataId);
            if (cmd)
            {
                FITKAbsGeoCommand* srcCmd = cmd->getSrcCommand();
                index = this->getDataIndex(srcCmd);
                if (index == -1)
                {
                    index = 0;
                }
            }
        }

        QList<FITKAbsGeoCommand*> subCmds;
        int nCmds = this->getDataCount();
        for (int i = index; i < nCmds; i++)
        {
            FITKAbsGeoCommand* subCmd = this->getDataByIndex(index);
            this->removeDataObjWithoutRelease(subCmd);
            subCmds.push_back(subCmd);
        }

        backtrackingShape();

        for (FITKAbsGeoCommand* subCmd : subCmds)
        {
            if (!subCmd)
            {
                continue;
            }

            subCmd->clearShape();

            // 不更新被移除或被修改命令以及移除类型命令。
            if (subCmd->getCommandStatus() == FITKGeoEnum::FITKGeoStatus::FGSModified ||
                subCmd->getCommandStatus() == FITKGeoEnum::FITKGeoStatus::FGSDeleted ||
                subCmd->getGeometryCommandType() == FITKGeoEnum::FITKGeometryComType::FGTDelete ||
                subCmd->getGeometryCommandType() == FITKGeoEnum::FITKGeometryComType::FGTMultiDelete)
            {
                this->appendDataObj(subCmd);
            }
            else
            {
                bool flag = this->addAndUpdate(subCmd, false);
                if (!flag)
                {
                    return subCmd->getDataObjectID();
                }
            }
        }

        return -1;
    }

    bool FITKAbsGeoPart::canUndo()
    {
        if (!m_redoList)
        {
            return false;
        }

        bool flag = this->getDataCount() != 0;

        return flag;
    }

    bool FITKAbsGeoPart::canRedo()
    {
        if (!m_redoList)
        {
            return false;
        }

        bool flag = m_redoList->getDataCount() != 0;

        return flag;
    }

    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPart;
    }

    bool FITKAbsGeoPart::undo()
    {
        if (!m_redoList)
        {
            return false;
        }

        int nCmds = this->getDataCount();
        if (nCmds == 0)
        {
            return false;
        }

        int iCmd = nCmds - 1;
        FITKAbsGeoCommand* cmd = this->getDataByIndex(iCmd);
        if (!cmd)
        {
            return false;
        }

        // 移动至重做命令列表。
        this->removeDataObjWithoutRelease(cmd);
        m_redoList->appendDataObj(cmd);

        // 更新模型。
        this->backtrackingShape();

        return true;
    }

    bool FITKAbsGeoPart::redo()
    {
        if (!m_redoList)
        {
            return false;
        }

        int nRedoCmds = m_redoList->getDataCount();
        if (nRedoCmds == 0)
        {
            return false;
        }

        int iCmd = nRedoCmds - 1;
        FITKAbsGeoCommand* cmd = m_redoList->getDataByIndex(iCmd);
        if (!cmd)
        {
            return false;
        }

        // 移动回部件命令列表。
        m_redoList->removeDataObjWithoutRelease(cmd);

        // 先添加至子命令列表。
        FITKGeoCommandManager::appendDataObj(cmd);

        // 尝试刷新数据。
        bool flag = cmd->update();
        if (!flag)
        {
            this->removeDataObj(cmd);
        }

        if (!flag && m_redoList)
        {
            m_redoList->clear();
        }

        return flag;
    }

    bool FITKAbsGeoPart::backtrackingShape()
    {
        return false;
    }

    void FITKAbsGeoPart::setAutoBool(bool flag)
    {
        m_autoBool = flag;
    }

    bool FITKAbsGeoPart::getAutoBool()
    {
        return m_autoBool;
    }

    void FITKAbsGeoPart::setAutoMergeSolid(bool flag)
    {
        m_autoMergeSolid = flag;
    }

    bool FITKAbsGeoPart::getAutoMergeSolid()
    {
        return m_autoMergeSolid;
    }

    bool FITKAbsGeoPart::update()
    {
        return true;
    }

    void FITKAbsGeoPart::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        // 清空重做列表。
        if (m_redoList)
        {
            m_redoList->clear();
        }

        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(item);
        if (!cmd)
        {
            return;
        }

        // 保存管理器。
        saveManager(cmd);

        _helper->appendData(item);
    }

    void FITKAbsGeoPart::appendDataObj(const QList<Core::FITKAbstractDataObject*> & items)
    {
        // 清空重做列表。
        if (m_redoList)
        {
            m_redoList->clear();
        }

        QMutexLocker locker(&_mutex);

        // 保存管理器。
        for (Core::FITKAbstractDataObject* item : items)
        {
            Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(item);
            if (cmd)
            {
                saveManager(cmd);
            }
        }

        _helper->appendData(items);
    }

    void FITKAbsGeoPart::removeDataObj(Core::FITKAbstractDataObject* obj)
    {
        QMutexLocker locker(&_mutex);
        FITKAbsGeoCommand* subCmd = dynamic_cast<FITKAbsGeoCommand*>(obj);
        if (!subCmd)
        {
            return;
        }

        subCmd->removeFromPart(this);
        _helper->removeDataObj(subCmd);
    }

    void FITKAbsGeoPart::removeDataObjWithoutRelease(Core::FITKAbstractDataObject* obj)
    {
        QMutexLocker locker(&_mutex);
        FITKAbsGeoCommand* subCmd = dynamic_cast<FITKAbsGeoCommand*>(obj);
        if (!subCmd)
        {
            return;
        }

        subCmd->removeFromPart(this);
        _helper->removeDataObjWithoutRelease(obj);
    }

    void FITKAbsGeoPart::removeDataByIndex(int index)
    {
        QMutexLocker locker(&_mutex);
        FITKAbsGeoCommand* subCmd = dynamic_cast<FITKAbsGeoCommand*>(_helper->getDataByIndex(index));
        if (!subCmd)
        {
            return;
        }

        subCmd->removeFromPart(this);
        _helper->removeDataObj(subCmd);
    }

    void FITKAbsGeoPart::removeDataByID(int id)
    {
        QMutexLocker locker(&_mutex);
        FITKAbsGeoCommand* subCmd = dynamic_cast<FITKAbsGeoCommand*>(_helper->getDataByID(id));
        if (!subCmd)
        {
            return;
        }

        subCmd->removeFromPart(this);
        _helper->removeDataObj(subCmd);
    }

    void FITKAbsGeoPart::removeDataByName(const QString & name, bool compSens)
    {
        QMutexLocker locker(&_mutex);
        FITKAbsGeoCommand* subCmd = dynamic_cast<FITKAbsGeoCommand*>(_helper->getDataByName(name, compSens));
        if (!subCmd)
        {
            return;
        }

        subCmd->removeFromPart(this);
        _helper->removeDataObj(subCmd);
    }
}