/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshAbstractAdaptor.h
 * @brief  抽象的Gmsh文件读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKGMSHABSTRACTADAPTOR_R_H__
#define __FITKGMSHABSTRACTADAPTOR_R_H__

#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"

namespace Interface
{
    class FITKAbstractTextReader;
    class FITKAbstractTextWriter;
}

namespace Gmsh
{
    /**
     * @brief  抽象的Gmsh文件读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshAbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(Gmsh, FITKGmshAbstractAdaptor);
    public:
        explicit FITKGmshAbstractAdaptor() = default;
        virtual ~FITKGmshAbstractAdaptor() = default;
        /**
          * @brief 设置INP文件读取器
          * @param reader INP读取类
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2024-11-29
          */
        void setFileReader(Interface::FITKAbstractTextReader* reader);
        /**
         * @brief 设置INP文件写出器
         * @param writer INP写出类
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void setFileWriter(Interface::FITKAbstractTextWriter* writer);

    protected:
        /**
         * @brief INP文件读取器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        Interface::FITKAbstractTextReader* _reader{};

        /**
         * @brief INP文件写出器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        Interface::FITKAbstractTextWriter* _writer{};
    };
}


#endif
