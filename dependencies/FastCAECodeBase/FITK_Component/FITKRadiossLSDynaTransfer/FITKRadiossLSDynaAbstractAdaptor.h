/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaAbstractAdaptor.h
 * @brief  LS-DYNA适配器抽象基类
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaAbstractAdaptor_H_
#define _FITKRadiossLSDynaAbstractAdaptor_H_

#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include <QString>
#include <QHash>
#include <functional>

namespace Radioss
{
    class FITKRadiossCase;
    class FITKRadiossLSDynaReader;
    class FITKRadiossLSDynaWriter;

    /**
     * @brief  LS-DYNA适配器抽象基类
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaAbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(Adaptor, FITKRadiossLSDynaAbstractAdaptor);
    public:
        /**
         * @brief    构造函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        explicit FITKRadiossLSDynaAbstractAdaptor() = default;
        /**
         * @brief    析构函数，纯虚函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        virtual ~FITKRadiossLSDynaAbstractAdaptor() = 0;

        /**
         * @brief    设置文件读取器
         * @param[i] reader 文件读取器
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        void setFileReader(FITKRadiossLSDynaReader* reader);

        /**
         * @brief    设置文件写入器
         * @param[i] writer 文件写入器
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        void setFileWriter(FITKRadiossLSDynaWriter* writer);

        /**
         * @brief    设置算例数据
         * @param[i] casedata 算例数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        void setCaseData(FITKRadiossCase* casedata);

        /**
         * @brief    创建适配器
         * @param[i] keywords 关键字
         * @return   FITKRadiossLSDynaAbstractAdaptor*
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        static FITKRadiossLSDynaAbstractAdaptor* createAdaptor(const QString& keywords);

        /**
         * @brief    注册适配器
         * @param[i] keywords 关键字
         * @param[i] creator 创建函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        static void registerAdaptor(const QString& keywords,
            std::function<FITKRadiossLSDynaAbstractAdaptor*()> creator);

    protected:
        /**
         * @brief  算例数据
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        FITKRadiossCase* _caseData{};

        /**
         * @brief  文件读取器
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        FITKRadiossLSDynaReader* _reader{};

        /**
         * @brief  文件写入器
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        FITKRadiossLSDynaWriter* _writer{};

        /**
         * @brief  适配器创建函数映射表
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        static QHash<QString, std::function<FITKRadiossLSDynaAbstractAdaptor*()>>& adaptorCreators();

    };

    /**
     * @brief  LS-DYNA适配器注册类
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class RadiossLSDynaReger
    {
    public:
        /**
         * @brief    构造函数
         * @param[i] keywords 关键字
         * @param[i] creator 创建函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        RadiossLSDynaReger(const QString& keywords, std::function<FITKRadiossLSDynaAbstractAdaptor*()> creator)
        {
            FITKRadiossLSDynaAbstractAdaptor::registerAdaptor(keywords, creator);
        }

        RadiossLSDynaReger(const RadiossLSDynaReger&) = delete;
        RadiossLSDynaReger& operator=(const RadiossLSDynaReger&) = delete;
    };

    /**
     * @brief 宏定义，用于注册LS-DYNA适配器
     * @param keywords 关键字
     * @param className 类名
     * @param ... 可选参数
     * @author guqingtao (15598887859@163.com)
     * @date 2026-05-07
     */
    #define RegLSDynaAdaptor(keywords, className, ...) \
    static RadiossLSDynaReger reg_##className##Adaptor##__VA_ARGS__(QString(#keywords), []() -> FITKRadiossLSDynaAbstractAdaptor* \
    { \
        return new className(); \
    });

}

#endif
