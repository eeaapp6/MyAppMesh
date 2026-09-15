/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKExtractGeometry.h
 * @brief       The filter for extracting a part of the data set.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 *********************************************************************/

#ifndef FITKEXTRACTGEOMETRY_H
#define FITKEXTRACTGEOMETRY_H

#include <vtkUnstructuredGridAlgorithm.h>

#include "FITKVTKAlgorithmAPI.h"

#include <QHash>

class vtkImplicitFunction;

class FITKVTKALGORITHMAPI FITKExtractGeometry : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(FITKExtractGeometry, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    /**
   * Construct object with ExtractInside turned on.
   */
    static FITKExtractGeometry* New();

    /**
   * Return the MTime taking into account changes to the implicit function
   */
    vtkMTimeType GetMTime() override;

    //@{
    /**
   * Specify the implicit function for inside/outside checks.
   */
    virtual void SetImplicitFunction(vtkImplicitFunction*);
    vtkGetObjectMacro(ImplicitFunction, vtkImplicitFunction);
    //@}

    //@{
    /**
   * Boolean controls whether to extract cells that are inside of implicit
   * function (ExtractInside == 1) or outside of implicit function
   * (ExtractInside == 0).
   */
    vtkSetMacro(ExtractInside, vtkTypeBool);
    vtkGetMacro(ExtractInside, vtkTypeBool);
    vtkBooleanMacro(ExtractInside, vtkTypeBool);
    //@}

    //@{
    /**
   * Boolean controls whether to extract cells that are partially inside.
   * By default, ExtractBoundaryCells is off.
   */
    vtkSetMacro(ExtractBoundaryCells, vtkTypeBool);
    vtkGetMacro(ExtractBoundaryCells, vtkTypeBool);
    vtkBooleanMacro(ExtractBoundaryCells, vtkTypeBool);
    vtkSetMacro(ExtractOnlyBoundaryCells, vtkTypeBool);
    vtkGetMacro(ExtractOnlyBoundaryCells, vtkTypeBool);
    vtkBooleanMacro(ExtractOnlyBoundaryCells, vtkTypeBool);
    //@}
     /**
     * @brief 获取框选中的所有原网格节点序号
     * @return  QList< int > 原网格节点序号
     * @time  2023/11/07
     * @author maguiwa(maguiwa520@163.com)
     */
    QList< int > getSelectOriginalPoints() const;

    /**
    * @brief 获取框选中的所有原网格单元序号
    * @return  QList< int > 原网格单元序号
    * @time  2023/11/07
    * @author maguiwa(maguiwa520@163.com)
    */
    QList< int > getSelectOriginalCells() const;

    /**
    * @brief 获取原网格与框选后网格的节点映射关系
    * @return  QHash< int, int >& 节点映射关系
    * @time  2023/11/07
    * @author maguiwa(maguiwa520@163.com)
    */
    const QHash< int, int >& getPointMap() const;

    /**
    * @brief 获取原网格与框选后网格的单元映射关系
    * @return  QHash< int, int >& 单元映射关系
    * @time  2023/11/07
    * @author maguiwa(maguiwa520@163.com)
    */
    const QHash< int, int >& getCellMap() const;

protected:
    FITKExtractGeometry(vtkImplicitFunction* f = nullptr);
    ~FITKExtractGeometry() override;

    // Usual data generation method
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

    int FillInputPortInformation(int port, vtkInformation* info) override;

    vtkImplicitFunction* ImplicitFunction;
    vtkTypeBool          ExtractInside;
    vtkTypeBool          ExtractBoundaryCells;
    vtkTypeBool          ExtractOnlyBoundaryCells;

private:
    FITKExtractGeometry(const FITKExtractGeometry&) = delete;
    void operator=(const FITKExtractGeometry&) = delete;

    QHash< int, int > _originalCurrentPointMap;
    QHash< int, int > _originalCurrentCellMap;
};

#endif  FITKEXTRACTGEOMETRY_H
