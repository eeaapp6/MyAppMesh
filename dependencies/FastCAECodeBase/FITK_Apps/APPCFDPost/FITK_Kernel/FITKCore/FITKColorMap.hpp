/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITKCOLORMAP_HPP__
#define __FITKCOLORMAP_HPP__

#include <QColor>
#include <QList>

namespace Core
{
    const static QList<QColor> colorMap = {
        QColor("#E0E0E0"), QColor("#FF99CC"),QColor("#FF99FF"),QColor("#CC99FF"),QColor("#9999FF"),QColor("#99CCFF"),

        QColor("#99FFFF"),QColor("#99FFCC"),QColor("#99FF99"),QColor("#CCFF99"),QColor("#FFFF99"),QColor("#FFCC99"),QColor("#FF9999"),

        QColor("#C0C0C0"),QColor("#FF66B2"),QColor("#FF66FF"),QColor("#B266FF"),QColor("#6666FF"),QColor("#66B2FF"),

        QColor("#66FFFF"),QColor("#66FFB2"),QColor("#66FF66"),QColor("#B2FF66"),QColor("#FFFF66"),QColor("#FFB266"),QColor("#FF6666"),

        QColor("#A0A0A0"),QColor("#FF3399"),QColor("#FF33FF"),QColor("#9933FF"),QColor("#3333FF"),QColor("#3399FF"),

        QColor("#33FFFF"),QColor("#33FF99"),QColor("#33FF33"),QColor("#99FF33"),QColor("#FFFF33"),QColor("#FF9933"),QColor("#FF3333"),

        QColor("#808080"),QColor("#FF007F"),QColor("#FF00FF"),QColor("#7F00FF"),QColor("#0000FF"),QColor("#0080FF"),

        QColor("#00FFFF"),QColor("#00FF80"),QColor("#00FF00"),QColor("#80FF00"),QColor("#FFFF00"),QColor("#FF8000"),QColor("#FF0000"),

        QColor("#606060"),QColor("#CC0066"),QColor("#CC00CC"),QColor("#6600CC"),QColor("#0000CC"),QColor("#0066CC"),

        QColor("#00CCCC"),QColor("#00CC66"),QColor("#00CC00"),QColor("#66CC00"),QColor("#CCCC00"),QColor("#CC6600"),QColor("#CC0000"),

        QColor("#404040"),QColor("#99004C"),QColor("#990099"),QColor("#4C0099"),QColor("#000099"),QColor("#004C99"),

        QColor("#009999"),QColor("#00994C"),QColor("#009900"),QColor("#4C9900"),QColor("#999900"),QColor("#994C00"),QColor("#990000")
   
    };

    static QColor GetcAColorFromColorMap(const unsigned int & index)
    {
        unsigned int i = index % colorMap.size();
        return colorMap.at(i);
    }
   
}

#endif // __FITKCOLORMAP_HPP__
