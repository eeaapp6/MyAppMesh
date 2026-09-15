/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractProgramDriver.h"
#include "FITKAppFramework.h"
#include "FITKSignalTransfer.h"
#include <QFileSystemWatcher>

namespace AppFrame
{

    /**
     * @brief FITKProgramInputInfo 类的析构函数
     */
    FITKProgramInputInfo::~FITKProgramInputInfo()
    {
    }
    /**
     * 设置程序输入信息的参数。
     *
     * 此函数用于设置FITKProgramInputInfo对象的参数。参数通过 QStringList 类型的 args 传递。
     * 目前，该函数实现为空，但预留了未来扩展的可能，用于处理输入参数的解析和配置。
     *
     * @param args 参数列表，以 QStringList 的形式传递。
     */
    void FITKProgramInputInfo::setArgs(const QStringList& args)
    {
    }

    /**
     * @brief 获取程序输入参数
     *
     * 该函数旨在返回程序的输入参数列表。当前实现中，它返回一个空的QStringList。
     * 在未来的开发中，这个函数可能会被用于返回实际的程序输入参数。
     *
     * @return QStringList 程序输入参数列表。当前返回空列表。
     */
    QStringList FITKProgramInputInfo::args()
    {
        return QStringList();
    }

    void FITKProgramInputInfo::setMonitorFiles(QStringList &monitorFiles)
    {

    }

    QStringList FITKProgramInputInfo::getMonitorFiles()
    {
        return QStringList();
    }

    void FITKProgramInputInfo::setMonitorPath(QStringList &monitorpath)
    {

    }

    QStringList FITKProgramInputInfo::getMonitorPath()
    {
        return QStringList();
    }

    FITKAbstractProgramerDriver::~FITKAbstractProgramerDriver()
    {
        if (_inputInfo) delete _inputInfo;
        if (_fileWatcher != nullptr) delete _fileWatcher;
    }

    /**
     * @brief 获取程序类型
     *
     * 该函数用于返回当前编程器支持的程序类型。然而，在这个实现中，它简单地返回-1，
     * 这可能表示不支持或未实现此功能。在继承这个类并实现这个功能时，
     * 应该根据具体的编程器类型返回相应的程序类型。
     *
     * @return int 程序类型。返回-1表示不支持或未实现。
     */
    int FITKAbstractProgramerDriver::getProgramType()
    {
        return -1;
    }

    QString FITKAbstractProgramerDriver::getProgramName()
    {
        return QString();
    }

    void FITKAbstractProgramerDriver::setInputInfo(FITKProgramInputInfo* info)
    {
        _inputInfo = info;
    }

    AppFrame::FITKProgramInputInfo* FITKAbstractProgramerDriver::getProgramInput()
    {
        return _inputInfo;
    }

    void FITKAbstractProgramerDriver::start()
    {

    }

    void FITKAbstractProgramerDriver::stop()
    {

    }

    /**
     * @brief 发送消息给程序驱动器
     *
     * 此函数用于向程序驱动器发送特定类型的消息。它首先检查FITKAPP是否为空，
     * 如果为空，则直接返回。然后，它获取信号传输对象，并检查该对象是否为空。
     * 如果信号传输对象不为空，它将发送消息给程序驱动器。
     *
     * @param messageType 消息的类型，用于标识不同类型的消息。
     * @param message 要发送的实际消息内容。
     */
    void FITKAbstractProgramerDriver::sendMessage(int messageType, const QString& message)
    {
        // 检查FITKAPP是否已经被初始化，如果没有，则直接返回。
        if (FITKAPP == nullptr) return;

        // 从FITKAPP中获取信号传输对象。
        FITKSignalTransfer* transfer = FITKAPP->getSignalTransfer();

        // 检查信号传输对象是否为空，如果为空，则直接返回。
        if (transfer == nullptr) return;

        // 发送消息 。
        emit transfer->sendProgramDriverMessageSig(this, messageType, message);
    }

    /**
     * 监视文件目录。
     *
     * 此函数用于启动或停止对特定文件和目录的监视。当文件或目录发生变化时，会触发相应的信号，
     * 从而调用连接的槽函数进行处理。如果不需要监视，可以通过将m参数设置为false来停止监视。
     *
     * @param m 指示是否开始监视的布尔值，默认为true，表示开始监视。
     */
    void FITKAbstractProgramerDriver::monitorFileDirectories(bool m /*= true*/)
    {
        // 如果已经存在文件监视器实例，则先释放资源
        if (_fileWatcher != nullptr)
            delete _fileWatcher;
        _fileWatcher = nullptr;

        // 如果不需监视（m为false）或输入信息为空，则直接返回
        if (!m || _inputInfo == nullptr)
            return;

        // 创建新的文件监视器实例
        _fileWatcher = new QFileSystemWatcher;

        // 获取需要监视的文件和目录路径
        QStringList f = _inputInfo->getMonitorFiles();
        QStringList p = _inputInfo->getMonitorPath();

        // 将文件和目录路径添加到文件监视器中
        _fileWatcher->addPaths(p);
        _fileWatcher->addPaths(f);

        // 连接文件变化和目录变化的信号到相应的槽函数
        connect(_fileWatcher, &QFileSystemWatcher::fileChanged, this,&FITKAbstractProgramerDriver::fileChangedSlot);
        connect(_fileWatcher, &QFileSystemWatcher::directoryChanged,this, &FITKAbstractProgramerDriver::directoryChangedSlot);
    }

    /**
     * 文件更改槽函数。
     *
     * 当监视的文件发生变化时，此槽函数被触发。它通过应用的信号传输器发出一个信号，
     * 指示文件已更改，并传递发生变化的文件路径。
     *
     * @param file 发生变化的文件路径。
     */
    void FITKAbstractProgramerDriver::fileChangedSlot(const QString& file)
    {
        if (FITKAPP == nullptr)  return;
        FITKSignalTransfer* transfer = FITKAPP->getSignalTransfer();
        if (transfer == nullptr)  return;
        emit transfer->fileDirectoryChangedSig(this, true, file);
    }

    /**
     * 目录更改槽函数。
     *
     * 当监视的目录发生变化时，此槽函数被激活。功能类似于`fileChangedSlot`，但针对的是目录变更，
     * 并通过信号传输器发出目录变化的信号。
     *
     * @param dir 发生变化的目录路径。
     */
    void FITKAbstractProgramerDriver::directoryChangedSlot(const QString& dir)
    {
        if (FITKAPP == nullptr)  return;
        FITKSignalTransfer* transfer = FITKAPP->getSignalTransfer();
        if (transfer == nullptr)  return;
        emit transfer->fileDirectoryChangedSig(this, true, dir);
    }

}


