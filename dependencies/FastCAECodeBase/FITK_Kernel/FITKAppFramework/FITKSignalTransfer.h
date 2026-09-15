/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKSignalTransfer.hpp
 * @brief 定义信号转接器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-05
 *
 */
#ifndef _FITKSIGNALTRANSFER_H__
#define _FITKSIGNALTRANSFER_H__

#include <QObject>
#include <QList>
#include <QHash>
#include "FITKAppFrameworkAPI.h"

namespace Core
{
    class FITKAbstractObject;
}

namespace AppFrame
{
    
    class FITKAbstractProgramerDriver;
    /**
     * @brief 信号转接器，处理组件间必须的信号转发
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-05
     */
    class FITKAppFrameworkAPI FITKSignalTransfer : public QObject
    {
        Q_OBJECT
    public:
        using QObject::QObject;
        /**
         * @brief Destroy the FITKSignalTransfer object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        virtual ~FITKSignalTransfer()  ;
         /**
         * @brief 追加信号理器，会根据名称自动关联signalTransfer的信号
         * @param[i]  obj           处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        void addSignalProcesser(QObject* obj);

    signals:
        /**
         * @brief 信息输出
         * @param[i]  type        信息类型 1-normal 2-warning 3-error 4-info
         * @param[i]  message      输出信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        void outputMessageSig(int type, QString message);

        /**
        * @brief 线程进度信号 0~100
        * @param[o]  sender         发送的进程任务
        * @param[o]  process        线程进度 0-100
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-26
        */
        void sendProcessSig(QObject* sender, int process);
        /**
        * @brief 第三方程序输出信息
        * @param[i]  driver        求解器驱动器
        * @oaram[i]  messageType   信息类型
        * @param[i]  message       信息内容
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-22
        */
        void sendProgramDriverMessageSig(FITKAbstractProgramerDriver* driver, int messageType, const QString& message);
        
        /**
        * @brief 文件/文件夹发生了变化
        * @param[i]  sender   触发对象
        * @oaram[i]  isFile   是否为文件，false-是文件夹
        * @param[i]  path     发生变化的文件或文件夹
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-22
        */
        void fileDirectoryChangedSig(Core::FITKAbstractObject* sender, bool isFile, const QString& path);
        
        /**
         * @brief 设置拾取对象类型
         * @param[i]  pickType       拾取对象类型  -1-关闭拾取 3001-网格节点 3002-网格单元 3003-几何点 3004-几何线 3005-几何面 3006-几何体
         * @note 该信号用于设置拾取对象类型，拾取对象类型会影响拾取的结果
         * @note 该信号会在图形界面中被调用，通常在图形界面中设置拾取对象类型时触发
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-26
         */
        void setPickableObjTypeSig(int pickType);
        /**
         * @brief 图形对象被选中
         * @param[i]  pickType   选中类型 3001-网格节点 3002-网格单元 3003-几何点 3004-几何线 3005-几何面 3006-几何体
         * @param[i]  objectId   对象ID
         * @param[i]  objInfo    对象信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-18
         */
        void graphObjectPickedSig(int pickType, int objectId, QHash<QString, void*> objInfo);

        /**
         * @brief 图形对象高亮
         * @param[i]  isHighlighted 是否高亮 true-高亮 false-取消高亮
         * @param[i]  objType    高亮对象类型
         * @param[i]  objectId   对象ID
         * @param[i]  objInfo    对象信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-25
         */
        void highlightGraphObjectSig(bool isHighlighted, int objType, int objectId, QHash<QString, void*> objInfo);
    private:
        /**
         * @brief 信号处理器链表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        QList<QObject*> _signalProcesser{};

    };
}

#endif