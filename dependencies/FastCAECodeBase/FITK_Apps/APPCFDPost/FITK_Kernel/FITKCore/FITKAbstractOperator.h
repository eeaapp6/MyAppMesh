/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractOperator.h
 * @brief 操作器抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef _FITKABSTRACT_OPERATOR_H__
#define _FITKABSTRACT_OPERATOR_H__

#include "FITKCoreAPI.h"
#include "FITKAbstractObject.hpp"
#include <QObject>
#include <QHash>

namespace Core
{
    /**
     * @brief 操作器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKCoreAPI FITKAbstractOperator : public QObject, public FITKAbstractObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractOperator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        explicit FITKAbstractOperator() = default;
        /**
         * @brief Destroy the FITKAbstractOperator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual ~FITKAbstractOperator() = 0;
        /**
         * @brief 设置触发对象
         * @param[i]  obj            触发对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual void setEmitter(QObject* obj);
        /**
         * @brief 设置参数
         * @param[i]  argName       参数名称
         * @param[i]  arg            参数对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void setArgs(const QString& argName, QVariant arg);
        /**
         * @brief  移除参数
         * @param[i]  argName        参数名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-03
         */
        void removeArgs(const QString& argName);
        /**
         * @brief 是否包含参数
         * @param[i]  argName      参数名称
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        bool hasArgs(const QString& argName);
        /**
         * @brief 清空全部参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual void clearArgs();

        template < typename T >
        /**
         * @brief  获取参数
         * @param[i]  name            参数名称
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-04-18
         */
        bool argValue(const QString& name, T& value)
        {
            if (!_operArgs.contains(name)) return false;
            auto arg = _operArgs.value(name);
            value = arg.value< T >();
            return true;
        }

        template < typename T >
        /**
         * @brief  获取参数，指针类型
         * @param[i]  name            参数名称
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-04-18
         */
        T* argValuePtr(const QString& name)
        {
            if (!_operArgs.contains(name)) return nullptr;
            auto arg = _operArgs.value(name);
            //先转成基类，防止QVarient直接类型转换需要QObject宏
            FITKAbstractObject* absObj = arg.value<FITKAbstractObject*>(); 
            return dynamic_cast< T* >(absObj);
        }

        /**
       * @brief 保存脚本
       * @param[i]  script  脚本
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-10-31
       */
        virtual void saveScript(const QStringList& script) ;
        /**
         * @brief 保存脚本
         * @param[i]  script  脚本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual void saveScript(const QString& script) ;



    signals:
        /**
         * @brief 操作器被释放信号
         * @param[o]  oper           被释放的指针（this）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void operatorDestoryedSig(FITKAbstractOperator* oper);
        /**
         * @brief 操作器抽象信号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-01
         */
        void operatorAbstractSig();
        void operatorAbstractSig1(FITKAbstractOperator* oper);
        void operatorAbstractSig2(FITKAbstractOperator* oper, const QVariant& value);
        void operatorAbstractSig3(FITKAbstractOperator* oper, void* value);
      
    protected:
        /**
         * @brief 参数列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QHash<QString, QVariant> _operArgs{};
        /**
         * @brief 触发器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QObject* _emitter{};

    };
}



#endif //