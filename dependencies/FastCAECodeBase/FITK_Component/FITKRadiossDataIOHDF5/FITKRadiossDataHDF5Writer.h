/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataHDF5Writer.h
 * @brief  HDF5文件写出
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef __FITKRadiossDataHDF5Writer_H__
#define __FITKRadiossDataHDF5Writer_H__

#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

ForwardDeclarNS(H5, Group);

namespace IO
{
    /**
     * @brief  HDF5文件写出
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-13
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataHDF5Writer : public FITKAbstractHDF5Writer
    {
    public:
        /**
         * @brief    Construct a new FITKACISHDF5Writer object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        explicit FITKRadiossDataHDF5Writer() = default;
        virtual ~FITKRadiossDataHDF5Writer() = default;
        /**
         * @brief    重写run函数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        void run() override;
        /**
         * @brief    发送计算进度
         * @param[i] Progress 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        void sendCalculateProgress(int Progress);
        /**
         * @brief    打印控制台消息
         * @param[i] level 
         * @param[i] str 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief    写出命令数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        bool writeRadiossDatas();
        /**
         * @brief    写出版本
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-13
         */
        bool writeVersion();

    private:

    };
}


#endif
