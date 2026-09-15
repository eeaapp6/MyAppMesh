/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFAbstractDictWriter.h
 * @brief 字典文件写出基类接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-19
 */
#ifndef __FITKOFABSTRACTDICTWRITER_H__
#define __FITKOFABSTRACTDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include <QString>

namespace DICT
{
    class FITKDictObject;
}

namespace IO
{
    /**
     * @brief FITKOFAbstractDictWriter 字典文件写出基类接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFAbstractDictWriter();
        virtual ~FITKOFAbstractDictWriter() = 0;

        /**
         * @brief 虚函数 设置文件路径
         * @param[i] filePath
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        virtual void setFilePath(const QString& filePath) = 0;
        /**
         * @brief 虚函数 获取文件
         * @return 文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        virtual QString getFileName() = 0;
        /**
         * @brief 虚函数 run函数
         * @return 状态 true:成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        virtual bool run() = 0;

    protected:
        /**
         * @brief 打开字典工程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        void openDictObject();
        /**
         * @brief 写出字典文件
         * @return 状态 true 成功 false 失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool writeDictFile();
        /**
         * @brief 关闭字典工程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void closeDictObject();
        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        virtual void consoleMessage(int level, const QString& str);

    protected:
        /**
         * @brief 文件路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_filePath{};
        /**
         * @brief 用于写出的
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        DICT::FITKDictObject* m_objectDict{};
    };
}



#endif
