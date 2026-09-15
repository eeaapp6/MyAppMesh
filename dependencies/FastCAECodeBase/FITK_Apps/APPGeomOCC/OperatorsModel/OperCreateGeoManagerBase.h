/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperCreateGeoManagerBase.h
 * @brief   几何创建相关操作器基类。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-29
 *********************************************************************/

#ifndef OPERCREATEGEOMANAGERBASE_H
#define OPERCREATEGEOMANAGERBASE_H

#include "OperManagerBase.h"

#include "OperatorsModelAPI.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/GraphInteractionOperator.h"

// Qt 
#include <QDialog>

namespace ModelOper
{
    /**
     * @brief   几何创建相关操作器基类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-29
     */
    class OperatorsModelAPI OperCreateGeoManagerBase : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        OperCreateGeoManagerBase() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        virtual ~OperCreateGeoManagerBase() = default;

        /**
         * @brief   执行界面相关功能。[虚函数][重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        virtual bool execGUI() override;

        /**
         * @brief   执行业务逻辑。[虚函数][重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        virtual bool execProfession() override;

        /**
         * @brief   结束操作。[虚函数][重写]
         * @param   updateCommand：是否更新命令[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        virtual void finalize(bool updateCommand = false) override;

        /**
         * @brief   递归更新命令。[虚函数]
         * @param   cmd：命令
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        virtual void updateCommandRecursively(Interface::FITKAbsGeoCommand* cmd);

    protected:
        /**
         * @brief   向下递归启用或禁用引用数据对象。
         * @param   cmd：命令对象
         * @param   flag：启用或禁用
         * @param   recDown：向下递归（被引用方向查询）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        virtual void setRefEnableRecursively(Interface::FITKAbsGeoCommand* cmd, bool flag, bool recDown = true);

        /**
         * @brief   显示模型参数窗口。[纯虚函数 TODO]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        virtual void showDialog() {};

        /**
         * @brief   初始化模型参数对话框。
         * @param   parent：对话框父窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        template<typename T, typename T2 = Interface::FITKAbsGeoCommand>
        T* initDialog()
        {
            T2 *obj{};
            if (m_OperType == Edit)
            {
                obj = dynamic_cast<T2*>(FITKDATAREPO->getDataByID(m_Id));
                if (obj == nullptr) {
                    printLog(3, tr("System Error."));
                    return nullptr;
                }
            }

            QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
            for (QDialog* iDlg : dlgs)
            {
                if (iDlg && !dynamic_cast<T*>(iDlg))
                {
                    iDlg->close();
                }
            }

            // 如果历史数据存在则隐藏自己并暂时关闭引用计数，防止对自己进行操作。
            // 在窗口关闭时进行还原。
            if (obj)
            {
                // 暂存ID。
                setArgs("Id", m_Id);

                EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

                // 基准元素仅禁用并刷新自己即可。
                if (obj->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTDatum)
                {
                    obj->enable(false);

                    if (operGraph)
                    {
                        operGraph->updateGraphVisibility(obj->getDataObjectID());
                    }
                }
                else
                {
                    // 先根据引用关系禁用状态。
                    setRefEnableRecursively(obj, false, true);

                    // 关闭引用。
                    obj->enableReferenceList(false);

                    // 向上向下刷新可视化对象。
                    if (operGraph)
                    {
                        operGraph->updateGraphRecursively(obj->getDataObjectID(), -1, true);
                        operGraph->updateGraphRecursively(obj->getDataObjectID(), -1, false);
                    }
                }
            }

            // 彻底关闭拾取状态。
            EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
            if (operPick)
            {
                operPick->clearPickMode();
            }

            T* dlg = T::New(m_MainWindow, this, obj);

            // 显示模型参数窗口。
            dlg->show();

            return dlg;
        }
    };

#ifndef DefineGeometryOperator
#define DefineGeometryOperator(actionName,thisClass) \
    class OperatorsModelAPI thisClass : public OperCreateGeoManagerBase \
    { \
    public: \
        explicit thisClass() = default; \
        virtual ~thisClass() = default; \
        bool execProfession() override; \
    protected: \
        void showDialog() override; \
    }; \
    Register2FITKOperatorRepo(thisClass, thisClass); \
    Register2FITKOperatorRepo(actionName, thisClass); \
    Register2FITKOperatorRepo(actionName##Create, thisClass); \
    Register2FITKOperatorRepo(actionName##Edit, thisClass); \
    Register2FITKOperatorRepo(actionName##Delete, thisClass); \
    Register2FITKOperatorRepo(actionName##Hide, thisClass); \
    Register2FITKOperatorRepo(actionName##Show, thisClass); \
    Register2FITKOperatorRepo(actionName##Rename, thisClass);
#endif // !DefineGeometryOperator

} // namespace ModelOper
#endif //!OPERCREATEGEOMANAGERBASE_H
