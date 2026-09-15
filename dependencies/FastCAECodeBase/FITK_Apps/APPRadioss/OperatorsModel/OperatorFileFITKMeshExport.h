/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    OperatorFileFITKMeshExport.h
 * @brief   OperatorsModel组件导出*.fitkmesh文件操作器类，用于操作*.fitkmesh文件导出
 * @author  lilongyuan (lilongyuan@diso.cn)
 * @date    2026-02-25
 *
 */
#ifndef __OPERATOR_FILE_FITKMESH_EXPORT_H__
#define __OPERATOR_FILE_FITKMESH_EXPORT_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Core
{
    class FITKNode;
}

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKAbstractElement;
    class FITKComponentManager;
    class FITKAbstractModelComponent;
    class FITKModelSet;
    class FITKMeshSurface;
}

namespace ModelOper
{
    /**
     * @brief   OperatorsModel组件导出*.fitkmesh文件操作器类
     * @author  lilongyuan (lilongyuan@diso.cn)
     * @date    2026-02-25
     */
    class OperatorFileFITKMeshExport : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数（默认）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        explicit OperatorFileFITKMeshExport();
        /**
         * @brief   析构函数（默认）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        virtual ~OperatorFileFITKMeshExport();
    public:
        /**
         * @brief   执行界面逻辑（重写基类，不能继续重写）
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        virtual bool execGUI() final;
        /**
         * @brief   执行业务逻辑（重写基类，不能继续重写）
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        virtual bool execProfession() final;
    protected:
        /**
         * @brief   参数预处理（重写基类，不能继续重写）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        virtual void preArgs() final;
    private slots:
        /**
         * @brief   IO线程执行完毕信号槽
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        void ioThreadFinishedSlot();
    private:
        /**
         * @brief   打印错误信息
         * @param   message [in] 错误信息
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-25
         */
        bool ERROR(const QString& message);
        /**
         * @brief   复制节点数据
         * @param   mesh [in] 网格数据
         * @param   node [in] 原始节点数据
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-26
         */
        bool copyNode(Interface::FITKUnstructuredMesh* mesh, Core::FITKNode* node);
        /**
         * @brief   复制单元数据
         * @param   mesh [in] 网格数据
         * @param   element [in] 原始单元数据
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-26
         */
        bool copyElement(Interface::FITKUnstructuredMesh* mesh, Interface::FITKAbstractElement* element);
        /**
         * @brief   复制组件数据
         * @param   managerComponent [in] 组件管理器
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyComponent(Interface::FITKComponentManager* managerComponent, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
        /**
         * @brief   复制节点集合数据
         * @param   dataSet [in] 节点集合数据
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyNodeSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
        /**
         * @brief   复制单元集合数据
         * @param   dataSet [in] 单元集合数据
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyElementSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
        /**
         * @brief   复制复合集合数据
         * @param   dataSet [in] 复合集合数据
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyMixSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
        /**
         * @brief   复制节点表面数据
         * @param   dataSurface [in] 节点表面数据
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyNodeSurface(Interface::FITKMeshSurface* dataSurface, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
        /**
         * @brief   复制单元表面数据
         * @param   dataSurface [in] 单元表面数据
         * @param   listComponent [in] 组件列表
         * @param   idMesh [in] 非结构化网格ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool copyElementSurface(Interface::FITKMeshSurface* dataSurface, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh);
    private:
        /**
         * @brief   集合ID映射表
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        QHash<int, int> m_mapSetID = {};
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionExportFITKMesh, OperatorFileFITKMeshExport);
}
#endif  // __OPERATOR_FILE_FITKMESH_EXPORT_H__