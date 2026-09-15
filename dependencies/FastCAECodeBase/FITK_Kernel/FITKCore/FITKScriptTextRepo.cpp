/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKScriptTextRepo.h"
#include <QFile>
#include <QTextStream>

namespace Core
{
    //静态变量初始化
    static FITKScriptTextRepoDeleator ____SCRIPTREPODELEATOR___;
    QMutex FITKScriptTextRepo::m_mutex;
    FITKScriptTextRepo* FITKScriptTextRepo::_instance = nullptr;

    FITKScriptTextBlock::FITKScriptTextBlock(const QString& textBlock)
        :_texts(textBlock)
    {
    }

    FITKScriptTextBlock::FITKScriptTextBlock(const QStringList& textBlock)
        :_texts(textBlock)
    {
    }

    void FITKScriptTextBlock::appendText(const QString& text)
    {
        _texts.append(text);
    }

    QStringList FITKScriptTextBlock::getText() const
    {
        return _texts;
    }

    void FITKScriptTextRepo::appendScript(const QStringList& script)
    {
        //不记录
        if (!_isEnableRecord) return;
        //生成新文本块
        FITKScriptTextBlock block(script);
        _textBlocks.append(block);
    }

    void FITKScriptTextRepo::appendScript(const QString& script)
    {
        //不记录
        if (!_isEnableRecord) return;
        //生成新的文本块
        FITKScriptTextBlock block(script);
        _textBlocks.append(block);
    }

    void FITKScriptTextRepo::clearRepo()
    {
        //清空
        _textBlocks.clear();
    }

    void FITKScriptTextRepo::removeLastBlock()
    {
        //移除最后一个
        _textBlocks.removeLast();
    }

    void FITKScriptTextRepo::initialize()
    {
        _textBlocks.clear();
    }

    void FITKScriptTextRepo::finalize()
    {
        _textBlocks.clear();
    }

    bool FITKScriptTextRepo::exportScriptFile(const QString& fileName)
    {
        //空的文本
        if (_textBlocks.isEmpty()) return false;

        QFile f(fileName);
        //打开文件写出
        if (!f.open(QIODevice::WriteOnly)) return false;
        QTextStream s(&f);
        //遍历文本块
        for (FITKScriptTextBlock block : _textBlocks)
        {
            QStringList text = block.getText();
            //遍历文本
            for (QString t : text)
                s << t << endl;
        }
        f.close();
        return true;
    }
    void FITKScriptTextRepo::enableRecord(bool en)
    {
        _isEnableRecord = en;
    }
    bool FITKScriptTextRepo::isRecordEnabled()
    {
        return _isEnableRecord;
    }
}


