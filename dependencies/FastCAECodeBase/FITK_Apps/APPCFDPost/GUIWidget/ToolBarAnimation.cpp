/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ToolBarAnimation.h"
#include "GUISpinBox.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostUnSteady.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include <QAction>
#include <QLabel>
#include <QSpinBox>
#include <QThread>

namespace GUI
{
    ToolBarAnimation::ToolBarAnimation(QWidget * parent) :
        ToolBarBase(parent)
    {
        _actionFirst = createAction("actionFirst", "://icons/first.png", QString(tr("first")));
        connect(_actionFirst, SIGNAL(triggered()), this, SLOT(slotFirstFrame()));
        addAction(_actionFirst);
        _actionPrevious = createAction("actionPrevious", "://icons/previous.png", QString(tr("previous")));
        connect(_actionPrevious, SIGNAL(triggered()), this, SLOT(slotPreviousFrame()));
        addAction(_actionPrevious);
        _actionRun = createAction("actionRun", "://icons/run.png", QString(tr("run")));
        connect(_actionRun, SIGNAL(triggered()), this, SLOT(slotRunAnimation()));
        addAction(_actionRun);
        _actionNext = createAction("actionNext", "://icons/next.png", QString(tr("next")));
        connect(_actionNext, SIGNAL(triggered()), this, SLOT(slotNextFrame()));
        addAction(_actionNext);
        _actionLast = createAction("actionLast", "://icons/end.png", QString(tr("last")));
        connect(_actionLast, SIGNAL(triggered()), this, SLOT(slotLastFrame()));
        addAction(_actionLast);
        addSeparator();

        auto lab = new QLabel(this);
        lab->setText("Step:");
        addWidget(lab);
        _stepSpinBox = new GUISpinBox(this);
        _stepSpinBox->setMaximum(1);
        _stepSpinBox->setSingleStep(1);
        connect(_stepSpinBox, SIGNAL(editingFinished()), this, SLOT(slotStepChange()));
        addWidget(_stepSpinBox);
        _stepLabel = new QLabel(this);
        _stepLabel->setText(QString(tr("Total: 0")));
        addWidget(_stepLabel);
        addSeparator();

        setEnabled(false);
    }

    ToolBarAnimation::~ToolBarAnimation()
    {

    }

    void ToolBarAnimation::updateCurrentGraphObj(int objID)
    {
        _currentDataID = findRootParentID(objID);

        setEnabled(false);
        _stepSpinBox->setValue(0);
        _stepLabel->setText(QString(tr("Total: 0")));
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(currentData);
        if (currentGraphObj == nullptr)return;

        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        _stepSpinBox->setMaximum(unSteadyData->getFiles().size() - 1);
        _stepSpinBox->setValue(unSteadyData->getCurrentIndex());
        _stepLabel->setText(QString(tr("Total: %1")).arg(unSteadyData->getFiles().size() - 1));

        updateWidget();
        setEnabled(true);
    }

    void ToolBarAnimation::updateWidget()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;

        QStringList files = unSteadyData->getFiles();
        int currentIndex = unSteadyData->getCurrentIndex();
        if (files.isEmpty())return;

        _actionFirst->setEnabled(true);
        _actionPrevious->setEnabled(true);
        _actionRun->setEnabled(true);
        _actionNext->setEnabled(true);
        _actionLast->setEnabled(true);

