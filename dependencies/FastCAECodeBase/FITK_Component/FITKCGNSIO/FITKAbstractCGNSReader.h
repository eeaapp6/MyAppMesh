/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractCGNSReader.h
 * @brief  CGNS 文件读取基类声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-04
 *********************************************************************/

#ifndef _FITKABSTRACTCGNSREADER_H__
#define _FITKABSTRACTCGNSREADER_H__

#include "FITKCGNSIOAPI.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractIO.h"

namespace IO
{
    /**
     * @brief  CGNS 文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-19
     */
    class FITKCGNSIOAPI FITKAbstractCGNSReader : public Interface::FITKAbstractIO
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        FITKAbstractCGNSReader() = default;
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        virtual ~FITKAbstractCGNSReader();

        /**
         * @brief    打开CGNS文件
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool openCGNSFile();

        /**
         * @brief    关闭文件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        void closeFile();
        /**
         * @brief    获取CGNS文件索引
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        int getCGNSFileIndex();

        /**
         * @brief    发送百分比
         * @param[i] p -1为自动计算，否则直接发送百分比 0-100
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        void sendCurrentPercent(int p = -1);

    protected:
        /**
         * @brief  CGNS文件索引号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-04
         */
        int m_index_file{ -1 };
    };

}

#endif