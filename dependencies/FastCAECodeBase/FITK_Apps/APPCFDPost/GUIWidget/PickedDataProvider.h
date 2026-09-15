/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _PickedDataProvider_H
#define _PickedDataProvider_H

#include <QObject>
#include "GUIWidgetAPI.h"

class vtkUnstructuredGrid;
class vtkDataSet;
class vtkActor;
class vtkActor2D;
class vtkPlanes;
class vtkExtractSelection;
class vtkSelectionNode;
class vtkIdTypeArray;

namespace GraphData
{
    class PickedData;

    class GUIWidgetAPI PickedDataProvider:public QObject
    {
        Q_OBJECT;
    public:
        static PickedDataProvider* getInstance();
        /**
         * @brief    清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        void clearAll();
        /**
         * @brief   清空拾取数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-18
         */
        void clearPickedData();
        /**
         * @brief   清空预选数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-30
         */
        void clearPickedDataPreview();
        /**
         * @brief   保存预选数据。
         * @param   actor：拾取演员
         * @param   index：拾取索引
         * @param   pickedWorldPos：拾取的世界坐标
         * @return  是否需要刷新
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-18
         */
        bool setPreviewPicked(vtkActor* actor, int index, double* pickedWorldPos);
        /**
         * @brief   添加单次点击拾取数据。
         * @param   actor：拾取演员
         * @param   index：拾取索引
         * @param   pickedWorldPos：拾取的世界坐标
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-18
         */
        void addPicked(vtkActor* actor, int index, double* pickedWorldPos);
        /**
         * @brief    获取预选高亮数据
         * @return   PickedData*    预选高亮数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        PickedData* getPickedPreview();
        /**
         * @brief    获取拾取数据
         * @return   PickedData*    拾取数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        PickedData* getPickDatas();
    signals:
        ;
        /**
         * @brief    拾取结束信号
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-21
         */
        void sigPickDataOver();
    private:
        PickedDataProvider();
        ~PickedDataProvider();
    private:
        static PickedDataProvider* _instance;
        /**
         * @brief  预选高亮数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-21
         */
        PickedData* _pickedPreviewData = nullptr;
        /**
         * @brief  拾取数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-22
         */
        PickedData* _pickedData = nullptr;
    };
}

#endif
