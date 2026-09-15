/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphPickedData.h
 * @brief   拾取数据类。（未在三维可视化对象管理）
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-04-17
 *********************************************************************/

#ifndef __GRAPHPICKEDDATA_H__
#define __GRAPHPICKEDDATA_H__

#include "PickDataProviderAPI.h"
#include "GUIPickInfo.h"

#include <QObject>
#include <QHash>
#include <QVariant>

#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCCommons.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

// 前置声明
namespace Exchange
{
    class FITKOCCGraphObject3D;
}

namespace Interface
{
    class FITKAbsVirtualTopo;
    class FITKAbsGeoShapeAgent;
}

namespace GraphData
{
    /**
     * @brief   拾取的数据类型。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-06
     */
    enum PickedDataType
    {
        OtherPick = -1,
        PointPick = 0,
        LinePick,
        FacePick,
        ShellPick,
        SolidPick
    };

    /**
     * @brief   进行拾取操作时的鼠标操作方式。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-06
     */
    enum PickedMouseType
    {
        PickedNoneType = -1,
        PickedMouseClick = 0,
        PickedMouseRubber,
        PickedMouseBox
    };

    /**
     * @brief   The data type.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-05
     */
    enum GraphDataType
    {
        OtherGraph = -1,
        ModelGraph = 0,
        MeshGraph,
        DatumPointGraph,
        DatumLineGraph,
        DatumPlaneGraph,
        SketchGraph,
        PickToolDirection,
        PickToolEdgePos
    };

    /**
     * @brief   Save the data information for graph object.
     *          ( This struct is mainly prepared for getting picked information,
     *          if the graph object is not a model, than the struct will only save
     *          the data object id. )
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-05
     */
    struct GraphInfo
    {
        // The type of the graph object.
        GraphDataType Type = OtherGraph;

        // The data object id.
        int DataObjId = -1;
    };

    /**
     * @brief   拾取数据管理类。（未在三维可视化对象管理）
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-05
     */
    class PICKDATAPROVIDERAPI GraphPickedData : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。（点击拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   agent     ：形状代理
         * @param   topoObject：虚拓扑对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Interface::FITKAbsGeoShapeAgent* agent, Interface::FITKAbsVirtualTopo* topoObject);

        /**
         * @brief   构造函数,基准面类型。（点击拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   DatumID：基准面ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GraphPickedData(GUI::GUIPickInfoStru pickedInfo, int DatumID);

        /**
         * @brief   构造函数。（点击拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   agent     ：形状代理
         * @param   topoObject：虚拓扑对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Interface::FITKAbsGeoShapeAgent* agent, QList<Interface::FITKAbsVirtualTopo*> topoObjects);

        /**
         * @brief   构造函数。（拾取信息方法专用）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   obj3d：存储的可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Exchange::FITKOCCGraphObject3D* obj3d);

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        ~GraphPickedData() = default;

        /**
         * @brief   获取当前数据是否为空。
         * @return  是否为空
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        bool isEmpty();

        /**
         * @brief   设置渲染对象信息。
         * @param   graphInfo ：渲染对象信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void setGraphInfo(GraphInfo graphInfo);

        /**
         * @brief   获取渲染对象信息。
         * @return  渲染对象信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        GraphInfo getGraphInfo();

        /**
         * @brief   获取拾取类型等信息。
         * @return  拾取方式信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GUI::GUIPickInfoStru getPickedInfo();

        /**
         * @brief   获取虚拓扑对象。
         * @return  拾取方式信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-16
         */
        Interface::FITKAbsVirtualTopo* getVirtualTopo();

        /**
         * @brief   获取所有虚拓扑对象。
         * @return  拾取方式信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-16
         */
        QList<Interface::FITKAbsVirtualTopo*> getVirtualTopoAll();

        /**
         * @brief   获取拾取到的全部虚拓扑索引。
         * @return  拾取到的虚拓扑索引列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        QList<int> getVirtualTopoIndice();

        /**
         * @brief   获取拾取到的全部虚拓扑信息。
         * @return  拾取到的虚拓扑索引列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        QList<Interface::VirtualShape> getVirtualShapes();

        /**
         * @brief   获取抽象几何形状代理器。
         * @return  抽象几何形状代理器
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-16
         */
        Interface::FITKAbsGeoShapeAgent* getGeoShapeAgent();

        int getDatumID();
        void setDatumID(int datumID);

        /**
         * @brief   保存拾取到的可视化对象。
         * @param   obj：可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void setGraphObject(Exchange::FITKOCCGraphObject3D* obj);

        /**
         * @brief   获取可视化对象中保存的输出信息。
         * @param   outputRole：输出端口[缺省]
         * @return  信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        QVariant getGraphOutputInfo(int outputRole = -1);

        /**
         * @brief   按类型获取可视化对象中保存的输出信息。
         * @param   outputRole：输出端口[缺省]
         * @return  信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        template<class T>
        T getGraphOutputInfoAs(int outputRole = -1)
        {
            QVariant var = getGraphOutputInfo(outputRole);
            return var.value<T>();
        }

        /**
         * @brief   追加虚拓扑。
         * @param   vTopo：虚拓扑对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        void appendVirtualTopo(Interface::FITKAbsVirtualTopo* vTopo);

    private:
        // 拾取时的信息。
        //@{
        /**
         * @brief   被拾取时的拾取类型。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GUI::GUIPickInfoStru _pickedInfo;

        /**
         * @brief   拾取时的虚拓扑对象,支持多选。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        QList<Interface::FITKAbsVirtualTopo*> _virtualTopo{};

        /**
         * @brief   渲染对象信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        GraphInfo m_graphInfo;

        /**
         * @brief    抽象几何形状代理器。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-16
         */
        Interface::FITKAbsGeoShapeAgent* _agent{};
        /**
         * @brief   基准面ID。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-30
         */
        int _datumID = -1;

        /**
         * @brief   拾取到的可视化对象。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        Exchange::FITKOCCGraphObject3D* _graphObj{ nullptr };

    };
}
#endif // __PICKEDDATAPROVIDER_H__