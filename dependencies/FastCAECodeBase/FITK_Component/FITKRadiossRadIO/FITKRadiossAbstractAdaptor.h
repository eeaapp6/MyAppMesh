/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAbstractAdaptor.h
 * @brief  Rad适配器抽象基类
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-11
 *********************************************************************/
#ifndef _FITKRADIOSSABSTRACTADAPTOR_H_
#define _FITKRADIOSSABSTRACTADAPTOR_H_

#include "FITKRadiossRadIOAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include <functional>
#include <QString>
#include <QHash>
#include <cmath>

namespace Radioss
{
    class FITKRadiossCase;
    class FITKRadiossRadReader;
    class FITKRadiossRadWriter;

    /**
     * @brief  Rad适配器抽象基类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-07-08
     */
    class FITKRadiossRadIOAPI  FITKRadiossAbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    { 

    public:
        explicit FITKRadiossAbstractAdaptor() = default;
        virtual ~FITKRadiossAbstractAdaptor() = 0;
        /**
         * @brief 设置文件读取器
         * @param[i]  reader     文件读取器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        void setFileReader(FITKRadiossRadReader* reader);
        /**
         * @brief 设置文件写入器
         * @param[i]  writer     文件写入器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        void setFileWriter(FITKRadiossRadWriter* writer);
        /**
         * @brief    设置算例数据
         * @param[i] casedata 算例数据
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-09
         */
        void setCaseData(FITKRadiossCase* casedata);         
        /**
         * @brief 创建适配器
         * @param[i]  keywords       关键字
         * @return FITKRadiossAbstractAdaptor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        static FITKRadiossAbstractAdaptor* createAdaptor(const QString & keywords);
        /**
         * @brief 注册适配器
         * @param[i]  keywords       关键字
         * @param[i]  creator        创建函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        static void registerAdaptor(const QString & keywords, 
             std::function<FITKRadiossAbstractAdaptor*()> creator);

    protected:

        //浮点数a占b个字符右对齐（默认宽度20）
        QString gapStr(double a, int b = 20) {
            if(a>1|| std::fabs(a) < DBL_EPSILON)
                return QString("%1").arg(QString::number(a, a < 1e12 ? 'f' : 'e', 6), b);
            else
                return QString("%1").arg(QString::number(a, a < 1e-1 ? 'e' : 'f', 6), b);
        };

        //整数a占b个字符右对齐（默认宽度10）,且ID类型小于0将输出空
        QString gapStr(int a, int b = 10, bool isID=true) {
            if(a<0 && isID)
                return QString("%1").arg("", b);
            return QString("%1").arg(a, b);
        };

    protected:
        /**
         * @brief  算例数据
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-09
         */
        FITKRadiossCase* _caseData{};
        /**
         * @brief  文件读取器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        FITKRadiossRadReader* _reader{};
        /**
         * @brief  文件写入器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        FITKRadiossRadWriter* _writer{};
        /**
         * @brief  适配器创建函数映射表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-08
         */
        static QHash<QString, std::function< FITKRadiossAbstractAdaptor*()>>& adaptorCreators();
        
    };
    
    /**
     * @brief  Rad适配器注册类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-07-08
     */
    class RadiossRadReger 
    { 
    public: 
        RadiossRadReger(const QString& keywords, std::function<FITKRadiossAbstractAdaptor*()> creator) 
        { 
            // 注册适配器
            FITKRadiossAbstractAdaptor::registerAdaptor(keywords, creator); 
        } 
        RadiossRadReger(const RadiossRadReger&) = delete; // 禁止拷贝构造
        RadiossRadReger& operator=(const RadiossRadReger&) = delete;
    };

    /**
     * @brief 宏定义，用于注册Rad适配器
     * @param keywords 关键字
     * @param className 类名
     * @param ... 可变参数
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-07-08
     */
    #define RegRadiossRadAdaptor(keywords, className, ...) \
    static RadiossRadReger reg_##className##Adaptor##__VA_ARGS__(QString(#keywords), []() -> FITKRadiossAbstractAdaptor* \
    { \
        return new className(); \
    }); \
    

} 
#endif // _FITKRADIOSSABSTRACTADAPTOR_H_