/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractEquationOfState.h"

namespace Radioss
{
    FITKAbstractEquationOfState::~FITKAbstractEquationOfState()
    {

    }

    int FITKAbstractEquationOfState::getMaterialIDCount()
    {
        return m_matIDs.size();
    }

    void FITKAbstractEquationOfState::addMaterialID(int matID, FITKEquationOfStateManager*mgr)
    {
        //若其他状态方程有该材料，则移除
        if (mgr) {
            for (int i = 0; i < mgr->getDataCount(); ++i)
            {
                FITKAbstractEquationOfState* eos = mgr->getDataByIndex(i);
                if (!eos)continue;
                eos->removeMaterialID(matID);
            }
        }
        if (m_matIDs.contains(matID))
            return;
        m_matIDs.append(matID);
    }

    QList<int> FITKAbstractEquationOfState::getMaterialIDs()
    {
        return m_matIDs;
    }

    void FITKAbstractEquationOfState::removeMaterialID(int matID)
    {
        m_matIDs.removeOne(matID);
    }

    bool FITKAbstractEquationOfState::containsMaterialID(int matID)
    {
        return m_matIDs.contains(matID);
    }

    void FITKAbstractEquationOfState::clearMaterialIDs()
    {
        m_matIDs.clear();
    }

    void FITKAbstractEquationOfState::setP0(double value)
    {
        //Set Reference density
        m_p0 = value;
    }

    double FITKAbstractEquationOfState::getP0()
    {
        //Get Reference density
        return m_p0;
    }
}