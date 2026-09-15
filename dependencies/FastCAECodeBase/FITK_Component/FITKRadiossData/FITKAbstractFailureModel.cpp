/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractFailureModel.h"

namespace Radioss
{

    FITKAbstractFailureModel::FITKAbstractFailureModel()
    {
    }

    FITKAbstractFailureModel::~FITKAbstractFailureModel()
    {

    }

    FITKAbstractFailureModel::FailureModelType FITKAbstractFailureModel::getFailureModelType()
    {
        return FITKAbstractFailureModel::FailureModelType::None;
    }

    int FITKAbstractFailureModel::getMaterialIDCount()
    {
        return m_matIDs.size();
    }

    void FITKAbstractFailureModel::addMaterialID(int matID, FITKRadiossFailureModelManager*mgr)
    {
        //若其他失效模型有该材料，则移除
        if (mgr) {
            for (int i = 0; i < mgr->getDataCount(); ++i)
            {
                FITKAbstractFailureModel* fail = mgr->getDataByIndex(i);
                if (!fail)continue;
                fail->removeMaterialID(matID);
            }
        }
        if (m_matIDs.contains(matID))
            return;
        m_matIDs.append(matID);
    }

    QList<int> FITKAbstractFailureModel::getMaterialIDs()
    {
        return m_matIDs;
    }

    void FITKAbstractFailureModel::removeMaterialID(int matID)
    {
        m_matIDs.removeOne(matID);
    }

    bool FITKAbstractFailureModel::containsMaterialID(int matID)
    {
        return m_matIDs.contains(matID);
    }

    void FITKAbstractFailureModel::clearMaterialIDs()
    {
        m_matIDs.clear();
    }

} // namespace Radioss
