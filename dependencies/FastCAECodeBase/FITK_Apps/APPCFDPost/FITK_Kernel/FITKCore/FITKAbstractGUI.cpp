/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGUI.h"
#include "FITKOperatorRepo.h"
#include "FITKGUIRepo.h"

namespace Core
{
    /**
     * @brief Destroy the FITKWidget::FITKWidget object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    FITKWidget::~FITKWidget()
    {
    }

    void FITKWidget::flush()
    {
//        this->repaint();
    }

    void FITKWidget::initOperator(const QString & operName)
    {
        _oper = FITKOperatorRepo::getInstance()->getOperator(operName);
    }

 
   

    /**
     * @brief Destroy the FITKMainWindow::FITKMainWindow object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    FITKMainWindow::~FITKMainWindow()
    {
    }

    void FITKMainWindow::closeEvent(QCloseEvent *event)
    {
        //关闭全部对话框
        FITKGUIREPO->closeAllDialog();
        QMainWindow::closeEvent(event);
    }

    void FITKMainWindow::flush()
    {
     //   this->repaint();
    }

    FITKRibbonMainWindow::~FITKRibbonMainWindow()
    {
    }

    void FITKRibbonMainWindow::closeEvent(QCloseEvent *event)
    {
        //关闭全部对话框
        FITKGUIREPO->closeAllDialog();
        QMainWindow::closeEvent(event);
    }

    void FITKRibbonMainWindow::flush()
    {

    }

}
