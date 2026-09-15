/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKComponentInterface.h
 * @brief  组件接口抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef __FITKCOMPONENT_INTERFACE_H__
#define __FITKCOMPONENT_INTERFACE_H__

#include "FITKAppFrameworkAPI.h"
#include <functional>
#include <QString>
#include <QWidget>
#include <QVariant>
#include <QHash>

//定义回调函数，参数为void*，返回值为int
typedef std::function<int(void*)> CallBackFuns;


namespace Core
{
    class FITKAbstractDataObject;
    class FITKThreadTask;
}
namespace AppFrame
{
    /**
     * @brief 组件接口抽象类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class  FITKAppFrameworkAPI FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new FITKComponentInterface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        explicit FITKComponentInterface() = default;
        /**
         * @brief Destroy the FITKComponentInterface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual ~FITKComponentInterface() = 0;

        /**
         * @brief 初始化，创建完成之后自动化调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual void init();
        /**
         * @brief 获取组件中的界面
         * @param[i]  indexPort     抽象定义接口
         * @return QWidget*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QWidget* getWidget(const int indexPort);
        /**
         * @brief 获取部件名称，不能重复
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QString getComponentName() = 0;
        /**
         * @brief 执行组件
         * @param[i]  indexPort      抽象接口定义
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual bool exec(const int indexPort);
        /**
         * @brief 设置名称
         * @param[i]  name           名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void setComponentObjName(const QString & name);
        /**
         * @brief 获取名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        QString getComponentObjName() const;
        /**
         * @brief 获取回调函数
         * @param[i]  key            键
         * @return CallBackFuns
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-25
         */
        CallBackFuns getCallBackFuns(int key);
        /**
         * @brief 设置回调函数，键相同的会覆盖
         * @param[i]  k              键
         * @param[i]  f              回调函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-25
         */
        void setCallBackFuns(int k, CallBackFuns f);
        /**
         * @brief 移除回调函数
         * @param[i]  k              键
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-25
         */
        void removeCallBackFun(int k);
        /**
         * @brief 设置数据对象
         * @param[i]  name           数据对象标记
         * @param[i]  data           数据对象指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-10
         */
        void setDataObject(const QString& name, Core::FITKAbstractDataObject* data);
        /**
         * @brief 获取数据对象
         * @param[i]  name           数据对象标记
         * @return Core::FITKAbstractDataobject* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-10
         */
        Core::FITKAbstractDataObject* getDataObject(const QString& name);

        template <class T>
        /**
         * @brief 获取数据对象并进行强制类型转换
         * @param[i]  name           名称
         * @return T* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-10
         */
        T* getDataObjectT(const QString& name)
        {
            return dynamic_cast<T*>(this->getDataObject(name));
        }

        /**
         * @brief 重置全部数据对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-10
         */
        void resetDataObjects();

        /**
         * @brief 获取打开工程文件不释放的数据对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-08-18
         */
        virtual QList<int> getUnResetDataObj();
        /**
         * @brief 设置父任务线程，一般用于封装文件读写线程，实现信号中继
         * @param[i] task 父任务线程
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-11-06
         */
        void setParentThreadTask(Core::FITKThreadTask* task);
    protected:
        /**
         * @brief 名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        QString _compObjName{};
        /**
         * @brief 回调函数
         * @author LiBaojun (libaojunqd@foxmail.com) 
         * @date 2024-04-25
         */
        QHash<int, CallBackFuns> _callBackFuns{};
        /**
         * @brief 数据对象hash
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-10
         */
        QHash<QString, Core::FITKAbstractDataObject*> _dataObjects{};
        /**
         * @brief 父任务线程
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-11-06
         */
        Core::FITKThreadTask* _parentTask{};
    };
}

#endif
