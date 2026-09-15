/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractQtXMLReader.h
 * @brief 利用Qt的XML解析器写出XML文件 
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-11
 * 
 */
#ifndef _FITKABSTRACT_XML_QT_WRITER_H___
#define _FITKABSTRACT_XML_QT_WRITER_H___

#include "FITKAbstractIO.h"

class QDomDocument;
class QFile;

namespace Interface
{
    /**
     * @brief 利用Qt的XML解析器写出XML文件，抽象类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-11
     */
    class FITKInterfaceIOAPI FITKAbstractQtXMLWriter
        :public FITKAbstractIO
    {
    public:
        /**
         * @brief Construct a new FITKAbstractQtXMLWriter object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        explicit FITKAbstractQtXMLWriter() = default;
        /**
         * @brief Destroy the FITKAbstractQtXMLWriter object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual ~FITKAbstractQtXMLWriter() = 0;
        
        /**
         * @brief 创建上下文，将文件与_document(QDomDocument)进行关联
         * @return true       创建成功
         * @return false      创建失败
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual bool createContent();
        /**
         * @brief 关闭文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual void closeFile();

    protected:
        /**
         * @brief qt的dom文档对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        QDomDocument* _document{};
        /**
         * @brief 文件对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        QFile* _file{};


    };
}


#endif