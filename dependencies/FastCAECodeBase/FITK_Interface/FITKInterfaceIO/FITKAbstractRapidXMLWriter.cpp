/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRapidXMLWriter.h"
#include <QFile>
#include "rapidxml.hpp"

// Adding declarations to make it compatible with gcc and greater
// See https://stackoverflow.com/a/55408678
namespace rapidxml {
    namespace internal {
        template <class OutIt, class Ch>
        inline OutIt print_children(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_attributes(OutIt out, const xml_node<Ch>* node, int flags);

        template <class OutIt, class Ch>
        inline OutIt print_data_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_cdata_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_element_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_declaration_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_comment_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_doctype_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_pi_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
    }
}

#include "rapidxml_print.hpp"

Interface::FITKAbstractRapidXMLWriter::~FITKAbstractRapidXMLWriter()
{
    //销毁_document
    if (_document) delete _document;
}

bool Interface::FITKAbstractRapidXMLWriter::createContent()
{
    if (_document) delete _document;
    //创建_document
    _document = new rapidxml::xml_document<char>;
    return true;
}

bool Interface::FITKAbstractRapidXMLWriter::writeXML()
{
    if (!_document) return false;

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    std::string xmlString;
    rapidxml::print(std::back_inserter(xmlString), *_document, 0);
    //将_document内容写出到文件中
    file.write(QString::fromStdString(xmlString).toUtf8());
    file.close();
    return true;
}

void Interface::FITKAbstractRapidXMLWriter::sendCurrentPercent(int p /*= -1*/)
{
    //发送信号
    emit sendProcessSig(this, p);
}
