/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKNastranBDFReader.h
 * @brief Nastran bdf读取
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-10-11
 *
 */
#ifndef _FITK_NASTRANBDF_READER__H___
#define _FITK_NASTRANBDF_READER__H___

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

namespace Nastran
{
    class FITKNastranBDFIOReaderTool;
    /**
     * @brief Nastran BDF读取类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-10-11
     */
    class FITKNastranBDFReader : public Interface::FITKAbstractTextReader
    {
    public:
        /**
         * @brief Construct a new FITKNastranBDFReader object
         * @param[o]  isSuccess      是否执行成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        explicit FITKNastranBDFReader(bool & success);
        /**
        * @brief Destroy the FITKVAOneXMLReader object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-10-11
        */
        virtual ~FITKNastranBDFReader();
        /**
        * @brief 执行读取操作
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-10-11
        */
        virtual void run() override;
        /**
          * @brief 打印控制台消息
          * @param[i] level 打印级别 1 normal 2 warning 3error
          * @param[i] str 打印的信息
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-03-08
          */
        void consoleMessage(int level, const QString& str) override;
        /**
         * @brief  获取读取工具
         * @return FITKNastranBDFIOReaderTool*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-06-21
         */
        FITKNastranBDFIOReaderTool* getReaderTool();
        /**
         * @brief    读取行
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        virtual QString readLine();

    private:
        /**
         * @brief    读取数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-05-29
         */
        bool reader();

    private:
        /**
         * @brief 是否执行成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        bool &_isSuccess;
        /**
         * @brief  读取工具
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        FITKNastranBDFIOReaderTool* _readerTool{};
    };
}


#endif
