/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKNastranBDFWriter.h
 * @brief Nastran bdf写入
 * @author shenzhuowei (2931275006@qq.com)
 * @date 2025-03-03
 *
 */
#ifndef _FITK_NASTRANBDF_WRITER__H___
#define _FITK_NASTRANBDF_WRITER__H___

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace AbaqusData
{
    class FITKDataCase;
}

namespace Nastran
{
    class FITKNastranBDFIOWriterTool;
    /**
     * @brief Nastran BDF写入类
     * @author shenzhuowei (2931275006@qq.com)
     * @date 2025-03-03
     */
    class FITKNastranBDFWriter : public Interface::FITKAbstractTextWriter
    {
    public:
        /**
         * @brief Construct a new FITKNastranBDFWriter object
         * @param[i]  interface      组件指针，可传递参数
         * @param[o]  isSuccess      是否执行成功
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2025-03-03
         */
        explicit FITKNastranBDFWriter(bool & success);
        /**
        * @brief Destroy the FITKVAOneXMLReader object
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2025-03-03
        */
        virtual ~FITKNastranBDFWriter();
        /**
        * @brief 执行读取操作
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2025-03-03
        */
        virtual void run() override;

        /**
        * @brief 打印控制台消息
        * @param[i] level 打印级别 1 normal 2 warning 3error
        * @param[i] str 打印的信息
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2025-03-03
        */
        void consoleMessage(int level, const QString& str) override;
        /**
         * @brief  获取写出工具
         * @return FITKNastranBDFIOReaderTool*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-06-21
         */
        FITKNastranBDFIOWriterTool* getWriterTool();

    private:
        /**
         * @brief    写出数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-05-29
         */
        bool writer();

    private:
        /**
         * @brief 是否执行成功
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2025-03-03
         */
        bool &_isSuccess;
        /**
         * @brief  写出工具
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        FITKNastranBDFIOWriterTool* _writerTool{};
    };
}


#endif
