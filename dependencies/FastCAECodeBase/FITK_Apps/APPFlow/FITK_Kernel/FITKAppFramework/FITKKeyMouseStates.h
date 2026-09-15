/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKKeyMouseStates.h
 * @brief 键盘与鼠标按键状态声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-19
 *
 */
#ifndef __FITKKeyMouseStates_H__
#define __FITKKeyMouseStates_H__

#include "FITKAppFrameworkAPI.h"
#include <QMap>
#include <QMutex>

namespace AppFrame
{
    /**
     * @brief 键盘与鼠标按键状态声明
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-19
     */
    class FITKAppFrameworkAPI FITKKeyMouseStates
    {

    public:
        explicit FITKKeyMouseStates() = default;
        virtual ~FITKKeyMouseStates() = default;

        /**
         * @brief 记录键盘按键状态
         * @param[i]  key            键盘按键类型
         * @param[i]  p              是否按下，true 按下
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        void keyPressed(Qt::Key key, bool p);
        /**
         * @brief 获取键盘按键状态
         * @param[i]  key            按键类型
         * @return true              按下
         * @return false             抬起
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool keyPressed(Qt::Key key);
        /**
         * @brief 记录鼠标按键状态
         * @param[i]  button         键盘按键类型
         * @param[i]  p              是否按下
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        void mousePressed(Qt::MouseButton button, bool p);
        /**
         * @brief 获取鼠标按键状态
         * @param[i]  button         按键类型
         * @return true              按下
         * @return false             抬起
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool mousePressed(Qt::MouseButton button);
        /**
         * @brief 获取鼠标是否按下
         * @return true              按下
         * @return false             未按下
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-09
         */
        bool isMousePressed();
        /**
         * @brief 获取鼠标是否移动
         * @return true              移动
         * @return false             未移动
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-09
         */
        bool isMouseMoved() const;
         /**
         * @brief 设置鼠标是否移动
         * @param[i]  m              是否移动
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-09
         */
        void mouseMoved(const bool& m);

    private:
        /**
         * @brief 键盘按键点击状态,true 按下
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        QMap<Qt::Key, bool> _keyStates;
        /**
         * @brief 鼠标按键点击状态,true 按下
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        QMap<Qt::MouseButton, bool> _mouseButtonStates;
        /**
         * @brief 鼠标是否移动标记
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-05-09
         */
        bool _mouseMoved{ false };
    };
}
#endif  // !__GUISETTING_H__
