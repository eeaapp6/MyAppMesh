/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaReader.h
 * @brief  LS-Dyna文件读取器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaReader_H_
#define _FITKRadiossLSDynaReader_H_

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

namespace Radioss
{
    class FITKRadiossLSDynaAbstractAdaptor;
    class FITKRadiossCase;

    /**
     * @brief  Ls-Dyna文件读取器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaReader : public Interface::FITKAbstractTextReader
    {
        Q_OBJECT
    public:
        explicit FITKRadiossLSDynaReader(bool & success);
        virtual ~FITKRadiossLSDynaReader() = default;
        /**
         * @brief 执行读取操作
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        virtual void run() override;
        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 打印的信息
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief  创建适配器
         * @param[i] keyword 关键字
         * @return FITKRadiossLSDynaAbstractAdaptor*
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        FITKRadiossLSDynaAbstractAdaptor* createAdaptor(const QString& keyword);

    private:
        /**
         * @brief  成功标记
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        bool & _success;
    };
}

#endif