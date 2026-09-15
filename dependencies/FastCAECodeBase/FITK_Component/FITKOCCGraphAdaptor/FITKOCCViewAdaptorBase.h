/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorBase.h
 * @brief       Abstract adaptor for graph object.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-05
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORBASE_H__
#define __FITKOCCVIEWADAPTORBASE_H__

#include "FITK_Kernel/FITKAdaptor/FITKAbstractViewAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"
#include "FITKOCCGraphAdaptorAPI.h"

 // Forward declaration
namespace Exchange
{
    class FITKOCCGraphObject3D;
}

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor base.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-05
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorBase : public Adaptor::FITKAbstractViewAdaptor
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorBase);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        explicit FITKOCCViewAdaptorBase() = default;

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        virtual ~FITKOCCViewAdaptorBase() = default;

        /**
         * @brief       Set the input data and set the update flag.[virtual][override]
         * @param[in]   data: The input data need to be exchanged
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data) override;

        /**
         * @brief       Set the details information.
         * @param[in]   details: The data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        void setDetails(QVariant details);

        ///**
        // * @brief       Set the input data object need to be exchanged.
        // * @param[in]   input: The data object
        // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date        2024-06-05
        // */
        //void setInputData(Core::FITKAbstractNDataObject* input);

        /**
         * @brief       Get the output graph data object.
         * @return      Graph data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        Exchange::FITKOCCGraphObject3D* getOutputData();

        /**
         * @brief       Execute the data exchange.[virtual]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        virtual bool update();

    protected:
        // The data object need to be exchanged
        // Core::FITKAbstractNDataObject* m_inputData{ nullptr };

        /**
         * @brief       The details information.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        QVariant m_details;

        /**
         * @brief       The output graph data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        Exchange::FITKOCCGraphObject3D* m_outputData{ nullptr };

        /**
         * @brief       If the data is new, then update and create output data, else return.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        bool m_needUpdate = false;

    };
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORBASE_H__