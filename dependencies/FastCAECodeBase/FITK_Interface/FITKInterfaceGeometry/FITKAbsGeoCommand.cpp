/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoShapeAgent.h"
#include "FITKGeoCommandProp.h"
// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceGeometry, FITKInterfaceGeometryVersion);


namespace Interface
{
    FITKAbsGeoCommand::FITKAbsGeoCommand()
    {
        // 创建引用命令管理器。
        m_referenceCmdList = new FITKGeoCommandManager;

        // 创建引用命令管理器。
        m_referencedCmdList = new FITKGeoCommandManager;

        //几何属性
        m_prop = new FITKGeoCommandProp;
    }

    FITKAbsGeoCommand::~FITKAbsGeoCommand()
    {
        // 删除几何属性
        if (m_prop != nullptr) delete m_prop;
        m_prop = nullptr;
        // 删除几何形状代理器
        if (_shapeAgent != nullptr) delete _shapeAgent;
        _shapeAgent = nullptr;

        // 析构引用数据管理器。（不析构管理数据对象）
        if (m_referenceCmdList)
        {
            // 遍历所有引用移除。
            int nRef = m_referenceCmdList->getDataCount();
            for (int i = nRef - 1; i >= 0; i--)
            {
                FITKAbsGeoCommand* cmd = m_referenceCmdList->getDataByIndex(i);
                if (!cmd)
                {
                    continue;
                }

                // 不减少引用对象引用计数并移出列表。
                m_referenceCmdList->removeDataObjWithoutRelease(cmd);
            }

            delete m_referenceCmdList;
            m_referenceCmdList = nullptr;
        }

        // 析构被引用数据管理器。（不析构管理数据对象）
        if (m_referencedCmdList)
        {
            // 遍历所有引用移除。
            int nRef = m_referencedCmdList->getDataCount();
            for (int i = nRef - 1; i >= 0; i--)
            {
                FITKAbsGeoCommand* cmd = m_referencedCmdList->getDataByIndex(i);
                if (!cmd)
                {
                    continue;
                }

                // 不减少引用对象引用计数并移出列表。
                m_referencedCmdList->removeDataObjWithoutRelease(cmd);
            }

            delete m_referencedCmdList;
            m_referencedCmdList = nullptr;
        }
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoCommand::getGeometryCommandType()
    {
        //抽象类，错误值
        return FITKGeoEnum::FGTNone;
    }

    void FITKAbsGeoCommand::setColor(const QColor & color)
    {
        m_color = color;
    }

    QColor FITKAbsGeoCommand::getColor() const
    {
        return m_color;
    }

    bool FITKAbsGeoCommand::update()
    {
        //错误值
        return false;
    }

    void FITKAbsGeoCommand::clearShape()
    {
        if (_shapeAgent)
        {
            _shapeAgent->clearShape();
        }
    }

    bool FITKAbsGeoCommand::hasShapeTopos()
    {
        if (_shapeAgent)
        {
            return _shapeAgent->hasTopos();
        }

        return false;
    }

    bool FITKAbsGeoCommand::undo()
    {
        // 执行撤销逻辑，将引用数据计数暂时屏蔽。
        if (!m_referenceCmdList)
        {
            return false;
        }

        // 遍历所有引用。
        enableReferenceList(false);

        // 撤销编辑。
        FITKAbsGeoCommand* cmdMod = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_modifiedCmdId);
        if (cmdMod)
        {
            cmdMod->setCommandStatus(FITKGeoEnum::FITKGeoStatus::FGSNormal);
        }

        return true;
    }

    bool FITKAbsGeoCommand::redo()
    {
        // 执行重做逻辑，将引用数据计数恢复。
        if (!m_referenceCmdList)
        {
            return false;
        }

        // 遍历所有引用。
        enableReferenceList(true);

        // 重做编辑。
        FITKAbsGeoCommand* cmdMod = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_modifiedCmdId);
        if (cmdMod)
        {
            cmdMod->setCommandStatus(FITKGeoEnum::FITKGeoStatus::FGSModified);
        }

