/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphMdiArea.h"


namespace GUI
{
    GraphMdiArea::GraphMdiArea(QWidget * parent)
        :Comp::FITKMdiArea(parent)
    {
        //todo
    }
    GraphArea* GraphMdiArea::getFirstGraphArea(GUI::GraphAreaType t)
    {
        //首先判断当前窗口
        GraphArea* area = this->getCurrentGraphWidget<GraphArea>();
        if (area && t == area->getGraphAreaType()) return area;
        //遍历全部窗口
        QList<QMdiSubWindow *> subWins = this->subWindowList();
        for (QMdiSubWindow* subW : subWins)
        {
            area = dynamic_cast<GraphArea*> (this->getWidget(subW));
            if (area && area->getGraphAreaType() == t)
                return area;
        }
        return nullptr;
    }

    QList<GraphArea*> GraphMdiArea::getAllGraphArea(GUI::GraphAreaType t)
    {
        QList<GraphArea*> areas = {};
        //遍历全部窗口
        QList<QMdiSubWindow *> subWins = this->subWindowList();
        for (QMdiSubWindow* subW : subWins)
        {
            GraphArea* area = dynamic_cast<GraphArea*> (this->getWidget(subW));
            if (area && area->getGraphAreaType() == t)
                areas.append(area);
        }
        return areas;
    }

    GraphArea* GraphMdiArea::getGraphAreaByID(int id)
    {
        GraphArea* area = dynamic_cast<GraphArea*> (this->getWidget(id));
        if (!area) return nullptr;
        return area;
    }

}
