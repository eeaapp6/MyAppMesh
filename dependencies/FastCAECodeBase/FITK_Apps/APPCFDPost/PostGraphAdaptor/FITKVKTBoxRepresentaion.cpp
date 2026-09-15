/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKVKTBoxRepresentaion.h"

#include <vtkPoints.h>

namespace Interface
{
    FITKVKTBoxRepresentaion* FITKVKTBoxRepresentaion::New()
    {
        return new FITKVKTBoxRepresentaion();
    }

    void FITKVKTBoxRepresentaion::setBounds(double* bounds)
    {
        this->Points->SetPoint(0, bounds[0], bounds[2], bounds[4]);
        this->Points->SetPoint(1, bounds[1], bounds[2], bounds[4]);
        this->Points->SetPoint(2, bounds[1], bounds[3], bounds[4]);
        this->Points->SetPoint(3, bounds[0], bounds[3], bounds[4]);
        this->Points->SetPoint(4, bounds[0], bounds[2], bounds[5]);
        this->Points->SetPoint(5, bounds[1], bounds[2], bounds[5]);
        this->Points->SetPoint(6, bounds[1], bounds[3], bounds[5]);
        this->Points->SetPoint(7, bounds[0], bounds[3], bounds[5]);

        for (int i = 0; i < 6; i++)
        {
            this->InitialBounds[i] = bounds[i];
        }
        this->InitialLength = sqrt((bounds[1] - bounds[0]) * (bounds[1] - bounds[0]) +
            (bounds[3] - bounds[2]) * (bounds[3] - bounds[2]) +
            (bounds[5] - bounds[4]) * (bounds[5] - bounds[4]));

        this->PositionHandles();
        this->ComputeNormals();
        this->ValidPick = 1; // since we have set up widget
        this->SizeHandles();
    }

    FITKVKTBoxRepresentaion::FITKVKTBoxRepresentaion():
        vtkBoxRepresentation()
    {

    }

    FITKVKTBoxRepresentaion::~FITKVKTBoxRepresentaion()
    {
        //vtkBoxRepresentation::~vtkBoxRepresentation();
    }
}