/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaLineParser.h
 * @brief  LS-DYNA行解析器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-08
 *********************************************************************/
#ifndef _FITKRadiossLSDynaLineParser_H_
#define _FITKRadiossLSDynaLineParser_H_

#include "FITKRadiossLSDynaTransferAPI.h"
#include <QString>
#include <QStringList>

namespace Radioss
{
    /**
     * @brief  LS-DYNA行解析器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-08
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaLineParser
    {
    public:
        /**
         * @brief    解析关键字行
         * @param[i] line 原始行
         * @return   QString 关键字
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static QString parseKeyword(const QString& line);

        /**
         * @brief    解析数据行
         * @param[i] line 原始行
         * @param[i] numFields 字段数量
         * @return   QStringList 字段列表
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static QStringList parseDataLine(const QString& line, int numFields);

        /**
         * @brief    跳过注释行（$开头）
         * @param[i] line 原始行
         * @return   bool 是否为注释行
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static bool isCommentLine(const QString& line);

        /**
         * @brief    是否为空行
         * @param[i] line 原始行
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static bool isEmptyLine(const QString& line);

        /**
         * @brief    关键字块检测（*KEYWORD 开始）
         * @param[i] keyword 关键字
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static bool isBlockStart(const QString& keyword);

        /**
         * @brief    关键字块结束检测（*END）
         * @param[i] line 原始行
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static bool isBlockEnd(const QString& line);

        /**
         * @brief    标准化关键字（转为大写）
         * @param[i] keyword 原始关键字
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static QString normalizeKeyword(const QString& keyword);

        /**
         * @brief    解析字段值
         * @param[i] fieldStr 字段字符串
         * @return   double
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static double parseFieldValue(const QString& fieldStr);

        /**
         * @brief    解析整数字段值
         * @param[i] fieldStr 字段字符串
         * @return   int
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        static int parseFieldIntValue(const QString& fieldStr);

    private:
        static const int LS_DYNA_LINE_LENGTH = 80;
    };
}

#endif
