/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKCoordinateAlgorithm.h
 * @brief       Filter for create axes grid data with label string array.
 *              ( The axes will be shown as a single line. )
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-27
 *********************************************************************/

#ifndef FITKCOORDINATEALGORITHM_H
#define FITKCOORDINATEALGORITHM_H

#include "vtkPolyDataAlgorithm.h"

#include "FITKVTKAlgorithmAPI.h"

class vtkTransform;

class FITKVTKALGORITHMAPI FITKCoordinateAlgorithm : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(FITKCoordinateAlgorithm, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    /**
     * Construct object with scaling on, scaling mode is by scalar value, scale factor = 1.0.
     */
    static FITKCoordinateAlgorithm* New();

    //@{
    /**
     * Set/Get the direction of three axes.
     */
    vtkSetVector3Macro(DirectionX, double);
    vtkGetVectorMacro(DirectionX, double, 3);
    vtkSetVector3Macro(DirectionY, double);
    vtkGetVectorMacro(DirectionY, double, 3);
    vtkSetVector3Macro(DirectionZ, double);
    vtkGetVectorMacro(DirectionZ, double, 3);
    //@}

    //@{
    /**
     * Set/Get the name of three axes.
     */
    vtkSetMacro(XLabel, const char*);
    vtkGetMacro(XLabel, const char*);
    vtkSetMacro(YLabel, const char*);
    vtkGetMacro(YLabel, const char*);
    vtkSetMacro(ZLabel, const char*);
    vtkGetMacro(ZLabel, const char*);
    void SetAxesLabels(const char* xLabel, const char* yLabel, const char* zLabel)
    {
        SetXLabel(xLabel);
        SetYLabel(yLabel);
        SetZLabel(zLabel);
    }
    //@}

    //@{
    /**
     * Get the name of the axes label text array name.
     */
    vtkGetMacro(LabelArrayName, const char*);
    //@}

    //@{
    /**
     * Set/get the desired precision for the output types. See the documentation
     * for the vtkAlgorithm::DesiredOutputPrecision enum for an explanation of
     * the available precision settings.
     */
    vtkSetMacro(OutputPointsPrecision, int);
    vtkGetMacro(OutputPointsPrecision, int);
    //@}

protected:
    FITKCoordinateAlgorithm();
    ~FITKCoordinateAlgorithm() = default;

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int, vtkInformation*) override;

    //@{
    /**
     * Method called in RequestData() to do the actual data processing. This will
     * glyph the \c input, filling up the \c output based on the filter
     * parameters.
     */
    virtual bool Execute(vtkDataSet* input, vtkInformationVector* sourceVector, vtkPolyData* output);
    virtual bool Execute(vtkDataSet* input, vtkInformationVector* sourceVector, vtkPolyData* output,
        vtkDataArray* inSScalars, vtkDataArray* inVectors);
    //@}

    const char* LabelArrayName = "Labels";
   
    int OutputPointsPrecision;

    double DirectionX[3];
    double DirectionY[3];
    double DirectionZ[3];

    const char* XLabel = "X";
    const char* YLabel = "Y";
    const char* ZLabel = "Z";

private:
    FITKCoordinateAlgorithm(const FITKCoordinateAlgorithm&) = delete;
    void operator=(const FITKCoordinateAlgorithm&) = delete;
};

#endif // FITKCOORDINATEALGORITHM_h
