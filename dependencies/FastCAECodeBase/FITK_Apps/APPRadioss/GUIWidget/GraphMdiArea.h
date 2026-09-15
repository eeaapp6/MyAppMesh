/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file GraphMdiArea.h
 * @brief 前后处理模型显示的mdi area区域
 * @author shenzhuowei (2931275006@qq.com)
 * @date 2024-12-19
 *
 */
#ifndef _GRAPH_MDI_AREA_H___
#define _GRAPH_MDI_AREA_H___
 
#include "GUIWidgetAPI.h"
#include "GraphArea.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"


namespace GUI
{ 
    /**
     * @brief 中心区域的MDI容器,继承自Comp::FITKMdiArea
     * @author shenzhuowei (2931275006@qq.com)
     * @date 2024-12-19
     */
    class GUIWIDGETAPI GraphMdiArea : public Comp::FITKMdiArea
    {
        Q_OBJECT;
    public:
        /**
         * @brief Create
         * @param parent父对象
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        explicit GraphMdiArea(QWidget* parent);
        /**
         * @brief Destroy
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        virtual ~GraphMdiArea() = default;

        /**
         * @brief 获取当前选中的显示窗口
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2024-12-19
         */
        template<typename T>
        T* getCurrentGraphWidget()
        { 
            return dynamic_cast<T*>(getCurrentWidget()); 
        }

        /**
         * @brief 获取第一个类型为t的窗口
         * @param[i] t 窗口类型
         * @return GraphArea 窗口指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-24
         */
        GraphArea* getFirstGraphArea(GUI::GraphAreaType t);
        /**
         * @brief    获取所有类型为t的窗口
         * @param[i] t 窗口类型
         * @return   QList<GraphArea*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        QList<GraphArea*> getAllGraphArea(GUI::GraphAreaType t);
        /**
         * @brief  获取指定id的窗口
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-26
         */
        GraphArea* getGraphAreaByID(int id);

    protected:
    private:
    };
}


#endif