        return true;
    }

    void FITKAbsGeoCommand::triangleShanpe()
    {
        if (_shapeAgent)
            _shapeAgent->triangulation();
    }

    void FITKAbsGeoCommand::setCommandStatus(FITKGeoEnum::FITKGeoStatus status)
    {
        // 获取命令状态。
        m_status = status;
    }

    FITKGeoEnum::FITKGeoStatus FITKAbsGeoCommand::getCommandStatus()
    {
        return m_status;
    }

    Interface::FITKAbsGeoShapeAgent* FITKAbsGeoCommand::getShapeAgent()
    {
        return _shapeAgent;
    }

    FITKShapeVirtualTopoManager* FITKAbsGeoCommand::getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType type)
    {
        if (!_shapeAgent)
        {
            return nullptr;
        }

        // 获取管理器。
        FITKVirtualTopoManager* vTopoMgr = _shapeAgent->getVirtualTopoManager();
        if (!vTopoMgr)
        {
            return nullptr;
        }

        // 获取指定形状虚拓扑管理器。
        return vTopoMgr->getShapeVirtualTopoManager(type);
    }

    Interface::FITKVirtualTopoManager* FITKAbsGeoCommand::getVirtualTopoManager()
    {
        if (!_shapeAgent)
        {
            return nullptr;
        }

        return _shapeAgent->getVirtualTopoManager();
    }

    FITKAbsVirtualTopo* FITKAbsGeoCommand::getVirtualTopoByIndex(FITKGeoEnum::VTopoShapeType type, int index)
    {
        FITKVirtualTopoManager* tMgr = getVirtualTopoManager();
        if (!tMgr)
        {
            return nullptr;
        }

        // 获取形状虚拓扑管理器。
        FITKShapeVirtualTopoManager* vsMgr = tMgr->getShapeVirtualTopoManager(type);
        if (!vsMgr)
        {
            return nullptr;
        }

        return vsMgr->getDataByIndex(index);
    }

    QList<FITKAbsVirtualTopo*> FITKAbsGeoCommand::getVirtualToposByIndice(FITKGeoEnum::VTopoShapeType type, QList<int> indice)
    {
        QList<FITKAbsVirtualTopo*> topos;
        FITKVirtualTopoManager* tMgr = getVirtualTopoManager();
        if (!tMgr)
        {
            return topos;
        }

        // 获取形状虚拓扑管理器。
        FITKShapeVirtualTopoManager* vsMgr = tMgr->getShapeVirtualTopoManager(type);
        if (!vsMgr)
        {
            return topos;
        }

        for (const int & index : indice)
        {
            FITKAbsVirtualTopo* topo = vsMgr->getDataByIndex(index);
            if (topo)
            {
                topos.push_back(topo);
            }
        }

        return topos;
    }

    void FITKAbsGeoCommand::enableReferenceList(bool flag)
    {
        if (flag && !m_enableRefer)
        {
            int nRef = m_referenceCmdList->getDataCount();
            for (int i = nRef - 1; i >= 0; i--)
            {
                FITKAbsGeoCommand* cmd = m_referenceCmdList->getDataByIndex(i);
                if (!cmd)
                {
                    continue;
                }

                // 增加引用对象引用计数。
                cmd->addReferencedCmdObj(this);
            }
        }
        else if (!flag && m_enableRefer)
        {
            int nRef = m_referenceCmdList->getDataCount();
            for (int i = nRef - 1; i >= 0; i--)
            {
                FITKAbsGeoCommand* cmd = m_referenceCmdList->getDataByIndex(i);
                if (!cmd)
                {
                    continue;
                }

                // 移除引用对象引用计数。
                cmd->removeReferencedCmdObj(this);
            }
        }

        // 保存状态。
        m_enableRefer = flag;
    }

    bool FITKAbsGeoCommand::getEnableReferenceList()
    {
        return m_enableRefer;
    }

    void FITKAbsGeoCommand::addReferenceCmdID(int id)
    {
        // 获取需要添加引用的命令数据对象。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
        addReferenceCmdObj(cmd);
    }

    void FITKAbsGeoCommand::addReferenceCmdObj(FITKAbsGeoCommand* cmd, bool isSet)
    {
        if (!cmd || !m_referenceCmdList)
        {
            return;
        }

        // 为目标对象添加引用关系。
        cmd->addReferencedCmdObj(this);

        // 判断数据是否已被引用。
        bool contains = m_referenceCmdList->isContains(cmd);
        if (contains)
        {
            return;
        }

        // 存储引用对象。
        m_referenceCmdList->appendDataObj(cmd);
    }

    void FITKAbsGeoCommand::removeReferenceCmdID(int id)
    {
        // 获取需要移除引用的命令数据对象。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
        removeReferenceCmdObj(cmd);
    }

    void FITKAbsGeoCommand::removeReferenceCmdObj(FITKAbsGeoCommand* cmd)
    {
        if (!cmd || !m_referenceCmdList)
        {
            return;
        }

        // 判断数据是否被引用。
        bool contains = m_referenceCmdList->isContains(cmd);
        if (!contains)
        {
            return;
        }

        // 从目标对象移除引用关系。
        cmd->removeReferencedCmdObj(this);

        // 移除引用对象。
        m_referenceCmdList->removeDataObjWithoutRelease(cmd);
    }

    void FITKAbsGeoCommand::clearReference()
    {
        if (!m_referenceCmdList)
        {
            return;
        }

        // 遍历所有引用移除。
        int nRef = m_referenceCmdList->getDataCount();
        for (int i = nRef - 1; i >= 0; i--)
        {
            FITKAbsGeoCommand* cmd = m_referenceCmdList->getDataByIndex(i);
            if (!cmd)
            {
                continue;
            }

            // 减少引用对象引用计数并移出列表。
            m_referenceCmdList->removeDataObjWithoutRelease(cmd);
            cmd->removeReferencedCmdObj(this);
        }
    }

    int FITKAbsGeoCommand::getReferenceCmdCount()
    {
        if (!m_referenceCmdList)
        {
            return 0;
        }

        // 获取引用命令列表长度。
        return m_referenceCmdList->getDataCount();
    }

    FITKAbsGeoCommand* FITKAbsGeoCommand::getReferenceCmdByIndex(int index)
    {
        if (!m_referenceCmdList)
        {
            return nullptr;
        }

        // 返回第i条命令。
        return m_referenceCmdList->getDataByIndex(index);
    }

    int FITKAbsGeoCommand::getReferencedCmdCount()
    {
        if (!m_referencedCmdList)
        {
            return 0;
        }

        // 获取被引用命令列表长度。
        return m_referencedCmdList->getDataCount();
    }

    FITKAbsGeoCommand* FITKAbsGeoCommand::getReferencedCmdByIndex(int index)
    {
        if (!m_referencedCmdList)
        {
            return nullptr;
        }

        // 返回第i条命令。
        return m_referencedCmdList->getDataByIndex(index);
    }

    void FITKAbsGeoCommand::addReferencedCmdID(int id)
    {
        // 获取需要添加引用的命令数据对象。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
        addReferencedCmdObj(cmd);
    }

    void FITKAbsGeoCommand::addReferencedCmdObj(FITKAbsGeoCommand* cmd)
    {
        if (!cmd || !m_referencedCmdList)
        {
            return;
        }

        // 判断数据是否已被引用。
        bool contains = m_referencedCmdList->isContains(cmd);
        if (contains)
        {
            return;
        }

        // 存储引用对象。
        m_referencedCmdList->appendDataObj(cmd);
    }

    void FITKAbsGeoCommand::removeReferencedCmdID(int id)
    {
        // 获取需要移除的被引用命令数据对象。
        FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
        removeReferencedCmdObj(cmd);
    }

    void FITKAbsGeoCommand::removeReferencedCmdObj(FITKAbsGeoCommand* cmd)
    {
        if (!cmd || !m_referencedCmdList)
        {
            return;
        }

        // 判断数据是否被引用。
        bool contains = m_referencedCmdList->isContains(cmd);
        if (!contains)
        {
            return;
        }

        // 移除引用对象。
        m_referencedCmdList->removeDataObjWithoutRelease(cmd);
    }

    bool FITKAbsGeoCommand::getDataValidInGUI()
    {
        // 被移除、被修改或者删除命令无需显示。
        if (this->getCommandStatus() == Interface::FITKGeoEnum::FITKGeoStatus::FGSDeleted ||
            this->getCommandStatus() == Interface::FITKGeoEnum::FITKGeoStatus::FGSModified ||
            this->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTDelete ||
            this->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTMultiDelete)
        {
            return false;
        }

        return true;
    }

    void FITKAbsGeoCommand::setModifiedCmdID(int id)
    {
        // 检查数据ID并保存。
        FITKAbsGeoCommand* cmd{ nullptr };
        if (m_cmdMgr)
        {
            cmd = m_cmdMgr->getDataByID(id);
        }
        else
        {
            cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(id);
        }

        if (cmd)
        {
            m_modifiedCmdId = id;
        }
    }

    void FITKAbsGeoCommand::setModifiedCmdObj(FITKAbsGeoCommand* cmd)
    {
        if (!cmd)
        {
            return;
        }

        m_modifiedCmdId = cmd->getDataObjectID();
    }

    int FITKAbsGeoCommand::getModifiedCmdID()
    {
        FITKAbsGeoCommand* cmd = getModifiedCmdObj();
        if (!cmd)
        {
            m_modifiedCmdId = -1;
        }

        return m_modifiedCmdId;
    }

    FITKAbsGeoCommand* FITKAbsGeoCommand::getModifiedCmdObj()
    {
        FITKAbsGeoCommand* cmd{ nullptr };
        if (m_cmdMgr)
        {
            cmd = m_cmdMgr->getDataByID(m_modifiedCmdId);
        }
        else
        {
            cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_modifiedCmdId);
        }

        return cmd;
    }

    bool FITKAbsGeoCommand::hasModifiedCommand()
    {
        // 被编辑命令不为空则为编辑命令。
        FITKAbsGeoCommand* cmd{ nullptr };
        if (m_cmdMgr)
        {
            cmd = m_cmdMgr->getDataByID(m_modifiedCmdId);
        }
        else
        {
            cmd = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_modifiedCmdId);
        }

        return cmd != nullptr;
    }

    QList<FITKAbsGeoCommand*> FITKAbsGeoCommand::getDownStreamCommands()
    {
        // 递归向上查询。
        QList<FITKAbsGeoCommand*> cmdList;
        getRefCmdRecursively(this, cmdList, true);
        return cmdList;
    }

    QList<FITKAbsGeoCommand*> FITKAbsGeoCommand::getUpStreamCommands()
    {
        // 递归向下查询。
        QList<FITKAbsGeoCommand*> cmdList;
        getRefCmdRecursively(this, cmdList, false);
        return cmdList;
    }

    QList<int> FITKAbsGeoCommand::getCreatedCommandIds()
    {
        // 默认仅返回自身数据ID。
        QList<int> ids;
        ids.push_back(this->getDataObjectID());
        return ids;
    }

    void FITKAbsGeoCommand::removeFromPart(FITKAbsGeoCommand* part)
    {
        Q_UNUSED(part);
        this->setGeoCommandManager(nullptr);
    }

    FITKGeoCommandManager* FITKAbsGeoCommand::getGeoCommandManager()
    {
        return m_cmdMgr;
    }

    FITKAbsGeoCommand* FITKAbsGeoCommand::getSrcCommand()
    {
        //// 检查该命令最终是否被移除。
        //FITKAbsGeoCommand* finalCmd = getFinalCommand();
        //if (!finalCmd)
        //{
        //    return nullptr;
        //}

        // 递归向上获取编辑的命令。
        FITKAbsGeoCommand* srcCmd{ nullptr };
        FITKAbsGeoCommand* tempCmd{ nullptr };
        while (tempCmd)
        {
            tempCmd = srcCmd->getModifiedCmdObj();
            if (tempCmd)
            {
                srcCmd = tempCmd;
            }
        }

        return srcCmd;
    }

    FITKAbsGeoCommand* FITKAbsGeoCommand::getFinalCommand()
    {
        if (m_status == FITKGeoEnum::FITKGeoStatus::FGSDeleted)
        {
            return nullptr;
        }

        FITKAbsGeoCommand* finalCmd = this;
        if (m_cmdMgr)
        {
            int nCmds = m_cmdMgr->getDataCount();
            for (int i = 0; i < nCmds; i++)
            {
                FITKAbsGeoCommand* cmd = m_cmdMgr->getDataByIndex(i);
                if (!cmd)
                {
                    continue;
                }

                if (finalCmd->getDataObjectID() == cmd->getModifiedCmdID())
                {
                    finalCmd = cmd;
                }
            }
        }

        return finalCmd;
    }

    void FITKAbsGeoCommand::getRefCmdRecursively(FITKAbsGeoCommand* cmd, QList<FITKAbsGeoCommand*> & cmdList, bool downStream)
    {
        if (!cmd)
        {
            return;
        }

        cmdList.push_back(cmd);

        // 获取引用对象数据。
        if (downStream)
        {
            int nRefed = cmd->getReferencedCmdCount();
            for (int i = 0; i < nRefed; i++)
            {
                Interface::FITKAbsGeoCommand* subCmd = cmd->getReferencedCmdByIndex(i);
                if (!subCmd)
                {
                    continue;
                }

                // 递归查询。
                getRefCmdRecursively(subCmd, cmdList, downStream);
            }
        }
        else
        {
            int nRef = cmd->getReferenceCmdCount();
            for (int i = 0; i < nRef; i++)
            {
                Interface::FITKAbsGeoCommand* subCmd = cmd->getReferenceCmdByIndex(i);
                if (!subCmd)
                {
                    continue;
                }

                // 递归查询。
                getRefCmdRecursively(subCmd, cmdList, downStream);
            }
        }
    }

    void FITKAbsGeoCommand::setGeoCommandManager(FITKGeoCommandManager* mgr)
    {
        m_cmdMgr = mgr;
    }

    bool FITKGeoCommandManager::canUndo()
    {
        return false;
    }

    bool FITKGeoCommandManager::canRedo()
    {
        return false;
    }

    bool FITKGeoCommandManager::undo()
    {
        return false;
    }

    bool FITKGeoCommandManager::redo()
    {
        return false;
    }

    QList<FITKAbsGeoCommand*> FITKGeoCommandManager::getRootCommands()
    {
        QList<FITKAbsGeoCommand*> cmdList;
        for (int i = 0; i < this->getDataCount(); i++)
        {
            FITKAbsGeoCommand* subCmd = this->getDataByIndex(i);
            if (subCmd->getReferencedCmdCount() != 0)
            {
                continue;
            }

            FITKAbsGeoShapeAgent* shapeAgent = subCmd->getShapeAgent();
            if (!shapeAgent)
            {
                continue;
            }

            if (shapeAgent->isValid())
            {
                cmdList.push_back(subCmd);
            }
        }

        return cmdList;
    }

    QList<FITKAbsGeoCommand*> FITKGeoCommandManager::getCommandsByType(FITKGeoEnum::FITKGeometryComType type)
    {
        QList<FITKAbsGeoCommand*> cmdList;
        for (int i = 0; i < this->getDataCount(); i++)
        {
            FITKAbsGeoCommand* subCmd = this->getDataByIndex(i);
            if (!subCmd)
            {
                continue;
            }

            if (subCmd->getGeometryCommandType() == type)
            {
                cmdList.push_back(subCmd);
            }
        }

        return cmdList;
    }

    bool FITKGeoCommandManager::addAndUpdate(Interface::FITKAbsGeoCommand* subCmd, bool deleteFailed)
    {
        if (!subCmd)
        {
            return false;
        }

        // 先添加至子命令列表。
        this->appendDataObj(subCmd);

        // 尝试刷新数据。
        bool flag = subCmd->update();
        if (!flag)
        {
            // 刷新失败则将命令从列表中移除。
            if (deleteFailed)
            {
                this->removeDataObj(subCmd);
            }
            else
            {  
                this->removeDataObjWithoutRelease(subCmd);
            }
        }

        return flag;
    }

    void FITKGeoCommandManager::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(item);
        if (cmd)
        {
            cmd->setGeoCommandManager(this);
        }  

        _helper->appendData(item, false);
    }

    void FITKGeoCommandManager::appendDataObj(const QList<Core::FITKAbstractDataObject*> & items)
    {
        QMutexLocker locker(&_mutex);

        for (Core::FITKAbstractDataObject* item : items)
        {
            Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(item);
            if (cmd)
            {
                cmd->setGeoCommandManager(this);
            }
        }

        _helper->appendData(items);
    }

    void FITKGeoCommandManager::insertDataObj(int index, Core::FITKAbstractDataObject* item)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(item);
        if (cmd)
        {
            cmd->setGeoCommandManager(this);
        }

        _helper->insertDataObj(index, item);
    }

    void FITKGeoCommandManager::removeDataObj(Core::FITKAbstractDataObject* obj)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(obj);
        if (cmd)
        {
            cmd->setGeoCommandManager(nullptr);
        }

        _helper->removeDataObj(obj);
    }

    void FITKGeoCommandManager::removeDataByIndex(int index)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_helper->getDataByIndex(index));
        if (cmd)
        {
            cmd->setGeoCommandManager(nullptr);
        }

        _helper->removeDataByIndex(index);
    }

    void FITKGeoCommandManager::removeDataByID(int id)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_helper->getDataByID(id));
        if (cmd)
        {
            cmd->setGeoCommandManager(nullptr);
        }

        _helper->removeDataByID(id);
    }

    void FITKGeoCommandManager::removeDataByName(const QString & name, bool compSens)
    {
        QMutexLocker locker(&_mutex);

        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_helper->getDataByName(name, compSens));
        if (cmd)
        {
            cmd->setGeoCommandManager(nullptr);
        }

        _helper->removeDataObj(cmd);
    }

    int FITKGeoCommandManager::reGenerate(int fromDataId)
    {
        return -1;
    }

    void FITKGeoCommandManager::saveManager(FITKAbsGeoCommand* cmd)
    {
        if (cmd)
        {
            cmd->setGeoCommandManager(this);
        }
    }
}
