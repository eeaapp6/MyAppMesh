/**
 * @file FITKHttpPythonExecutor.h
 * @brief FITKHttpPythonDriver Python 命令执行器声明
 * @author libaojunAI
 * @date 2026-05-26
 */
#ifndef __FITK_HTTP_PYTHON_EXECUTOR_H__
#define __FITK_HTTP_PYTHON_EXECUTOR_H__

class QString;

namespace HttpPython
{
    class HttpClient;
    /**
     * @brief 通过 FITKPythonInterface 执行 Python 命令
     * @author libaojunAI
     * @date 2026-05-26
     */
    class FITKHttpPythonExecutor
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojunAI
         * @date 2026-05-26
         */
        explicit FITKHttpPythonExecutor(HttpClient* httpClient);
        /**
         * @brief 析构函数
         * @author libaojunAI
         * @date 2026-05-26
         */
        virtual ~FITKHttpPythonExecutor() = default;

        /**
         * @brief 从文本执行 Python 命令
         * @param commandText 包含 Python 命令的文本
         * @param errorInfo 执行失败时返回错误信息
         * @return 成功返回 true，失败返回 false
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool executeCommandFromText(const QString& commandText, QString& errorInfo);

        /**
         * @brief 执行一条 Python 命令
         * @param command Python 命令字符串
         * @param errorInfo 执行失败时返回错误信息
         * @return 成功返回 true，失败返回 false
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool executeCommand(const QString& command, QString& errorInfo);

    private:
        HttpClient* _httpClient{};
    };
}

#endif
