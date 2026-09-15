/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKNastranAbstractAdaptor.h
 * @brief  抽象的Nastran文件读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-03-13
 *********************************************************************/
#ifndef _FITKNASTRANABSTRACTADAPTOR_H
#define _FITKNASTRANABSTRACTADAPTOR_H

#include "FITKNastranBDFReader.h"
#include "FITKNastranBDFWriter.h"

#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Nastran
{
    /**
     * @brief  抽象的Nastran文件读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-03-13
     */
    class FITKNastranAbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(Nastran, FITKNastranAbstractAdaptor);

    public:
        /**
        * @brief  Create
        * @author shenzhuowei (2931275006@qq.com)
        * @date   2025-03-05
        */
        explicit FITKNastranAbstractAdaptor() = default;

        /**
        * @brief  Destory
        * @author shenzhuowei (2931275006@qq.com)
        * @date   2025-03-05
        */
        virtual ~FITKNastranAbstractAdaptor() = default;

        /**
         * @brief    Set the reader for the FITKNastranAbstractAdaptor.
         * @param[i] reader 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        void setReader(FITKNastranBDFReader *reader);

        /**
         * @brief    Set the writer for the FITKNastranAbstractAdaptor.
         * @param[i] writer 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        void setWriter(FITKNastranBDFWriter *writer);


    protected:
        /**
         * @brief BDF文件读取器
         * @author zhourp (zhourunping@diso.cn)
         * @date 2024-03-15
         */
        FITKNastranBDFReader *_reader{};

        /**
         * @brief BDF文件写入器
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2025-03-03
         */
        FITKNastranBDFWriter *_writer{};
    };
} // namespace Nastran
#endif // FITKNASTRANABSTRACTADAPTOR_H
