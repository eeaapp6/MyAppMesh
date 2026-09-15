/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   AppInitializer.h
 * @brief  初始化器
 * @author   wangning(2185896382@qq.com)
 * @date     2026-06-09
 *********************************************************************/
#ifndef _APP_INITIALIZER_H___
#define _APP_INITIALIZER_H___

#include "FITK_Kernel/FITKAppFramework/FITKAbstractAppInitializer.h"

#include <QString>

/**
  * @brief  初始化器声明
  */
class AppInitializer : public AppFrame::FITKAbstractAppInitializer
{
public:
    /**
     * @brief Construct a new App Initializer object
     */
    explicit AppInitializer() = default;
    /**
     * @brief Destroy the App Initializer object
     */
    virtual ~AppInitializer() = default;
    /**
     * @brief 执行初始化操作，初始化失败将直接退出程序
     * @return true  初始化成功
     * @return false  初始化失败
     */
    bool init() override;

private:
  /**
   * @brief    初始化 AI 助手 skill 和 mcp 资源
   * @return   true 初始化成功
   * @return   false 初始化失败
   */
  bool initAIAssistantSkillResources();

  /**
   * @brief    递归拷贝资源目录到目标目录
   * @param    resourcePath[in] 资源目录路径
   * @param    targetPath[in]   目标目录路径
   * @return   true 拷贝成功
   * @return   false 拷贝失败
   * @author   wangning(2185896382@qq.com)
   * @date     2026-06-09
   */
  bool copyDirectoryFromResource(const QString& resourcePath, const QString& targetPath) const;

  /**
   * @brief    拷贝单个资源文件到目标路径
   * @param    resourceFilePath[in] 资源文件路径
   * @param    targetFilePath[in]   目标文件路径
   * @return   true 拷贝成功
   * @return   false 拷贝失败
   * @author   wangning(2185896382@qq.com)
   * @date     2026-06-09
   */
  bool copyFileFromResource(const QString& resourceFilePath, const QString& targetFilePath) const;

  /**
   * @brief    获取 AI 助手资源根目录目标路径
   * @return   QString 目标路径
   * @author   wangning(2185896382@qq.com)
   * @date     2026-06-09
   */
  QString getAIAssistantTargetRootPath() const;

};


#endif
