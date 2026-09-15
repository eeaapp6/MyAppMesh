/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKExtractCellsByDim.h
 * @brief       Extract the cells by the given dimension.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-04-14
 *********************************************************************/

#ifndef FITKEXTRACTCELLSBYDIM_H
#define FITKEXTRACTCELLSBYDIM_H

#include "vtkUnstructuredGridAlgorithm.h"

#include "FITKVTKAlgorithmAPI.h"

 // Forward declaration
class vtkIdList;
class FITKExtractCellsByDimSTLCloak;

/**
 * @brief       Extract the cells by the given dimension.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-04-14
 */
class FITKVTKALGORITHMAPI FITKExtractCellsByDim : public vtkUnstructuredGridAlgorithm
{
public:
    //@{
    /**
     * Standard methods for construction, type info, and printing.
     */
    vtkTypeMacro(FITKExtractCellsByDim, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    static FITKExtractCellsByDim* New();
    //@}

    //@{
    /**
     *  Set or get the extract dimension, default is 2.
     */
    vtkSetMacro(ExtractDimension, int);
    vtkGetMacro(ExtractDimension, int);
    //@}

    //@{
    /**
     * If all cells are being extracted, this filter can use fast path to speed up
     * the extraction. In that case, one can set this flag to true. When set to
     * true, cell ids added via the various methods are simply ignored.
     * Defaults to false.
     */
    vtkSetMacro(ExtractAllCells, bool);
    vtkGetMacro(ExtractAllCells, bool);
    vtkBooleanMacro(ExtractAllCells, bool);
    //@}

protected:
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

    FITKExtractCellsByDim();
    ~FITKExtractCellsByDim() override;

    void Copy(vtkDataSet* input, vtkUnstructuredGrid* output);
    vtkIdType ReMapPointIds(vtkDataSet* grid);

    void CopyCellsDataSet(vtkDataSet* input, vtkUnstructuredGrid* output);
    void CopyCellsUnstructuredGrid(vtkDataSet* input, vtkUnstructuredGrid* output);

    /**
     * Add the supplied list of cell IDs to those that will be included in the
     * output vtkUnstructuredGrid. The cell ids should be >=0.
     */
    void AddCellList(vtkIdList* l);

    FITKExtractCellsByDimSTLCloak* CellList = nullptr;
    vtkIdType SubSetUGridCellArraySize = 0;
    vtkIdType SubSetUGridFacesArraySize = 0;
    bool InputIsUgrid = false;
    bool ExtractAllCells = false;
    int ExtractDimension = 2;

private:
    FITKExtractCellsByDim(const FITKExtractCellsByDim&) = delete;
    void operator=(const FITKExtractCellsByDim&) = delete;

};

#endif
