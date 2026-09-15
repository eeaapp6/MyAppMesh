/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOperatorRepoPrivate.h"
#include "FITKAbstractOperator.h"

namespace Core
{
    FITKOperatorRepoPrivate::FITKOperatorRepoPrivate(QHash<QString, std::function<FITKAbstractOperator*()>>& funs)
        :_createFuns(funs)
    {
    }

    FITKOperatorRepoPrivate::~FITKOperatorRepoPrivate()
    {
        //遍历释放全部存在的operator
        QList<FITKAbstractOperator*> opers = _existOperators.values();
        for (auto oper : opers)
            if(oper != nullptr)
                delete oper;
        //容器清空
        _existOperators.clear();
    }

    Core::FITKAbstractOperator* FITKOperatorRepoPrivate::getOperator(const QString & key)
    {
        //如果已经存在则返回原来的operator
        if (_existOperators.contains(key))
            return _existOperators.value(key);
        //不存在则直接创建
        return this->createOperator(key);
    }

    void FITKOperatorRepoPrivate::removeOperator(const QString &key, bool removeFun)
    {
        //已经创建的对象
        if (_existOperators.contains(key))
        {
            FITKAbstractOperator* oper = _existOperators.value(key);
            _existOperators.remove(key);
            if (oper != nullptr) delete oper;
        }
        //移除创建函数
        if (!removeFun) return;
        if (_createFuns.contains(key))
        {
            _createFuns.remove(key);
        }

    }

    void FITKOperatorRepoPrivate::clear()
    {
        //先断开容器关系，再释放对象，避免析构信号触发重复移除
        QList<FITKAbstractOperator*> opers = _existOperators.values();
        _existOperators.clear();
        for (auto oper : opers)
            if (oper != nullptr)
                delete oper;
    }

    void FITKOperatorRepoPrivate::registerOperatorFunction(const QString& key, std::function<FITKAbstractOperator*()> fun)
    {
        _createFuns.insert(key, fun);
    }

    Core::FITKAbstractOperator* FITKOperatorRepoPrivate::createOperator(const QString & key)
    {
        //查找创建函数
        auto fun = _createFuns.value(key);
        if (!fun) return nullptr;
        //执行创建操作
        FITKAbstractOperator* op = fun();
        if (op == nullptr) return nullptr;
        //纳入到容器管理，并关联槽函数
        _existOperators.insert(key, op);
        connect(op, &FITKAbstractOperator::operatorDestoryedSig,
            this, &FITKOperatorRepoPrivate::operatorDestoryedSlot);
        return op;
    }

    void FITKOperatorRepoPrivate::operatorDestoryedSlot(FITKAbstractOperator* oper)
    {
        //operator被释放  delete
        const QString name = _existOperators.key(oper);
        if (!_existOperators.contains(name)) return;
        //直接从管理器移除，不要重复delete
        _existOperators.remove(name);
    }

}
