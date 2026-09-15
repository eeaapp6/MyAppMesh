/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPolygonSource.h
 * @brief       Geometry source for polygons.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-04-11
 *********************************************************************/

#ifndef FITKPOLYGONSOURCE_H
#define FITKPOLYGONSOURCE_H

#include "vtkPolyDataAlgorithm.h"

#include "FITKVTKAlgorithmAPI.h"

class FITKVTKALGORITHMAPI FITKPolygonSource : public vtkPolyDataAlgorithm
{
public:
    //@{
    /**
     * Standard methods for instantiation, obtaining type and printing instance values.
     */
    static FITKPolygonSource* New();
    vtkTypeMacro(FITKPolygonSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    //@}

    //@{
    /**
     * Set/Get the number of sides of the polygon. By default, the number of sides
     * is set to six, if the number is 2, then this is a 'X' shape source.
     */
    vtkSetClampMacro(NumberOfSides, int, 2, VTK_INT_MAX);
    vtkGetMacro(NumberOfSides, int);
    //@}

    //@{
    /**
     * Set/Get the center of the polygon. By default, the center is set at the
     * origin (0,0,0).
     */
    vtkSetVector3Macro(Center, double);
    vtkGetVectorMacro(Center, double, 3);
    //@}

    //@{
    /**
     * Set/Get the normal to the polygon. The ordering of the polygon will be
     * counter-clockwise around the normal (i.e., using the right-hand rule).
     * By default, the normal is set to (0,0,1).
     */
    vtkSetVector3Macro(Normal, double);
    vtkGetVectorMacro(Normal, double, 3);

    /**
     * Set/Get the view up of the plane.
     */
    vtkSetVector3Macro(PlaneViewUp, double);
    vtkGetVectorMacro(PlaneViewUp, double, 3);
    //@}

    //@{
    /**
     * Set/Get the radius of the polygon. By default, the radius is set to 0.5.
     */
    vtkSetMacro(Radius, double);
    vtkGetMacro(Radius, double);
    //@}

    //@{
    /**
     * Control whether a polygon is produced. By default, GeneratePolygon is enabled.
     */
    vtkSetMacro(GeneratePolygon, vtkTypeBool);
    vtkGetMacro(GeneratePolygon, vtkTypeBool);
    vtkBooleanMacro(GeneratePolygon, vtkTypeBool);
    //@}

    //@{
    /**
     * Control whether a polyline is produced. By default, GeneratePolyline is enabled.
     */
    vtkSetMacro(GeneratePolyline, vtkTypeBool);
    vtkGetMacro(GeneratePolyline, vtkTypeBool);
    vtkBooleanMacro(GeneratePolyline, vtkTypeBool);
    //@}

    //@{
    /**
     * Set/get the desired precision for the output points.
     * vtkAlgorithm::SINGLE_PRECISION - Output single-precision floating point.
     * vtkAlgorithm::DOUBLE_PRECISION - Output double-precision floating point.
     */
    vtkSetMacro(OutputPointsPrecision, int);
    vtkGetMacro(OutputPointsPrecision, int);
    //@}

protected:
    FITKPolygonSource();
    ~FITKPolygonSource() override {}

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

    int NumberOfSides;
    double Center[3];
    double Normal[3];
    double PlaneViewUp[3];
    double Radius;
    vtkTypeBool GeneratePolygon;
    vtkTypeBool GeneratePolyline;
    int OutputPointsPrecision;

private:
    FITKPolygonSource(const FITKPolygonSource&) = delete;
    void operator=(const FITKPolygonSource&) = delete;
};

#endif // FITKPOLYGONSOURCE_H
