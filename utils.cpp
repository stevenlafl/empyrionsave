#include "utils.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/quazipnewinfo.h"
#include <QFile>

Utils::Utils() {

}

bool Utils::copyPath(QString sourceDir, QString destinationDir, bool overWriteDirectory) {
    QDir originDirectory(sourceDir);

    if (!originDirectory.exists()) {
        return false;
    }

    QDir destinationDirectory(destinationDir);

    if (destinationDirectory.exists() && !overWriteDirectory) {
        return false;
    }
    else if (destinationDirectory.exists() && overWriteDirectory) {
        destinationDirectory.removeRecursively();
    }

    originDirectory.mkpath(destinationDir);

    foreach (QString directoryName, originDirectory.entryList(QDir::Dirs | \
                                                              QDir::NoDotAndDotDot)) {
        QString destinationPath = destinationDir + "/" + directoryName;
        originDirectory.mkpath(destinationPath);
        copyPath(sourceDir + "/" + directoryName, destinationPath, overWriteDirectory);
    }

    foreach (QString fileName, originDirectory.entryList(QDir::Files)) {
        QFile::copy(sourceDir + "/" + fileName, destinationDir + "/" + fileName);
    }

    /*! Possible race-condition mitigation? */
    QDir finalDestination(destinationDir);
    finalDestination.refresh();

    if (finalDestination.exists()) {
        return true;
    }

    return false;
}

/**
 * Adapted from StackOverflow user mosg.
 *
 * @brief Utils::archive
 * @param filePath The zip file path.
 * @param fileDir The path of the directory to archive.
 * @param comment The zip comment.
 * @return bool
 *
 * @note QuaZip is LGPL License.
 * @see https://stackoverflow.com/questions/2598117/zipping-a-folder-file-using-qt
 */
bool Utils::archive(QString filePath, QString fileDir, QString comment) {

    QDir dir(fileDir);

    QuaZip zip(filePath);
    zip.setFileNameCodec("IBM866");

    if (!zip.open(QuaZip::mdCreate)) {
        return false;
    }

    if (!dir.exists()) {
        return false;
    }

    QFile inFile;

    // Retrieving a list of files and folders recursively
    QStringList sl;
    recurseAddDir(dir, sl);

    // Create an array of QFileInfo objects
    QFileInfoList files;
    foreach (QString fn, sl) files << QFileInfo(fn);

    QuaZipFile outFile(&zip);

    char c;
    foreach(QFileInfo fileInfo, files) {

        if (!fileInfo.isFile())
            continue;

        // If the file is in a subdirectory, then add the name of this subdirectory to the file names
        // for example: fileInfo.filePath () = "D:\Work\Sources\SAGO\svn\sago\Release\tmp_DOCSWIN\Folder\123.opn"
        // then after deleting part of the line fileNameWithSubFolders will be equal to "Folder\123.opn", etc.
        QString fileNameWithRelativePath = dir.dirName() + "/" + fileInfo.filePath().remove(0, dir.absolutePath().length() + 1);

        inFile.setFileName(fileInfo.filePath());

        if (!inFile.open(QIODevice::ReadOnly)) {
            return false;
        }

        if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {
            return false;
        }

        while (inFile.getChar(&c) && outFile.putChar(c));

        if (outFile.getZipError() != UNZ_OK) {
            return false;
        }

        outFile.close();

        if (outFile.getZipError() != UNZ_OK) {
            return false;
        }

        inFile.close();
    }

    // + comment
    if (!comment.isEmpty())
        zip.setComment(comment);

    zip.close();

    if (zip.getZipError() != 0) {
        return false;
    }

    return true;
}

/* Recursively searches for all files in the \a directory and adds \b to the list */
void Utils::recurseAddDir(QDir d, QStringList &list) {

    QStringList qsl = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString file, qsl) {

        QFileInfo finfo(QString("%1/%2").arg(d.path()).arg(file));

        if (finfo.isSymLink())
            return;

        if (finfo.isDir()) {

            QString dirname = finfo.fileName();
            QDir sd(finfo.filePath());

            recurseAddDir(sd, list);

        } else
            list << QDir::toNativeSeparators(finfo.filePath());
    }
}

QStringList Utils::listFiles(QString directory) {
    QDir dir(directory);
    return dir.entryList(QDir::Dirs | \
                              QDir::NoDotAndDotDot);
}
