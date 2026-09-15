/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief 按钮组件文件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-12
 */
#ifndef LOCKERBUTTON_H
#define LOCKERBUTTON_H

#include <QPushButton>

class QLabel;
class QPixmap;

namespace Comp
{
    /**
     * @brief 按钮组件文件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-12
     */
    class FITKLockerButton :public QPushButton
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKLockerButton object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        FITKLockerButton(QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKLockerButton object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        virtual ~FITKLockerButton();
        /**
         * @brief 设置标题
         * @param[i]  text           标题
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        void setText(const QString& text);
        /**
         * @brief 设置图标
         * @param[i]  image          图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        void setImage(const QPixmap& image);
    private:
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        void initialize();
    private:
        /**
         * @brief 图标
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QLabel* m_iconLabel = nullptr;
        /**
         * @brief 文本信息
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-12
         */
        QLabel* m_textLabel = nullptr;
    };
}

#endif // LOCKERBUTTON_H
