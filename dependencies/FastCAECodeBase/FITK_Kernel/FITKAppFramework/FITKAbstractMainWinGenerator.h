/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractMainWinGenerator.h
 * @brief 主界面生成器抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef __FITKABSMAINWINDOWGENERATOR_H__
#define __FITKABSMAINWINDOWGENERATOR_H__

#include "FITKAppFrameworkAPI.h"
#include <QString>
#include <QObject>

class QWidget;

namespace AppFrame
{
    /**
     * @brief 主界面抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class FITKAppFrameworkAPI FITKAbstractMainwindowGenerator : public QObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractMainwindowGenerator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKAbstractMainwindowGenerator();
        /**
         * @brief Destroy the FITKAbstractMainwindowGenerator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKAbstractMainwindowGenerator() = default;
        /**
         * @brief 启用高DPI支持
         * @author libaojun 
         * @date 2025-12-31
         */
        static void EnableHighDPIScaling();
        /**
         * @brief 加载qss
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-05
         */
        virtual bool loadQssStyle();
        /**
         * @brief 加载翻译文件
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual bool loadTranslatorQmFiles();
        /**
         * @brief 生成主界面，子类必须重写该函数。创建出主界面，并对主界面的属性完成设置
         * @return QWidget*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual QWidget* genMainWindow() = 0;
        /**
         * @brief 最大化显示
         * @param[i]  max            最大化显示
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-03
         */
        void showMaximize(bool max);
        /**
         * @brief 是否最大化
         * @return true
         * @return false
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-03
         */
        bool showMaximize();
        /**
         * @brief 显示欢迎页面，通常是一张图片
         * @return true
         * @return false
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-10-12
         */
        virtual void showWelcomePage();
        /**
         * @brief 显示预设置页面，用于设置工作路径等
         * @return true   
         * @return false  结束初始化进程，退出程序
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-10-12
         */
        virtual bool showPreSettingPage();
 

    private:
        /**
         * @brief 获取qss文件 loadQssStyle调用
         * @return QString
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual QString getQssStyle();
        /**
         * @brief 获取翻译文件，loadTranslatorQmFiles调用
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual QStringList getTranslatorQmFiles();

    private:
        /**
         * @brief 最大化显示
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-03
         */
        bool _showMaximize{ true };

    };
}



#endif // __FITKABSMAINWINDOWGENERATOR_H__