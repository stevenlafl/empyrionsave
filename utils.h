#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>

class Utils {
public:
    Utils();
    static bool Utils::copyPath(QString sourceDir, QString destinationDir, bool overWriteDirectory);
    static QStringList Utils::listFiles(QString directory);
    static bool Utils::archive(QString filePath, QString fileDir, QString comment = "");
    static void Utils::recurseAddDir(QDir d, QStringList &list);
};

#endif // UTILS_H
