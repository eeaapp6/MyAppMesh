/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKActorClipTool.h
 * @brief       Clip tools for vtkActor and its sub-class.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-04-10
 *********************************************************************/

#ifndef __FITKACTORCLIPTOOL_H__
#define __FITKACTORCLIPTOOL_H__

#include "FITKVTKAlgorithmAPI.h"

#include <QColor>

// Forward declaration
class vtkMapper;
class vtkActor;
class vtkDataObject;
class vtkDataSetMapper;
class vtkPassThrough;
class vtkAlgorithm;
class vtkAlgorithmOutput;
class vtkImplicitFunction;

/**
 * @brief       The clip type.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-04-10
 */
enum ClipType
{
    NoneType = -1,
    Clip = 0,
    Cut,
    ExtractGeometry

};

/**
 * @brief  裁切方向
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-18
 */
enum class ClipInside {
    None = -1,
    Off = 0,
    On = 1
};

/**
 * @brief       Sub-class of vtkActor, add some new interfaces for setting input.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-03-25
 */
class FITKVTKALGORITHMAPI FITKActorClipTool
{
public:
    /**
     * @brief       Constructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    FITKActorClipTool(vtkActor* actor);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    virtual ~FITKActorClipTool();

    /**
     * @brief       Set input data to the actor's mapper.
     * @param[in]   data: Input data
     * @param[in]   port: Input port[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    void setInputDataObject(vtkDataObject* data, int port = 0);

    /**
     * @brief       Set connection to the actor's mapper.
     * @param[in]   input: Input connection
     * @param[in]   port: Input port[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    void setInputConnection(vtkAlgorithmOutput* input, int port = 0);

    /**
     * @brief       Change the clip filter by type.
     * @param[in]   type: The clip type
     * @param[in]   deleteOldFilter: Auto delete the old filter[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    void setClipType(ClipType type, bool deleteOldFilter = true);
    /**
     * @brief    设置裁切方向
     * @param[i] type                   方向
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-18
     */
    void setClipInside(ClipInside type);

    /**
     * @brief       Set the clipped function.
     * @param[in]   func: The implicit function of the clip filter
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    void setClipImplicitFunction(vtkImplicitFunction* func);

protected:
    /**
     * @brief       Set the clipped algorithm.
     * @param[in]   func: The algorithm for extracting the actor input
     * @param[in]   deleteOldFilter: Auto delete the old filter[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    void setClipAlgorithm(vtkAlgorithm* func, bool deleteOldFilter = true);

    /**
     * @brief       Set a new mapper for the clip tools.
     * @param[in]   mapper: The new mapper
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-11-10
     */
    void setNewMapper(vtkMapper* mapper);

protected:
    /**
     * @brief       The actor for clipping.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    vtkActor* m_actor{ nullptr };

    /**
     * @brief       Current clip type.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    ClipType m_clipType = ClipType::NoneType;
    /**
     * @brief  裁切方向指定
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-04-18
     */
    ClipInside m_insideType = ClipInside::None;

    /**
     * @brief       The pass through algorithm.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    vtkPassThrough* m_passThrough{ nullptr };

    /**
     * @brief       The clip algorithm.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-10
     */
    vtkAlgorithm* m_postAlg{ nullptr };

};

#endif // __FITKACTORCLIPTOOL_H__
