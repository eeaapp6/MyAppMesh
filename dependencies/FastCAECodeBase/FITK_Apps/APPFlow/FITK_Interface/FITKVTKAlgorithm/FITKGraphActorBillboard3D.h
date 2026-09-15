/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGraphActorBillboard3D.h
 * @brief       External vtkBillboardTextActor3D with some common functions.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-02-11
 *********************************************************************/

#ifndef __FITKGRAPHACTORBILLBOARD3D_H__
#define __FITKGRAPHACTORBILLBOARD3D_H__

#include <vtkBillboardTextActor3D.h>

#include "FITKVTKAlgorithmAPI.h"

#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>

// Forward declaration
namespace Comp
{
    class FITKGraphObjectVTK;
}

/**
 * @brief       Override vtkBillboardTextActor3D, add some new interfaces for setting input.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-02-11
 */
class FITKVTKALGORITHMAPI FITKGraphActorBillboard3D : public vtkBillboardTextActor3D
{
public:
    vtkTypeMacro(FITKGraphActorBillboard3D, vtkBillboardTextActor3D);

    /**
     * @brief       VTK new.
     * @return      The instance
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    static FITKGraphActorBillboard3D* New();

    /**
     * @brief       Set the data object.
     * @param[in]   obj: Data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    void setGraphObject(Comp::FITKGraphObjectVTK* obj);

    /**
     * @brief       Get the data object.
     * @return      Data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    Comp::FITKGraphObjectVTK* getGraphObject();

    /**
     * @brief       Get the graph object as the given type.[template]
     * @return      The graph object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    template<class T>
    T* getGraphObjectAs()
    {
        return dynamic_cast<T*>(m_graphObj);
    }

protected:
    /**
     * @brief       Constructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    FITKGraphActorBillboard3D();

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    virtual ~FITKGraphActorBillboard3D();

    /**
     * @brief       Copy constructor.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    FITKGraphActorBillboard3D(const FITKGraphActorBillboard3D&) = delete;

    /**
     * @brief       Operator overloading.
     * @param[in]   The object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    void operator=(const FITKGraphActorBillboard3D&) = delete;

protected:
    /**
     * @brief       Shallow copy.[override]
     * @param[in]   prop: The source prop
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    void ShallowCopy(vtkProp* prop) override;
    //@}

protected:
    /**
     * @brief       The data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-02-11
     */
    Comp::FITKGraphObjectVTK* m_graphObj{ nullptr };

};

#endif // __FITKGRAPHACTORBILLBOARD3D_H__
