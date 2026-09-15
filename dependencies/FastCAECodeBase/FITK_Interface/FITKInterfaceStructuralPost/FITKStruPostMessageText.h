/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKStruPostMessageText.h
 * @brief  后处理信息文本数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-03
 *********************************************************************/
#ifndef _FITKStruPostMessageText_H
#define _FITKStruPostMessageText_H

#include "FITKInterfaceStructuralPostAPI.h"
#include <QObject>
#include <QColor>

namespace Interface
{
    /**
     * @brief  后处理信息文本数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-03
     */
    class FITKInterfaceStructuralPostAPI FITKStruPostMessageText : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        FITKStruPostMessageText();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        virtual ~FITKStruPostMessageText();
        /**
         * @brief    设置文本
         * @param[i] text        文本
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void setText(const QString& text);
        /**
         * @brief    获取文本
         * @return   QString     文本
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        QString getText();
        /**
         * @brief    获取颜色
         * @return   QColor        颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        QColor getColor();
        /**
         * @brief    设置颜色
         * @param[i] color         颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void setColor(QColor& color);
        /**
         * @brief    设置位置
         * @param[i] pos         位置[2]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void setPos(double* pos);
        /**
         * @brief    获取位置
         * @param[o] pos         位置[2]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void getPos(double* pos);
        /**
         * @brief    获取是否可用
         * @return   bool        是否可用
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        bool getIsEnable();
        /**
         * @brief    设置是否可用
         * @param[i] isEnable   是否可用
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void setIsEnable(bool isEnable);
        /**
         * @brief    获取文本大小
         * @return   int       文本大小
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        int getTextSize();
        /**
         * @brief    设置文本大小
         * @param[i] size     文本大小
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void setTextSize(int size);
    signals:
        ;
        /**
         * @brief    更新信号
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        void sigUpdate();
    protected:
        /**
         * @brief  文本
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        QString _text = "";
        /**
         * @brief  位置[2]
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        double _position[2] = { 200,50 };
        /**
         * @brief  颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        QColor _color = QColor(255, 255, 255);
        /**
         * @brief  是否可用
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        bool _isEnable = true;
        /**
         * @brief  文本大小
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        int _size = 16;
    };
}

#endif