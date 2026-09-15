/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractHDF5Writer.h
 * @brief HDF5文件写出基类声明
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-29
 *
 */
#ifndef  _FITKABSTRACTHDF5WIRTE_H__
#define  _FITKABSTRACTHDF5WIRTE_H__

#include "FITKInterfaceIOAPI.h"
#include "FITKAbstractIO.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QTextStream>
#include <QFile>

ForwardDeclarNS(H5, H5File);

namespace IO
{
    class FITKHDF5FileTool;
    /**
     * @brief HDF5文件写出基类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-29
     */
    class FITKInterfaceIOAPI FITKAbstractHDF5Writer : public Interface::FITKAbstractIO
    {
    public:
        explicit FITKAbstractHDF5Writer() = default;
        virtual  ~FITKAbstractHDF5Writer();

        /**
         * @brief 设置H5根节点
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        void setH5Root(H5::H5File* h5File);

        /**
         * @brief   设置版本
         * param    v 版本号
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-05-29
         */
        void setVersion(double v);
        /**
         * @brief 创建文件指针
         * @param[o]  error         错误输出
         * @return true   创建成功
         * @return false  创建失败
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-18
         */
        bool createContext(QString & error);
        /**
         * @brief 关闭文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-18
         */
        void closeFile();
        /**
         * @brief 获取HDF5文件指针
         * @return H5::H5File* HDF5文件指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-06-04
         */
        H5::H5File* getHDF5File();
        /**
         * @brief 获取HDF5文件工具类
         * @return FITKHDF5FileTool* HDF5文件工具类指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-06-04
         */

        FITKHDF5FileTool* getHDF5FileTool();
    protected:
        /**
         * @brief H5的文件指针
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        H5::H5File* _h5File{};

        /**
         * @brief 版本号
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        double _version = -1;

    private:
        /**
         * @brief 是否类内创建的文件指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-18
         */
        bool _createFileContext{ false };

        FITKHDF5FileTool* _tools{};

    };
}

#endif
