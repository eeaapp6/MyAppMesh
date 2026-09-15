/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    PickedData.h
 * @brief   拾取数据类。（并未三维可视化对象）
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-19
 *********************************************************************/

#ifndef __PICKEDDATA_H__
#define __PICKEDDATA_H__

#include <QObject>

#include "PickDataProviderAPI.h"

#include "GUIPickInfo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

#include <QHash>

 // 前置声明
class vtkActor;
class vtkActor2D;
class vtkPlanes;
class vtkUnstructuredGrid;

namespace Exchange
{
    class GraphVTKObject3D;
}

namespace GraphData
{
    /**
     * @brief  拾取数据参数结构体
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-05-14
     */
    struct PickDataInfo
    {
    public:
        PickDataInfo() = default;
        PickDataInfo(int id, QList<int> otherInfo = QList<int>()) {
            _id = id;
            _otherInfo = otherInfo;
        }

    public:
        //数据id
        int _id = -1;
        //其他参数
        QList<int> _otherInfo = {};
    };
    /**
     * @brief   进行拾取操作时的鼠标操作方式。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-19
     */
    enum PickedMouseType
    {
        PickedNoneType = -1,
        PickedMouseClick = 0,
        PickedMouseRubber
    };

    /**
     * @brief   拾取数据类。（并未三维可视化对象）
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-19
     */
    class PickDataProviderAPI PickedData : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数。（点击拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   pickedActor：被拾取的演员
         * @param   pickedIndex：拾取时的索引
         * @param   pickedWorldPos：拾取的世界坐标
         * @param   isPreview：是否为预选
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor* pickedActor, int pickedIndex, double* pickedWorldPos, bool isPreview = false);

        /**
         * @brief   构造函数。（框选拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   pickedActor：被拾取的演员
         * @param   cutPlane：拾取时拾取器裁切平面
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor* pickedActor, vtkPlanes* cutPlane);

        /**
         * @brief   构造函数。（点击拾取）
         * @param   pickedInfo：拾取时的类型、方法等相关信息
         * @param   pickedActor：被拾取的演员
         * @param   isPreview：是否为预选
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor2D* pickedActor, bool isPreview = false);
        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        ~PickedData();

        /**
         * @brief   拷贝数据。（此方法仅用于预选高亮转拾取高亮使用。）
         * @return  拷贝的数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedData* getCopy();
        /**
         * @brief   判断两份数据是否相同。
         * @param   data：待检测数据
         * @return  是否为相同模型
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool isSameAs(PickedData* data);

        /**
         * @brief   为ID列表数据进行排序。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void sort();

        /**
         * @brief   合并两份数据。
         * @param   data：待合并数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void add(PickedData* data);
        /**
         * @brief    保存拾取时键盘按下情况
         * @param[i] qKey              键盘按键值
         * @param[i] flag              是否按下
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        void saveKeyFlags(Qt::Key qKey, bool flag);
        /**
         * @brief   移除部分数据。
         * @param   data：待移除数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void subtract(PickedData* data);

        /**
         * @brief   判断该次拾取是否有效。
         * @return  是否有效
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool isValid();

        /**
         * @brief   获取当前数据是否为空。
         * @return  是否为空
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool isEmpty();
        /**
         * @brief    判断该次拾取是否结束
         * @return   bool                     是否结束
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        bool isFinished();
        /**
         * @brief   判断当前数据是否包含给定的演员与索引。
         * @param   actor：演员
         * @param   index：索引
         * @return  是否包含
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool contains(vtkActor* actor, int index);

        /**
         * @brief   判断当前数据是否包含给定的演员。
         * @param   actor：演员
         * @return  是否包含
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool contains(vtkActor2D* actor);

        /**
         * @brief   获取拾取类型等信息。
         * @return  拾取方式信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        GUI::GUIPickInfoStru getPickedInfo();

        /**
         * @brief   获取被拾取的演员。
         * @return  被拾取的演员
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkActor* getPickedActor();

        /**
         * @brief   获取被拾取的索引。
         * @return  被拾取索引
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        int getPickedIndex();

        /**
         * @brief   获取拾取时的鼠标拾取方式。
         * @return  鼠标拾取方式（点击或框选，如果该数据已被计算成实际的拾取信息则返回-1）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedMouseType getPickedMouseType();

        /**
         * @brief   获取拾取时的裁切屏幕。
         * @return  裁切平面信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkPlanes* getCutPlane();

        /**
         * @brief    通过当前拾取信息生成VTK数据存储至传入的网格数据集(通用)
         * @param[i] ugrid                     输入空网格并输出带有拾取信息的网格数据集
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-14
         */
        void getDataSet(vtkUnstructuredGrid* ugrid);

        /**
         * @brief   获取拾取到的世界坐标。
         * @param   pos：世界坐标
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void getPickedWorldPosition(double* pos);

        /**
         * @brief   高亮/预选高亮可视化对象。（内部根据拾取数据类型自动判断）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void highlight();

        /**
         * @brief   取消高亮。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void clearHighlight();

        /**
         * @brief   设置计算完成标识。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void calculateFinsish();

        /**
         * @brief   是否需要进行计算。
         * @return  是/否
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool needToCalculate();
        /**
         * @brief    获取拾取数据的类型
         * @return   GUI::GUIPickInfo::PickObjType           拾取数据的类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-25
         */
        GUI::GUIPickInfo::PickObjType getPickedDataType();
        /**
         * @brief    获取拾取数据的额外参数
         * @return   QList<PickDataInfo>       额外参数列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-14
         */
        QList<PickDataInfo> getPickOtherInfos();
        /**
         * @brief    添加拾取数据额外参数
         * @param[i] info                           额外参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-14
         */
        void addPickOtherInfo(PickDataInfo info);
        /**
         * @brief    获取拾取数据id
         * @return   int          id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-30
         */
        int getPickedDataObjId();
        /**
         * @brief    添加额外数据id
         * @param[i] pickId                 数据id
         * @param[i] pickOtherId            额外数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-02
         */
        void appendPickedOtherId(int pickId, int pickOtherId);
        /**
         * @brief    获取额外数据id
         * @return   QHash<int,int>          <数据id，额外数据id>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-02
         */
        QHash<int, int> getPickedOtherIDs();

        /**
         * @brief   获取拾取到的可视化对象。
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        Exchange::GraphVTKObject3D* getPickedGraphObejct();
        /**
         * @brief    提取拾取到的部分单元或节点信息
         * @param[i] data
         * @param[i] ugrid
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-20
         */
        void getPartMeshEleNodeSelection(vtkUnstructuredGrid* ugrid);
        /**
         * @brief    提取拾取到的部件表面单元与特征边数据
         * @param[i] data
         * @param[i] ugrid
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-20
         */
        void getPartMeshEleSurfSelection(vtkUnstructuredGrid* ugrid);
    public:
        /**
         * @brief  拾取到的表面字典。(键：单元ID，值：所有构成单元面的节点ID)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-10
         */
        QHash<int, QList<QList<int>>> FaceMap;
        /**
         * @brief  拾取到的表面字典。(键：单元ID，值：单元面vtk索引)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-10
         */
        QHash<int, QList<int>> FaceMapInternal;
        /**
         * @brief  拾取到的ID列表。（非VTK索引）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-10
         */
        QList<int> Ids;
        /**
         * @brief  拾取到的额外参数列表。(键：id，值：额外参数列表)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-15
         */
        QHash<int, int> m_otherIds = {};
    private:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedData();

        /**
         * @brief   初始化，获取拾取演员中存储的信息。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void init();

        /**
         * @brief   初始化2D，获取拾取演员中存储的信息。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void init2D();

        /**
         * @brief   初始化手动添加数据，判断数据类型并反向填充拾取数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void initManual();

        /**
         * @brief   内部方法，设置拾取到的可视化对象指针。
         * @param   obj：可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void setPickedGraphObject(Exchange::GraphVTKObject3D* obj);
        /**
         * @brief    通过id获取额外参数列表
         * @param[i] eleID                         id
         * @param[o] isOk                          是否成功
         * @return   QList<int>&                   额外参数列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-14
         */
        QList<int>& getOtherInfo(int eleID, bool& isOk);
        /**
         * @brief    移除对应id数据拾取数据
         * @param[i] eleID                      id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-14
         */
        void removeOtherInfo(int eleID);
    private slots:
        /**
         * @brief   清空可视化对象指针。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void slot_resetGraphObject();

    private:
        // 拾取时的信息。
        //@{
        /**
         * @brief   被拾取时的拾取类型。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        GUI::GUIPickInfoStru m_pickedInfo;

        /**
         * @brief   拾取时的演员。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkActor* m_pickedActor{ nullptr };

        /**
         * @brief   拾取时的2D演员。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkActor2D* m_pickedActor2D{ nullptr };

        /**
         * @brief   拾取时的索引。（VTK数据索引）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        int m_pickedIndex = -1;

        /**
         * @brief   拾取时的鼠标操作。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        PickedMouseType m_mouseOper = PickedMouseType::PickedNoneType;

        /**
         * @brief   拾取到的世界坐标。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        double m_pickedWorldPos[3]{ 0, 0, 0 };

        /**
         * @brief   框选拾取时的裁切平面。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkPlanes* m_cutPlane{ nullptr };

        /**
         * @brief   是否需要进行计算。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool m_needToCal = false;

        /**
         * @brief   是否有效。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool m_isValid = false;

        /**
         * @brief   是否为预选。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        bool m_isPreview = false;
        /**
         * @brief  拾取类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-25
         */
        GUI::GUIPickInfo::PickObjType m_type = GUI::GUIPickInfo::PickObjType::POBNone;

        /**
         * @brief   被拾取到的可视化对象。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        Exchange::GraphVTKObject3D* m_graphObject{ nullptr };
        /**
         * @brief  拾取到的其他参数列表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-14
         */
        QList<PickDataInfo> m_pickDataInfo = {};
        // 数据ID。
        //@{
        /**
         * @brief   数据对象ID。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        int m_dataObjId = -1;
        //@}
        /**
         * @brief  键盘按下情况字典。
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-02
         */
        QHash<Qt::Key, bool> m_keyFlagsHash = {};
    };
}

#endif // __PICKEDDATA_H__
