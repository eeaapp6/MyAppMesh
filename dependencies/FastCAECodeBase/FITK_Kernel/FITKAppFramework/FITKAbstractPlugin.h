/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ABSTRACT_PLUGIN_H___
#define _FITK_ABSTRACT_PLUGIN_H___
 
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp" 
#include "FITKAppFrameworkAPI.h"
#include <QString>

class QLibrary;

namespace AppFrame
{
    class FITKPluginsManager;
    /**
     * @brief 插件抽象类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-12-04
     */
    class FITKAppFrameworkAPI FITKAbstractPlugin : public Core::FITKVarientParams, public Core::FITKAbstractObject
    {
        friend FITKPluginsManager;
    public:
        /**
         * @brief Construct a new FITKAbstractPlugin object
         * @param[i]  dylibrary      动态库句柄
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        explicit FITKAbstractPlugin(QLibrary* dylibrary);
        /**
         * @brief Destroy the FITKAbstractPlugin object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        virtual ~FITKAbstractPlugin() = 0;
        /**
         * @brief 设置动态库路径
         * @param[i]  path         动态库路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        void setLibraryPath(const QString& path);
        /**
         * @brief 获取动态库路径
         * @return QString 动态库路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        QString getLibraryPath() const;
        /**
         * @brief 获取插件名称，自定义
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        virtual QString getPluginName() = 0;
        /**
         * @brief 执行命令
         * @return bool 是否执行成功
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        virtual bool exec();        
        
        /**
         * @brief 读取工程文件
         * @param[in] filePointer 文件指针
         * @param[in] fileType 文件类型
         * @param[out] error 错误信息
         * @return bool 是否读取成功
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-11
         */
        virtual bool read(void* filePointer, QString& error, int fileType = 0) ;

        /**
         * @brief 写入工程文件
         * @param[in] filePointer 文件指针
         * @param[in] fileType 文件类型
         * @param[out] error 错误信息
         * @return bool 是否写入成功
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-11
         */
        virtual bool write(void* filePointer, QString& error, int fileType = 0) ;


    private:
        /**
         * @brief 插件加载
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        virtual void install() = 0;
        /**
         * @brief 插件卸载
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        virtual void unInstall() = 0;
        /**
         * @brief 获取动态库句柄
         * @return QLibrary*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        QLibrary* getLibrary();

    private:
        /**
         * @brief 动态库
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        QLibrary* _library{};
        /**
         * @brief 动态库路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-12-04
         */
        QString _libPath{};

    };
}

/**
 * @brief 动态库中的c格式的加载函数指针
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-12-04
 */
typedef AppFrame::FITKAbstractPlugin*(*pluginCreateFun)(QLibrary*);
/**
 * @brief 动态库中的c格式的加载函数名称
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-12-04
 */
#define FITKLibraryLoadFunName "FITKLibraryLoadFun"

 /**
  * @brief 动态库中的c格式的函数指针
  * @author libaojun (libaojunqd@foxmail.com)
  * @date 2024-12-04 
  */
typedef QString(*pluginRecognizeFun)();
/**
 * @brief 动态库中的c格式识别函数名称
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-12-04
 */
#define FITKLibraryRecognizeFunName "FITKLibraryRecognizeFun"


#endif
