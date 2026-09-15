/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file   FITKAbstractRapidXMLWriter.h
 * @brief  利用RapidXML的XML解析器写出XML文件
 * 
 * @author TanDongYang
 * @date   2024-11-12
 *********************************************************************/

#ifndef FITKABSTRACTRAPIDXMLWRITER_H
#define FITKABSTRACTRAPIDXMLWRITER_H

#include "FITKAbstractIO.h"

namespace rapidxml
{
    template<class Ch>
    class xml_document;
}

namespace Interface
{
    /**
     * @brief   利用RapidXML的XML解析器写出XML文件，抽象类
     * @author  TanDongYang
     * @date    2024-11-12 
     */
    class FITKInterfaceIOAPI FITKAbstractRapidXMLWriter
        :public FITKAbstractIO
    {
    public:
        /**
         * @brief   Construct a new FITKAbstractRapidXMLWriter object
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        explicit FITKAbstractRapidXMLWriter() = default;

        /**
         * @brief   Destroy the FITKAbstractRapidXMLWriter object
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        virtual ~FITKAbstractRapidXMLWriter() = 0;

        /**
         * @brief  创建上下文
         * @return true       创建成功
         * @return false      创建失败
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        virtual bool createContent();

        /**
         * @brief   写出XML内容
         * @return true       写出成功
         * @return false      写出失败
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        virtual bool writeXML();

        /**
         * @brief   发送百分比
         * @param[i]   p
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        void sendCurrentPercent(int p = -1);

    protected:

        /**
         * @brief   Rapidxml的dom文档对象
         * @author  TanDongYang
         * @date    2024-11-12 
         */
        rapidxml::xml_document<char> *_document{};
    };

}


#endif // FITKABSTRACTRAPIDXMLWRITER_H
