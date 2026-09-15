/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITKABAINTERACTION_CONTACT_GEO_PROPERTY_H__
#define __FITKABAINTERACTION_CONTACT_GEO_PROPERTY_H__

#include "FITKAbaInteractionContactBehavior.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface
{
    class FITKInterfaceStructuralAPI FITKAbaInteractionContactGeometricProperty :
        public FITKAbaInteractionContactBehavior
    {
    public:
        explicit FITKAbaInteractionContactGeometricProperty() = default;
        virtual ~FITKAbaInteractionContactGeometricProperty() = default;

        FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType getBehaviorType() const override;
    
        void setSurfaceThicknessOrCrossSecArea(const double v);
        double getSurfaceThicknessOrCrossSecArea() const;

        void enableThicknessOfInterLayer(bool enable);
        bool isEnableThicknessOfInterLayer()const;

        void setThicknessOfInterLayerValue(double v);
        double getThicknessOfInterLayerValue();




    private:
        double _surfaceThicknessOrCrossSecArea {1.0};

        bool _enableThicknessOfInterLayer{ false };

        double _thicknessOfInterLayerValue{ 0.0 };
    
    };
}


#endif //__FITKABAINTERACTION_CONTACT_GEO_PROPERTY_H__
