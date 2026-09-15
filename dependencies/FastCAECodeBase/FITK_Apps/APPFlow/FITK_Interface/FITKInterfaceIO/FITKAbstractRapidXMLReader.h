/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractQtXMLReader.h
 * @brief 利用RapidXML的XML解析器读取XML文件
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-11
 *
 */
#ifndef _FITK_ABSTRACT_RAPID_XML_READER_H___
#define _FITK_ABSTRACT_RAPID_XML_READER_H___
 
#include "FITKAbstractIO.h"

namespace rapidxml
{
    template<class Ch>
    class xml_document;
    template<class Ch>
    class file;
}

namespace Interface
{
    /**
     * @brief 利用RapidXML解析器读取XML文件，抽象类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-11
     */
    class FITKInterfaceIOAPI FITKAbstractRapidXMLReader
        :public FITKAbstractIO
    {
    public:
        /**
        * @brief Construct a new FITKAbstractRapidXMLReader object
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-10-11
        */
        explicit FITKAbstractRapidXMLReader() = default;
        /**
         * @brief Destroy the FITKAbstractRapidXMLReader object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual ~FITKAbstractRapidXMLReader() = 0;

        /**
         * @brief 创建上下文，将文件与_document(rapidxml)进行关联
         * @return true       创建成功
         * @return false      创建失败
         * @param[o] errorMessage 错误信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual bool createContent(QString & errorMessage);
        /**
         * @brief 发送百分比
         * param p -1为自动计算，否则直接发送百分比 0-100
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        void sendCurrentPercent(int p = -1);

    protected:
        /**
         * @brief Rapidxml的文档对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-23
         */
        rapidxml::file<char>* _file{};
        /**
         * @brief Rapidxml的dom文档对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        rapidxml::xml_document<char> *_document{};
    };

 
}


#endif
