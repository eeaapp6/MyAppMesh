/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAppScriptHelper.h
 * @brief    脚本辅助类
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-31
 * 
 */
#ifndef _FITK_APP_SCRIPT_HELPER_H___
#define _FITK_APP_SCRIPT_HELPER_H___
 
#include "FITKAppFrameworkAPI.h"
#include <functional>
#include <QString>

namespace Core
{
    class FITKScriptTextRepo;
}

namespace AppFrame
{
    /**
     * @brief 脚本文件辅助类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-31
     */
    class FITKAppFrameworkAPI FITKAppScriptHelper
    {
    public:
        /**
         * @brief Construct a new FITKAppScriptHelper object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        explicit FITKAppScriptHelper() = default;
        /**
         * @brief Destroy the FITKAppScriptHelper object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual ~FITKAppScriptHelper() = default;
        /**
         * @brief 清理脚本文本仓库
         * @return void
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void clearTextRepo();
        /**
         * @brief 获取脚本文本仓库
         * @return Core::FITKScriptTextRepo* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        Core::FITKScriptTextRepo* getScriptTextRepo();
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
         * @brief 执行脚本（宏）文件
         * @param[i]  fileName       文件名
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        void execScriptFile(const QString& fileName);
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
        /**
         * @brief 注册脚本（宏）文本执行函数
         * @param[i]  f              脚本（宏）文本执行函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */ 
        void regExecScriptFun(std::function< void(const QString&) > f) ;
    private:
         /**
          * @brief 执行脚本函数
          * @author libaojun (libaojunqd@foxmail.com)
          * @date 2024-11-05
          */
        std::function< void(const QString&) > _execFun{};
    }; 
}


#endif
