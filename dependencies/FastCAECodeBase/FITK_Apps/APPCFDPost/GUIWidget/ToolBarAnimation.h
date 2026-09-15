/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ToolBarAnimation.h
 * @brief  动画播放工具条
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _ToolBarAnimation_H
#define _ToolBarAnimation_H

#include "GUIWidgetAPI.h"
#include "ToolBarBase.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

class QSpinBox;
class QLabel;
class QStringList;

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    class GUISpinBox;
    class AnimationThread;
    /**
     * @brief  动画播放工具条
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-11
     */
    class GUIWidgetAPI ToolBarAnimation : public ToolBarBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent     父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        ToolBarAnimation(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        ~ToolBarAnimation();
        /**
         * @brief    更新渲染对象
         * @param[i] objID     渲染对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual void updateCurrentGraphObj(int objID) override;
        /**
         * @brief    更新界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void updateWidget();
    private slots:
        ;
        /**
         * @brief    初始时间步点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotFirstFrame();
        /**
         * @brief    上一时间步点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotPreviousFrame();
        /**
         * @brief    动画播放点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotRunAnimation();
        /**
         * @brief    下一时间步点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotNextFrame();
        /**
         * @brief    最后时间步点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotLastFrame();
        /**
         * @brief    时间步修改事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotStepChange();

        void slotAniReadSuccess(int index);

        void slotAniReadFinish();
    private:
        /**
         * @brief    寻找根父对象数据id
         * @param[i] id          当前数据对象id
         * @return   int         多时间步数据对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-27
         */
        int findRootParentID(int id);
    private:
        GUISpinBox* _stepSpinBox = nullptr;
        QLabel* _stepLabel = nullptr;
        QAction* _actionFirst = nullptr;
        QAction* _actionPrevious = nullptr;
        QAction* _actionRun = nullptr;
        QAction* _actionNext = nullptr;
        QAction* _actionLast = nullptr;
        bool _isStop = true;
        AnimationThread* _aniThread = nullptr;
    };

    //----------------------------------------------------------------------------------------------------

        /**
         * @brief 导入文件读取线程
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
    class AnimationThread :public Core::FITKThreadTask
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Import Read Thread object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        AnimationThread(QStringList files, int dataID);
        /**
         * @brief Destroy the Import Read Thread object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~AnimationThread() = default;
        /**
         * @brief    设置当前的索引
         * @param[i] index       当前索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setCurrentIndex(int index);
        /**
         * @brief    设置是否终止
         * @param[i] isStop      是否终止
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setIsStop(bool isStop);
        /**
         * @brief    设置是否暂停
         * @param[i] isPause     是否暂停
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setIsPause(bool isPause);
        /**
         * @brief 执行函数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void run();
    signals:
        ;
        void sigReadSuccess(int index);
        /**
         * @brief 导入完成信号
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void sigImportFinish();
    public:
        /**
         * @brief 文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        QStringList _files = {};
        /**
         * @brief  数据对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        int _dataID = -1;
        /**
         * @brief  当前索引
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        int _currentIndex = -1;
        /**
         * @brief  是否暂停
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        bool _isStop = false;
        /**
         * @brief  设置是否暂停
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        bool _isPause = false;
    };
}
#endif