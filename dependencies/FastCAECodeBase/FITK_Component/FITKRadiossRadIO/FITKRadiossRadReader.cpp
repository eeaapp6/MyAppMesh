/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossRadReader.h"
#include "FITKRadiossAdaptorCase.h"
#include "RadReadInformationMapper.h"
#include "RadKeyLineSpliter.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
namespace Radioss
{
    FITKRadiossRadReader::FITKRadiossRadReader(bool & success)
        : _success(success)
    {
        _keyLineSpliter = new RadKeyLineSpliter();
        _informationReadRadMapper = new RadReadInformationMapper();
    }

    FITKRadiossRadReader::~FITKRadiossRadReader()
    {
        if(nullptr != _keyLineSpliter)
        {
            delete _keyLineSpliter;
            _keyLineSpliter = nullptr;
        }
        if(nullptr != _informationReadRadMapper)
        {
            delete _informationReadRadMapper;
            _informationReadRadMapper = nullptr;
        }
    }

    void FITKRadiossRadReader::run()
    {
        // 执行读取操作
        //获取算例数据
        FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        _informationReadRadMapper->setCaseData(caseData);
        if (!caseData)
        {
            consoleMessage(3, "Failed to get Radioss case data.");
            _success = false;
            return; 
        }
        this->clearSolverSettingRequest();
        bool ok = this->loadFile();
        if (!ok) return;

        do
        {
            const QString line = this->readLine();
            if (line.isEmpty()) continue;
            if (!line.startsWith("/")) continue; 
            //如果遇到/END关键字，剩余内容全部忽略
            if (line == "/END") 
                continue;
            // 处理关键字行
            FITKRadiossAbstractAdaptor* adaptor = this->createAdaptor(line);
            if (!adaptor)
            {
                this->consoleMessage(2, QString("No adaptor found for key line: %1").arg(line));
                continue;
            }
            adaptor->setDataObject(_data);
            adaptor->setFileReader(this);
            adaptor->setCaseData(caseData);
            adaptor->adaptR();
            delete adaptor;
        } while (!this->atEnd());

        //执行数据映射
        if(_informationReadRadMapper)
            _informationReadRadMapper->map();

        //读取完成
        this->sendCurrentPercent(100);
    }

    bool FITKRadiossRadReader::loadFile()
    {
        //判断是不是两个文件
        QStringList fileToRead;
        fileToRead << _fileName;
        //后缀是"rad"或者“RAD”
        const QString suffix = _fileName.right(3);
        if(_fileName.endsWith("0000."+suffix, Qt::CaseSensitive) )
        {
            fileToRead << _fileName.replace("0000."+suffix, "0001."+suffix);
        }
        else if (_fileName.endsWith("0001."+suffix, Qt::CaseSensitive))
        {
            fileToRead .insert(0, _fileName.replace("0001."+suffix, "0000."+suffix));
        }
        QString OPath = QFileInfo(fileToRead[0]).absolutePath();
        //加载文件
        for (int i = 0;i< fileToRead.size();++i)
        {
            QString file = fileToRead.value(i);
            QFile f(file);
            //文件存在性
            if (f.exists() && f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                //支持两种换行符
                QByteArray context = f.readAll();
                QByteArrayList contextList = context.split('\n');

                //字符数大于100且没有换行符 认为分割失败，尝试使用\r进行分割
                if(context.size() > 100 && contextList.size() < 2)
                {
                    //如果没有换行符，则直接读取
                    contextList = context.split('\r\n');
                }

                _totalContext << contextList;
                //获取内嵌文件路径
                foreach(const QByteArray& byteArr, contextList) 
                {
                    QString str = QString::fromUtf8(byteArr);
                    if (str.contains("#include"))
                    {
                        QStringList pathList = str.split(" ", Qt::SkipEmptyParts);
                        for (int i = 1; i < pathList.size();++i) 
                        {
                            QString path = pathList.value(i).trimmed();
                            if (path.isEmpty())
                                continue;

                            path.replace("\\", "/");
                            path.replace("//", "/");
                            while (path.startsWith("./"))
                            {
                                path = path.mid(2);
                            }

                            if (!QDir::isAbsolutePath(path))
                            {
                                path = QDir(OPath).filePath(path);
                            }
                            path = QDir::cleanPath(path);

                            fileToRead.insert(fileToRead.size()-1, path);
                        }
                    }
                }

            }
            f.close();
              
        }
        _totalLine = _totalContext.size();
        return _totalContext.size() > 2; //至少有两行数据
    }

    RadReadInformationMapper * FITKRadiossRadReader::getReadRadInformationMapper()
    {
        return _informationReadRadMapper;
    }

    RadKeyLineSpliter *FITKRadiossRadReader::getKeyLineSpliter()
    {
        return _keyLineSpliter;
    }

    void FITKRadiossRadReader::consoleMessage(int level, const QString& str)
    {
        // 打印控制台消息
        switch (level)
        {
        case 1:
            AppFrame::FITKMessageNormal(str);
            break;
        case 2:
            AppFrame::FITKMessageWarning(str);
            break;
        case 3:
            AppFrame::FITKMessageError(str);
            break;
        default:
            AppFrame::FITKMessageError(str);
            break;
        }
    }

    FITKRadiossAbstractAdaptor *FITKRadiossRadReader::createAdaptor(const QString & line)
    {
        // 获取适配器
        if (nullptr == _keyLineSpliter) return nullptr;
        _keyLineSpliter->setText(line);
        
        const int n = _keyLineSpliter->getKeyWordCount();
        if (n <= 0) return nullptr;
        QString key;
        for (int i = 0; i < n; i++)
        {
            key += _keyLineSpliter->getKey(i);
            // 尝试查找适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKRadiossAbstractAdaptor::createAdaptor(key);
            if (adaptor) return adaptor;
        }
        // 如果没有找到适配器，则返回nullptr
        return nullptr;
    }

    void Radioss::FITKRadiossRadReader::clearSolverSettingRequest()
    {
        //获取算例数据
        FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData) return;
        FITKRadiossSolverSettings* setting = caseData->getSolverSettings();
        FITKRadiossResultRequest* request = caseData->getResultRequest();
        if (!setting || !request) return;
        setting->clearParam();
        request->clearParam();
    }

    void FITKRadiossRadReader::setSkipBlankLine(bool isSkip)
    {
        _isSkipBlankLine = isSkip;
    }

    QString FITKRadiossRadReader::readLine()
    {
        QString line;
        while (!atEnd())
        {
            line = FITKAbstractTextReader::readLine();
            // 空行跳过
            /*if (_isSkipBlankLine && line.isEmpty())
                continue;*/
            // 注释行跳过
            if (line.startsWith("#")) 
                continue;
            break;
        }
        return line;
    }

    void FITKRadiossRadReader::backLine()
    {
        FITKAbstractTextReader::backLine();
        if (_isSkipBlankLine && this->getCurrentLine().isEmpty() && this->getCurrentLineNum() > 1)
            this->backLine();
    }
}
