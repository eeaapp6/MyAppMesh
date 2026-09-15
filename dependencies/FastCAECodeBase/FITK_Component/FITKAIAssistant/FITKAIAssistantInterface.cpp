/**
 * @file FITKAIAssistantInterface.cpp
 * @brief 实现 FITKAIAssistant 组件接口。
 */
#include "FITKAIAssistantInterface.h"
#include "FITKAIAssistantWindow.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>

FITKLIBINFOREGISTER(FITKAIAssistant, FITKAIASSISTANT_VERSION);

namespace AIAssistant
{
    FITKAIAssistantInterface::FITKAIAssistantInterface()
    {/*
        //在程序的执行目录下创建一个专门存放 AI 助手相关文件的目录，方便后续功能扩展使用。
        QString basePath = QApplication::applicationDirPath();
        Core::CreateDir(basePath + "/FastCAEAssistantPy");
        Core::CreateDir(basePath + "/FastCAEAssistantPy/fastcaeassistant");
        Core::CreateDir(basePath + "/FastCAEAssistantPy/.FastCAEAI");
        Core::CreateDir(basePath + "/FastCAEAssistantPy/.FastCAEAI/MCP");
        Core::CreateDir(basePath + "/FastCAEAssistantPy/.FastCAEAI/agents");
        Core::CreateDir(basePath + "/FastCAEAssistantPy/.FastCAEAI/skills");

        // 拷贝 FITKAIPy.qrc 资源文件到对应目录，保持目录结构
        const QStringList resourceFiles = {
            "FastCAEAssistantPy/.env",
            "FastCAEAssistantPy/FastCAEAssistant.py",
            "FastCAEAssistantPy/requirements.txt",
            "FastCAEAssistantPy/fastcaeassistant/__init__.py",
            "FastCAEAssistantPy/fastcaeassistant/cli.py",
            "FastCAEAssistantPy/fastcaeassistant/fastcae_context.py",
            "FastCAEAssistantPy/fastcaeassistant/llm_client.py",
            "FastCAEAssistantPy/fastcaeassistant/mcp_registry.py",
            "FastCAEAssistantPy/fastcaeassistant/mcp_smoke.py",
            "FastCAEAssistantPy/fastcaeassistant/orchestrator.py",
            "FastCAEAssistantPy/fastcaeassistant/settings.py",
            "FastCAEAssistantPy/.FastCAEAI/MCP/demo_tools.py",
            "FastCAEAssistantPy/.FastCAEAI/MCP/extra_tools.py",
            "FastCAEAssistantPy/.FastCAEAI/MCP/submitPython.py"
        };

        for (const QString& resPath : resourceFiles) {
            QString outPath = basePath + "/" + resPath;
            QFile outFile(outPath);
            if(outFile.exists()) {
                continue; // 已存在则跳过
            }
            QFile inFile(":" + resPath);
            QFileInfo outInfo(outPath);
            QDir().mkpath(outInfo.path());
            if (inFile.open(QIODevice::ReadOnly)) {
                if (outFile.open(QIODevice::WriteOnly)) {
                    outFile.write(inFile.readAll());
                    outFile.close();
                }
                inFile.close();
            }
        }
            */
    }



    
    /** @brief 返回组件注册名。 */
    QString FITKAIAssistantInterface::getComponentName()
    {
        return "FITKAIAssistant";
    }

    /** @brief 创建并返回组件主界面窗口。 */
    QWidget* FITKAIAssistantInterface::getWidget(const int indexPort)
    {
        Q_UNUSED(indexPort);
        return new FITKAIAssistantWindow();
    }
}