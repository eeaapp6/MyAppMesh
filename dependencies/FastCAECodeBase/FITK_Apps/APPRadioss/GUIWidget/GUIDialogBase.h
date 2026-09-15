/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIDialogBase.h
 * @brief  对话框基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-07-10
 *********************************************************************/
#ifndef _GUIDialogBase_H
#define _GUIDialogBase_H

#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace GUI
{
    /**
     * @brief  对话框基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-07-10
     */
    class GUIWIDGETAPI GUIDialogBase : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent          父对象
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-10
         */
        GUIDialogBase(Core::FITKAbstractDataObject* data, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-10
         */
        virtual ~GUIDialogBase();
        
    protected slots:
        /**
         * @brief    对话框关闭事件重写
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-22
         */
        virtual void reject() override;
        /**
         * @brief    对话框确定事件重写
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-22
         */
        virtual void accept() override;

    protected:
        /**
         * @brief    对话框高亮数据接口
         * @param[i] objID              数据对象ID
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-16
         */
        void hightGraphObj(const int& objID);
        /**
         * @brief    对话框取消高亮数据接口
         * @param[i] objID              数据对象ID
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-16
         */
        void disHightGraphObj(const int& objID);
        /**
         * @brief    对话框高亮几何虚拓扑数据接口
         * @param[i] objID              数据对象ID
         * @param[i] topoID             几何虚拓扑ID
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-16
         */
        void highlightPartGeomTopo(const int& objID, const int& topoID);
        /**
         * @brief    对话框高亮网格数据接口
         * @param[i] objID              数据对象ID
         * @param[i] elemID             网格单元ID或节点ID
         * @param[i] type               单元或节点类型（0:节点 1:单元）
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-16
         */
        void highlightMeshData(const int& objID, const int& elemID, const int& type);
        /**
         * @brief    取消全部高亮接口
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-16
         */
        void clearAllHightGraph();

    protected:
        /**
         * @brief  数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-22
         */
        Core::FITKAbstractDataObject* _data{};
        /**
         * @brief  是否创建
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-22
         */
        bool _isCreate{ false };
    };
}

#endif
