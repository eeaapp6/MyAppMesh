/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKDeformFilter.h
 * @brief       Algorithm for adding displacement to a data set.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 *********************************************************************/

#ifndef FITKDEFORMFILTER_H
#define FITKDEFORMFILTER_H

#include <vtkPointSetAlgorithm.h>

#include "FITKVTKAlgorithmAPI.h"

class FITKVTKALGORITHMAPI FITKDeformFilter : public vtkPointSetAlgorithm
{
public:
    static FITKDeformFilter* New();
    vtkTypeMacro(FITKDeformFilter, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    //@{
    /**
     *  Set or get the scale factor of the deformation, default is 1.0.
     */
    vtkSetMacro(ScaleFactor, double);
    vtkGetMacro(ScaleFactor, double);
    //@}

    //@{
    /**
     *  Set or get the array name of the deformation's tuple.
     */
    vtkSetStringMacro(DeformArrayName);
    vtkGetStringMacro(DeformArrayName);
    //@}

    //@{
    /**
     *  Set or get the index of x-direction deformation array component, default is 0.
     */
    vtkSetMacro(UComponentIndex, int);
    vtkGetMacro(UComponentIndex, int);
    //@}

    //@{
    /**
     *  Set or get the index of y-direction deformation array component, default is 1.
     */
    vtkSetMacro(VComponentIndex, int);
    vtkGetMacro(VComponentIndex, int);
    //@}

    //@{
    /**
     *  Set or get the index of z-direction deformation array component, default is 2.
     */
    vtkSetMacro(WComponentIndex, int);
    vtkGetMacro(WComponentIndex, int);
    //@}

    //@{
    /**
     *  Enable or disable the deform, default is On.
     */
    vtkSetMacro(Deform, vtkTypeBool);
    vtkGetMacro(Deform, vtkTypeBool);
    vtkBooleanMacro(Deform, vtkTypeBool);
    //@}

    /**
     * @brief       Set the deformation components indice.
     * @param[in]   ui: The x-direction deformation component index
     * @param[in]   vi: The y-direction deformation component index
     * @param[in]   wi: The z-direction deformation component index
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void SetComponentsIndice(int ui, int vi, int wi)
    {
        SetUComponentIndex(ui);
        SetVComponentIndex(vi);
        SetWComponentIndex(wi);
    }

protected:
    /**
     * @brief       Constructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    FITKDeformFilter();

    /**
     * @brief       Destructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    ~FITKDeformFilter();

    /**
     * @brief       The algorithm excute.
     * @param[in]   request: The requested data
     * @param[in]   inputVector: The input vector
     * @param[in]   outputVector: The ouput data
     * @return      Is valid
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);

private:
    /**
     * @brief       Copy constructor.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    FITKDeformFilter(const FITKDeformFilter&);

    /**
     * @brief       Operator overloading.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void operator+= (const FITKDeformFilter&);

private:
    /**
     * @brief       Enable or disable of the deformation.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    vtkTypeBool Deform = 1;

    /**
     * @brief       The scale factor value.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    double ScaleFactor = 1.;

    /**
     * @brief       The deform array name.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    char* DeformArrayName;

    /**
     * @brief       The x-direction deformation component index.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    int UComponentIndex = 0;

    /**
     * @brief       The y-direction deformation component index.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    int VComponentIndex = 1;

    /**
     * @brief       The z-direction deformation component index.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    int WComponentIndex = 2;
};

#endif // FITKDEFORMFILTER_H
