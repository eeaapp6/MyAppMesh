/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEICouplingControl.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEICouplingControl::FITKPHengLEICouplingControl()
		:_participantName("Fluid"),          
		_couplingInterfaceName("Interface"),    
		_couplingMeshName_1("Fluid-Mesh-Centers"),      
		_couplingMeshName_2("Fluid-Mesh-Nodes"),       
		_couplingMeshType_1("faceCenters"),       
		_couplingMeshType_2("faceNodes"),       
		_writeDataName("Force"),            
		_readDataName("Displacement"),             
		_couplingEnabled(false)
	{
		
	}

	FITKPHengLEICouplingControl::~FITKPHengLEICouplingControl()
	{
		
	}
	// --- 耦合启用状态的实现 ---

	void FITKPHengLEICouplingControl::setCouplingEnabled(bool enabled)
	{
		_couplingEnabled = enabled;
	}

	bool FITKPHengLEICouplingControl::isCouplingEnabled() const
	{
		return _couplingEnabled;
	}

	// --- 各个耦合参数的 Setter 和 Getter 实现 ---

	void FITKPHengLEICouplingControl::setParticipantName(const QString& name)
	{
		_participantName = name;
	}

	QString FITKPHengLEICouplingControl::getParticipantName() const
	{
		return _participantName;
	}

	void FITKPHengLEICouplingControl::setCouplingInterfaceName(const QString& name)
	{
		_couplingInterfaceName = name;
	}

	QString FITKPHengLEICouplingControl::getCouplingInterfaceName() const
	{
		return _couplingInterfaceName;
	}

	void FITKPHengLEICouplingControl::setCouplingMeshName_1(const QString& name)
	{
		_couplingMeshName_1 = name;
	}

	QString FITKPHengLEICouplingControl::getCouplingMeshName_1() const
	{
		return _couplingMeshName_1;
	}

	void FITKPHengLEICouplingControl::setCouplingMeshName_2(const QString& name)
	{
		_couplingMeshName_2 = name;
	}

	QString FITKPHengLEICouplingControl::getCouplingMeshName_2() const
	{
		return _couplingMeshName_2;
	}

	void FITKPHengLEICouplingControl::setCouplingMeshType_1(const QString& type)
	{
		_couplingMeshType_1 = type;
	}

	QString FITKPHengLEICouplingControl::getCouplingMeshType_1() const
	{
		return _couplingMeshType_1;
	}

	void FITKPHengLEICouplingControl::setCouplingMeshType_2(const QString& type)
	{
		_couplingMeshType_2 = type;
	}

	QString FITKPHengLEICouplingControl::getCouplingMeshType_2() const
	{
		return _couplingMeshType_2;
	}

	void FITKPHengLEICouplingControl::setWriteDataName(const QString& name)
	{
		_writeDataName = name;
	}

	QString FITKPHengLEICouplingControl::getWriteDataName() const
	{
		return _writeDataName;
	}

	void FITKPHengLEICouplingControl::setReadDataName(const QString& name)
	{
		_readDataName = name;
	}

	QString FITKPHengLEICouplingControl::getReadDataName() const
	{
		return _readDataName;
	}

}
