/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataHDF5Reader.h
 * @brief  HDF5文件读取
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef __FITKRadiossDataHDF5Reader_H__
#define __FITKRadiossDataHDF5Reader_H__

#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"

ForwardDeclarNS(H5, Group);

namespace IO
{
    /**
     * @brief  HDF5文件读取
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-28
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataHDF5Reader : public FITKAbstractHDF5Reader
    {
    public:
        /**
         * @brief    Construct a new FITKACISHDF5Reader object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        explicit FITKRadiossDataHDF5Reader() = default;
        virtual ~FITKRadiossDataHDF5Reader() = default;

        /**
         * @brief    重写run函数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void run() override;

        /**
         * @brief    打印控制台消息
         * @param[i] level 
         * @param[i] str 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief    读取数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool readRadiossDatas();
        /**
         * @brief    读取版本
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool readVersion();
    private:
    };
}


#endif