        if (currentIndex == 0) {
            _actionFirst->setEnabled(false);
            _actionPrevious->setEnabled(false);
        }
        else if(currentIndex > 0 && currentIndex < (files.size() - 1)) {

        }
        else if (currentIndex == (files.size() - 1)) {
            _actionRun->setEnabled(false);
            _actionNext->setEnabled(false);
            _actionLast->setEnabled(false);
        }
    }

    void ToolBarAnimation::slotFirstFrame()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        _stepSpinBox->setValue(0);
        unSteadyData->setFile(files[0]);
        unSteadyData->update();
        updateWidget();
        reRender();
    }

    void ToolBarAnimation::slotPreviousFrame()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        int currentIndex = unSteadyData->getCurrentIndex();
        if ((currentIndex - 1) >= files.size())return;
        _stepSpinBox->setValue(currentIndex - 1);
        unSteadyData->setFile(files[currentIndex - 1]);
        unSteadyData->update();
        updateWidget();
        reRender();
    }

    void ToolBarAnimation::slotRunAnimation()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        int currentIndex = unSteadyData->getCurrentIndex();
        if ((currentIndex + 1) >= files.size())return;

        if (_isStop) {
            _actionRun->setIcon(QIcon("://icons/stop.png"));
            _isStop = false;
            _aniThread = new AnimationThread(files, _currentDataID);
            _aniThread->setCurrentIndex(currentIndex);
            _aniThread->setIsStop(_isStop);
            //单个文件读取后连接（阻塞式连接）
            connect(_aniThread, SIGNAL(sigReadSuccess(int)), this, SLOT(slotAniReadSuccess(int)), Qt::BlockingQueuedConnection);
            connect(_aniThread, SIGNAL(sigImportFinish()), this, SLOT(slotAniReadFinish()));
            Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
            pool->execTask(_aniThread);
        }
        else {
            _actionRun->setIcon(QIcon("://icons/run.png"));
            _isStop = true;
            if (_aniThread) {
                _aniThread->setIsStop(_isStop);
            }
        }
    }

    void ToolBarAnimation::slotNextFrame()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        int currentIndex = unSteadyData->getCurrentIndex();
        if ((currentIndex + 1) >= files.size())return;
        _stepSpinBox->setValue(currentIndex + 1);
        unSteadyData->setFile(files[currentIndex + 1]);
        unSteadyData->update();
        updateWidget();
        reRender();
    }

    void ToolBarAnimation::slotLastFrame()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        _stepSpinBox->setValue(files.size() - 1);
        unSteadyData->setFile(files[files.size() - 1]);
        unSteadyData->update();
        updateWidget();
        reRender();
    }

    void ToolBarAnimation::slotStepChange()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QStringList files = unSteadyData->getFiles();
        if (files.size() == 0)return;
        int currentIndex = unSteadyData->getCurrentIndex();
        int index = _stepSpinBox->value();
        if (index == currentIndex)return;
        unSteadyData->setFile(files[index]);
        unSteadyData->update();
        updateWidget();
        reRender();
    }

    void ToolBarAnimation::slotAniReadSuccess(int index)
    {
        Q_UNUSED(index);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostUnSteady* unSteadyData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(currentData);
        if (unSteadyData == nullptr)return;
        QSignalBlocker blocker1(_stepSpinBox);
        _stepSpinBox->setValue(unSteadyData->getCurrentIndex());
        reRender();
        updateWidget();
    }

    void ToolBarAnimation::slotAniReadFinish()
    {
        _actionRun->setIcon(QIcon("://icons/run.png"));
        _isStop = true;
        if (_aniThread) {
            _aniThread->setIsStop(_isStop);
        }
        reRender();
        updateWidget();
    }

    int ToolBarAnimation::findRootParentID(int id)
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(id);
        if (currentData == nullptr)return -1;
        int parentID = currentData->getCFDPostParentID();
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(parentID);
        if (parentData == nullptr)return currentData->getDataObjectID();

        return findRootParentID(parentID);
    }

    //----------------------------------------------------------------------------------------------

    AnimationThread::AnimationThread(QStringList files, int dataID) :
        _files(files), _dataID(dataID)
    {

    }

    void AnimationThread::setCurrentIndex(int index)
    {
        _currentIndex = index;
    }

    void AnimationThread::setIsStop(bool isStop)
    {
        _isStop = isStop;
    }

    void AnimationThread::setIsPause(bool isPause)
    {
        _isPause = isPause;
    }

    void AnimationThread::run()
    {
        Interface::FITKCFDPost3DManager* postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (postDataManager == nullptr) return;
        Interface::FITKAbstractCFDPostData* data = postDataManager->getDataByID(_dataID);
        if (data == nullptr)return;
        auto type = data->getPostDataType();

        for (int i = _currentIndex + 1; i < _files.size(); i++) {
            QString file = _files[i];
            
            switch (type) {
            case Interface::FITKPostDataType::Post_UnSteady: {
                Interface::FITKCFDPostUnSteady* vtkData = dynamic_cast<Interface::FITKCFDPostUnSteady*>(data);
                if (vtkData == nullptr)return;
                vtkData->setFile(file);
                break;
            }
            default:
                break;
            }

            if (data) {
                data->update();
                //采用连接方式为阻塞连接，完成渲染后再执行下一个文件读取
                emit sigReadSuccess(i);
            }
            if (_isStop) break;
        }
        emit sigImportFinish();
    }
}