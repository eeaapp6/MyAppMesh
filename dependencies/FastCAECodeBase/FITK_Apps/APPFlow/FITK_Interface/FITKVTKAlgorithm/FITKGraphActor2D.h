/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGraphActor2D.h
 * @brief       External vtkActor2D with some common functions.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 *********************************************************************/

#ifndef __FITKGRAPHACTOR2D_H__
#define __FITKGRAPHACTOR2D_H__

#include <vtkActor2D.h>

#include "FITKVTKAlgorithmAPI.h"

#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>

// Forward declaration
namespace Comp
{
    class FITKGraphObjectVTK;
}

/**
 * @brief       The type of the actor2D.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 */
enum ActorType2D
{
    // Other actor.
    OtherActor2D = -1,

    // Text label actor.
    TextActor = 0,

    // The poly glyph actor.
    GlyphActor,
};

/**
 * @brief       Override vtkActor2D, add some new interfaces for setting input.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 */
class FITKVTKALGORITHMAPI FITKGraphActor2D : public vtkActor2D
{
public:
    vtkTypeMacro(FITKGraphActor2D, vtkActor2D);

    /**
     * @brief       VTK new.
     * @return      The instance
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    static FITKGraphActor2D* New();

    /**
     * @brief       Set the data object.
     * @param[in]   obj: Data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void setGraphObject(Comp::FITKGraphObjectVTK* obj);

    /**
     * @brief       Get the data object.
     * @return      Data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    Comp::FITKGraphObjectVTK* getGraphObject();

    /**
     * @brief       Get the graph object as the given type.[template]
     * @return      The graph object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-05
     */
    template<class T>
    T* getGraphObjectAs()
    {
        return dynamic_cast<T*>(m_graphObj);
    }
    
    /**
     * @brief       Set the actor's data type.
     * @param[in]   type: Surface or edge
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void setActorType(ActorType2D type);

    /**
     * @brief       Get the actor's data type.
     * @return      Is a surface or an edge
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    ActorType2D getActorType();

    /**
     * @brief       Update the actor's mapper.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void update();

protected:
    /**
     * @brief       Constructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    FITKGraphActor2D();

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    virtual ~FITKGraphActor2D();

    /**
     * @brief       Copy constructor.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    FITKGraphActor2D(const FITKGraphActor2D&) = delete;

    /**
     * @brief       Operator overloading.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void operator=(const FITKGraphActor2D&) = delete;

protected:
    /**
     * @brief       Shallow copy.[override]
     * @param[in]   prop: The source prop
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    void ShallowCopy(vtkProp* prop) override;
    //@}

protected:
    /**
     * @brief       The data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    Comp::FITKGraphObjectVTK* m_graphObj{ nullptr };

    /**
     * @brief       The type of actor's data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-05-16
     */
    ActorType2D m_actorType = OtherActor2D;

};

#endif // __FITKGRAPHACTOR2D_H__
