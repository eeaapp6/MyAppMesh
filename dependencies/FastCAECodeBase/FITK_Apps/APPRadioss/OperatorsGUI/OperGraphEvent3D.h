/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperGraphEvent3D.h
 * @brief  三维可视化事件操作器抽象类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-05-22
 *********************************************************************/

#ifndef _OperGraphEvent3D_H_
#define _OperGraphEvent3D_H_

#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GraphArea.h"

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Exchange {
    class GraphVTKObject3D;
}

namespace GUIOper
{
    /**
     * @brief  可视化操作器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-05-22
     */
    class OperGraphEvent3D : public EventOper::GraphEventOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief  刷新
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void render() override;
        /**
         * @brief  自适应
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void fitView() override;
    protected:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        explicit OperGraphEvent3D() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        virtual ~OperGraphEvent3D() = default;
        /**
         * @brief  获取三维可视化窗口
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        GUI::GraphArea* getGraphWidget(GUI::GraphAreaType type);
        /**
         * @brief    创建三维可视化窗口
         * @param[i] type 
         * @return   GUI::GraphArea *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        GUI::GraphArea* createGraphWidget(GUI::GraphAreaType type);
        /**
         * @brief    获取所有类型为type的三维可视化窗口
         * @param[i] type 
         * @return   QList<GUI::GraphArea*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        QList<GUI::GraphArea*> getAllGraphWidget(GUI::GraphAreaType type);
        /**
         * @brief       获取所有三维窗口。
         * @return      三维窗口列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-05-16
         */
        QList<Comp::FITKGraph3DWindowVTK*> getGraphWindows();
        /**
         * @brief  添加可视化对象至三维窗口
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void addGraphObjectToWidget(Exchange::GraphVTKObject3D* obj, GUI::GraphArea* graphWidget, bool fitView = false);
        /**
         * @brief    添加可视化对象至三维窗口
         * @param[i] obj                        可视化对象
         * @param[i] graphWidget                三维窗口
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        void addGraphHightlightObjectToWidget(Exchange::GraphVTKObject3D* obj, GUI::GraphArea* graphWidget);
        /**
         * @brief  通过数据对象id获取可视化对象（没有则创建）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QList<Exchange::GraphVTKObject3D*> getGraphObjectsByDataId(int dataObjId, QVariant otherInfo = QVariant());
        /**
         * @brief    通过数据id获取部件几何渲染对象
         * @param[i] dataObjId                                   数据id
         * @return   QList<Exchange::GraphVTKObject3D*>          可视化对象列表
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-18
         */
        QList<Exchange::GraphVTKObject3D*> getPartGeoGraphObjectsByDataId(int dataObjId);
        /**
         * @brief    获取所有可视化对象。（没有不会创建）
         * @return   QList<Exchange::GraphVTKObject3D*>              可视化链表
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        QList<Exchange::GraphVTKObject3D*> getAllGraphObject();
        /**
         * @brief    获取全部几何可视化对象。（没有不会创建）
         * @return   QList<Exchange::GraphVTKObject3D*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        QList<Exchange::GraphVTKObject3D*> getAllGeoGraphObject();
        /**
         * @brief    获取全部网格可视化对象。（没有不会创建）
         * @return   QList<Exchange::GraphVTKObject3D*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        QList<Exchange::GraphVTKObject3D*> getAllMeshGraphObject();
        /**
         * @brief    根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建）
         * @param[i] dataObjId                                 数据对象ID
         * @param[i] refLayer                                  更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param[i] recDown                                   向下递归（被引用方向查询）[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true) override;
    };

}


#endif



