/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAIS_ObjectBase.h
 * @brief       Base class for OCC AIS_Interactive object.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKAIS_OBJECTBASE_H__
#define __FITKAIS_OBJECTBASE_H__

#include "FITKOCCGraphAdaptorAPI.h"

// Forward declaration
namespace Core
{
    class FITKAbstractNDataObject;
}

namespace Exchange
{
    class FITKOCCGraphObject3D;
}

/**
 * @brief       Base class for OCC AIS_Interactive object.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 */
class FITKOCCGRAPHADAPTORAPI FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constrctor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    FITKAIS_ObjectBase();

    /**
     * @brief       Destructor.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    virtual ~FITKAIS_ObjectBase();

    /**
     * @brief       Save the graph object pointer.( For picking )
     * @param[in]   gObj: The graph object pointer
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    void setGraphObject(Exchange::FITKOCCGraphObject3D* gObj);

    /**
     * @brief       Get the saved graph object pointer.
     * @return      The graph object pointer
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    Exchange::FITKOCCGraphObject3D* getGraphObject();

    /**
     * @brief       Save the data object id.
     * @param[in]   dataObjId: The data object id
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    void setDataObjectId(int dataObjId);

    /**
     * @brief       Get the saved data object id.
     * @return      The data object id
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    int getDataObjectId();

    /**
     * @brief       Get the data object by the saved data object id.
     * @return      The data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    Core::FITKAbstractNDataObject* getDataObject();

    /**
     * @brief       Get the data object by the saved data object id.[template]
     * @return      The data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    template<class T>
    T* getDataObjectAs()
    {
        return dynamic_cast<T*>(getDataObject());
    }

private:
    /**
     * @brief       The parent graph object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    Exchange::FITKOCCGraphObject3D* m_graphObj{ nullptr };

    /**
     * @brief       The data object id of the graph obejct.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    int m_dataObjId = -1;
};

#endif // __FITKAIS_OBJECTBASE_H__
