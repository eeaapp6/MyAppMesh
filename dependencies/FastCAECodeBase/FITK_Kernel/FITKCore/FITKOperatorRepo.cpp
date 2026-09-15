/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOperatorRepo.h"
#include <QMutexLocker>
#include "FITKOperatorRepoPrivate.h"

namespace Core
{
    /**
     * @brief 程序结束自动释放内存
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKOperatorDeletor
    {
    public:
        FITKOperatorDeletor() = default;
        ~FITKOperatorDeletor()
        {
            //回收内存
            auto p = FITKOPERREPO->_private;
            delete p;
            FITKOPERREPO->_private = nullptr;
        }
    };
    //静态变量声明
    static FITKOperatorDeletor __OPER_DELEATOR__;
    FITKOperatorRepo* FITKOperatorRepo::_instance = nullptr;
    QMutex FITKOperatorRepo::m_mutex;

    FITKAbstractOperator* FITKOperatorRepo::getOperator(const QString& key)
    {
        //线程安全，实际操作由private完成
        QMutexLocker locker(&m_mutex);
        return _private->getOperator(key);
    }

    void FITKOperatorRepo::removeOperator(const QString &key, bool removeFun /*= true*/)
    {
        //线程安全，实际操作由private完成
        QMutexLocker locker(&m_mutex);
        _private->removeOperator(key,removeFun);
    }

    void FITKOperatorRepo::clear()
    {
        //线程安全，实际操作由private完成
        QMutexLocker locker(&m_mutex);
        _private->clear();
    }

    void FITKOperatorRepo::registerOperatorFunction(const QString& key, std::function<FITKAbstractOperator*()> fun)
    {
        //线程安全，实际操作由private完成
        QMutexLocker locker(&m_mutex);
        _private->registerOperatorFunction(key, fun);
    }

    void FITKOperatorRepo::initialize()
    {
        //初始化，构造函数调用
        _private = new FITKOperatorRepoPrivate(_createFuns);
    }



    void FITKOperatorRepo::finalize()
    {

    }

}



