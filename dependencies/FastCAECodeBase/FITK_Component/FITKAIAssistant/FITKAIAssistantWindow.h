/**
 * @file   FITKAIAssistantWindow.h
 * @brief  AI Assistant 图形界面窗口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2026-05-14
 */
#ifndef __FITK_AI_ASSISTANT_WINDOW_H__
#define __FITK_AI_ASSISTANT_WINDOW_H__

#include "FITK_Component/FITKAIAssistant/FITKAIAssistantAPI.h"

#include <QProcess>
#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QString;

namespace AIAssistant
{
    class FITKAIPythonAgentLauncher;
    class FITKAIAssistantInputHistory;
    class RunTimeMessage;
}

namespace AIAssistant
{
    /**
     * @brief AI 对话窗口，负责输入命令并展示 markdown 输出
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2026-05-14
     */
    class FITKAIASSISTANTAPI FITKAIAssistantWindow : public QWidget
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造界面并初始化桥接连接
         * @param parent 父窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        explicit FITKAIAssistantWindow(QWidget* parent = nullptr);
        /**
         * @brief 析构函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        virtual ~FITKAIAssistantWindow() = default;
 
        /**
         * @brief 获取运行时消息缓存器
         * @return 运行时消息缓存器指针
         * author libaojunAI
         * @date 2026-05-26
         */
        RunTimeMessage* getRunTimeMessage() const ;

    private slots:
        /**
         * @brief 处理提交命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotSubmitCommand();
        /**
         * @brief 处理 Python 标准输出
         * @param text 输出文本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotAppendOutput(const QString& text);
        /**
         * @brief 处理 Python 标准错误
         * @param text 错误文本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotAppendError(const QString& text);
        /**
         * @brief 处理脚本启动事件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotScriptStarted();
        /**
         * @brief 处理脚本退出事件
         * @param exitCode 退出码
         * @param exitStatus 退出状态
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotScriptFinished(int exitCode, QProcess::ExitStatus exitStatus);

    private:
        /**
         * @brief 追加 markdown 文本并刷新显示（使用 QTextDocumentFragment 保留格式）
         * @param text 追加文本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void appendMarkdownText(const QString& text);
        /**
         * @brief    追加用户消息文本（分隔线 + 时间戳 + 右对齐浅灰色气泡样式）
         * @param[i] text  消息文本
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-06-10
         */
        void appendUserText(const QString& text);
        /**
         * @brief 更新命令运行状态并刷新按钮展示
         * @param running true表示正在执行命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-15
         */
        void setCommandRunningState(bool running);

    private:
        /**
         * @brief 输出窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QTextEdit* _output{};
        /**
         * @brief 输入框
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QLineEdit* _input{};
        /**
         * @brief 提交按钮
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QPushButton* _submit{};
        /**
         * @brief Python 桥接启动器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        FITKAIPythonAgentLauncher* _launcher{};
        /**
         * @brief 运行时消息缓存
         * @author libaojunAI
         * @date 2026-05-26
         */
        RunTimeMessage* _runTimeMessage{};
        /**
         * @brief 输入历史管理器
         * @author libaojunAI
         * @date 2026-05-19
         */
        FITKAIAssistantInputHistory* _inputHistory{};
        /**
         * @brief markdown 文本累计缓存
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString _markdownBuffer{};
        /**
         * @brief 当前是否存在正在运行的命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-15
         */
        bool _isCommandRunning{ false };
    };
}

#endif
