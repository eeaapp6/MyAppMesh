/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAppHistoryFiles.h
 * @brief 历史文件记录声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-03-19
 * 
 */
#ifndef _FITK_APP_HISTORY_FILES_H___
#define _FITK_APP_HISTORY_FILES_H___
 
#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>

namespace AppFrame
{
    /**
     * @brief 历史文件信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-03-19
     */
    class FITKAppFrameworkAPI FITKAppHistoryFileInfo : 
        public QObject, public Core::FITKAbstractObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 历史文件类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        enum HistoryFileType
        {
            HFTNone,    ///< 无
            HFTGeo,     ///< 几何文件
            HFTGeo_1,   ///< 几何文件
            HFTGeo_2,   ///< 几何文件
            HFTGeo_3,   ///< 几何文件
            HFTGeo_4,   ///< 几何文件
            HFTMesh,    ///< 网格文件
            HFTMesh_1,   ///< 网格文件
            HFTMesh_2,   ///< 网格文件
            HFTMesh_3,   ///< 网格文件
            HFTMesh_4,   ///< 网格文件
            HFTProj,    ///< 工程文件
            HFTPost,    ///< 后处理文件
            HFTPost_1,   ///< 后处理文件
            HFTPost_2,   ///< 后处理文件
            HFTPost_3,   ///< 后处理文件
            HFTPost_4,   ///< 后处理文件
            HFTUserDef,  ///< 用户自定义文件
            HFTUserDef_1,   ///< 用户自定义文件
            HFTUserDef_2,   ///< 用户自定义文件
            HFTUserDef_3,   ///< 用户自定义文件
            HFTUserDef_4,   ///< 用户自定义文件
        };
        Q_ENUM(HistoryFileType);


    public:
        /**
         * @brief 构造函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param path 文件路径
         * @param type 文件类型
         */
        explicit FITKAppHistoryFileInfo(const QString& path,
            const HistoryFileType & type = HistoryFileType::HFTNone);
        /**
         * @brief 析构函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        virtual ~FITKAppHistoryFileInfo() = default;
    
        /**
         * @brief 获取文件路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return QString 文件路径
         */
        QString getFilePath() const;
        /**
         * @brief 设置文件类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param type 文件类型
         */
        void setFileType(const HistoryFileType & type);
        /**
         * @brief 获取文件类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return HistoryFileType 文件类型
         */
        HistoryFileType getFileType() const;
        /**
         * @brief 更新时间
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param time 时间
         */
        void updateTime(QDateTime* time = nullptr);
        /**
         * @brief 获取时间
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return QDateTime 时间
         */
        QDateTime getTime() const;


    private:
        /**
         * @brief 文件路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        QString _filePath{};
        /**
         * @brief 文件类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        HistoryFileType _fileType{ HFTNone };
        /**
         * @brief 最后一次访问时间
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        QDateTime _lastTime{};

    };
    /**
     * @brief 历史文件记录
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-03-19
     */
    class FITKAppFrameworkAPI FITKAppHistoryFiles :
        public QObject, public Core::FITKAbstractObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAppHistoryFiles object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        explicit FITKAppHistoryFiles() = default;
        /**
         * @brief Destroy the FITKAppHistoryFiles object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        virtual ~FITKAppHistoryFiles();

        /**
         * @brief 添加历史文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param path 文件路径
         * @param type 文件类型
         */
        void addFile(const QString& path,
            const FITKAppHistoryFileInfo::HistoryFileType & type = FITKAppHistoryFileInfo::HistoryFileType::HFTNone);
        /**
         * @brief 删除历史文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param filePath 文件路径
         */
        void removeFile(const QString & filePath);
        /**
         * @brief 设置最大文件数量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param n 最大文件数量
         */
        void setMaxFileNum(const int n );
        /**
         * @brief 获取最大文件数量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return int 最大文件数量
         */
        int getMaxFileNum() const;
        /**
         * @brief 获取文件数量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return int 文件数量
         */ 
        const int  getFileCount() const;
        /**
         * @brief 获取文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param index 文件索引
         * @return FITKAppHistoryFileInfo* 文件信息
         */
        FITKAppHistoryFileInfo* getFileInfo(const int index);
        /**
         * @brief 获取文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param filePath 文件路径
         * @return FITKAppHistoryFileInfo* 文件信息
         */
        FITKAppHistoryFileInfo* getFileInfo(const QString & filePath);
        /**
         * @brief 获取历史文件路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @return QList<QString> 历史文件路径
         */
        QList<QString> getHistoryFiles() const;
        /**
         * @brief 获取历史文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param type 文件类型
         * @return QList<FITKAppHistoryFileInfo*> 历史文件信息
         */
        QList<FITKAppHistoryFileInfo*> getFileInfos(const FITKAppHistoryFileInfo::HistoryFileType & type);
        /**
         * @brief 从设置中加载历史文件记录
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        void loadFromSetting();
        /**
         * @brief 写入设置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        void write2Setting();

    private:
        /**
         * @brief 获取文件索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         * @param fileName 文件名
         * @return int 文件索引
         */
        int getIndex(const QString &fileName);
        /**
         * @brief 调整文件链表大小
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        void resize();

    private:
        /**
         * @brief 最大文件数量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        int _maxFileNumber{ 10 };
        /**
         * @brief 文件链表
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-19
         */
        QList<FITKAppHistoryFileInfo*> _historyFiles{};
    };
}


#endif
