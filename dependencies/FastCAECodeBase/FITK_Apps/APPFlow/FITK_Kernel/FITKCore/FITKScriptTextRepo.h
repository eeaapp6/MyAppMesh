/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKScriptTextRepo.h
 * @brief 脚本（宏）文本存储相关类声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-31
 * 
 */
#ifndef _FITK_SCRIPT_TEXT_REPO_H___
#define _FITK_SCRIPT_TEXT_REPO_H___
 
#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include <QStringList>
#include <QString>
#include <QList>
#include <QMutex>

namespace Core
{
    /**
     * @brief 脚本（宏）文本块
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-31
     */
    class  FITKScriptTextBlock
    {
    public:
        /**
         * @brief Construct a new FITKScriptTextBlock object
         * @param[i]  textBlock      文本块
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        explicit FITKScriptTextBlock(const QString& textBlock);
        /**
         * @brief Construct a new FITKScriptTextBlock object
         * @param[i]  textBlock      文本块
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        explicit FITKScriptTextBlock(const QStringList& textBlock);
        /**
         * @brief Destroy the FITKScriptTextBlock object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual ~FITKScriptTextBlock() = default;
        /**
         * @brief 在块中追加文本
         * @param[i]  text         追加的文本
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void appendText(const QString& text);
        /**
         * @brief 获取文本
         * @return QStringList 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        QStringList getText() const;

    private:
        /**
         * @brief 文本内容
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        QStringList _texts{};

    };

    class FITKScriptTextRepoDeleator;
    /**
     * @brief 脚本（宏）文本管理仓库，单例
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-31
     */
    class FITKCoreAPI FITKScriptTextRepo
    {
        DeclSingleton(FITKScriptTextRepo);
        friend FITKScriptTextRepoDeleator;
 
    public:
         /**
         * @brief 追加脚本（宏）文本
         * @param[i]  script         脚本（宏）文本
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void appendScript(const QStringList& script);
        /**
         * @brief 追加脚本（宏）文本
         * @param[i]  script         脚本（宏）文本
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void appendScript(const QString& script);
        /**
         * @brief 清空脚本（宏）文本仓库
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void clearRepo();
        /**
         * @brief 删除最后一个文本块
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void removeLastBlock();
        /**
         * @brief 导出脚本（宏）文本到文件
         * @param[i]  fileName       文件名
         * @return true              导出成功
         * @return false             导出失败
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        bool exportScriptFile(const QString& fileName);
        /**
         * @brief 开启/关闭脚本（宏）文本记录
         * @param[i]  en             开启/关闭
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void enableRecord(bool en = true);
        /**
         * @brief 是否开启脚本（宏）文本记录
         * @return true              开启
         * @return false             关闭
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        bool isRecordEnabled();

    private:
        /**
         * @brief 初始化
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void initialize();
        /**
         * @brief 销毁
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void finalize();

    private:
        /**
         * @brief 是否开启脚本（宏）文本记录
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        bool _isEnableRecord{ false };
        /**
         * @brief 脚本（宏）文本块列表
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        QList<FITKScriptTextBlock> _textBlocks{};
    };
    /**
     * @brief 脚本（宏）文本仓库单例销毁器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-31
     */
    class FITKScriptTextRepoDeleator
    {
    public:
        /**
         * @brief Construct a new FITKScriptTextRepoDeleator object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        explicit FITKScriptTextRepoDeleator() = default;
        /**
         * @brief Destroy the FITKScriptTextRepoDeleator object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual ~FITKScriptTextRepoDeleator()
        {
            delete FITKScriptTextRepo::_instance;
            FITKScriptTextRepo::_instance = nullptr;
        }
    };
    
 
}


#endif
