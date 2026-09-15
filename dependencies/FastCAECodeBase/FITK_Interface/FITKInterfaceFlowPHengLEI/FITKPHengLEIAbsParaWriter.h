/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEIAbsParaWriter.h
 * @brief 参数文件写出抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-11
 * 
 */
#ifndef _FGITKPHENGLEI_ABS_PAEAM_WRITER___H___
#define _FGITKPHENGLEI_ABS_PAEAM_WRITER___H___

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QString>

namespace Interface
{
    /**
     * @brief 参数文件写出抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-11
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIAbsParaWriter : public FITKAbstractTextWriter
    {
    public:
        /**
         * @brief Construct a new FITKPHengLEIAbsParaWriter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        explicit FITKPHengLEIAbsParaWriter() = default;
        /**
         * @brief Destroy the FITKPHengLEIAbsParaWriter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual ~FITKPHengLEIAbsParaWriter() = default;
        /**
         * @brief 执行写出
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual bool writeParamFile() = 0;
        /**
         * @brief 写出cfd_para.hypara文件
         * @param[i]  fileName       文件路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        static  bool WriteCFDParam(const QString& fileName);

		static bool CopyPHengLEIprogram();

		void consoleMessage(int level, const QString& str) override;

		void run() override;
    };
}


#endif
