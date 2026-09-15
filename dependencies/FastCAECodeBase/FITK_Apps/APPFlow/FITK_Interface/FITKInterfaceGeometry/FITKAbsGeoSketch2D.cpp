/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoSketch2D.h" 
#include <cmath>

namespace Interface
{

    FITKAbsGeoSketch2D::FITKAbsGeoSketch2D()
    {
        // 初始化重做列表。
        m_redoList = new FITKGeoCommandManager;

        // 初始化构造线管理器。
        m_constructionMgr = new FITKConstructionManager;
    }

    FITKAbsGeoSketch2D::~FITKAbsGeoSketch2D()
    {
        // 析构重做列表。
        if (m_redoList)
        {
            delete m_redoList;
            m_redoList = nullptr;
        }

        // 析构构造线管理器。
        if (m_constructionMgr)
        {
            delete m_constructionMgr;
            m_constructionMgr = nullptr;
        }
    }

    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoSketch2D::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSketch2D;
    }

    // 更新子数据。
    bool FITKAbsGeoSketch2D::update()
    {
        bool flag = true;
        for (int i = 0; i < getDataCount(); i++)
        {
            // 获取子数据并尝试进行更新。
            Interface::FITKAbsGeoCommand* subCmd = getDataByIndex(i);
            if (subCmd)
            {
                flag &= subCmd->update();
            }
            else
            {
                flag = false;
            }
        }

        // 返回是否存在子数据更新失败。
        return flag;
    }

    void FITKAbsGeoSketch2D::Normalize(double* db3)
    {
        double dbN3[3]{ 0., 0., 0. };
        double norm = sqrt(db3[0] * db3[0] + db3[1] * db3[1] + db3[2] * db3[2]);

        for (int i = 0; i < 3; i++)
        {
            db3[i] /= norm;
        }
    }

    int FITKAbsGeoSketch2D::reGenerate(int fromDataId)
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

        for (FITKAbsGeoCommand* subCmd : subCmds)
        {
            if (!subCmd)
            {
                continue;
            }

            subCmd->clearShape();
            bool flag = this->addAndUpdate(subCmd, false);
            if (!flag)
            {
                return subCmd->getDataObjectID();
            }
        }

        return -1;
    }

    void FITKAbsGeoSketch2D::setPlane(double* pos, double* normal, double* up)
    {
        // 设置全部信息。
        if (!pos || !normal || !up)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            m_pos[i] = pos[i];
            m_nor[i] = normal[i];
            m_up[i] = up[i];
        }

        // 归一化。
        Normalize(m_nor);
        Normalize(m_up);
    }

    void FITKAbsGeoSketch2D::getPlane(double* pos, double* normal, double* up)
    {
        // 获取全部信息。
        if (!pos || !normal || !up)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            pos[i] = m_pos[i];
            normal[i] = m_nor[i];
            up[i] = m_up[i];
        }
    }

    void FITKAbsGeoSketch2D::setPosition(double* pos)
    {
        // 保存位置。
        if (pos)
        {
            m_pos[0] = pos[0];
            m_pos[1] = pos[1];
            m_pos[2] = pos[2];
        }
    }

    void FITKAbsGeoSketch2D::getPosition(double* pos)
    {
        if (pos)
        {
            pos[0] = m_pos[0];
            pos[1] = m_pos[1];
            pos[2] = m_pos[2];
        }
    }

    void FITKAbsGeoSketch2D::setNormal(double* nor)
    {
        // 保存方向。
        if (nor)
        {
            m_nor[0] = nor[0];
            m_nor[1] = nor[1];
            m_nor[2] = nor[2];
        }

        // 归一化。
        Normalize(m_nor);
    }

    void FITKAbsGeoSketch2D::getNormal(double* nor)
    {
        if (nor)
        {
            nor[0] = m_nor[0];
            nor[1] = m_nor[1];
            nor[2] = m_nor[2];
        }
    }

    void FITKAbsGeoSketch2D::setUp(double* up)
    {
        // 保存方向。
        if (up)
        {
            m_up[0] = up[0];
            m_up[1] = up[1];
            m_up[2] = up[2];
        }

        // 归一化。
        Normalize(m_up);
    }

    void FITKAbsGeoSketch2D::getUp(double* up)
    {
        if (up)
        {
            up[0] = m_up[0];
            up[1] = m_up[1];
            up[2] = m_up[2];
        }
    }

    void FITKAbsGeoSketch2D::setDefaultSize(double size)
    {
        m_defaultSize = size;
    }

    double FITKAbsGeoSketch2D::getDefaultSize()
    {
        return m_defaultSize;
    }

    bool FITKAbsGeoSketch2D::undo()
    {
        int nSubCnmd = this->getDataCount();
        if (nSubCnmd == 0)
        {
            return false;
        }

        // 移动命令。
        FITKAbsGeoCommand* subCmd = this->getDataByIndex(nSubCnmd - 1);
        if (!subCmd)
        {
            return false;
        }

        this->removeDataObjWithoutRelease(subCmd);
        m_redoList->appendDataObj(subCmd);
        this->update();
        return true;
    }

    bool FITKAbsGeoSketch2D::redo()
    {
        if (!m_redoList)
        {
            return false;
        }

        int nSubCnmd = m_redoList->getDataCount();
        if (nSubCnmd == 0)
        {
            return false;
        }

        // 移动命令。
        FITKAbsGeoCommand* subCmd = m_redoList->getDataByIndex(nSubCnmd - 1);
        if (!subCmd)
        {
            return false;
        }

        m_redoList->removeDataObjWithoutRelease(subCmd);

        // 需要跳过清空Redo列表逻辑进行添加。
        FITKGeoCommandManager::appendDataObj(subCmd);

        this->update();

        return true;
    }

    void FITKAbsGeoSketch2D::appendDataObj(Core::FITKAbstractDataObject* obj)
    {
        // 清空重做列表。
        if (m_redoList)
        {
            m_redoList->clear();
        }

        // 执行插入数据。
        FITKGeoCommandManager::appendDataObj(obj);
    }

    bool FITKAbsGeoSketch2D::canUndo()
    {
        FITKGeoCommandManager* list = this->getUndoList();
        if (!list)
        {
            return false;
        }

        return list->getDataCount() != 0;
    }

    bool FITKAbsGeoSketch2D::canRedo()
    {
        FITKGeoCommandManager* list = this->getRedoList();
        if (!list)
        {
            return false;
        }

        return list->getDataCount() != 0;
    }

    FITKGeoCommandManager* FITKAbsGeoSketch2D::getUndoList()
    {
        // 当前命令列表即为撤销列表。
        return this;
    }

    FITKGeoCommandManager* FITKAbsGeoSketch2D::getRedoList()
    {
        return m_redoList;
    }

    FITKConstructionManager* FITKAbsGeoSketch2D::getConstructionManager()
    {
        return m_constructionMgr;
    }

    FITKAbsConstruction* FITKAbsGeoSketch2D::getCurrentConstruction()
    {
        if (!m_constructionMgr)
        {
            return nullptr;
        }

        return m_constructionMgr->getLastConstruction();
    }

    bool FITKAbsGeoSketch2D::isEmpty(bool construction)
    {
        bool ret = getDataCount() == 0;
        if (ret && construction)
        {
            if (m_constructionMgr)
            {
                ret = m_constructionMgr->getDataCount() == 0;
            }
        }

        return ret;
    }

    bool FITKAbsGeoSketch2D::isClosed() {
        return false;
    }

    bool FITKAbsGeoSketch2D::isSelfIntersection()
    {
        return false;
    }

    bool FITKAbsGeoSketch2D::isConnected()
    {
        // 无法判断默认为真。
        return true;
    }

    void FITKAbsGeoSketch2D::checkState(bool& isClosed, bool& isSelfIntersection) {
        isClosed = false;
        isSelfIntersection = false;
    }

    bool FITKAbsGeoSketch2D::getFirstPointAndDir(double* point, double* dir) { return false; }

    bool FITKAbsGeoSketch2D::getLastPointAndDir(double* point, double* dir) { return false; }

    //bool FITKAbsGeoSketch2D::hasConstructionLine() {
    //    for (auto it = m_ConstrctionList.crbegin(); it != m_ConstrctionList.crend(); ++it) {
    //        if ((*it).type == 1) { return true; }
    //    }
    //    return false;
    //}

    //bool FITKAbsGeoSketch2D::getLastConstructionLine(FITKAbsGeoSketch2DConstruction& line) {
    //    for (auto it = m_ConstrctionList.crbegin(); it != m_ConstrctionList.crend(); ++it) {
    //        if ((*it).type == 1) { line = (*it); return true; }
    //    }
    //    return false;
    //}

    //void FITKAbsGeoSketch2D::addConstruction(FITKAbsGeoSketch2DConstruction construct)
    //{
    //    m_ConstrctionList.push_back(construct);
    //}
    //
    //const QVector<FITKAbsGeoSketch2DConstruction> & FITKAbsGeoSketch2D::getConstructions()
    //{
    //    return m_ConstrctionList;
    //}
}