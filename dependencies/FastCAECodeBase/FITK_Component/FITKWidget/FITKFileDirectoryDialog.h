/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIFileDirectoryDialog.h
 * @brief  文件选择对话框
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-23
 *********************************************************************/
#ifndef _FILEDIALOG_H_
#define _FILEDIALOG_H_

#include "FITKWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
// #include "qfiledialog.h"
// #include "qtreewidget.h"
#include <QDirModel>


class QTreeWidgetItem;
class QTreeWidget;


namespace Ui { 
    class FITKFileDirectoryDialog;
}

namespace Core {
    class FITKActionOperator;
}

namespace GUI
{
    class FITKWidgetAPI FITKFileDirectoryDialog : 
        public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief 构造函数
         * @param parent-父对象指针,默认为空
         * @since Version: 1.0.0
        **/
        explicit FITKFileDirectoryDialog(Core::FITKActionOperator* oper, QWidget *parent = nullptr);
        /**
         * @brief 析构函数
         * @since Version: 1.0.0
        **/
        virtual ~FITKFileDirectoryDialog();

        /**
         * @brief 返回当前全路径(包含文件名/文件组名)
         * @return 全路径字符串
         * @since Version: 1.0.0
        **/
        QStringList getCurrentFullPath();

        /**
         * @brief 判断当前选中的是不是文件组
         * @param path-当前钻中的路径
         * @return true-是,false-否
         * @since Version: 1.0.0
        **/
        static bool isGourpFiles(const QString path);

        /**
         * @brief 初始化文件后缀过滤
         * @param filterString-文件类型
         * @note  文件类型要求格式为 类型解释 (*.文件后缀)，例如 plot3D文件(*.xyz *.x)。
         * @since Version: 1.0.0
        **/
        void iniFileFilterType(QString filterString);

        /**
         * @brief 初始化对话框打开路径
         * @param dirStr-默认打开的路径
         * @since Version: 1.0.0
        **/
        void initWin(QString dirStr);

    private:
        /**
         * @brief    为路径补充目录分隔符
         * @param[i] path                     输入路径
         * @return   QString                  补齐后的路径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-04-29
         */
        static QString appendDirectorySeparator(const QString& path);

        /**
         * @brief    解析 Windows 快捷方式的真实目标路径
         * @param[i] shortcutPath             快捷方式路径
         * @return   QString                  快捷方式目标路径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-04-29
         */
        static QString resolveWindowsShortcutTarget(const QString& shortcutPath);

        /**
         * @brief 初始化Ui内容
         * @since Version: 1.0.0
        **/
        void initUi();

        /**
         * @brief 获取当前电脑磁盘列表目录
         * @since Version: 1.0.0
        **/
        void SetDirectoryForDrives();

        /**
         * @brief 打开一个路径
         * @param record 是否记录历史路径
         * @since Version: 1.0.0
        **/
        void openFilePath(bool recordHistory);

        /**
         * @brief 在树型中归类显示
         * @since Version: 1.0.0
        **/
        void BuildFilePathTree();

        /**
         * @brief 刷新文件列表
         * @since Version: 1.0.0
        **/
        void refreshRightView();

        /**
         * @brief 根据目录名获取目录下的文件
         * @param dirName-目录名
         * @since Version: 1.0.0
        **/
        void GetFileList(QString dirName);

        /**
         * @brief 根据文件类型中的字符串获取每一个文件过滤类型
         * @param filter-过滤描述字符串
         * @return 文件后缀过滤集合
         * @since Version: 1.0.0
        **/
        QStringList GetFilterFromString(const QString& filter);

        /**
         * @brief 路径添加/
         * @param curPath-需要添加的路径
         * @return 返回添加之后的路径
         * @since Version: 1.0.0
        **/
        QString appendSlash(QString curPath);

        /**
         * @brief 获取文件扩展名
         * @param fileName-文件名
         * @return 返回扩展名
         * @since Version: 1.0.0
        **/
        QString GetFileExt(QString fileName);

        /**
         * @brief 获取文件名的全路径
         * @param index-索引
         * @return 全路径
         * @since Version: 1.0.0
        **/
        QString FindFullFilename(const QModelIndex& index);

        /**
         * @brief 按数字前的字符串形成文件分类列表，并在树型中显示
         * @since Version: 1.0.0
        **/
        void enterDirectory();

        /**
         * @brief 更新过滤文件列表槽函数
         * @param filter-过滤描述字符串
         * @since Version: 1.0.0
        **/
        void filterChange(QString filter);
        /**
         * @brief    获取组文件链表
         * @param[i] path 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-23
         */
        QStringList getSelectGroupFiles(const QString path);

