/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFileDirectoryDialog.h"
#include "ui_FITKFileDirectoryDialog.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopServices>
#include <QStandardItemModel>
#include <QDir>
#include <QCollator>
#include <QStyle>
#include <QFileInfo>
//QDirModel
#include <QDirModel>
#include <QDesktopServices>
#include <QStandardItemModel>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <objbase.h>
#include <shobjidl.h>
#endif

namespace GUI
{
    const QString FITKFileDirectoryDialog::fatherNodeText = "FileName"; //根节点名称
    const QString FITKFileDirectoryDialog::splitExt = "@";    //用于文件头类型分割符，如12@dat

    QString FITKFileDirectoryDialog::appendDirectorySeparator(const QString& path)
    {
        QString normalizedPath = QDir::cleanPath(path);
        normalizedPath.replace("\\", "/");
        if (normalizedPath.right(1) != "/")
        {
            normalizedPath += "/";
        }
        return normalizedPath;
    }

    QString FITKFileDirectoryDialog::resolveWindowsShortcutTarget(const QString& shortcutPath)
    {
        QString targetPath;
#ifdef Q_OS_WIN
        const HRESULT initResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        const bool needUninitialize = (initResult == S_OK || initResult == S_FALSE);
        if (FAILED(initResult) && initResult != RPC_E_CHANGED_MODE)
        {
            return targetPath;
        }

        IShellLinkW* shellLink = nullptr;
        const HRESULT createResult = CoCreateInstance(CLSID_ShellLink, nullptr,
            CLSCTX_INPROC_SERVER, IID_IShellLinkW, reinterpret_cast<void**>(&shellLink));
        if (FAILED(createResult) || shellLink == nullptr)
        {
            if (needUninitialize == true)
            {
                CoUninitialize();
            }
            return targetPath;
        }

        IPersistFile* persistFile = nullptr;
        const HRESULT queryResult = shellLink->QueryInterface(IID_IPersistFile,
            reinterpret_cast<void**>(&persistFile));
        if (SUCCEEDED(queryResult) && persistFile != nullptr)
        {
            const std::wstring nativeShortcutPath = QDir::toNativeSeparators(shortcutPath).toStdWString();
            const HRESULT loadResult = persistFile->Load(nativeShortcutPath.c_str(), STGM_READ);
            if (SUCCEEDED(loadResult))
            {
                WIN32_FIND_DATAW findData;
                wchar_t resolvedPath[MAX_PATH] = { 0 };
                const HRESULT getPathResult = shellLink->GetPath(resolvedPath, MAX_PATH, &findData,
                    SLGP_UNCPRIORITY | SLGP_RAWPATH);
                if (SUCCEEDED(getPathResult) && resolvedPath[0] != L'\0')
                {
                    targetPath = QDir::fromNativeSeparators(QString::fromWCharArray(resolvedPath));
                }
            }
            persistFile->Release();
        }

        shellLink->Release();
        if (needUninitialize == true)
        {
            CoUninitialize();
        }
#else
        Q_UNUSED(shortcutPath);
#endif
        return targetPath;
    }

    FITKFileDirectoryDialog::FITKFileDirectoryDialog(Core::FITKActionOperator* oper, QWidget *parent)
        : Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::FITKFileDirectoryDialog;
        _ui->setupUi(this);

        //ini ui
        initUi();

        //
        connect(_ui->OKChoice, SIGNAL(clicked()), this, SLOT(onOkClicked()));
        connect(_ui->Cancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
        connect(_ui->back, SIGNAL(clicked()), this, SLOT(onBackClicked()), Qt::UniqueConnection);
        connect(_ui->front, SIGNAL(clicked()), this, SLOT(onFrontClicked()), Qt::UniqueConnection);
        connect(_ui->up, SIGNAL(clicked()), this, SLOT(onUpPushButtonClicked()));
        connect(_ui->FileType, SIGNAL(currentIndexChanged(QString)), this, SLOT(onFilterChange(QString)));
        connect(_ui->FilePath, SIGNAL(returnPressed()), this, SLOT(onFilePathReturnPressed()));
        connect(_ui->FilePath, SIGNAL(editingFinished()), this, SLOT(onFilePathReturnPressed()));
        //
        connect(_ui->PathTree, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(pathTreeViewDoubleClick(const QModelIndex)));
        //
        _fileTree = _ui->FileTree;
        _fileTree->setSelectionMode(QAbstractItemView::SingleSelection); // 仅限单选
        _fileTree->setSelectionBehavior(QAbstractItemView::SelectRows);
        connect(_fileTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onDoubleClickedObject(QTreeWidgetItem*, int)));
        connect(_fileTree, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));

        QStringList names;
        names << FITKFileDirectoryDialog::fatherNodeText;
        _builtinItem = new QTreeWidgetItem(_fileTree, names);

        //set path
        _docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        _deskTopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        userName = "";
        rootName = "";
