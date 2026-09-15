/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDynamicStruPostVTK.h"
#include "FITKStruPostMessageText.h"

#include <QFile>
#include <QFileInfo>
#include <QCollator>

#include <vtkDataSetReader.h>

namespace Interface
{
    FITKDynamicStruPostVTK::FITKDynamicStruPostVTK(StruPostFileType type):
        FITKAbstractStructuralPostVTK(type)
    {
    }

    FITKDynamicStruPostVTK::~FITKDynamicStruPostVTK()
    {
    }

    StruPostType FITKDynamicStruPostVTK::getPostStructualType()
    {
        //显式分析
        return SPTDynamicExplict;
    }

    void FITKDynamicStruPostVTK::setFiles(const QStringList & files)
    {
        _isH5 = false;
        //文件列表
        _fileList = files;

        //对文件排序
        QCollator collator;
        collator.setNumericMode(true);
        std::sort(_fileList.begin(), _fileList.end(),
            [&collator](const QString & str1, const QString & str2)
            {
                return collator.compare(str1, str2) < 0;
            });
        if (_fileList.isEmpty()) return;

        this->setFileIndex(_fileList.size() - 1);

        // 初始化显示分组All。
        this->resetDisplayGroup();
    }

    void FITKDynamicStruPostVTK::setFiles(const QString & fileMesh, const QString & fileResult)
    {
        if (fileMesh.isEmpty() || fileResult.isEmpty()) {
            return;
        }
        if (!QFile::exists(fileMesh) || !QFile::exists(fileResult)) {
            return;
        }
        _isH5 = true;
        _meshFile = fileMesh;
        _resultFile = fileResult;
        this->setInputFile(fileMesh, fileResult);
    }

    void FITKDynamicStruPostVTK::appendFile(const QString & fileName, bool isUpdate)
    {
        if (fileName.isEmpty() == true) {
            return;
        }
        _fileList.append(fileName);

        if (isUpdate == true) {
            this->setFileIndex(_fileList.size() - 1);
            // 初始化显示分组All。
            this->resetDisplayGroup();
        }
    }

    void FITKDynamicStruPostVTK::nextFrame()
    {
        if (_isH5) {
            QPair<int, int> pair = this->getH5FileStep();
            int currentStep = pair.first;
            int totalStep = pair.second;
            if (currentStep + 1 < 0 || currentStep + 1 >= totalStep) {
                return;
            }
            if (QFile::exists(_meshFile) && QFile::exists(_resultFile)) {
                this->setInputFile(_meshFile, _resultFile, currentStep + 1);
            }
            return;
        }
        //当前文件索引
        const int index = this->getFileIndex();
        //正确性判断
        if ( index + 1 < 0 || index +1 >= _fileList.size() ) return;
        this->setFileIndex(index + 1);
    }

    void FITKDynamicStruPostVTK::previousFrame()
    {
        if (_isH5) {
            QPair<int, int> pair = this->getH5FileStep();
            int currentStep = pair.first;
            int totalStep = pair.second;
            if (currentStep - 1 < 0 || currentStep - 1 >= totalStep) {
                return;
            }
            if (QFile::exists(_meshFile) && QFile::exists(_resultFile)) {
                this->setInputFile(_meshFile, _resultFile, currentStep - 1);
            }
            return;
        }
        //当前文件索引
        const int index = this->getFileIndex();
        //正确性判断
        if (index - 1 < 0 || index - 1 >= _fileList.size()) return;
        this->setFileIndex(index - 1);
    }

    void FITKDynamicStruPostVTK::firstFrame()
    {
        if (_isH5) {
            QPair<int, int> pair = this->getH5FileStep();
            int currentStep = pair.first;
            if (currentStep == 0) {
                return;
            }
            if (QFile::exists(_meshFile) && QFile::exists(_resultFile)) {
                this->setInputFile(_meshFile, _resultFile, 0);
            }
            return;
        }
        //当前文件索引
        const int index = this->getFileIndex();
        //已经是第一个
        if (index == 0) return;
        this->setFileIndex(0);
    }

    void FITKDynamicStruPostVTK::lastFrame()
    {
        if (_isH5) {
            QPair<int, int> pair = this->getH5FileStep();
            int currentStep = pair.first;
            int totalStep = pair.second;
            if (currentStep == totalStep - 1) {
                return;
            }
            if (QFile::exists(_meshFile) && QFile::exists(_resultFile)) {
                this->setInputFile(_meshFile, _resultFile, totalStep - 1);
            }
            return;
        }
        //当前文件索引
        const int index = this->getFileIndex();
        //已经是最后一个
        if (index == _fileList.size()-1) return;
        this->setFileIndex(_fileList.size() - 1);
    }

    void FITKDynamicStruPostVTK::setCurFrame(int curFrame)
    {
        if (_isH5) {
            QPair<int, int> pair = this->getH5FileStep();
            int totalStep = pair.second;
            if (curFrame >= totalStep || curFrame < 0) {
                return;
            }
            if (QFile::exists(_meshFile) && QFile::exists(_resultFile)) {
                this->setInputFile(_meshFile, _resultFile, curFrame);
            }
            return;
        }
        if (curFrame >= _fileList.size() || curFrame < 0) {
            return;
        }
        //当前文件索引
        const int index = this->getFileIndex();
        if(curFrame == index){
            return;
        }
        this->setFileIndex(curFrame);
    }

    int FITKDynamicStruPostVTK::frameNum()
    {
        if (_isH5) {
            return this->getH5FileStep().second;
        }
        return _fileList.size();
    }

    int FITKDynamicStruPostVTK::getCurFrame()
    {
        if (_isH5) {
            return this->getH5FileStep().first;
        }
        return this->getFileIndex();
    }

    void FITKDynamicStruPostVTK::updateText()
    {
        if (_isH5) {
            QFile fMesh(_meshFile);
            QFile fResult(_resultFile);
            QFileInfo meshInfo(fMesh);
            QFileInfo resultInfo(fResult);

            QString meshName = meshInfo.fileName();
            QString resultName = resultInfo.fileName();
            QPair<int, int> steps = this->getH5FileStep();
            QString message = QString("Mesh File : %1 \nResult File: %2\nCurrent Step: %3 Total Steps: %4").arg(meshName).arg(resultName)
                .arg(steps.first).arg(steps.second - 1);
            _textData->setText(message);
            return;
        }
        if (_textData) {
            QString filePath = (getInputFile());
            QFile file(filePath);
            QFileInfo fileInfo(file);
            QString fileName = fileInfo.fileName();
            //QString message = QString("VTK: %1  \nStep: %2  \nIncrement %3  \nStep Time = %4").arg(fileName).arg("").arg("").arg("").arg("");
            QString message = QString("VTK: %1 ").arg(fileName);
            _textData->setText(message);
        }
    }

    void FITKDynamicStruPostVTK::setFileIndex(const int index)
    {
        if (index >= _fileList.size() || index < 0) return;
        const QString f = _fileList.at(index);
        if (!QFile::exists(f)) return;
        this->setInputFile(f);
    }

    int FITKDynamicStruPostVTK::getFileIndex()
    {
        QString currentFile = this->getInputFile();
        return _fileList.indexOf(currentFile);
    }

}



