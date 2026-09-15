/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruPostDisplayGroup.h"

// VTK
#include <vtkAlgorithm.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetReader.h>

namespace Interface
{
    FITKStruPostDisplayGroup::FITKStruPostDisplayGroup()
    {

    }

    FITKStruPostDisplayGroup::~FITKStruPostDisplayGroup()
    {
        // 清空数据。
        m_members.clear();
    }

    void FITKStruPostDisplayGroup::setDisplayGroupEleIndice(QList<int> indice)
    {
        m_members = indice;
    }

    QList<int> FITKStruPostDisplayGroup::getDisplayGroupEleIndice()
    {
        return m_members;
    }

    int FITKStruPostDisplayGroup::getNumberOfDisplayGroupMember()
    {
        return m_members.count();
    }

    FITKStruPostDisplayGroup::DisplayGroupType FITKStruPostDisplayGroup::getDisplayGroupType()
    {
        return m_type;
    }

    void FITKStruPostDisplayGroup::setPostDataID(int postDataID)
    {
        // 保存数据。
        m_postDataID = postDataID;
    }

    int FITKStruPostDisplayGroup::getPostDataID()
    {
        return m_postDataID;
    }

    // Manager.
    //@{
    FITKPostGroupManager::FITKPostGroupManager()
    {
        // 初始化默认"All"显示分组。
        FITKStruPostDisplayGroup* groupAll = new FITKStruPostDisplayGroup;
        groupAll->m_type = FITKStruPostDisplayGroup::DisplayGroupType::DGT_All;
        groupAll->setDataObjectName("All");
        this->appendDataObj(groupAll);
    }

    void FITKPostGroupManager::setCurrentGroupID(int groupID)
    {
        m_currentGroupID = groupID;
    }

    void FITKPostGroupManager::setCurrentGroup(FITKStruPostDisplayGroup* groupData)
    {
        if (!groupData)
        {
            return;
        }

        // 保存分组ID。
        m_currentGroupID = groupData->getDataObjectID();
    }

    int FITKPostGroupManager::getCurrentGroupID()
    {
        // 尝试获取数据对象。
        FITKStruPostDisplayGroup* groupData = this->getDataByID(m_currentGroupID);
        if (!groupData)
        {
            m_currentGroupID = -1;
        }

        return m_currentGroupID;
    }

    FITKStruPostDisplayGroup* FITKPostGroupManager::getCurrentGroup()
    {
        // 尝试获取数据对象。
        FITKStruPostDisplayGroup* groupData = this->getDataByID(m_currentGroupID);
        if (!groupData)
        {
            m_currentGroupID = -1;
        }

        return groupData;
    }
    //@}
}
