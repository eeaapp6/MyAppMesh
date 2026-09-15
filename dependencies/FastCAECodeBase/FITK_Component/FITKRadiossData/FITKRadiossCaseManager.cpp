/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossCaseManager.h"

namespace Radioss
{
    void FITKRadiossCaseManager::setCurrentCaseID(int caseID)
    {
        _currentCaseID = caseID;
    }

    int FITKRadiossCaseManager::getCurrentCaseID() const
    {
        return _currentCaseID;
    }

    FITKRadiossCase* FITKRadiossCaseManager::getCurrentCase()
    {
        FITKRadiossCase* currentCase = this->getDataByID(_currentCaseID);
        if (!currentCase) return currentCase;
        //寻找第一个不是nullptr的元素
        const int n = this->getDataCount();
        for (int i = 0; i<n; ++i)
        {
            currentCase = this->getDataByIndex(i);
            if (currentCase != nullptr)
            {
                _currentCaseID = currentCase->getDataObjectID();
                return currentCase;
            }
        }
        return nullptr;
    }

} // namespace Radioss
