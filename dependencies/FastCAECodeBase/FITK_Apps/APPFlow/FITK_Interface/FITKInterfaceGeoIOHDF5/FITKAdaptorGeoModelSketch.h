/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelSketch.h
 * @brief  草绘适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELSKETCH_H
#define FITKADAPTORGEOMODELSKETCH_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

ForwardDeclarNS(Core, FITKAbstractNDataObject);

namespace Interface
{
    /**
     * @brief  草绘适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelSketch : public FITKAdaptorGeoCommand
    {
    public:
        
        /**
         * @brief    构造
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        explicit FITKAdaptorGeoModelSketch() = default;
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual ~FITKAdaptorGeoModelSketch() = default;

   
    protected:
        /**
         * @brief    参数数据写出
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeParameters(H5::Group & g) override;
        /**
         * @brief    参数数据读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readParameters() override;
         

    private:
        // 添加需要的私有成员
        /**
         * @brief    根据枚举名称实例化该类 GeoModel
         * @param[i] enumName 
         * @return   Core::FITKAbstractNDataObject *
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-05-23
         */
        Core::FITKAbstractNDataObject* getGeoCommand(QString enumName);
    };
    /**
     * @brief  草图删除适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-27
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorModelSketchDeleter : public FITKAdaptorGeoCommand
    {
    public:

        explicit FITKAdaptorModelSketchDeleter() = default;
        virtual ~FITKAdaptorModelSketchDeleter() = default;

    protected:

        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELSKETCH_H
