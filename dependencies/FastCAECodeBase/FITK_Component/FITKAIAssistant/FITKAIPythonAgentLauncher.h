/**
 * @file   FITKAIPythonAgentLauncher.h
 * @brief  Python子进程启动与标准输入输出桥接类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2026-05-14
 */
#ifndef __FITK_AI_PYTHON_AGENT_LAUNCHER_H__
#define __FITK_AI_PYTHON_AGENT_LAUNCHER_H__

#include "FITK_Component/FITKAIAssistant/FITKAIAssistantAPI.h"

#include <QObject>
#include <QProcess>
#include <QStringList>

class QString;

namespace AIAssistant
{
    class FITKPythonExecutor;
    class FITKAIAssistantWindow;
}

namespace AIAssistant
{
    /**
     * @brief Python agent 启动器，负责启动 Python 脚本并捕获输出
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2026-05-14
     */
    class FITKAIASSISTANTAPI FITKAIPythonAgentLauncher : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        explicit FITKAIPythonAgentLauncher(FITKAIAssistantWindow* parent);
        /**
         * @brief 析构函数，释放并停止子进程
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        virtual ~FITKAIPythonAgentLauncher();

        /**
         * @brief 启动桥接 Python 进程
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void start();
        /**
         * @brief 停止桥接 Python 进程
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void stop();
        /**
         * @brief 判断桥接进程是否在运行
         * @return 运行返回 true，否则返回 false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        bool isRunning() const;

        /**
         * @brief 向子进程标准输入写入一行命令
         * @param input 待发送命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void writeToProcess(const QString& input);

    signals:
        void sigOutput(const QString& text);
        void sigError(const QString& text);
        void sigStarted();
        void sigFinished(int exitCode, QProcess::ExitStatus exitStatus);

    private slots:
        /**
         * @brief 读取标准输出并向外转发
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotReadyReadStandardOutput();
        /**
         * @brief 读取标准错误并向外转发
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void slotReadyReadStandardError();
        /**
         * @brief 进程结束时刷新残留标准输出并转发结束信号
         * @param exitCode 退出码
         * @param exitStatus 退出状态
         */
        void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    private:
        /**
         * @brief 获取当前平台 Python 可执行文件路径
         * @return Python 可执行文件绝对路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString getPythonExecutablePath() const;
        /**
         * @brief 获取 FastCAEAssistant.py 的默认路径
         * @return 脚本绝对路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString getAssistantScriptPath() const;
        /**
         * @brief 处理 agent 输出中的 HOSTPY 命令行
         * @param commandLine HOSTPY 命令文本（不含前缀）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        void handleHostPythonCommand(const QString& commandLine);
        /**
         * @brief 转义结果文本用于桥接回写
         * @param text 原始文本
         * @return 转义后的单行文本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString escapeForBridge(const QString& text) const;


 
    private: 
        /**
         * @brief 子进程工作目录，默认为应用程序目录
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString _workDir{};
        /**
         * @brief 累积输出缓存，便于诊断与回放
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString _allOutput{};
        /**
         * @brief 标准输出缓存（用于跨 chunk 识别 HOSTPY 行）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QString _stdoutBuffer{};
        /**
         * @brief 执行 Python 桥接脚本的进程对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        QProcess* _process{};
        /**
         * @brief 宿主 Python 命令执行器（仅用于执行命令，不负责启动 agent）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        FITKPythonExecutor* _pythonExecutor{};
        /**
         * @brief 交互窗口
         * @author LiBaojun AI
         * @date 2026-06-09
         */
        FITKAIAssistantWindow* _assistantWindow{};
    };
}

#endif
