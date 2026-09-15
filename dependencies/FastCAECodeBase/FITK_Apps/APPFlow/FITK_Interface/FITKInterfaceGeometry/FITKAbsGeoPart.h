/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoPart.h
 * @brief       模型部件命令数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-11-27
 *********************************************************************/

#ifndef  FITKABSGEOPART_H
#define  FITKABSGEOPART_H

#include "FITKAbsGeoCommand.h"

#include "FITKGeoEnum.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       模型部件命令数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-11-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoPart : public FITKAbsGeoCommand, public FITKGeoCommandManager
    {
        FITKCLASS(Interface, FITKAbsGeoPart);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKAbsGeoPart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        virtual ~FITKAbsGeoPart();

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       重建部件。[虚函数][重写]
         * @param[in]   fromDataId：从指定ID的命令开始更新，为-1则从第一条命令开始更新[缺省]
         * @return      执行中断命令ID，-1则表示重构模型正常退出
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-22
         */
        virtual int reGenerate(int fromDataId = -1) override;

        /**
         * @brief       获取是否可以执行撤销操作。[虚函数][重写]
         * @return      是否可以撤销
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool canUndo() override;

        /**
         * @brief       获取是否可以执行重做操作。[虚函数][重写]
         * @return      是否可以重做
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool canRedo() override;

        /**
         * @brief       撤销部件建模操作。[虚函数][重写]
         * @return      是否撤销成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool undo() override;

        /**
         * @brief       重做部件建模操作。[虚函数][重写]
         * @return      是否重做成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual bool redo() override;

        /**
         * @brief   回溯最终的形状[虚函数]
         * @return  是否回溯成功
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-27
         */
        virtual bool backtrackingShape();

        /**
         * @brief       设置该部件是否自动布尔。
         * @param[in]   flag：是否自动布尔
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        void setAutoBool(bool flag);

        /**
         * @brief       获取该部件是否自动布尔。
         * @return      是否自动布尔
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        bool getAutoBool();

        /**
         * @brief       设置该部件自动布尔是否合并相邻实体。(关闭则执行非正则布尔，部分几何引擎不支持)
         * @param[in]   flag：是否合并实体
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-28
         */
        void setAutoMergeSolid(bool flag);

        /**
         * @brief       获取该部件自动布尔是否合并相邻实体。
         * @return      是否合并实体
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-28
         */
        bool getAutoMergeSolid();

        /**
         * @brief       更新命令。[虚函数][重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        virtual bool update() override;

        /**
         * @brief       重写追加对象功能，增加重置重做列表逻辑。[虚函数][重写]
         * @param[in]   item：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual void appendDataObj(Core::FITKAbstractDataObject* item) override;

        /**
         * @brief       重写追加对象功能，追加一组对象，增加重置重做列表逻辑。[虚函数][重写]
         * @param[in]   items：对象列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        virtual void appendDataObj(const QList<Core::FITKAbstractDataObject*> & items) override;

        /**
         * @brief       重写移除数据对象功能。[虚函数][重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief       重写移除数据对象功能。[虚函数][重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeDataObjWithoutRelease(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief       根据索引移除数据。[虚函数][重写]
         * @param[in]   index：数据索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeDataByIndex(int index) override;

        /**
         * @brief       根据ID移除数据。[虚函数][重写]
         * @param[in]   id：数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeDataByID(int id) override;

        /**
         * @brief       根据名称移除数据。[虚函数][重写]
         * @param[in]   name：数据名称
         * @param[in]   compSens：大小写敏感[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeDataByName(const QString & name, bool compSens = false) override;

    protected:
        /**
         * @brief       是否开启模型自动布尔。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        bool m_autoBool = true;

        /**
         * @brief       自动布尔是否合并相邻实体。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-28
         */
        bool m_autoMergeSolid = true;

        /**
         * @brief       重做列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-04
         */
        FITKGeoCommandManager* m_redoList{ nullptr };

    };
}

#endif // !FITKABSGEOPART_H
