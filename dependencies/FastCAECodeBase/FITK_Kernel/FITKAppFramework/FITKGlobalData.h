/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGolbalData.h
 * @brief 全局数据总线声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef __FITKLGOLBALDATA_H__
#define __FITKLGOLBALDATA_H__

#include <QObject>
#include <QHash>
#include "FITKAppFrameworkAPI.h"
#include <QMutex>
#include <QMutexLocker>


class QWidget;

namespace Core
{
    class FITKAbstractDataObject;
}

namespace AppFrame
{
    /**
     * @brief 全局数据类型，主要数据
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    enum  FITKGlobalDT
    {
        GDTNone = 0,
        GDTMesh,
        GDTGeom,
        GDTPhysics,
        GDTPost, 
        GDTOther = 1000,
        GDTEND = 3000,
    };
    class FITKAbstractGlobalDataFactory;
    class FITKRunTimeSetting;
    class FITKKeyMouseStates;
    class FITKAppHistoryFiles;
    /**
     * @brief 全局数据总线
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class FITKAppFrameworkAPI FITKGlobalData : public QObject
    {
        friend FITKAbstractGlobalDataFactory;
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKGolbalData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKGlobalData();
        /**
         * @brief Destroy the FITKGolbalData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKGlobalData();
        /**
         * @brief 记录主界面
         * @param[i]  mw     主界面指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void setMainWindow(QWidget* mw);

        /**
         * @brief 获取主界面
         * @return    QWidget*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QWidget* getMainWindow();

        template<class T>
        /**
         * @brief 获取主界面并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-08
         */
        T* getMainWindowT()
        {
            QMutexLocker locker(&_mutex);
            return dynamic_cast<T*>(this->getMainWindow());
        }

        template<class T>
        /**
         * @brief 获取网格数据并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T* getMeshData()
        {
            QMutexLocker locker(&_mutex);
            return dynamic_cast<T*>(_gData.value(GDTMesh));
        }

        template<class T>
        /**
         * @brief 获取几何数据并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T* getGeometryData()
        {
            QMutexLocker locker(&_mutex);
            return dynamic_cast<T*>(_gData.value(GDTGeom));
        }
        template<class T>
        /**
         * @brief 获取物理数据并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T* getPhysicsData()
        {
            return dynamic_cast<T*>(_gData.value(GDTPhysics));
        }

        template<class T>
        /**
         * @brief 获取后处理数据并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T* getPostData()
        {
            QMutexLocker locker(&_mutex);
            return dynamic_cast<T*>(_gData.value(GDTPost));
        }

//        template<class T>
//         /**
//          * @brief 获取运行时数据
//          * @return T*
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-02-29
//          */
//         T* getRunTimeData()
//         {
//             return dynamic_cast<T*>(_gData.value(GDTRunTime));
//         }

        template<class T>
        /**
         * @brief 根据类型获取数据并进行类型转换
         * @param[i]  d        数据类型，参考FITKGlobalDT枚举
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        T* getData(int d)
        {
            QMutexLocker locker(&_mutex);
            return dynamic_cast<T*>(_gData.value(d));
        }
        /**
         * @brief 获取运行时参数
         * @return FITKRunTimeSetting*
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        FITKRunTimeSetting* getRunTimeSetting();
        /**
         * @brief 获取键盘鼠标状态
         * @return FITKRunTimeSetting*
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-30
         */
        FITKKeyMouseStates* getKeyMouseStates();
        /**
         * @brief 获取全局数据ID
         * @return QList<int>
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-30
         */
        QList<int> getGlobalDataIDs();
        /**
         * @brief 获取历史文件管理
         * @return FITKAppHistoryFiles*
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        FITKAppHistoryFiles* getHistoryFiles();

    private:
        /**
         * @brief 插入数据管理
         * @param[i]  dt             数据类型，参考FITKGlobalDT枚举
         * @param[i]  d              数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        void insertData(int dt, Core::FITKAbstractDataObject* d);
 
    private:
        /**
         * @brief 线程锁
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        static QMutex _mutex;
        /**
         * @brief 主界面指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QWidget* _mainwindow{};
        /**
         * @brief 全局数据管理
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        QHash<int, Core::FITKAbstractDataObject*> _gData{};
        /**
         * @brief 运行时参数
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        FITKRunTimeSetting* _runtimeSetting{};
        /**
         * @brief 键盘鼠标状态
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        FITKKeyMouseStates* _keyMouseStates{};
        /**
         * @brief 历史文件管理
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        FITKAppHistoryFiles* _historyFiles{};
    };
}



#endif // !__FITKLGOLBALDATA_H__
