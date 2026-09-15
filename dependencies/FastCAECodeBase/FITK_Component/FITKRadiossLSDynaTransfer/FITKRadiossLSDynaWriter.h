/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaWriter.h
 * @brief  LS-Dyna文件写入器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaWriter_H_
#define _FITKRadiossLSDynaWriter_H_

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace Radioss
{
    class FITKRadiossLSDynaAbstractAdaptor;

    /**
     * @brief  LS-Dyna文件写入器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaWriter : public Interface::FITKAbstractTextWriter
    {
        Q_OBJECT
    public:
        explicit FITKRadiossLSDynaWriter(bool & success);
        virtual ~FITKRadiossLSDynaWriter() = default;
        /**
         * @brief 执行写入操作
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
         * @brief  成功标记
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        bool & _success;
    };
}

#endif