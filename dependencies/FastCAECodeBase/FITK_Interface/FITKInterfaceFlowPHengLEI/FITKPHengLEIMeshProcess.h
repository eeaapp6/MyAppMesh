/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_MESHPROCESS___H___
#define _FITK_PHENGLEI_MESHPROCESS___H___
 
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"

namespace Core
{
	class FITKParameter;
}

namespace Interface
{
	class FITKFlowPHengLEIAPI FITKPHengLEIMeshProcess : public Core::FITKAbstractNDataObject
	{
	public:
		FITKPHengLEIMeshProcess();
		virtual ~FITKPHengLEIMeshProcess();

		Core::FITKParameter* getAdditionalMeshData();
		void setAdditionalMeshData(Core::FITKParameter* data);

		Core::FITKParameter* getAdditionalMeshPartion();

		void setAdditionalMeshPartion(Core::FITKParameter* data);

		Core::FITKParameter* getAdditionalMeshConversion();

		void setAdditionaleshConversion(Core::FITKParameter* data);

		void initMeshProcess();

	private:
		QString _name{};
		Core::FITKParameter* _additionalMeshData{};

		Core::FITKParameter* _additionalMeshPartion{};

		Core::FITKParameter* _additionalMeshConversion{};
	};
}


#endif
