/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractQtJsonReader.h
 * @brief 利用Qt的JSON解析器读取JSON文件
 * @author libaojunAI
 * @date 2026-04-13
 * 
 */
#ifndef _FITKABSTRACT_JSON_QT_READER_H___
#define _FITKABSTRACT_JSON_QT_READER_H___
 
#include "FITKAbstractIO.h"

class QJsonDocument;
class QFile;


namespace Interface
{
    /**
     * @brief 利用Qt的JSON解析器读取JSON文件，抽象类
     * @author libaojunAI
     * @date 2026-04-13
     */
    class FITKInterfaceIOAPI FITKAbstractQtJsonReader
        :public FITKAbstractIO
    {
    public:
       /**
        * @brief Construct a new FITKAbstractQtJsonReader object
        * @author libaojunAI
        * @date 2026-04-13
        */
        explicit FITKAbstractQtJsonReader() = default;
        /**
         * @brief Destroy the FITKAbstractQtJsonReader object
         * @author libaojunAI
         * @date 2026-04-13
         */
        virtual ~FITKAbstractQtJsonReader() = 0;
        /**
         * @brief 创建上下文，将文件与_document(QJsonDocument)进行关联
         * @return true       创建成功
         * @return false      创建失败
         * @author libaojunAI
         * @date 2026-04-13
         */
        virtual bool createContent();
        /**
         * @brief 关闭文件
         * @author libaojunAI
         * @date 2026-04-13
         */
        virtual void closeFile();

    protected:
        /**
         * @brief qt的json文档对象
         * @author libaojunAI
         * @date 2026-04-13
         */
        QJsonDocument* _document{};
        /**
         * @brief 文件对象
         * @author libaojunAI
         * @date 2026-04-13
         */
        QFile* _file{};


    };
}


#endif