/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGUIRepo.h
 * @brief  声明GUI仓库
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef __FITKGUI_REPO_H___
#define __FITKGUI_REPO_H___

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QMutex>
#include <QList>
#include <QHash>

namespace Core
{
    class FITKDialog;
    class FITKAbstractGUIObject;
    class FITKGUIRepoDeleator;
    /**
     * @brief GUI仓库，软件生命周期的全部界面都在仓库中
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKGUIRepo : public FITKAbstractObject
    {
        friend FITKAbstractGUIObject;
        friend FITKGUIRepoDeleator;

        DeclSingleton(FITKGUIRepo);

    public:
        /**
         * @brief 根据ID获取UI
         * @param[i]  id          Ui的ID
         * @return FITKAbstractGUIObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        FITKAbstractGUIObject* getFITKGUIObject(const int id);
        /**
         * @brief  根据ID关闭GUI
         * @param[i]  objID          ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void closeGUIObject(const int objID);
        /**
         * @brief 关闭UI对象
         * @param[i]  obj          关闭对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void closeGUIObject(FITKAbstractGUIObject* obj);
        /**
         * @brief 关闭全部除了exception之外的对话框对象
         * @param[i] exception 不关闭的对话框
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */ 
        void closeAllDialog( const QList<FITKDialog*>& exception = QList<FITKDialog*>() );
        /**
          * @brief 显示/隐藏全部除了exception之外的对话框对象
          * @param[i] isShow 是否显示 true 显示 
          * @param[i] exception 不做操作的对话框
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-11-22
          */
        void showAllDialog(bool isshow, const QList<FITKDialog*>& exception = QList<FITKDialog*>());
        /**
         * @brief 根据附加信息筛选对象
         * @param[i]  key            附加信息标记
         * @param[i]  va             附加信息
         * @return QList<FITKAbstractGUIObject*> 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        QList<FITKAbstractGUIObject*> fileterGUIObject(const QString& key, const QVariant& va);
 
        template <class T>
        /**
         * @brief 根据附加信息筛选类型为T的对象
         * @param[i]  key            附加信息标记
         * @param[i]  va             附加信息
         * @return QList<FITKAbstractGUIObject*> 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        QList<T*> fileterGUIObjectT(const QString& key, const QVariant& va)
        {
            QList<T*> objts;
            QList<FITKAbstractGUIObject*> objs = this->fileterGUIObject(key, va);
            //遍历类型判断
            for (auto o: objs)
            {
                T* t = dynamic_cast<T*>(o);
                if (t) objts.append(t);
            }
            return objts;
        }
        /**
         * @brief 仓库中是否包含对象
         * @param[i]  obj           判断的obj
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        bool isContains(FITKAbstractGUIObject* obj);

        template <class T>
        /**
         * @brief 根据id获取对象并转换类型
         * @param[i]  id             界面对象id
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        T* getFITKGUIObjectT(const int id)
        {
            return dynamic_cast<T*>(this->getFITKGUIObject(id));
        }

        template<class T>
        /**
         * @brief 获取T类型的全部对象
         * @return QList<T*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        QList<T*> getGUIObjects()
        {
            QList<T*> tobjs;
            auto guiObjects = _idObjectHash.values();
            for (auto obj : guiObjects)
            {
                auto t = dynamic_cast<T*>(obj);
                if (t) tobjs.append(t);
            }
            return tobjs;
        }
        /**
         * @brief 获取激活的对话框ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-29
         */
        void setActiveDialog(const int & id);

        template<class T>
        /**
         * @brief 获取激活的对话框对象
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-29
         */
        T* getActiveDialog()
        {
            FITKAbstractGUIObject* obj = getFITKGUIObject(_activeID);
            return dynamic_cast<T*>(obj);
        }

    private:
        /**
         * @brief 插入对象
         * @param[i]  obj            插入的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void  insertGUIObject(FITKAbstractGUIObject* obj);
        /**
         * @brief 移除对象
         * @param[i]  obj           移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void  removeGUIObject(FITKAbstractGUIObject* obj);
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void finalize();

    private:
        /**
         * @brief GUI对象管理hash，键是GUI的ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        QHash<int, FITKAbstractGUIObject*> _idObjectHash{};
        /**
         * @brief激活的窗口ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-29
         */
        int _activeID{ -1 };
    };

    class FITKGUIRepoDeleator
    {
    public:
        explicit FITKGUIRepoDeleator() = default;
        virtual ~FITKGUIRepoDeleator();
    };

}
/**
 * @brief 定义快速访问
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 */
#ifndef FITKGUIREPO
#define FITKGUIREPO (Core::FITKGUIRepo::getInstance())
#endif

#endif