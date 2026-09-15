/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbsGeoCommand.h
 * @brief 几何命令创建基类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-12
 *
 */
#ifndef _FITKABS_GEO_COMMAND_H___
#define _FITKABS_GEO_COMMAND_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITKGeoEnum.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h" 
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKGeoCommandManager;

    /**
     * @brief  虚拓扑形状.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-22
     */
    class VirtualShape {
    public:

        VirtualShape() = default;
        VirtualShape(int cmdID, int virtualTopoId, int virtualTopoIndex)
        {
            CmdId = cmdID;
            VirtualTopoId = virtualTopoId;
            VirtualTopoIndex = virtualTopoIndex;
        }

        VirtualShape(int cmdID, int virtualTopoIndex)
        {
            CmdId = cmdID;
            VirtualTopoIndex = virtualTopoIndex;
        }

        /**
         * @brief  命令CommandID.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        int CmdId{ -1 };
        /**
         * @brief  虚拓扑ID.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        int VirtualTopoId{ -1 };
        /**
         * @brief  虚拓扑索引.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-23
         */
        int VirtualTopoIndex{ -1 };

        /**
         * @brief  虚拓扑类型.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-23
         */
        FITKGeoEnum::VTopoShapeType Type{ FITKGeoEnum::VTopoShapeType::VSNone };
        /**
         * @brief  是否为空形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        bool isNull() const { return CmdId <= 0 && VirtualTopoId <= 0 && VirtualTopoIndex < 0; }
        /**
          * @brief  重置形状，置空(isNull() == true).
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-22
          */
        void reset() { CmdId = -1; VirtualTopoId = -1; VirtualTopoIndex = -1; }
        /**
          * @brief  重载运算符.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-22
          */
        bool operator==(const VirtualShape& other) {
            return (CmdId == other.CmdId) && (VirtualTopoId == other.VirtualTopoId) && (VirtualTopoIndex == other.VirtualTopoIndex) && (Type == other.Type);
        }
        friend bool operator==(const VirtualShape& shape1, const VirtualShape& shape2);
    };

    inline bool operator==(const VirtualShape& shape1, const VirtualShape& shape2) {
        return (shape1.CmdId == shape2.CmdId) && (shape1.VirtualTopoId == shape2.VirtualTopoId) && (shape1.VirtualTopoIndex == shape2.VirtualTopoIndex) && (shape1.Type == shape2.Type);
    }

    class FITKGeoCommandProp;

    /**
     * @brief 几何命令抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-12
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoCommand : public Core::FITKAbstractNDataObject,
        public Core::FITKVarientParams
    {
        friend class FITKGeoCommandManager;
        FITKCLASS(Interface, FITKAbsGeoCommand);
        Q_OBJECT
            
    public:
        /**
         * @brief Construct a new FITKAbsGeoCommand object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        explicit FITKAbsGeoCommand();
        /**
         * @brief Destroy the FITKAbsGeoCommand object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        virtual ~FITKAbsGeoCommand() = 0;
        /**
        * @brief 获取几何命令类型
        * @return FITKGeoEnum::FITKGeometryComType
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() = 0;
        /**
         * @brief 获取几何属性
         * @return FITKGeoCommandProp* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-08
         */
        FITKGeoCommandProp* getCommandProp() { return m_prop; };

        /**
         * @brief 更新模型
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        virtual bool update() = 0;

        /**
         * @brief       清除命令内的模型数据。（非必须）[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-22
         */
        virtual void clearShape();

        /**
         * @brief       执行撤销逻辑。（非必须）[虚函数]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool undo();

        /**
         * @brief       执行重做逻辑。（非必须）[虚函数]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool redo();
        /**
         * @brief 三角化形状
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-11
         */
        void triangleShanpe();

        /**
         * @brief       设置命令状态。
         * @param[in]   status：状态枚举
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void setCommandStatus(FITKGeoEnum::FITKGeoStatus status);

        /**
         * @brief       获取命令状态。
         * @return      状态枚举
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoEnum::FITKGeoStatus getCommandStatus();

        /**
          * @brief 获取形状代理器
          * @return FITKAbsGeoShapeAgent
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-07-30
          */
        FITKAbsGeoShapeAgent* getShapeAgent();

        /**
         * @brief       获取指定类型形状虚拓扑管理器。
         * @return      type：虚拓扑管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-22
         */
        FITKShapeVirtualTopoManager* getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType type);
        /**
         * @brief  获取虚拓扑
         * @param  type 拓扑类型
         * @param  id 虚拓扑ID
         * @return 返回虚拓扑
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-16
         */
        FITKAbsVirtualTopo* getVirtualTopo(Interface::FITKGeoEnum::VTopoShapeType type, int id) {
            auto shapeAgent = getShapeAgent();
            if (shapeAgent == nullptr) return nullptr;

            auto manager = shapeAgent->getVirtualTopoManager();
            if (manager == nullptr) return nullptr;
            auto shapeManager = manager->getShapeVirtualTopoManager(type);
            if (shapeManager == nullptr) return nullptr;
            return shapeManager->getDataByID(id);
        }
        /**
         * @brief  获取shape.
         * @param  type 拓扑类型
         * @param  id 虚拓扑ID
         * @return shape
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-20
         */
        template<typename T>
        T* getShapeT(Interface::FITKGeoEnum::VTopoShapeType type, int id) {
            auto shapeAgent = getShapeAgent();
            if (shapeAgent == nullptr) return nullptr;

            auto manager = shapeAgent->getVirtualTopoManager();
            if (manager == nullptr) return nullptr;
            auto shapeManager = manager->getShapeVirtualTopoManager(type);
            if (shapeManager == nullptr) return nullptr;
            auto virShape = shapeManager->getDataByID(id);
            if (virShape == nullptr) return nullptr;
            return virShape->getShapeT<T>();
        }

        /**
         * @brief       通过虚拓扑索引与类型获取虚拓扑形状。
         * @param[in]   type：形状类型
         * @param[in]   index：虚拓扑索引
         * @return      虚拓扑形状对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        template<typename T>
        T* getShapeTopoByIndexT(Interface::FITKGeoEnum::VTopoShapeType type, int index) 
        {
            auto shapeAgent = getShapeAgent();
            if (shapeAgent == nullptr) return nullptr;

            auto manager = shapeAgent->getVirtualTopoManager();
            if (manager == nullptr) return nullptr;
            auto shapeManager = manager->getShapeVirtualTopoManager(type);
            if (shapeManager == nullptr) return nullptr;
            auto virShape = shapeManager->getDataByIndex(index);
            if (virShape == nullptr) return nullptr;
            return virShape->getShapeT<T>();
        }

        /**
         * @brief  获取shape.
         * @param  id 虚拓扑ID
         * @return shape
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-20
         */
        template<typename T>
        T* getShapeT(int id) {
            auto shapeAgent = getShapeAgent();
            if (shapeAgent == nullptr) return nullptr;

            auto manager = shapeAgent->getVirtualTopoManager();
            if (manager == nullptr) return nullptr;

            for (int type = static_cast<int>(FITKGeoEnum::VTopoShapeType::VSPoint); type < static_cast<int>(FITKGeoEnum::VTopoShapeType::VSAssembly); ++type) {
                auto shapeManager = manager->getShapeVirtualTopoManager(static_cast<FITKGeoEnum::VTopoShapeType>(type));
                if (shapeManager == nullptr) continue;
                auto virShape = shapeManager->getDataByID(id);
                if (virShape == nullptr) continue;
                return virShape->getShapeT<T>();
            }

            return nullptr;
        }

        /**
         * @brief       获取虚拓扑管理器。
         * @return      ShapeAgent中的虚拓扑管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKVirtualTopoManager* getVirtualTopoManager();

        /**
         * @brief       根据形状类型与索引获取虚拓扑对象。
         * @param[in]   type：形状类型
         * @param[in]   index：形状索引
         * @return      虚拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        FITKAbsVirtualTopo* getVirtualTopoByIndex(FITKGeoEnum::VTopoShapeType type, int index);

        /**
         * @brief       根据形状类型与索引列表获取虚拓扑对象。
         * @param[in]   type：形状类型
         * @param[in]   indice：形状索引列表
         * @return      虚拓扑对象列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        QList<FITKAbsVirtualTopo*> getVirtualToposByIndice(FITKGeoEnum::VTopoShapeType type, QList<int> indice);

        template<class T>
        /**
          * @brief 获取形状代理器
          * @return FITKAbsGeoShapeAgent
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-07-30
          */
        T* getTShapeAgent()
        {
            return dynamic_cast<T*>(_shapeAgent);
        }

        /**
         * @brief  打印信息到控制台.
         * @param  type 信息类型 1-normal 2-warning 3-error 4-info
         * @param  msg 信息内容
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        void printLog(QString msg, int type = 1)
        {
            AppFrame::FITKSignalTransfer* sigTrans = FITKAPP->getSignalTransfer();
            if (sigTrans)
            {
                sigTrans->outputMessageSig(type, msg);
            }
        }

        /**
         * @brief       开启或关闭引用数据计数。
         * @param[in]   flag：开启或关闭。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void enableReferenceList(bool flag);

        /**
         * @brief       过去引用数据计数状态。
         * @return      开启或关闭。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        bool getEnableReferenceList();

        /**
         * @brief       添加引用的命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void addReferenceCmdID(int id);

        /**
         * @brief       添加引用的命令对象。
         * @param[in]   cmd：命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void addReferenceCmdObj(FITKAbsGeoCommand* cmd, bool isSet = false);

        /**
         * @brief       移除引用的命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void removeReferenceCmdID(int id);

        /**
         * @brief       移除引用的命令对象。
         * @param[in]   cmd：命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void removeReferenceCmdObj(FITKAbsGeoCommand* cmd);

        /**
         * @brief       清空引用命令。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        virtual void clearReference();

        /**
         * @brief       获取当前命令所引用的命令数量。
         * @return      引用命令列表长度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        int getReferenceCmdCount();

        /**
         * @brief       根据索引获取当前命令引用的命令。
         * @param[in]   index：命令索引
         * @return      引用命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        FITKAbsGeoCommand* getReferenceCmdByIndex(int index);

        /**
         * @brief       获取引用当前命令的命令数量。
         * @return      被引用命令列表长度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        int getReferencedCmdCount();

        /**
         * @brief       根据索引获取引用当前命令的命令。
         * @param[in]   index：命令索引
         * @return      引用命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        FITKAbsGeoCommand* getReferencedCmdByIndex(int index);

        /**
         * @brief       添加被引用的命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        void addReferencedCmdID(int id);

        /**
         * @brief       添加被引用的命令对象。
         * @param[in]   cmd：命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        void addReferencedCmdObj(FITKAbsGeoCommand* cmd);

        /**
         * @brief       移除被引用的命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        void removeReferencedCmdID(int id);

        /**
         * @brief       移除被引用的命令对象。
         * @param[in]   cmd：命令对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        void removeReferencedCmdObj(FITKAbsGeoCommand* cmd);

        /**
         * @brief       获取数据的界面有效性：指是否为当前实际需要显示在模型树的数据。
         * @return      是否需要显示在模型树
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        bool getDataValidInGUI();

        /**
         * @brief       保存编辑的命令数据ID。（需要在刷新命令并放入管理器前执行）
         * @param[in]   id：编辑的命令数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        void setModifiedCmdID(int id);

        /**
         * @brief       保存编辑的命令数据对象。（需要在刷新命令并放入管理器前执行）
         * @param[in]   cmd：编辑的命令数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-14
         */
        void setModifiedCmdObj(FITKAbsGeoCommand* cmd);

        /**
         * @brief       获取编辑的命令数据ID。
         * @return      编辑的命令数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        int getModifiedCmdID();

        /**
         * @brief       获取编辑的命令数据对象。
         * @return      编辑的命令数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-14
         */
        FITKAbsGeoCommand* getModifiedCmdObj();

        /**
         * @brief       是否为编辑类型命令。
         * @param       是否为编辑类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        bool hasModifiedCommand();

        /**
         * @brief       向下获取引用命令。（被引用方向查询）
         * @return      引用命令列表（包含自己）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-24
         */
        QList<FITKAbsGeoCommand*> getDownStreamCommands();

        /**
         * @brief       向上获取引用命令。（引用方向查询）
         * @return      引用命令列表（包含自己）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-24
         */
        QList<FITKAbsGeoCommand*> getUpStreamCommands();

        /**
         * @brief       获取由本命令创建出的所有新命令ID列表。[虚函数]
         * @return      ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        virtual QList<int> getCreatedCommandIds();

        /**
         * @brief       从部件中移除后的处理逻辑。[虚函数]
         * @param[in]   几何部件命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeFromPart(FITKAbsGeoCommand* part);

        /**
         * @brief       获取该命令所在的几何命令管理器。
         * @return      几何命令管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        FITKGeoCommandManager* getGeoCommandManager();

        /**
         * @brief       获取该命令最初的命令。
         *              （等于自身则未发生变化，不等于自身且不为空则是其修改的命令。）
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        FITKAbsGeoCommand* getSrcCommand();

        /**
         * @brief       获取经过编辑或删除后的最终命令。
         *              （为空则为被删除命令，等于自身则未发生变化，不等于自身且不为空则是被修改后的命令。）
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        FITKAbsGeoCommand* getFinalCommand();

    protected:
        /**
         * @brief       内部方法 - 递归向上或向下查找引用命令。
         * @param[in]   cmd：源命令
         * @param[out]  cmdList：命令列表[引用]
         * @param[in]   downStream：向下递归
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-24
         */
        void getRefCmdRecursively(FITKAbsGeoCommand* cmd, QList<FITKAbsGeoCommand*> & cmdList, bool downStream);

        /**
         * @brief       设置该命令所在的几何命令管理器。
         * @param[in]   mgr：几何命令管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        void setGeoCommandManager(FITKGeoCommandManager* mgr);

    protected:
        /**
          * @brief 几何形状代理器
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-07-30
          */
        FITKAbsGeoShapeAgent* _shapeAgent{};

        /**
         * @brief       命令状态。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoEnum::FITKGeoStatus m_status = FITKGeoEnum::FGSNormal;

        /**
         * @brief       引用的命令列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        FITKGeoCommandManager* m_referenceCmdList{ nullptr };

        /**
         * @brief       被引用的命令列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-23
         */
        FITKGeoCommandManager* m_referencedCmdList{ nullptr };

        /**
         * @brief       编辑的命令数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        int m_modifiedCmdId = -1;

        /**
         * @brief       是否启用引用计数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        bool m_enableRefer = true;

        /**
         * @brief  几何命令属性
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-08
         */
        FITKGeoCommandProp* m_prop{ nullptr }; 

        /**
         * @brief       该几何命令所在的几何命令管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        FITKGeoCommandManager* m_cmdMgr{ nullptr };

    };

    /**
     * @brief       几何命令容器。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-06
     */
    class FITKInterfaceGeometryAPI FITKGeoCommandManager : public Core::FITKAbstractDataManager<FITKAbsGeoCommand>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        explicit FITKGeoCommandManager() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual ~FITKGeoCommandManager() = default;

        /**
         * @brief       获取是否可以执行撤销操作。[虚函数]
         * @return      是否可以撤销
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool canUndo();

        /**
         * @brief       获取是否可以执行重做操作。[虚函数]
         * @return      是否可以重做
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool canRedo();

        /**
         * @brief       撤销。[虚函数]
         * @return      是否撤销成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool undo();

        /**
         * @brief       重做。[虚函数]
         * @return      是否重做成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool redo();

        /**
         * @brief       获取根节点命令（未被引用的几何命令）。[虚函数]
         * @return      命令列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-15
         */
        virtual QList<FITKAbsGeoCommand*> getRootCommands();

        /**
         * @brief       获取指定类型的命令。[虚函数]
         * @param[in]   type：子命令类型
         * @return      命令列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-22
         */
        virtual QList<FITKAbsGeoCommand*> getCommandsByType(FITKGeoEnum::FITKGeometryComType type);

        /**
         * @brief       尝试刷新数据并添加到子命令列表。（刷新失败不会添加到列表且不自动析构）[虚函数]
         * @param[in]   subCmd：子命令对象
         * @param[in]   deleteFailed：刷新失败是否删除该命令[缺省]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        virtual bool addAndUpdate(Interface::FITKAbsGeoCommand* subCmd, bool deleteFailed = true);

        /**
         * @brief       递归查询数据。[虚函数][重写]
         * @param[in]   id：数据对象ID
         * @return      查询到的对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-03
         */
        virtual FITKAbsGeoCommand* getDataByID(const int id) override
        {
            // 尝试获取数据对象。
            Core::FITKAbstractDataObject* dataObj = Core::FITKAbstractDataManager<FITKAbsGeoCommand>::getDataByID(id);
            if (dataObj)
            {
                return dynamic_cast<FITKAbsGeoCommand*>(dataObj);
            }

            // 否则遍历所有管理器查询子命令。
            for (int i = 0; i < this->getDataCount(); i++)
            {
                FITKGeoCommandManager* dataMgr = dynamic_cast<FITKGeoCommandManager*>
                    (Core::FITKAbstractDataManager<FITKAbsGeoCommand>::getDataByIndex(i));
                if (!dataMgr)
                {
                    continue;
                }

                Core::FITKAbstractDataObject* subDataObj = dataMgr->getDataByID(id);
                if (subDataObj)
                {
                    return dynamic_cast<FITKAbsGeoCommand*>(subDataObj);
                }
            }

            return nullptr;
        }

        /**
         * @brief       追加对象。[虚函数][重写]
         * @param[in]   item：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void appendDataObj(Core::FITKAbstractDataObject* item) override;

        /**
         * @brief       追加一组对象。[虚函数][重写]
         * @param[in]   items：对象列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void appendDataObj(const QList<Core::FITKAbstractDataObject*> & items) override;

        /**
         * @brief       插入数据。[虚函数][重写]
         * @param[in]   index：索引
         * @param[in]   item：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void insertDataObj(int index, Core::FITKAbstractDataObject* item) override;

        /**
         * @brief       移除对象。[虚函数][重写]
         * @param[in]   obj：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void removeDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief       通过索引移除对象。[虚函数][重写]
         * @param[in]   index：索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void removeDataByIndex(int index) override;

        /**
         * @brief       通过ID移除对象。[虚函数][重写]
         * @param[in]   id：数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void removeDataByID(int id) override;

        /**
         * @brief       通过名称移除对象。[虚函数][重写]
         * @param[in]   name：名称
         * @param[in]   compSens：大小写敏感
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void removeDataByName(const QString & name, bool compSens = false) override;

        /**
         * @brief       全部重新生成几何数据。[虚函数]
         * @param[in]   fromDataId：从指定ID的命令开始更新，为-1则从第一条命令开始更新[缺省]
         * @return      执行中断命令ID，-1则表示重构模型正常退出
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-14
         */
        virtual int reGenerate(int fromDataId = -1);

    protected:
        /**
         * @brief       将管理器对象存储至命令内。[虚函数]
         * @param[in]   cmd：几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual void saveManager(FITKAbsGeoCommand* cmd);

    };
}

// 注册元对象类型。
Q_DECLARE_METATYPE(Interface::VirtualShape);

#endif