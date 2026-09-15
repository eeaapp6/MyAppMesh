/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundaryTypeManager.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITKPHengLEIBoundaryType.h"
#include "FITKPHengLEIBoundaryFarfieldType.h"
#include "FITKPHengLEIBoundaryInFlowType.h"
#include "FITKPHengLEIBoundarySolidSurfaceType.h"

namespace Interface
{
    FITKPHengLEIBoundaryTypeManager::FITKPHengLEIBoundaryTypeManager()
    {
        this->initBoundaryTypes();

    }

    FITKPHengLEIBoundaryTypeManager::~FITKPHengLEIBoundaryTypeManager()
    {
			QList<FITKPHengLEIBoundaryTypeInfo*>::iterator iter = _boundaryTypes.begin();
			while (iter != _boundaryTypes.end())
			{
				if (*iter) // 确保不是空指针
				{
					delete *iter;  // 删除指向的对象
				}
				++iter;
			}
    }

	QStringList FITKPHengLEIBoundaryTypeManager::filterBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType bType)
	{
		QStringList listType;
		// 遍历_boundaryTypes
		for (QList<FITKPHengLEIBoundaryTypeInfo*>::iterator iter = _boundaryTypes.begin();iter != _boundaryTypes.end();++ iter)
		{
			// 进行过滤
			if ((*iter)->bType == bType)
			{
				listType.append((*iter)->_BoundaryTypeTypeName);
			}
		}
		return listType;
	}

	FITKPHengLEIBoundaryTypeInfo FITKPHengLEIBoundaryTypeManager::getBoundaryType(QString boundaryTypeName, FITKPHengLEIBoundary::PHengLEIBoundaryType bType)
	{
		for (QList<FITKPHengLEIBoundaryTypeInfo*>::iterator iter = _boundaryTypes.begin();iter != _boundaryTypes.end(); ++ iter)
		{
			if ((*iter)->bType == bType && (*iter)->_BoundaryTypeTypeName == boundaryTypeName)
			{
				return *(*iter);
			}
		}
		return FITKPHengLEIBoundaryTypeInfo();
	}

	void FITKPHengLEIBoundaryTypeManager::initBoundaryTypes()
    {
        //注册BoundaryPressureInlet
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryPressureInlet = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryPressureInlet->_BoundaryTypeTypeName = "PressureInlet";
		infoBoundaryPressureInlet->_createFuns = [&] {return new PressureInlet; };
		infoBoundaryPressureInlet->bType = FITKPHengLEIBoundary::PHBPressureInlet;

		//注册BoundaryPressureOutlet
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryPressureOutlet = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryPressureOutlet->_BoundaryTypeTypeName = "PressureOutlet";
		infoBoundaryPressureOutlet->_createFuns = [&] {return new PressureOutlet; };
		infoBoundaryPressureOutlet->bType = FITKPHengLEIBoundary::PHBPressureOutlet;

		//注册BoundaryMassFlowInlet
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryMassFlowInlet = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryMassFlowInlet->_BoundaryTypeTypeName = "MassFlowInlet";
		infoBoundaryMassFlowInlet->_createFuns = [&] {return new MassFlowInlet; };
		infoBoundaryMassFlowInlet->bType = FITKPHengLEIBoundary::PHBMassFlowInlet;

		//注册BoundaryMassFlowOutlet
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryMassFlowOutlet = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryMassFlowOutlet->_BoundaryTypeTypeName = "MassFlowOutlet";
		infoBoundaryMassFlowOutlet->_createFuns = [&] {return new MassFlowOutlet; };
		infoBoundaryMassFlowOutlet->bType = FITKPHengLEIBoundary::PHBMassFlowOutlet;

		//注册BoundarySYMMETRY
		FITKPHengLEIBoundaryTypeInfo* infoBoundarySYMMETRY = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundarySYMMETRY->_BoundaryTypeTypeName = "SYMMETRY";
		infoBoundarySYMMETRY->_createFuns = [&] {return new SYMMETRY; };
		infoBoundarySYMMETRY->bType = FITKPHengLEIBoundary::PHBSYMMETRY;

		//注册BoundaryOutFLow
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryOutFLow = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryOutFLow->_BoundaryTypeTypeName = "OutFlow";
		infoBoundaryOutFLow->_createFuns = [&] {return new OutFLow; };
		infoBoundaryOutFLow->bType = FITKPHengLEIBoundary::PhBOutFlow;

		_boundaryTypes.append(infoBoundaryPressureInlet);
		_boundaryTypes.append(infoBoundaryPressureOutlet);
		_boundaryTypes.append(infoBoundaryMassFlowInlet);
		_boundaryTypes.append(infoBoundaryMassFlowOutlet);
		_boundaryTypes.append(infoBoundarySYMMETRY);
		_boundaryTypes.append(infoBoundaryOutFLow);

		//注册BoundaryFarfield1
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield1 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield1->_BoundaryTypeTypeName = "Farfield1";
		infoBoundaryFarfield1->_createFuns = [&] {return new FarfieldOne; };
		infoBoundaryFarfield1->bType = FITKPHengLEIBoundary::PHBFarField;

		//注册BoundaryFarfield2
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield2 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield2->_BoundaryTypeTypeName = "Farfield2";
		infoBoundaryFarfield2->_createFuns = [&] {return new FarfieldTwo; };
		infoBoundaryFarfield2->bType = FITKPHengLEIBoundary::PHBFarField;

		//注册BoundaryFarfield3
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield3 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield3->_BoundaryTypeTypeName = "Farfield3";
		infoBoundaryFarfield3->_createFuns = [&] {return new FarfieldThree; };
		infoBoundaryFarfield3->bType = FITKPHengLEIBoundary::PHBFarField;

		//注册BoundaryFarfield4
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield4 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield4->_BoundaryTypeTypeName = "Farfield4";
		infoBoundaryFarfield4->_createFuns = [&] {return new FarfieldFour; };
		infoBoundaryFarfield4->bType = FITKPHengLEIBoundary::PHBFarField;

		//注册BoundaryFarfield5
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield5 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield5->_BoundaryTypeTypeName = "Farfield5";
		infoBoundaryFarfield5->_createFuns = [&] {return new FarfieldFive; };
		infoBoundaryFarfield5->bType = FITKPHengLEIBoundary::PHBFarField;

		//注册BoundaryFarfield6
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfield6 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfield6->_BoundaryTypeTypeName = "Farfield6";
		infoBoundaryFarfield6->_createFuns = [&] {return new FarfieldSix; };
		infoBoundaryFarfield6->bType = FITKPHengLEIBoundary::PHBFarField;


		//注册BoundaryFarfieldNone
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryFarfieldNone = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryFarfieldNone->_BoundaryTypeTypeName = "FarfieldNone";
		infoBoundaryFarfieldNone->_createFuns = [&] {return new FarfieldNone; };
		infoBoundaryFarfieldNone->bType = FITKPHengLEIBoundary::PHBFarField;

		_boundaryTypes.append(infoBoundaryFarfieldNone);
		_boundaryTypes.append(infoBoundaryFarfield1);
		_boundaryTypes.append(infoBoundaryFarfield2);
		_boundaryTypes.append(infoBoundaryFarfield3);
		_boundaryTypes.append(infoBoundaryFarfield4);
		_boundaryTypes.append(infoBoundaryFarfield5);
		_boundaryTypes.append(infoBoundaryFarfield6);


		//注册BoundaryInFlow1
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow1 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow1->_BoundaryTypeTypeName = "InFlow1";
		infoBoundaryInFlow1->_createFuns = [&] {return new InflowOne; };
		infoBoundaryInFlow1->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow2
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow2 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow2->_BoundaryTypeTypeName = "InFlow2";
		infoBoundaryInFlow2->_createFuns = [&] {return new InflowTwo; };
		infoBoundaryInFlow2->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow3
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow3 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow3->_BoundaryTypeTypeName = "InFlow3";
		infoBoundaryInFlow3->_createFuns = [&] {return new InflowThree; };
		infoBoundaryInFlow3->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow4
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow4 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow4->_BoundaryTypeTypeName = "InFlow4";
		infoBoundaryInFlow4->_createFuns = [&] {return new InflowFour; };
		infoBoundaryInFlow4->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow5
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow5 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow5->_BoundaryTypeTypeName = "InFlow5";
		infoBoundaryInFlow5->_createFuns = [&] {return new InflowFive; };
		infoBoundaryInFlow5->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow6
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow6 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow6->_BoundaryTypeTypeName = "InFlow6";
		infoBoundaryInFlow6->_createFuns = [&] {return new InflowSix; };
		infoBoundaryInFlow6->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlow7
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlow7 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlow7->_BoundaryTypeTypeName = "InFlow7";
		infoBoundaryInFlow7->_createFuns = [&] {return new InflowSeven; };
		infoBoundaryInFlow7->bType = FITKPHengLEIBoundary::PHBInFlow;

		//注册BoundaryInFlowNone
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryInFlowNone = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryInFlowNone->_BoundaryTypeTypeName = "InFlowNone";
		infoBoundaryInFlowNone->_createFuns = [&] {return new InflowNone; };
		infoBoundaryInFlowNone->bType = FITKPHengLEIBoundary::PHBInFlow;

		_boundaryTypes.append(infoBoundaryInFlowNone);
		_boundaryTypes.append(infoBoundaryInFlow1);
		_boundaryTypes.append(infoBoundaryInFlow2);
		_boundaryTypes.append(infoBoundaryInFlow3);
		_boundaryTypes.append(infoBoundaryInFlow4);
		_boundaryTypes.append(infoBoundaryInFlow5);
		_boundaryTypes.append(infoBoundaryInFlow6);
		_boundaryTypes.append(infoBoundaryInFlow7);

		//注册BoundaryWall1
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryWall1 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryWall1->_BoundaryTypeTypeName = "Wall1";
		infoBoundaryWall1->_createFuns = [&] {return new WallOne; };
		infoBoundaryWall1->bType = FITKPHengLEIBoundary::PHBWall;

		//注册BoundaryWall2
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryWall2 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryWall2->_BoundaryTypeTypeName = "Wall2";
		infoBoundaryWall2->_createFuns = [&] {return new WallTwo; };
		infoBoundaryWall2->bType = FITKPHengLEIBoundary::PHBWall;

		//注册BoundaryWall3
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryWall3 = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryWall3->_BoundaryTypeTypeName = "Wall3";
		infoBoundaryWall3->_createFuns = [&] {return new WallThree; };
		infoBoundaryWall3->bType = FITKPHengLEIBoundary::PHBWall;

		//注册BoundaryWallNone
		FITKPHengLEIBoundaryTypeInfo* infoBoundaryWallNone = new FITKPHengLEIBoundaryTypeInfo;
		infoBoundaryWallNone->_BoundaryTypeTypeName = "WallNone";
		infoBoundaryWallNone->_createFuns = [&] {return new WallNone; };
		infoBoundaryWallNone->bType = FITKPHengLEIBoundary::PHBWall;

		_boundaryTypes.append(infoBoundaryWallNone);
		_boundaryTypes.append(infoBoundaryWall1);
		_boundaryTypes.append(infoBoundaryWall2);
		_boundaryTypes.append(infoBoundaryWall3);

    }
}




