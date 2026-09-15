/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperPost3D.h
 * @brief  3D后处理事件操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef __OPERPOST3D_H__
#define __OPERPOST3D_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include <QString>

class QTimer;

namespace Interface
{
    class FITKAbstractStructuralPostVTK;
}

namespace OperModel
{
    /**
     * @brief  3D后处理事件操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-08
     */
    class OperPost3D : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-08
         */
        explicit OperPost3D();
        /**
         * @brief    构造函数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        ~OperPost3D();
        /**
         * @brief    UI逻辑
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        bool execGUI() override;
        /**
         * @brief    业务逻辑
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        bool execProfession() override;

    private:
        /**
         * @brief    第一帧
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void playFirstFrame();
        /**
         * @brief    上一帧
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void playPreviousFrame();
        /**
         * @brief    下一帧
         * @param[o] 是否是最后一帧
         * @return   void 
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void playNextFrame(bool* isLastFrame = nullptr);
        /**
         * @brief    最后一帧
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void playLastFrame();
        /**
         * @brief    开始播放
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void startPlaying();
        /**
         * @brief    停止播放
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-17
         */
        void stopPlaying();

        /**
         * @brief    更新工具栏的结果下拉框
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void updateToolBarFieldComboBox();

        void resetViewport();

    private:
        //已导入的文件链表
        QStringList _filePathList{};

        QTimer* _timer{};
    };

    Register2FITKOperatorRepo(actionFirstFrame, OperPost3D);
    Register2FITKOperatorRepo(actionPreviousFrame, OperPost3D);
    Register2FITKOperatorRepo(actionNextFrame, OperPost3D);
    Register2FITKOperatorRepo(actionStartPlaying, OperPost3D);
    Register2FITKOperatorRepo(actionLastFrame, OperPost3D);
    Register2FITKOperatorRepo(actionUpdateToolBarField, OperPost3D);

    /**
     * @brief  3D后处理结果更新
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class OperPost3DResultUpdate : public Core::FITKActionOperator
    {
    public :
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        explicit OperPost3DResultUpdate() = default;
        /**
         * @brief    构造函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        ~OperPost3DResultUpdate() = default;
        /**
         * @brief    业务逻辑
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        bool execProfession() override;

    };

    Register2FITKOperatorRepo(actionUpdatePostGraph, OperPost3DResultUpdate);
}

#endif // !__MODELOPERATORS_INMPORT_INP_H__
