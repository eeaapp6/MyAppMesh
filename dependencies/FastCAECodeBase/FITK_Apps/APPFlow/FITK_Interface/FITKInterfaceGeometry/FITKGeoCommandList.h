/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGeoCommandList.h
 * @brief 几何命令列表（撤销列表）
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-30
 *
 */
#ifndef _FITK_GEO_COMMANDLIST__H___
#define _FITK_GEO_COMMANDLIST__H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKAbsGeoSketch2D.h"
#include "FITKGlobalGeoComponent.h"

namespace Interface
{
    /**
     * @brief       撤销重做ID列表结构体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-12-04
     */
    struct UndoRedoIds
    {
    public:
        UndoRedoIds() {}
        UndoRedoIds(QList<int> ids) : CmdMgrIds(ids)
        {

        }

        /**
         * @brief       需要执行撤销或重做的命令管理器ID列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        QList<int> CmdMgrIds;

        /**
         * @brief       运算符重载。
         * @param[in]   other：另一个对象[引用]
         * @return      是否相等
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        bool operator==(const UndoRedoIds & other) const
        {
            return CmdMgrIds == other.CmdMgrIds;
        }
    };

    /**
     * @brief 几何命令列表（撤销列表）
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-30
     */
    class FITKInterfaceGeometryAPI FITKGeoCommandList : public Core::FITKAbstractDataObject, public FITKGeoCommandManager
    {
        FITKCLASS(Interface, FITKGeoCommandList);
    public:
        /**
         * @brief Construct a new FITKGeoCommandList object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-30
         */
        explicit FITKGeoCommandList();
        /**
         * @brief Destroy the FITKGeoCommandList object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-30
         */
        virtual ~FITKGeoCommandList();

        /**
         * @brief       数据名称检查。[虚函数][重写]
         * @param[in]   被检测的名称[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual QString checkName(const QString & name) override;

        /**
         * @brief       获取基准元素管理器。
         * @return      基准元素类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKDatumList* getDatumManager();

        /**
         * @brief       设置当前激活草图数据ID。
         * @param[in]   sketchDataId：草图数据ID（-1或ID为非草图数据则清除当前激活草图）[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        void setCurrentSketchID(int sketchDataId = -1);

        /**
         * @brief       获取当前激活中的草图数据。
         * @return      草图对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKAbsGeoSketch2D* getCurrentSketchData();

        /**
         * @brief       获取全局几何分组管理器。
         * @return      全局几何分组管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        FITKGlobalGeoComponentManager* getGlobalGeoCompManager();

        /**
         * @brief       获取是否可以执行撤销操作。[虚函数][重写]
         * @return      是否可以撤销
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool canUndo() override;

        /**
         * @brief       获取是否可以执行重做操作。[虚函数][重写]
         * @return      是否可以重做
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool canRedo() override;

        /**
         * @brief       撤销。[虚函数][重写]
         * @return      是否撤销成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool undo() override;

        /**
         * @brief       重做。[虚函数][重写]
         * @return      是否重做成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool redo() override;

        /**
         * @brief       重写添加数据对象功能。（添加命令并清空RedoList）[虚函数][重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual void appendDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
        * @brief 获取全部模型包围盒
        * @param[o] minPt 最小点位置
        * @param[o] maxPt 最大点位置
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-30
        */
        void getBoundaryBox(double* minPt, double* maxPt);

        /**
         * @brief       获取撤销列表。
         * @return      撤销列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoCommandManager* getUndoList();

        /**
         * @brief       获取重做列表。
         * @return      重做列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoCommandManager* getRedoList();

        ///**
        // * @brief       从用户定义撤销列表中移除最后一条记录并返回。
        // * @return      用户定义撤销列表最后一条撤销ID列表
        // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date        2025-12-04
        // */
        //UndoRedoIds takeLastUndoIds();

        ///**
        // * @brief       从用户定义重做列表中移除最后一条记录并返回。
        // * @return      用户定义重做列表最后一条重做ID列表
        // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date        2025-12-04
        // */
        //UndoRedoIds takeLastRedoIds();

        /**
         * @brief       追加用户定义的撤销ID列表。
         * 
         *              （注意，此处应传入被撤销的管理器命令ID，而不是需要
         *              被撤销的ID，比如：
         * 
         *              情况1：一次性在多个部件内产生新命令时，此处需要将所
         *              有产生新子命令的部件ID传入
         *              情况2：一次性在单个部件内产生多条命令时，此处需要多
         *              次传入产生命令的部件ID，次数与产生的子命令数量一致
         *              情况3：在无部件情况下产生一条新命令，此处需要将Commond 
         *              List数据对象ID传入）
         * 
         * @param[in]   ids：ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        void appendUserDefUndoIds(QList<int> ids);

        /**
         * @brief       获取用户定义撤销列表。
         * @return      用户定义撤销列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        QList<UndoRedoIds> getUserDefUndoList() const;

        /**
         * @brief       获取用户定义重做列表。
         * @return      用户定义重做列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        QList<UndoRedoIds> getUserDefRedoList() const;

        /**
         * @brief       获取未被引用的命令列表。
         * @return      根节点命令列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-07
         */
        QList<FITKAbsGeoCommand*> getRootCommandList();

        /**
         * @brief   获取全局数据ID列表。
         * @return  全局数据ID列表
         * @author  libaojun
         * @date    2025-04-28
         */
        QList<int> getGlobalDataIDList();

    private:
        /**
         * @brief       私有撤销方法。
         * @param[in]   forceUndoPart：是否强制将部件撤销[缺省]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        bool undoInternal(bool forceUndoPart = false);

        /**
         * @brief       私有重做方法。
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        bool redoInternal();

    protected:
        /**
         * @brief       重做列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoCommandManager* m_redoList{ nullptr };

        /**
         * @brief       用户自定义撤销列表，维护每次需要撤销的数据对象ID列表，
         *              执行撤销时优先判断是否存在用户自定义ID，不存在则使用
         *              管理器默认撤销逻辑。[BETA]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        QList<UndoRedoIds> m_userDefUndoList;

        /**
         * @brief       用户自定义重做列表，维护每次需要重做的数据对象ID列表，
         *              执行重做时优先判断是否存在用户自定义ID，不存在则使用
         *              管理器默认重做逻辑。[BETA]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        QList<UndoRedoIds> m_userDefRedoList;

        /**
         * @brief       基准元素管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKDatumList* m_datumMgr{ nullptr };

        /**
         * @brief       全局几何分组管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        FITKGlobalGeoComponentManager* m_globalGeoCompMgr{ nullptr };

        /**
         * @brief       当前激活草图数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        int m_currentSketchID = -1;
    };
}

// 注册元对象类型。
Q_DECLARE_METATYPE(Interface::UndoRedoIds);


#endif
