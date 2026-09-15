/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKMaxMinValueFilter.h
 * @brief       Algorithm for save the array's max value's point or cell center to a data set.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-22
 *********************************************************************/

#ifndef FITKMAXMINVALUEFILTER_H
#define FITKMAXMINVALUEFILTER_H

#include <vtkUnstructuredGridAlgorithm.h>

#include "FITKVTKAlgorithmAPI.h"

class vtkCell;

enum PeakType
{
    PeakMax = 1,
    PeakMin
};

class FITKVTKALGORITHMAPI FITKMaxMinValueFilter : public vtkUnstructuredGridAlgorithm
{
public:
    static FITKMaxMinValueFilter* New();
    vtkTypeMacro(FITKMaxMinValueFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    //@{
    /**
     *  Set or get return after getting the first data.
     */
    vtkSetMacro(GetFirst, bool);
    vtkGetMacro(GetFirst, bool);
    //@}

    //@{
    /**
     *  Set or get the data type.
     *  1: Max
     *  2: Min
     */
    vtkSetMacro(PeakDataType, int);
    vtkGetMacro(PeakDataType, int);
    //@}

    //@{
    /**
     *  Set or get the array type.
     *  1: Point array
     *  2: Cell array
     */
    vtkSetMacro(ArrayType, int);
    vtkGetMacro(ArrayType, int);
    //@}

    //@{
    /**
     *  Set or get the array name.
     */
    vtkSetStringMacro(ArrayName);
    vtkGetStringMacro(ArrayName);
    //@}

    //@{
    /**
     *  Set or get the index of x-direction deformation array component, default is 0.
     *  The index -1 means the magnitude of the tuple.
     */
    vtkSetMacro(ComponentIndex, int);
    vtkGetMacro(ComponentIndex, int);
    //@}

    //@{
    /**
     * Set/Get the label array name.
     */
    vtkSetMacro(LabelArrayName, const char*);
    vtkGetMacro(LabelArrayName, const char*);
    //@}

protected:
    /**
     * @brief       Constructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    FITKMaxMinValueFilter();

    /**
     * @brief       Destructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    ~FITKMaxMinValueFilter();

    /**
     * @brief       Fill the input port's information.[override]
     * @param[in]   port: The input port
     * @param[in]   info: Information
     * @return      Is valid
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int FillInputPortInformation(int port, vtkInformation* info) override;

    /**
     * @brief       Fill the output port's information.[override]
     * @param[in]   port: The input port
     * @param[in]   info: Information
     * @return      Is valid
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int FillOutputPortInformation(int port, vtkInformation* info) override;

    /**
     * @brief       Preprocess function before the algorithm begin.[override]
     * @param[in]   request: The requested data
     * @param[in]   inputVector: The input vector
     * @param[in]   outputVector: The ouput data
     * @return      Is valid
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector) override;

    /**
     * @brief       The algorithm excute.[override]
     * @param[in]   request: The requested data
     * @param[in]   inputVector: The input vector
     * @param[in]   outputVector: The ouput data
     * @return      Is valid
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector) override;

private:
    /**
     * @brief       Copy constructor.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    FITKMaxMinValueFilter(const FITKMaxMinValueFilter&);

    /**
     * @brief       Operator overloading.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    void operator+= (const FITKMaxMinValueFilter&);

    /**
     * @brief       Get the center point of the cells.
     * @param[in]   cell: The cell
     * @param[out]  center: The center of the cell
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    void getCellCenter(vtkCell* cell, double* center);

    /**
     * @brief       Get the direction of the line with point 1 and point 2.
     * @param[in]   pt1: The point 1
     * @param[in]   pt2: The point 2
     * @param[out]  direc: The direction of the line
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-23
     */
    void getLineDirection(double* pt1, double* pt2, double* direc);

private:
    /**
     * @brief       Return after getting the first max or min value.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    bool GetFirst = true;

    /**
     * @brief       The data type.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int PeakDataType = 0;

    /**
     * @brief       The array type.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int ArrayType = 0;

    /**
     * @brief       The array name.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    char* ArrayName;

    /**
     * @brief       The array's component index.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-22
     */
    int ComponentIndex = 0;

    /**
     * @brief       The array's name of text labels.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-07-17
     */
    const char* LabelArrayName = "Labels";

};

#endif // FITKMAXMINVALUEFILTER_H
