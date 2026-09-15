/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_FLOW___H___
#define _FITK_PHENGLEI_FLOW___H___
 
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"

namespace Core
{
	class FITKParameter;
}

namespace Interface
{
	class FITKFlowPHengLEIAPI FITKPHengLEIFlow : public Core::FITKAbstractNDataObject
	{
	public:
		FITKPHengLEIFlow();
		virtual ~FITKPHengLEIFlow();

		Core::FITKParameter* getAdditionalSolverName();
		void setAdditionalSolverName(Core::FITKParameter* data);

		Core::FITKParameter* getAdditionalInitialValue();

		void setAdditionalInitialValue(Core::FITKParameter* data);

		Core::FITKParameter* getAdditionalMomentumEquation();

		void setAdditionalMomentumEquation(Core::FITKParameter* data);
		
		Core::FITKParameter* getAdditionalPressurePossionEquation();

		void setAdditionalPressurePossionEquation(Core::FITKParameter* data);

		Core::FITKParameter* getAdditionalSurfaceFlux();

		void setAdditionalSurfaceFlux(Core::FITKParameter* data);
	private:
		QString _name{};
		Core::FITKParameter* _additionalSolverName{};

		Core::FITKParameter* _additionalInitialValue{};

		Core::FITKParameter* _additionalMomentumEquation{};

		Core::FITKParameter* _additionalPressurePossionEquation{};

		Core::FITKParameter* _additionalSurfaceFlux{};
	};
}


#endif
