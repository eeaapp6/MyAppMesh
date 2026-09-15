/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    OperatorFileFITKMeshImport.h
 * @brief   OperatorsModel组件导入*.fitkmesh文件操作器类，用于操作*.fitkmesh文件导入
 * @author  lilongyuan (lilongyuan@diso.cn)
 * @date    2026-02-04
 *
 */
#ifndef __OPERATOR_FILE_FITKMESH_IMPORT_H__
#define __OPERATOR_FILE_FITKMESH_IMPORT_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace IO { class FITKMeshIOInterface; }

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
    class FITKModelSet;
    class FITKMeshSurfaceNode;
    class FITKMeshSurfaceElement;
}

namespace Radioss
{
    class FITKRadiossMeshModel;
    class FITKRadiossNodes;
    class FITKRadiossPartManager;
}

namespace ModelOper
{
    /**
     * @brief   OperatorsModel组件导入*.fitkmesh文件操作器类
     * @author  lilongyuan (lilongyuan@diso.cn)
     * @date    2026-02-04
     */
    class OperatorFileFITKMeshImport : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数（默认）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        explicit OperatorFileFITKMeshImport();
        /**
         * @brief   析构函数（默认）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        virtual ~OperatorFileFITKMeshImport();
    public:
        /**
         * @brief   执行界面逻辑（重写基类，不能继续重写）
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        virtual bool execGUI() final;
        /**
         * @brief   执行业务逻辑（重写基类，不能继续重写）
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        virtual bool execProfession() final;
    protected:
        /**
         * @brief   参数预处理（重写基类，不能继续重写）
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        virtual void preArgs() final;
    private slots:
        /**
         * @brief   IO线程执行完毕信号槽
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-04
         */
        void ioThreadFinishedSlot();
    private:
        /**
         * @brief   组件数据转换全局数据
         * @param   dataMeshModel [in] Radioss网格模型数据
         * @param   api [in] FITKMeshIO组件接口
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convert(Radioss::FITKRadiossMeshModel* dataMeshModel, IO::FITKMeshIOInterface* api);
        /**
         * @brief   转换节点数据
         * @param   managerNode [in] 节点管理器
         * @param   mesh [in] 非结构化网格数据
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convertNode(Radioss::FITKRadiossNodes* managerNode, Interface::FITKUnstructuredMesh* mesh);
        /**
         * @brief   转换单元数据
         * @param   managerPart [in] 部件管理器
         * @param   mesh [in] 非结构化网格数据
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convertElement(Radioss::FITKRadiossPartManager* managerPart, Interface::FITKUnstructuredMesh* mesh);
        /**
         * @brief   转换节点集合数据
         * @param   managerComponent [in] 组件管理器
         * @param   dataSet [in] 节点集合数据
         * @param   idMeshModel [in] Radioss网格模型ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convertNodeSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel);
        /**
         * @brief   转换单元集合数据
         * @param   managerComponent [in] 组件管理器
         * @param   dataSet [in] 单元集合数据
         * @param   idMeshModel [in] Radioss网格模型ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convertElementSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel);
        /**
         * @brief   转换混合集合数据
         * @param   managerComponent [in] 组件管理器
         * @param   dataSet [in] 混合集合数据
         * @param   idMeshModel [in] Radioss网格模型ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool convertMixSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel);
        /**
         * @brief   转换节点表面数据
         * @param   managerComponent [in] 组件管理器
         * @param   dataSurface [in] 节点表面数据
         * @param   idMeshModel [in] Radioss网格模型ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-28
         */
        bool convertNodeSurface(Interface::FITKComponentManager* managerComponent, Interface::FITKMeshSurfaceNode* dataSurface, int idMeshModel);
        /**
         * @brief   转换单元表面数据
         * @param   managerComponent [in] 组件管理器
         * @param   dataSurface [in] 单元表面数据
         * @param   idMeshModel [in] Radioss网格模型ID
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-28
         */
        bool convertElementSurface(Interface::FITKComponentManager* managerComponent, Interface::FITKMeshSurfaceElement* dataSurface, int idMeshModel);
        /**
         * @brief   打印错误信息
         * @param   message [in] 错误信息
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-10
         */
        bool ERROR(const QString& message);
        /**
         * @brief   刷新界面
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        bool updateUI();
        /**
         * @brief   获取集合值映射表
         * @param   dataSet [in] 集合数据
         * @param   value [in] 值
         * @param   mapSetValue [in] 集合值映射表
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-28
         */
        bool getSetValueMap(Interface::FITKModelSet* dataSet, double value, QHash<int, double>& mapSetValue);
        /**
         * @brief   获取单元ID面索引映射表
         * @param   dataSet [in] 集合数据
         * @param   mapElementIDFaceIndex [in] 单元ID面索引映射表
         * @param   faceindex [in] 面索引
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-03-02
         */
        bool getElementIDFaceIndexMap(Interface::FITKModelSet* dataSet, QHash<int, int>& mapElementIDFaceIndex, int faceindex);
    private:
        /**
         * @brief   节点ID映射表
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        QHash<int, QHash<int, int>> m_mapNodeID = {};
        /**
         * @brief   单元ID映射表
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        QHash<int, QHash<int, int>> m_mapElemID = {};
        /**
         * @brief   集合ID映射表
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2026-02-27
         */
        QHash<int, int> m_mapSetID = {};
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionImportFITKMesh, OperatorFileFITKMeshImport);
}
#endif  // __OPERATOR_FILE_FITKMESH_IMPORT_H__