#ifdef Q_OS_UNIX
        userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        rootPath = "/";
        userName = userPath.section("/", -1, -1);
        rootName = "Computer";
#endif

        SetDirectoryForDrives();

        // ini path
        _currentFilePath = _ui->FilePath->text();
        _currentIndex = _dirModel->index(_currentFilePath);

        //
        filterChange(_ui->FileType->currentText());
        refreshRightView();
    }

    FITKFileDirectoryDialog::~FITKFileDirectoryDialog()
    {
        _fileTree->clear();
        _pathBackVec.clear();
        _pathFrontVec.clear();
    }

    void FITKFileDirectoryDialog::initUi()
    {
        //ini combox
        _ui->FilePath->setReadOnly(false);

        _currentFilePath = QDir::cleanPath(qApp->applicationDirPath());
        _treeWidgetFilename = _currentFilePath;
        _ui->FilePath->setText(_currentFilePath);

        //ini button icons
        QIcon iconBack;
        iconBack.addFile(QString::fromUtf8(":/back.png"));
        QIcon iconFront;
        iconFront.addFile(QString::fromUtf8(":/front.png"));
        QIcon iconUp;
        iconUp.addFile(QString::fromUtf8(":/up.png"));
        _ui->back->setIcon(iconBack);
        _ui->front->setIcon(iconFront);
        _ui->up->setIcon(iconUp);

        _ui->back->setEnabled(false);
        _ui->front->setEnabled(false);

        //ini spliter
        _ui->splitter->setStretchFactor(0, 3);
        _ui->splitter->setStretchFactor(1, 7);
    }

    void FITKFileDirectoryDialog::BuildFilePathTree()
    {
        _fileTree->clear();
        QStringList names;
        names << FITKFileDirectoryDialog::fatherNodeText;
        _builtinItem = new QTreeWidgetItem(_fileTree, names);
        QTreeWidgetItem* parentItem = _builtinItem;
        QMap<QString, QVector<QString>>::iterator it;
        for (it = _fileGroup.begin(); it != _fileGroup.end(); it++)
        {
            QString FilePath = it.key();
            QVector<QString> FileNames = it.value();
            QTreeWidgetItem *itm = new QTreeWidgetItem(_fileTree);
            itm->setText(0, FilePath);
            itm->setData(0, Qt::UserRole + 1, "Group");
            if (FileNames.count() > 1)
            {
                parentItem->addChild(itm);
                for (int j = 0; j < FileNames.count(); j++)
                {
                    QTreeWidgetItem *itmchild = new QTreeWidgetItem(itm);
                    itmchild->setText(0, FileNames[j]);
                    itmchild->setData(0, Qt::UserRole + 1, "Child");
                    itm->addChild(itmchild);
                }
            }
        }
    }


    void FITKFileDirectoryDialog::initWin(QString dirStr)
    {
        if (dirStr.isEmpty()) return;

        QDir dir(dirStr);
        if (!dir.exists()) return;

        _ui->FilePath->setText(dirStr);
        filterChange(_ui->FileType->currentText());
        refreshRightView();
        _currentIndex = _dirModel->index(dirStr);
        _treeWidgetFilename = dirStr;
    }

    QString FITKFileDirectoryDialog::appendSlash(QString curPath)
    {
        int index = curPath.lastIndexOf("/", curPath.size());
        if ((index <= 0) && (curPath.size() > 3))
        {
            curPath = curPath + "/";
        }
        return curPath;
    }

    void FITKFileDirectoryDialog::openFilePath(bool recordHistory)
    {
        // 前进后退不记录历史路径信息，正常打开文件记录文件信息并添加"\"后缀
        if (recordHistory)
        {
            QString strPath = _currentFilePath;
            if (strPath.right(1) == "/")
            {
                strPath = _currentFilePath.mid(0, _currentFilePath.length() - 1);
            }
            _pathBackVec.push_back(strPath);
            _ui->back->setEnabled(true);
        }

        _currentFilePath = _treeWidgetFilename;
        _ui->FilePath->setText(_currentFilePath);


        if (!_currentFilePath.isEmpty())
        {
            _ui->FileName->setText("");
            filterChange(_ui->FileType->currentText());
        }
    }

    QString FITKFileDirectoryDialog::GetFileExt(QString fileName)
    {
        QFileInfo fileInfo;
        fileInfo = QFileInfo(fileName);
        return fileInfo.suffix();
    }

    QStringList FITKFileDirectoryDialog::GetFilterFromString(const QString& filter)
    {
        QString f = filter;
        int start, end;
        start = filter.indexOf('(');
        end = filter.lastIndexOf(')');
        if (start != -1 && end != -1)
        {
            f = f.mid(start + 1, end - start - 1);
        }
        else if (start != -1 || end != -1)
        {
            f = QString();
        }
        QStringList fs = f.split(QRegExp("[\\s+;]"), QString::SkipEmptyParts);

        return fs;
    }

    void FITKFileDirectoryDialog::onDoubleClickedObject(QTreeWidgetItem* pitem, int column)
    {
        QString name = pitem->text(column);

        int rowNum = 0;
#ifdef Q_OS_WIN
        while (true)
        {
            QModelIndex index = _currentIndex.child(rowNum, 0);
            if (!index.isValid())
            {
                //error
                return;
            }
            if (index.data() == name)
            {
                int dotIndex = name.lastIndexOf(".");
                QString fileExt = name.mid(dotIndex, name.length() - dotIndex);
                // 包含在当前过滤文件类型中即可
                if (_ui->FileType->currentText().indexOf(fileExt) != -1)
                {
                    break;
                }
                func_PathTreeViewDoubleClick(index);
                return;
            }
            rowNum++;
        }
#endif
#ifdef Q_OS_UNIX
        while (true)
        {
            QString fileExt = name.right(name.indexOf("."));
            if (_ui->FileType->currentText().indexOf(fileExt) != -1)
            {
                break;
            }
            unix_PathTreeViewDoubleClick(name);
            return;
            rowNum++;
        }
#endif
        _ui->OKChoice->click();
    }


    void FITKFileDirectoryDialog::onFilterChange(QString filter)
    {
        _filterList.clear();
        _filterList = GetFilterFromString(filter);

        enterDirectory();

        refreshRightView();
    }

    void FITKFileDirectoryDialog::filterChange(QString filter)
    {
        _filterList.clear();
        _filterList = GetFilterFromString(filter);

        enterDirectory();
    }

    void FITKFileDirectoryDialog::GetFileList(QString dirName)
    {
        _fileList.clear();

        QFileInfo fileInfo;
        QString filePath;

        fileInfo = QFileInfo(dirName);
        filePath = fileInfo.absolutePath();
        QDir dir(dirName);
        if (!dir.exists())
            return;
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot); //QDir::Dirs;
        dir.setSorting(QDir::Name);

        dir.setNameFilters(_filterList);
        QFileInfoList list = dir.entryInfoList();
        if (list.count() > 0)
        {
            for (int i = 0; i < list.count(); i++)
            {
                _fileList.push_back(list.at(i).fileName());
            }
        }

        //对文件进行大小排序
        QCollator collator;
        collator.setNumericMode(true);
        std::sort(_fileList.begin(), _fileList.end(),
            [&collator](const QString & str1, const QString & str2)
        {
            return collator.compare(str1, str2) < 0;
        });
    }

    QStringList FITKFileDirectoryDialog::getCurrentFullPath()
    {
        return getSelectedFullPaths();
    }

    bool FITKFileDirectoryDialog::isGourpFiles(const QString path)
    {
        if (path.indexOf("...") > 0)
        {
            return true;
        }
        return false;
    }

    void FITKFileDirectoryDialog::iniFileFilterType(QString filterString)
    {
        _ui->FileType->insertItem(0, filterString);
        _ui->FileType->setCurrentIndex(0);
    }

    void FITKFileDirectoryDialog::onCancelClicked()
    {
        close();
    }

    void FITKFileDirectoryDialog::onOkClicked()
    {
        if (_selectionText.isEmpty()) return;

        QString filePath = _currentFilePath;
        const QStringList fullPaths = getSelectedFullPaths();
        if (fullPaths.count() > 0)
        {
            QFileInfo fileInfo(fullPaths.first());
            filePath = appendDirectorySeparator(fileInfo.absolutePath());
        }
        else
        {
            // 暂时解决 C:/Users/Administrator/Desktop/valve.cgns/valve.cgns
            // 路径中文件名字重复的问题
            QString f = _currentFilePath + _selectionText;
            QStringList fs = f.split(QRegExp("[/]"), QString::SkipEmptyParts);
            if (fs[fs.count() - 1] == fs[fs.count() - 2])
            {
                QString path = _currentFilePath.mid(0, _currentFilePath.size() - 1);
                int index = path.lastIndexOf("/");
                filePath = path.left(index + 1);
            }
        }

        //执行操作器
        if (_oper) {
            _oper->setArgs("FilePath", filePath);
            _oper->setArgs("FileNames", fullPaths);
            _oper->execProfession();
        }
        accept();
    }

    void FITKFileDirectoryDialog::enterDirectory()
    {
        QString tempFileHeader;
        QString pathText = _ui->FilePath->text().trimmed();
        QFileInfo pathInfo(pathText);
        if (pathInfo.exists() == true && pathInfo.isFile() == true)
        {
            _currentFilePath = appendDirectorySeparator(pathInfo.absolutePath());
        }
        else
        {
            _currentFilePath = pathText;
            if (_currentFilePath.right(1) != "/")
            {
                _currentFilePath += "/";
            }
        }

        GetFileList(_currentFilePath);

        _fileGroup.clear();
        _headFile.reserve(_fileList.count() * 2);
        _headFile.clear();
        for (int i = 0; i < _fileList.count(); i++)
        {
            QString fileName = _fileList.at(i);
            QString fileExt = GetFileExt(fileName);
            QString fileHead, fileHeadExt;
            int endPos = fileName.length() - fileExt.length() - 2, beginPos;
            for (beginPos = endPos; beginPos >= 0; --beginPos) {
                if (fileName[beginPos] > '9' || fileName[beginPos] < '0') break;
            }
            fileHead = fileName.mid(0, beginPos + 1);
            fileHeadExt = fileHead + FITKFileDirectoryDialog::splitExt + fileExt;
            if (_headFile.contains(fileHeadExt) == false)
            {
                _headFile.insert(fileHeadExt, 1);
            }
            else
            {
                _headFile[fileHeadExt] += 1;
            }
        }

        QString headExt, headFront;
        QMap<QString, int>::iterator it;
        for (int i = 0; i < _fileList.count(); i++)
        {
            QString fileName = _fileList.at(i);
            QString fileExt = GetFileExt(fileName);
            QString fileHead, fileHeadExt, NodeText;
            int endPos = fileName.length() - fileExt.length() - 2;
            int beginPos;
            for (beginPos = endPos; beginPos >= 0; --beginPos) {
                if (fileName[beginPos] > '9' || fileName[beginPos] < '0') break;
            }
            fileHead = fileName.mid(0, beginPos + 1);
            fileHeadExt = fileHead + FITKFileDirectoryDialog::splitExt + fileExt;
            if (_headFile[fileHeadExt] > 1)
            {
                NodeText = fileHead + "..." + fileExt;
            }
            else
            {
                NodeText = fileName;
            }
            if (_fileGroup.contains(NodeText) == false)
                _fileGroup.insert(NodeText, QVector<QString>());
            _fileGroup[NodeText].push_back(fileName);
        }
        BuildFilePathTree();
    }

    void FITKFileDirectoryDialog::SetDirectoryForDrives()
    {
        QStandardItemModel* tep_model = new QStandardItemModel;
        QStyle* style = QApplication::style();
        QIcon desktopIcon = style->standardIcon(QStyle::SP_DesktopIcon);
        QIcon documentIcon = style->standardIcon(QStyle::SP_DirHomeIcon);
        QIcon driveIcon = style->standardIcon(QStyle::SP_DriveHDIcon);
        QIcon directoryIcon = style->standardIcon(QStyle::SP_DirIcon);

#ifdef Q_OS_WIN

        _dirModel = new QDirModel;

        QStringList pathTreeHead;
        pathTreeHead.append("Disk List");
        tep_model->setHorizontalHeaderLabels(pathTreeHead);

        _dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
        _dirModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

        int num_row = _dirModel->rowCount(QModelIndex());

        QStandardItem* desktopItem = new QStandardItem(desktopIcon, tr("DeskTop"));
        QStandardItem* documentItem = new QStandardItem(documentIcon, tr("My Documents"));
        tep_model->appendRow(desktopItem);
        tep_model->appendRow(documentItem);


        for (int i = 0; i < num_row; i++)
        {
            QStandardItem* driveItem = new QStandardItem(driveIcon, _dirModel->index(i, 0).data().toString());
            tep_model->appendRow(driveItem);
        }

#endif
#ifdef Q_OS_UNIX
        _dirModel = new QDirModel;
        _dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
        _dirModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

        QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        QString userName = userPath.section("/", -1, -1);
        QStandardItem* rootItem = new QStandardItem(driveIcon, rootName);
        QStandardItem* userItem = new QStandardItem(directoryIcon, userName);
        tep_model->appendRow(rootItem);
        tep_model->appendRow(userItem);

#endif 
        _ui->PathTree->setModel(tep_model);
        _ui->PathTree->setAnimated(true);
        _ui->PathTree->setIndentation(20);
        _ui->PathTree->setSortingEnabled(false);
        //_ui->PathTree->header()->hide();
        _ui->PathTree->hideColumn(1);
        _ui->PathTree->hideColumn(2);
        _ui->PathTree->hideColumn(3);

        _ui->PathTree->setItemsExpandable(false);
        _ui->PathTree->resizeColumnToContents(0);
        _ui->PathTree->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        _ui->PathTree->show();
    }

    void FITKFileDirectoryDialog::onItemSelectionChanged()
    {
        QList<QTreeWidgetItem*> selectItems = _fileTree->selectedItems();
        _selectedFullPaths.clear();

        if (selectItems.count() > 0)
        {
            _selectionText = "";
        }

        for (int i = 0; i < selectItems.count(); i++)
        {
            QTreeWidgetItem *curitem = selectItems.at(i);
            if (curitem->text(0).contains("FileName"))
            {
                continue;
            }

            QString currentText = curitem->text(0);
            QString displayText = currentText;
            if (currentText.indexOf("...") > 0)
            {
                _selectedFullPaths = getSelectGroupFiles(QString("%1%2").arg(_currentFilePath).arg(currentText));
            }
            else
            {
                QString selectedPath = QString("%1%2").arg(_currentFilePath).arg(currentText);
                selectedPath = QDir::cleanPath(selectedPath);
#ifdef Q_OS_WIN
                QFileInfo fileInfo(selectedPath);
                if (fileInfo.suffix().compare("lnk", Qt::CaseInsensitive) == 0)
                {
                    const QString targetPath = resolveWindowsShortcutTarget(selectedPath);
                    if (targetPath.isEmpty() == false)
                    {
                        selectedPath = QDir::cleanPath(targetPath);
                        displayText = selectedPath;
                    }
                }
#endif
                _selectedFullPaths.clear();
                _selectedFullPaths.append(selectedPath);
            }

            if (i == selectItems.size() - 1)
            {
                _selectionText += displayText;
            }
            else
            {
                _selectionText += displayText;
                _selectionText += ";";
            }
        }

        _ui->FileName->setText(_selectionText);
        if (_selectedFullPaths.count() == 1)
        {
            _ui->FilePath->setText(_selectedFullPaths.first());
        }
        else
        {
            _ui->FilePath->setText(_currentFilePath);
        }
    }

    void FITKFileDirectoryDialog::func_PathTreeViewDoubleClick(const QModelIndex& index)
    {
        _currentIndex = index;

        if ((index.data().toString() == ".") | (index.data().toString() == ".."))
            return;

        _treeWidgetFilename = FindFullFilename(index);

        if (_treeWidgetFilename.indexOf("C:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "C:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "C:");
        }
        else if (_treeWidgetFilename.indexOf("D:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "D:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "D:");
        }
        else if (_treeWidgetFilename.indexOf("E:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "E:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "E:");
        }
        else if (_treeWidgetFilename.indexOf("F:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "F:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "F:");
        }
        else if (_treeWidgetFilename.indexOf("G:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "G:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "G:");
        }
        else if (_treeWidgetFilename.indexOf("H:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "H:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "H:");
        }
        else if (_treeWidgetFilename.indexOf("I:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "I:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "I:");
        }
        else if (_treeWidgetFilename.indexOf(tr("DeskTop")) >= 0)
        {
            _treeWidgetFilename = _deskTopPath;
        }
        else if (_treeWidgetFilename.indexOf(tr("My Documents")) >= 0)
        {
            _treeWidgetFilename = _docPath;
        }
        else return;

#ifdef Q_OS_WIN
        QFileInfo fileInfo(_treeWidgetFilename);
        if (fileInfo.suffix().compare("lnk", Qt::CaseInsensitive) == 0)
        {
            const QString targetPath = resolveWindowsShortcutTarget(_treeWidgetFilename);
            QFileInfo targetInfo(targetPath);
            if (targetPath.isEmpty() == false && targetInfo.exists() == true && targetInfo.isDir() == true)
            {
                _treeWidgetFilename = QDir::cleanPath(targetPath);
                _currentIndex = _dirModel->index(_treeWidgetFilename);
            }
        }
#endif

        openFilePath(true);
        refreshRightView();
    }

    void FITKFileDirectoryDialog::unix_PathTreeViewDoubleClick(QString itemName)
    {
        _treeWidgetFilename = _ui->FilePath->text();
        if (_treeWidgetFilename != "/")
            _treeWidgetFilename += "/";
        _treeWidgetFilename += itemName;
        openFilePath(true);

        refreshRightView();
    }

    void FITKFileDirectoryDialog::pathTreeViewDoubleClick(const QModelIndex& index)
    {
#ifdef Q_OS_WIN

        QString cur_name = index.data().toString();
        if ((cur_name == tr("DeskTop")) || (cur_name == tr("My Documents")))
        {
            _treeWidgetFilename = cur_name;
            if (cur_name == tr("DeskTop"))
                _currentIndex = _dirModel->index(_deskTopPath);
            else
                _currentIndex = _dirModel->index(_docPath);
        }
        else
        {
            int i;
            for (i = 0; i < _dirModel->rowCount(QModelIndex()); i++)
            {
                if (_dirModel->index(i, 0).data().toString() == index.data().toString())
                {
                    _currentIndex = _dirModel->index(i, 0);
                    break;
                }
            }
            if (i == _dirModel->rowCount(QModelIndex()))
                return;

            qDebug() << index.parent().data().toString() << endl;

            if ((index.data().toString() == ".") | (index.data().toString() == ".."))
                return;

            _treeWidgetFilename = FindFullFilename(index);
        }

        if (_treeWidgetFilename.indexOf("C:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "C:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "C:");
        }
        else if (_treeWidgetFilename.indexOf("D:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "D:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "D:");
        }
        else if (_treeWidgetFilename.indexOf("E:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "E:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "E:");
        }
        else if (_treeWidgetFilename.indexOf("F:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "F:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "F:");
        }
        else if (_treeWidgetFilename.indexOf("G:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "G:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "G:");
        }
        else if (_treeWidgetFilename.indexOf("H:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "H:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "H:");
        }
        else if (_treeWidgetFilename.indexOf("I:") != -1)
        {
            if (-1 == _treeWidgetFilename.indexOf("/", 1))
            {
                _treeWidgetFilename = "I:";
            }
            else
                _treeWidgetFilename.replace(0, _treeWidgetFilename.indexOf("/", 1), "I:");
        }
        else if (_treeWidgetFilename.indexOf(tr("DeskTop")) >= 0)
        {
            _treeWidgetFilename = _deskTopPath;
        }
        else if (_treeWidgetFilename.indexOf(tr("My Documents")) >= 0)
        {
            _treeWidgetFilename = _docPath;
        }
        else return;

        openFilePath(true);

#endif
#ifdef Q_OS_UNIX

        QString cur_name = index.data().toString();
        if ((cur_name == rootName) || (cur_name == userName))
        {
            _treeWidgetFilename = cur_name;
        }


        if (_treeWidgetFilename == rootName)
        {
            _treeWidgetFilename = rootPath;
        }
        else if (_treeWidgetFilename == userName)
        {
            _treeWidgetFilename = userPath;
        }
        else return;
        qDebug() << _treeWidgetFilename << endl;

        openFilePath(true);

#endif
        refreshRightView();
    }

    QString FITKFileDirectoryDialog::FindFullFilename(const QModelIndex& index)
    {
        if (index.data().toString() == NULL) return NULL;
        return FindFullFilename(index.parent()) + "/" + index.data().toString();
    }

    void FITKFileDirectoryDialog::refreshRightView()
    {
        QDir dir(_currentFilePath);
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

        QFileInfoList list = dir.entryInfoList();

        QStyle* style = QApplication::style();
        QIcon icon = style->standardIcon(QStyle::SP_DirIcon);
        for (int iCnt = 0; iCnt < list.size(); ++iCnt)
        {
            QTreeWidgetItem * item = new QTreeWidgetItem(_fileTree);
            item->setText(0, list.at(list.size() - 1 - iCnt).fileName());
            item->setIcon(0, icon);
            _fileTree->addTopLevelItem(item);
        }
    }

    void FITKFileDirectoryDialog::onUpPushButtonClicked()
    {
#ifdef Q_OS_WIN
        if (_currentIndex.parent().isValid())
        {
            _currentIndex = _currentIndex.parent();
            QDir dir(_treeWidgetFilename);
            if (dir.cdUp())
            {
                _treeWidgetFilename = dir.absolutePath();
                openFilePath(true);
                refreshRightView();
            }
        }
#endif
#ifdef Q_OS_UNIX
        QString str = _ui->FilePath->text();
        QStringList tepList = str.split("/", QString::SkipEmptyParts);
        if (tepList.count() >= 1)
        {
            QDir dir(_treeWidgetFilename);
            if (dir.cdUp())
            {
                _treeWidgetFilename = dir.absolutePath();
                openFilePath(true);
                refreshRightView();
            }
        }
#endif
    }

    void FITKFileDirectoryDialog::onBackClicked()
    {
        if (_pathBackVec.size() > 0)
        {
            QString strPath;
            if (_currentFilePath.right(1) == "/")
            {
                strPath = _currentFilePath.mid(0, _currentFilePath.length() - 1);
            }
            _pathFrontVec.push_back(strPath); //前进列表新增
            _ui->front->setEnabled(true);   //前进键可用

            _currentFilePath = _pathBackVec.back();
            _pathBackVec.pop_back();     //后退列表更新

            if (_pathBackVec.size() == 0)
            {
                _ui->back->setEnabled(false);   //后退键不可用
            }

            _currentIndex = _dirModel->index(_currentFilePath);;
            QDir dir(_treeWidgetFilename);
            if (dir.cd(_currentFilePath))
            {
                _treeWidgetFilename = _currentFilePath;
                openFilePath(false);
                refreshRightView();
            }
        }
    }

    void FITKFileDirectoryDialog::onFrontClicked()
    {
        if (_pathFrontVec.size() > 0)
        {
            QString strPath;
            if (_currentFilePath.right(1) == "/")
            {
                strPath = _currentFilePath.mid(0, _currentFilePath.length() - 1);
            }
            _pathBackVec.push_back(strPath);  //后退列表新增
            _ui->back->setEnabled(true);   //后退键可用

            _currentFilePath = _pathFrontVec.back();
            _pathFrontVec.pop_back();     //前进列表更新

            if (_pathFrontVec.size() == 0)
            {
                _ui->front->setEnabled(false); //前进键不可用
            }
            _currentIndex = _dirModel->index(_currentFilePath);;
            QDir dir(_treeWidgetFilename);
            if (dir.cd(_currentFilePath))
            {
                _treeWidgetFilename = _currentFilePath;
                openFilePath(false);
                refreshRightView();
            }
        }

    }

    void FITKFileDirectoryDialog::onFilePathReturnPressed()
    {
        // 兼容复制路径时附带的换行和 Windows 分隔符。
        QString inputPath = _ui->FilePath->text().trimmed();
        if (inputPath.isEmpty())
        {
            return;
        }

        if (inputPath.left(1) == "\"")
        {
            inputPath.remove(0, 1);
        }
        if (inputPath.right(1) == "\"")
        {
            inputPath.chop(1);
        }

        inputPath.replace("\\", "/");
        inputPath = QDir::cleanPath(inputPath);

        QDir dir(inputPath);
        if (dir.exists() == false)
        {
            _ui->FilePath->setText(_currentFilePath);
            return;
        }

        _treeWidgetFilename = inputPath;
        _currentIndex = _dirModel->index(inputPath);
        openFilePath(true);
        refreshRightView();
    }

    QStringList FITKFileDirectoryDialog::getSelectGroupFiles(const QString path)
    {
        QStringList groupFiles;

        //截取目录路径
        int indexSlash = path.lastIndexOf("/");
        QString directory = path.left(indexSlash);

        QDir dir(directory);
        if (!dir.exists())
        {
            //path error
            return QStringList();
        }

        if (dir.cd(directory) == false)return groupFiles;

        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        //后缀名过滤
        int indexDot = path.lastIndexOf(".");
        QString ext = path.mid(indexDot, path.length() - indexDot);
        QStringList fiter{};
        fiter.append("*" + ext);
        dir.setNameFilters(fiter);

        //截取组名
        QString regGroupName = path.mid(indexSlash + 1, path.length() - indexSlash);
        int indexFirstDot = regGroupName.indexOf(".");
        QString prefix = regGroupName.left(indexFirstDot);

        QFileInfoList list = dir.entryInfoList();

        //文件列表为空，返回
        if (list.count() <= 0)return groupFiles;

        //进行文件识别
        for (int i = 0; i < list.count(); i++) {
            QFileInfo fileInfo = list.at(i);
            QString fileName = fileInfo.fileName();
            //判断文件是否符合分组
            if (fileName.mid(0, prefix.length()) == prefix) {
                //判断文件后缀是否只包含时间步
                QString fileStep = fileName.mid(prefix.length(), fileName.size());
                fileStep = fileStep.remove(ext);
                bool isOnlyIncludeNumber = false;
                isOnlyIncludeNumber = fileStep.toDouble();
                if (isOnlyIncludeNumber == false)continue;
                groupFiles.append(fileInfo.absoluteFilePath());
            }
        }


        //对文件列表进行排序
        QCollator collator;
        collator.setNumericMode(true);
        std::sort(groupFiles.begin(), groupFiles.end(),
            [&collator](const QString & str1, const QString & str2)
        {
            return collator.compare(str1, str2) < 0;
        }
        );


        return groupFiles;
    }

    QStringList FITKFileDirectoryDialog::getSelectedFullPaths() const
    {
        if (_selectedFullPaths.count() > 0)
        {
            return _selectedFullPaths;
        }

        QStringList files;
        if (_selectionText.indexOf("...") > 0)
        {
            files = const_cast<FITKFileDirectoryDialog*>(this)->getSelectGroupFiles(
                QString("%1%2").arg(_currentFilePath).arg(_selectionText));
        }
        else if (_selectionText.isEmpty() == false)
        {
            files << QDir::cleanPath(QString("%1%2").arg(_currentFilePath).arg(_selectionText));
        }
        return files;
    }
}