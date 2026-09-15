/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundary.h"
#include "FITKPHengLEIBoundaryType.h"

namespace Interface
{
    FITKPHengLEIBoundary::FITKPHengLEIBoundary(PHengLEIBoundaryType type)
        :_boundaryType(type)
    {
    }

    FITKPHengLEIBoundary::PHengLEIBoundaryType FITKPHengLEIBoundary::getBoundaryEnumType()
    {
        return _boundaryType;
    }

    int FITKPHengLEIBoundary::getMeshBoundaryID()
    {
        return  _meshBoundaryID;
    }

    void FITKPHengLEIBoundary::setMeshBoundaryID(const int & id)
    {
        _meshBoundaryID = id;
    }

	void FITKPHengLEIBoundary::setBoundaryType(FITKPHengLEIBoundaryType * boundaryType)
	{
		if (_absboundType != boundaryType && _absboundType) delete _absboundType;
		_absboundType = boundaryType;
	}

	FITKPHengLEIBoundaryType * FITKPHengLEIBoundary::getBoundaryType()
	{
		return _absboundType;
	}

    FITKPHengLEIBoundary* FITKPHengLEIBoundaryManager::getBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType t)
    {
        const int n = this->getDataCount();
        for ( int i = 0; i < n;  i++)
        {
            FITKPHengLEIBoundary* boundary = this->getDataByIndex(i);
            if(boundary == nullptr) continue;
            if (boundary->getBoundaryEnumType() == t) return boundary;
        }
        return nullptr;
    }

	FITKPHengLEIBoundary * FITKPHengLEIBoundaryManager::getBoundary(int MeshBoundaryID)
	{
		const int n = this->getDataCount();
		for (int i = 0; i < n; i++)
		{
			FITKPHengLEIBoundary* bd = this->getDataByIndex(i);
			if (bd == nullptr) continue;
			if (bd->getMeshBoundaryID() == MeshBoundaryID)return bd;
		}
		return nullptr;
	}
}


