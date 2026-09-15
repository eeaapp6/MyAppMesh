/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    PickedDataProvider.h
 * @brief   拾取数据管理类。（并未三维可视化对象管理）
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-06
 *********************************************************************/

#ifndef __GRAPHPICKEDDATAPROVIDER_H__
#define __GRAPHPICKEDDATAPROVIDER_H__

#include <QObject>

#include "PickDataProviderAPI.h"

#include <QHash>

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

 // Interactive
class AIS_InteractiveContext;

namespace AppFrame
{
    class FITKKeyMouseStates;
}

namespace Render
{
    class FITKGraph3DWindowOCC;
}

namespace GraphData
{
    class GraphPickedData;


    /**
     * @brief   拾取数据管理类。（未在三维可视化对象管理）
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-06
     */
    class PICKDATAPROVIDERAPI GraphPickedDataProvider : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief   获取单例。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        static GraphPickedDataProvider* getInstance();

        /**
         * @brief   手动销毁。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        static void Delete();

        /**
         * @brief   清空拾取数据。
         * @param   clearSelecttion：是否清除窗口拾取[缺省]
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        void clearPickedData(bool clearSelecttion = true);

        /**
         * @brief   返回拾取状态。
         * @return  拾取状态
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        bool isPickStatus();

        /**
         * @brief   初始化。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        void init();

        /**
         * @brief   获取拾取信息列表。
         * @return  拾取信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        QList<GraphPickedData*> getPickedList();

        /**
         * @brief   移除选中给定虚拓扑形状。
         * @param   cmdId：命令ID
         * @param   sType：形状类型
         * @param   vTopoIndex：虚拓扑索引
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-30
         */
        void removeSelected(int cmdId, Interface::FITKGeoEnum::VTopoShapeType sType, int vTopoIndex);

    signals:
        /**
         * @brief   信号 - 有数据被拾取到。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        void sig_dataPicked();

        /**
         * @brief   信号 - 拾取工具数据发生变化。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-18
         */
        void sig_directionPicked();

        /**
         * @brief   信号 - 拾取投影点数据发生变化。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-18
         */
        void sig_projPosPicked();

    protected:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        GraphPickedDataProvider();

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        ~GraphPickedDataProvider();


    private slots:
        /**
         * @brief   更新拾取数据。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        void updatePickData();

    private:
        /**
         * @brief   更新拾取形状。（模型、草图、基准元素等）
         * @return  是否刷新成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        bool updatePickShapes();

        /**
         * @brief   更新拾取信息。（边方向、面方向、边投影坐标等）
         * @return  是否刷新成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        bool updatePickInfo();

        /**
         * @brief   根据命令ID获取拾取数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        GraphPickedData* getPickedDataByCmdId(int cmdId);

    private:
        /**
         * @brief   实例。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        static GraphPickedDataProvider* s_instance;

        /**
         * @brief   窗口对象。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        Render::FITKGraph3DWindowOCC* m_graphWidget{ nullptr };

        /**
         * @brief   系统设置单例。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        AppFrame::FITKKeyMouseStates* m_settings{ nullptr };

        /**
         * @brief   拾取状态。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        bool m_isPick;

        /**
         * @brief   所有拾取信息。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        QList<GraphPickedData*> m_pickedDataList;

        // For OCC widget.
        //@{
        /**
         * @brief   The interactive context.
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-06
         */
        AIS_InteractiveContext* m_context{ nullptr };
    };
}
#endif // __PICKEDDATAPROVIDER_H__