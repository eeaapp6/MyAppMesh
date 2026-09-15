/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPolyDataNormals.h
 * @brief       The filter for calculating polydata's normals.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-10
 *********************************************************************/

#ifndef FITKPOLYDATANORMALS_H
#define FITKPOLYDATANORMALS_H

#include "vtkPolyDataAlgorithm.h"

#include "FITKVTKAlgorithmAPI.h"

class vtkFloatArray;
class vtkIdList;
class vtkPolyData;

class FITKVTKALGORITHMAPI FITKPolyDataNormals : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(FITKPolyDataNormals, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    /**
     * Construct with feature angle=30, splitting and consistency turned on,
     * flipNormals turned off, and non-manifold traversal turned on.
     * ComputePointNormals is on and ComputeCellNormals is off.
     */
    static FITKPolyDataNormals* New();

    //@{
    /**
     * Turn on/off the computation of point normals.
     */
    vtkSetMacro(ComputePointNormals, vtkTypeBool);
    vtkGetMacro(ComputePointNormals, vtkTypeBool);
    vtkBooleanMacro(ComputePointNormals, vtkTypeBool);
    //@}

    //@{
    /**
     * Turn on/off the computation of cell normals.
     */
    vtkSetMacro(ComputeCellNormals, vtkTypeBool);
    vtkGetMacro(ComputeCellNormals, vtkTypeBool);
    vtkBooleanMacro(ComputeCellNormals, vtkTypeBool);
    //@}

    //@{
    /**
     * Set/get the desired precision for the output types. See the documentation
     * for the vtkAlgorithm::DesiredOutputPrecision enum for an explanation of
     * the available precision settings.
     */
    vtkSetClampMacro(OutputPointsPrecision, int, SINGLE_PRECISION, DEFAULT_PRECISION);
    vtkGetMacro(OutputPointsPrecision, int);
    //@}

protected:
    FITKPolyDataNormals();
    ~FITKPolyDataNormals() override {}

    // Usual data generation method
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

    vtkTypeBool ComputePointNormals;
    vtkTypeBool ComputeCellNormals;
    int OutputPointsPrecision;

private:
    vtkPolyData* OldMesh;
    vtkPolyData* NewMesh;
    vtkFloatArray* PolyNormals;

private:
    FITKPolyDataNormals(const FITKPolyDataNormals&) = delete;
    void operator=(const FITKPolyDataNormals&) = delete;
};

#endif // FITKPOLYDATANORMALS_H
