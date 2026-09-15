#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include <QString>
#include <QWidget>

#include <hdf5.h>

#include <iostream>

namespace
{
bool hasArgument(int argc, char* argv[], const QString& expected)
{
    for (int index = 1; index < argc; ++index)
    {
        if (QString::fromLocal8Bit(argv[index]) == expected)
        {
            return true;
        }
    }
    return false;
}

int runSmokeTest()
{
    unsigned int major = 0;
    unsigned int minor = 0;
    unsigned int release = 0;
    if (H5get_libversion(&major, &minor, &release) < 0)
    {
        std::cerr << "HDF5 runtime query failed" << std::endl;
        return 2;
    }

    if (major != H5_VERS_MAJOR || minor != H5_VERS_MINOR)
    {
        std::cerr << "HDF5 header/runtime mismatch" << std::endl;
        return 3;
    }

    std::cout << "APPMesh bootstrap OK; config=" << APPMESH_BUILD_CONFIG
              << "; Qt=" << qVersion()
              << "; HDF5=" << major << '.' << minor << '.' << release
              << std::endl;
    return 0;
}
}

int main(int argc, char* argv[])
{
    if (hasArgument(argc, argv, QStringLiteral("--smoke-test")))
    {
        QCoreApplication application(argc, argv);
        return runSmokeTest();
    }

    QApplication application(argc, argv);
    QApplication::setApplicationName(QStringLiteral("APPMesh"));
    QApplication::setApplicationVersion(QStringLiteral(APPMESH_VERSION));

    QWidget window;
    window.setWindowTitle(QStringLiteral("APPMesh"));
    window.resize(960, 640);

    auto* label = new QLabel(
        QStringLiteral("APPMesh FastCAE bootstrap\nBuild: %1")
            .arg(QStringLiteral(APPMESH_BUILD_CONFIG)),
        &window);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(window.rect());

    window.show();
    return application.exec();
}