        /**
         * @brief    获取当前选择项对应的真实全路径列表
         * @return   QStringList              真实全路径列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-04-29
        **/
        QStringList getSelectedFullPaths() const;

    private slots:
        ;
        /**
         * @brief 取消槽函数
         * @since Version: 1.0.0
        **/
        void onCancelClicked();
        /**
         * @brief 确定槽函数
         * @since Version: 1.0.0
        **/
        void onOkClicked();

        /**
         * @brief 过滤条件改变槽函数
         * @since Version: 1.0.0
        **/
        void onFilterChange(QString filter);

        /**
         * @brief 执行确定按钮槽函数
         * @since Version: 1.0.0
        **/
        void onDoubleClickedObject(QTreeWidgetItem* pitem, int column);

        /**
         * @brief 左侧路径区域双击操作(处理路径中的盘符)
         * @param model index
         * @since Version: 1.0.0
        **/
        void pathTreeViewDoubleClick(const QModelIndex&);

        /**
         * @brief 路径树双击操作
         * @param model index
         * @since Version: 1.0.0
        **/
        void func_PathTreeViewDoubleClick(const QModelIndex& index);
        void unix_PathTreeViewDoubleClick(QString itemName);

        /**
         * @brief 选择项发生变化触发这个槽，更新显示选中文件
         * @since Version: 1.0.0
        **/
        void onItemSelectionChanged();

        /**
         * @brief 返回上一级目录
         * @since Version: 1.0.0
        **/
        void onUpPushButtonClicked();

        /**
         * @brief 后退
         * @since Version: 1.0.0
        **/
        void onBackClicked();

        /**
         * @brief 前进
         * @since Version: 1.0.0
        **/
        void onFrontClicked();

        /**
         * @brief 路径输入框回车后刷新文件列表
         * @since Version: 1.0.0
        **/
        void onFilePathReturnPressed();

    private:
        Ui::FITKFileDirectoryDialog* _ui{};
        /**
         * @brief 操作器数据
         * @since Version: 1.0.0
        **/
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief 路径变化记录实现前进后退功能
         * @brief 路径变化记录前进、后退路径容器
         * @since Version: 1.0.0
        **/
        QVector<QString> _pathBackVec{};
        QVector<QString> _pathFrontVec{};

        /**
         * @brief 当前选择路径
         * @since Version: 1.0.0
        **/
        QString _currentFilePath;

        /**
         * @brief 目录模型
         * @since Version: 1.0.0
        **/
        QDirModel* _dirModel;

        /**
         * @brief 当前电脑"我的文档"的路径
         * @brief 当前电脑"桌面"的路径
         * @brief Unix操作系统的相关路径
         * @since Version: 1.0.0
        **/
        QString _docPath;
        QString _deskTopPath;
        QString userPath, rootPath, userName, rootName;

        /**
         * @brief 模型数据索引
         * @since Version: 1.0.0
        **/
        QModelIndex _currentIndex;

        /**
         * @brief key为12 ...dat，value为分类下对应的文件列表
         * @since Version: 1.0.0
        **/
        QMap<QString, QVector<QString>> _fileGroup;

        /**
         * @brief 用于保存一个或多个文件分类，用;分割
         * @since Version: 1.0.0
        **/
        QString _selectionText;

        /**
         * @brief    用于保存当前选中项对应的真实全路径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-04-29
        **/
        QStringList _selectedFullPaths;

        /**
         * @brief 用于保存文件类型列表
         * @since Version: 1.0.0
        **/
        QStringList _filterList;

        /**
         * @brief 文件列表
         * @since Version: 1.0.0
        **/
        QTreeWidget* _fileTree;

        /**
         * @brief 根节点
         * @since Version: 1.0.0
        **/
        QTreeWidgetItem* _builtinItem;

        /**
         * @brief key为12@data,value为扩展名dat,这样用是因为相同文件名，扩展名可能会不同，属于不同的分类
         * @since Version: 1.0.0
        **/
        QHash<QString, int> _headFile;

        /**
         * @brief 某个文件夹下的所有文件列表
         * @since Version: 1.0.0
        **/
        QVector<QString> _fileList;

        /**
         * @brief 保存左侧路径QTreeView双击后的文件路径
         * @since Version: 1.0.0
        **/
        QString _treeWidgetFilename;

        /**
         * @brief 字符串常量
         * @since Version: 1.0.0
        **/
        const static QString fatherNodeText;
        const static QString splitExt;
    };
}

#endif
