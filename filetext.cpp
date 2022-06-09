#include "filetext.h"

int FileText::ErrorNone = 0;
int FileText::ErrorSaving = 1;
int FileText::ErrorLoading = 1;
int FileText::ErrorCompression = 2;
int FileText::ErrorName = 3;
int FileText::ErrorCopy = 4;

FileText::FileText()
{
    fileLoaded = "null";
}

FileText::FileText(QString file)
{
    load(file);
}

FileText::FileText(const FileText &copy)
{
    for(int progress(0); progress < copy.size(); progress++)
    {
        write(copy.read(progress));
    }
    fileLoaded = copy.currentFile();
}

FileText::~FileText()
{
    removeAll();
}

void FileText::write(const QString &addText)
{
    File.append(new QString(onWrite(addText)));
}

void FileText::write(const QString* &addText)
{
    File.append(new QString(onWrite(*addText)));
}

void FileText::write(const QVector<QString> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        write(addText[progress]);
    }
}

void FileText::write(const QVector<QString *> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        write(*addText[progress]);
    }
}

void FileText::write(const QVector<QString> *&addText)
{
    write(*addText);
}

void FileText::write(const QVector<QString *> *&addText)
{
    write(*addText);
}

void FileText::write(const QList<QString> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        write(addText[progress]);
    }
}

void FileText::write(const QList<QString *> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        write(*addText[progress]);
    }
}

void FileText::write(const QList<QString> *&addText)
{
    write(*addText);
}

void FileText::write(const QList<QString *> *&addText)
{
    write(*addText);
}

void FileText::write(const FileText &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        write(addText.read(progress));
    }
}

void FileText::write(const FileText * &addText)
{
    for(int progress(0); progress < addText->size(); progress++)
    {
        write(addText->read(progress));
    }
}

void FileText::insert(int l, const QString &addText)
{
    File.insert(l, new QString(onWrite(addText)));
}

void FileText::insert(int l, const QString *&addText)
{
    File.insert(l, new QString(onWrite(*addText)));
}

void FileText::insert(int l, const QVector<QString> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        insert(l + progress, addText[progress]);
    }
}

void FileText::insert(int l, const QVector<QString *> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        insert(l + progress, *addText[progress]);
    }
}

void FileText::insert(int l, const QVector<QString> *&addText)
{
    insert(l, *addText);
}

void FileText::insert(int l, const QVector<QString *> *&addText)
{
    insert(l, *addText);
}

void FileText::insert(int l, const QList<QString> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        insert(l + progress, addText[progress]);
    }
}

void FileText::insert(int l, const QList<QString *> &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        insert(l + progress, *addText[progress]);
    }
}

void FileText::insert(int l, const QList<QString> *&addText)
{
    insert(l, *addText);
}

void FileText::insert(int l, const QList<QString *> *&addText)
{
    insert(l, *addText);
}

void FileText::insert(int l, const FileText &addText)
{
    for(int progress(0); progress < addText.size(); progress++)
    {
        insert(l + progress, addText[progress]);
    }
}

void FileText::insert(int l, const FileText *&addText)
{
    for(int progress(0); progress < addText->size(); progress++)
    {
        insert(l + progress, addText[progress]);
    }
}

void FileText::replace(int l, const QString &addText)
{
    delete File[l];
    File.replace(l, new QString(onWrite(addText)));
}

void FileText::replace(int l, const QString *&addText)
{
    delete File[l];
    File.replace(l, new QString(onWrite(*addText)));
}

void FileText::replaceMultiple(int start, int end, const QVector<QString> &addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QVector<QString *> &addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QVector<QString> *&addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QVector<QString *> *&addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QList<QString> &addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QList<QString *> &addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QList<QString> *&addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::replaceMultiple(int start, int end, const QList<QString *> *&addText)
{
    remove(start, (end - start) + 1);
    insert(start, addText);
}

void FileText::remove(int l, int count)
{
    for(int progress(0); progress < count; progress++)
    {
        delete File[l + progress];
    }
    File.remove(l, count);
}

void FileText::removeList(QList<int> l)
{
    std::vector<int> toRemove;
    for(int progress(0); progress < l.size(); progress++)
    {
        delete File[l[progress]];
        toRemove.push_back(l[progress]);
    }
    std::sort(toRemove.begin(), toRemove.end());
    for(int progress(toRemove.size() - 1); progress >= 0; progress--)
    {
        File.remove(toRemove[progress]);
    }
}

void FileText::removeList(QVector<int> l)
{
    std::vector<int> toRemove;
    for(int progress(0); progress < l.size(); progress++)
    {
        delete File[l[progress]];
        toRemove.push_back(l[progress]);
    }
    std::sort(toRemove.begin(), toRemove.end());
    for(int progress(toRemove.size() - 1); progress >= 0; progress--)
    {
        File.remove(toRemove[progress]);
    }
}

void FileText::removeLast(int n)
{
    for(int progress(0); progress < n; progress++)
    {
        remove(size() - 1);
    }
}

void FileText::removeFront(int n)
{
    for(int progress(0); progress < n; progress++)
    {
        remove(0);
    }
}

void FileText::removeAll()
{
    for(int progress(File.size() - 1); progress >= 0; progress--)
    {
        delete File[progress];
    }
    File.clear();
}

void FileText::moveItem(int from, int to)
{
    if(!(from >= 0 && to >= 0 && from < File.size() && to < File.size()))
    {
        return;
    }
    if(from == to)
    {
        return;
    }
    if(from > to)
    {
        File.insert(to, File[from]);
        File.remove(from + 1);
    }
    else
    {
        File.insert(to, File[from]);
        File.remove(from);
    }
}

void FileText::swapItems(int l1, int l2)
{
    if(!(l1 >= 0 && l2 >= 0 && l1 < File.size() && l2 < File.size()))
    {
        return;
    }
    QString *saveFirst(File[l1]);
    File[l1] = File[l2];
    File[l2] = saveFirst;
}

QVector<QString *> FileText::getAll() const
{
    return File;
}

std::vector<QString *> FileText::getStdAll() const
{
    return File.toStdVector();
}

QVector<QString> FileText::copy() const
{
    QVector<QString> returned;
    for(int progress(0); progress < File.size(); progress++)
    {
        returned.append(QString(*File[progress]));
    }
    return returned;
}

std::vector<QString> FileText::copyToStd() const
{
    return copy().toStdVector();
}

QString FileText::read(int l) const
{
    return *File[l];
}

QString FileText::readAll() const
{
    QString returned;
    for(int progress(0); progress < File.size(); progress++)
    {
        returned.append(QString(*File[progress]));
        if(progress < File.size() - 1)
        {
            returned.append('\n');
        }
    }
    return returned;
}

QString FileText::currentFile() const
{
    return fileLoaded;
}

void FileText::setCurrentFile(QString file)
{
    fileLoaded = file;
}

int FileText::expand()
{
    int amount(0);
    int reading(0);
    while (reading < size()) {
        if(File[reading]->contains('\n'))
        {
            QStringList separation(File[reading]->split('\n'));
            replaceMultiple(reading, reading, separation);
            amount++;
        }
        reading++;
    }
    return amount;
}

int FileText::size() const
{
    return File.size();
}

bool FileText::isEmpty() const
{
    return File.isEmpty();
}

int FileText::charactersCount() const
{
    int number(0);
    for(int progress(0); progress < File.size(); progress++)
    {
        number += File[progress]->length();
    }
    return number;
}

int FileText::searchLine(QString line, int start) const
{
    for(int progress(start); progress < File.size(); progress++)
    {
        if(File[progress] == line)
        {
            return progress;
        }
    }
    return -1;
}

QList<int> FileText::searchAllLines(QString line) const
{
    QList<int> result;
    for(int progress(0); progress < File.size(); progress++)
    {
        if(File[progress] == line)
        {
            result.append(progress);
        }
    }
    return result;
}

int FileText::searchSentence(QString sentence, Qt::CaseSensitivity cs, int start) const
{
    for(int progress(start); progress < File.size(); progress++)
    {
        if(File[progress]->contains(sentence, cs))
        {
            return progress;
        }
    }
    return -1;
}

QList<int> FileText::searchAllSentences(QString sentence, Qt::CaseSensitivity cs) const
{
    QList<int> result;
    for(int progress(0); progress < File.size(); progress++)
    {
        if(File[progress]->contains(sentence, cs))
        {
            result.append(progress);
        }
    }
    return result;
}

bool FileText::searchAndReplaceLine(const QString line, const QString replacement, int start)
{
    int lineFound(searchLine(line, start));
    if(lineFound == -1)
    {
        return false;
    }
    replace(lineFound, replacement);
    return true;
}

int FileText::searchAndReplaceAllLines(const QString line, const QString replacement)
{
    QList<int> linesFound(searchAllLines(line));
    for(int progress(0); progress < linesFound.size(); progress++)
    {
        replace(linesFound[progress], replacement);
    }
    return linesFound.size();
}

bool FileText::searchAndReplaceSentence(const QString sentence, const QString replacement, Qt::CaseSensitivity cs, int start)
{
    int lineFound(searchSentence(sentence, cs, start));
    if(lineFound == -1)
    {
        return false;
    }
    File[lineFound]->replace(sentence, replacement, cs);
    return true;
}

int FileText::searchAndReplaceAllSentences(const QString sentence, const QString replacement, Qt::CaseSensitivity cs)
{
    QList<int> linesFound(searchAllSentences(sentence, cs));
    for(int progress(0); progress < linesFound.size(); progress++)
    {
        File[progress]->replace(sentence, replacement, cs);
    }
    return linesFound.size();
}

QList<int> FileText::sort(Qt::CaseSensitivity cs, bool reversed, bool returnOldToNew)
{
    QList<QString> oldOrder;
    QStringList toSort;
    for(int progress(0); progress < File.size(); progress++)
    {
        oldOrder.append(*File[progress]);
        toSort.append(*File[progress]);
    }
    toSort.sort(cs);
    if(reversed)
    {
        QStringList toReverse(toSort);
        toSort.clear();
        for(int progress(0); progress < toReverse.size(); progress++)
        {
            toSort.insert(0, toReverse[progress]);
        }
    }
    QStringList copyDelete(toSort);
    QString unused("null");
    int endNumber(0);
    while (copyDelete.contains(unused + QString::number(endNumber))) {
        endNumber++;
    }
    unused += QString::number(endNumber);
    QList<int> returned;
    if(returnOldToNew)
    {
        for(int progress(0); progress < toSort.size(); progress++)
        {
            int index(copyDelete.indexOf(oldOrder[progress]));
            returned.append(index);
            copyDelete[index] = unused;
        }
    }
    else
    {
        for(int progress(0); progress < toSort.size(); progress++)
        {
            int index(oldOrder.indexOf(copyDelete[progress]));
            returned.append(index);
            oldOrder[index] = unused;
        }
    }
    removeAll();
    write(toSort);
    return returned;
}

bool FileText::load(QString file)
{
    QFile read(file);
    if(read.open(QIODevice::ReadOnly))
    {
        removeAll();
        QByteArray readAll(read.readAll());
        QString spliting(readAll);
        QStringList splited(spliting.split('\n'));
        for(int progress(0); progress < splited.size(); progress++)
        {
            File.append(new QString(splited[progress]));
        }
        fileLoaded = file;
        searchAndReplaceAllSentences("\r", "");
        onLoaded();
        return true;
    }
    else
    {
        return false;
    }
}

bool FileText::loadCompressed(QString file, int* getError)
{
    QString searchName;
    int testNumber(-1);
    do {
        testNumber++;
        searchName = "TemporaryFileTextUncompressed" + QString::number(testNumber) + ".txt";
    } while(FileText::exists(searchName) && testNumber < 1000);
    if(testNumber == 1000)
    {
        if(getError != nullptr)
        {
            *getError = ErrorName;
        }
        return false;
    }
    if(!FileText::copyFile(file, searchName))
    {
        if(getError != nullptr)
        {
            *getError = ErrorCopy;
        }
        return false;
    }
    if(!FileText::uncompress(searchName))
    {
        FileText::deleteFile(searchName);
        if(getError != nullptr)
        {
            *getError = ErrorCompression;
        }
        return false;
    }
    if(!load(searchName))
    {
        FileText::deleteFile(searchName);
        if(getError != nullptr)
        {
            *getError = ErrorLoading;
        }
        return false;
    }
    setCurrentFile(file);
    FileText::deleteFile(searchName);
    if(getError != nullptr)
    {
        *getError = ErrorNone;
    }
    return true;
}

bool FileText::save(QString file) const
{
    if(file == "")
    {
        file = fileLoaded;
    }
    QFile write(file);
    if(write.open(QIODevice::WriteOnly))
    {
        for(int progress(0); progress < File.size(); progress++)
        {
            if(progress + 1 < File.size())
            {
                write.write(File[progress]->toUtf8() + "\n");
            }
            else
            {
                write.write(File[progress]->toUtf8());
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool FileText::saveWithDirectories(QString file) const
{
    QDir createDirectories;
    if(!createDirectories.exists(createDirectories.filePath(file)))
    {
        QString fileCopy(file);
        while (fileCopy[fileCopy.size() - 1] != "/" && fileCopy.size() >= 2) {
            fileCopy.remove(fileCopy.size() - 1, 1);
        }
        if(fileCopy.size() < 2)
        {
            return false;
        }
        if(!createDirectories.mkpath(createDirectories.filePath(fileCopy)))
        {
            return false;
        }
    }
    return save(file);
}

bool FileText::saveAndCompress(QString file, int compressionLevel, int* getError) const
{
    if(save(file))
    {
        if(FileText::compress(file, compressionLevel))
        {
            if(getError != nullptr)
            {
                *getError = ErrorNone;
            }
            return true;
        }
        else
        {
            if(getError != nullptr)
            {
                *getError = ErrorCompression;
            }
            return false;
        }
    }
    if(getError != nullptr)
    {
        *getError = ErrorSaving;
    }
    return false;
}

bool FileText::saveWithDirectoriesAndCompress(QString file, int compressionLevel, int* getError) const
{
    if(saveWithDirectories(file))
    {
        if(FileText::compress(file, compressionLevel))
        {
            if(getError != nullptr)
            {
                *getError = ErrorNone;
            }
            return true;
        }
        else
        {
            if(getError != nullptr)
            {
                *getError = ErrorCompression;
            }
            return false;
        }
    }
    if(getError != nullptr)
    {
        *getError = ErrorSaving;
    }
    return false;
}

FileText FileText::fuse(FileText &file1, FileText &file2)
{
    FileText fusion(file1);
    for(int progress(0); progress < file2.size(); progress++)
    {
        fusion.write(file2.read(progress));
    }
    return fusion;
}

bool FileText::compress(QString file, int compressionLevel)
{
    QFile read(file);
    if(read.open(QIODevice::ReadOnly))
    {
        QByteArray compressedFile(qCompress(read.readAll(), compressionLevel));
        read.close();
        if(compressedFile.isEmpty())
        {
            return false;
        }
        QFile write(file);
        if(write.open(QIODevice::WriteOnly))
        {
            write.write(compressedFile);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool FileText::uncompress(QString file)
{
    QFile read(file);
    if(read.open(QIODevice::ReadOnly))
    {
        QByteArray uncompressedFile(qUncompress(read.readAll()));
        read.close();
        if(uncompressedFile.isEmpty())
        {
            return false;
        }
        QFile write(file);
        if(write.open(QIODevice::WriteOnly))
        {
            write.write(uncompressedFile);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool FileText::isCompressed(QString file, int* getError)
{
    QFile read(file);
    if(read.open(QIODevice::ReadOnly))
    {
        QByteArray uncompressedFile(qUncompress(read.readAll()));
        read.close();
        if(uncompressedFile.isEmpty())
        {
            if(getError != nullptr)
            {
                *getError = ErrorNone;
            }
            return false;
        }
        else
        {
            if(getError != nullptr)
            {
                *getError = ErrorNone;
            }
            return true;
        }
    }
    else
    {
        if(getError != nullptr)
        {
            *getError = ErrorLoading;
        }
        return false;
    }
}

bool FileText::exists(QString file)
{
    return QFile::exists(file);
}

bool FileText::deleteFile(QString file)
{
    QFile toDelete(file);
    return toDelete.remove();
}

bool FileText::copyFile(QString from, QString to)
{
    return QFile::copy(from, to);
}

quint64 FileText::fileSize(QString file)
{
    QFile getSize(file);
    return getSize.size();
}

bool FileText::rename(QString oldPath, QString newName)
{
    QFile renamed(oldPath);
    return renamed.rename(newName);
}

void FileText::operator+=(const QString &addText)
{
    write(addText);
}

void FileText::operator+=(const QString *&addText)
{
    write(addText);
}

void FileText::operator+=(const QVector<QString> &addText)
{
    write(addText);
}

void FileText::operator+=(const QVector<QString *> &addText)
{
    write(addText);
}

void FileText::operator+=(const QVector<QString> *&addText)
{
    write(addText);
}

void FileText::operator+=(const QVector<QString *> *&addText)
{
    write(addText);
}

void FileText::operator+=(const QList<QString> &addText)
{
    write(addText);
}

void FileText::operator+=(const QList<QString *> &addText)
{
    write(addText);
}

void FileText::operator+=(const QList<QString> *&addText)
{
    write(addText);
}

void FileText::operator+=(const QList<QString *> *&addText)
{
    write(addText);
}

void FileText::operator+=(const FileText &addText)
{
    write(addText);
}

void FileText::operator+=(const FileText *&addText)
{
    write(addText);
}

QString FileText::operator[](int l) const
{
    return read(l);
}

FileText FileText::operator=(const FileText &copy)
{
    return FileText(copy);
}

void FileText::onLoaded()
{

}

QString FileText::onWrite(QString written)
{
    return written;
}

FileText operator+(const FileText &a, const FileText &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const FileText *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QString *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QString &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QVector<QString> &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QVector<QString *> &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QVector<QString> *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QVector<QString *> *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QList<QString> &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QList<QString *> &b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QList<QString> *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}

FileText operator+(const FileText &a, const QList<QString *> *&b)
{
    FileText copie(a);
    copie += b;
    return copie;
}


bool operator==(const FileText &a, const FileText &b)
{
    if(a.size() != b.size())
    {
        return false;
    }
    for(int progress(0); progress < a.size(); progress++)
    {
        if(a.read(progress) != b.read(progress))
        {
            return false;
        }
    }
    return true;
}

bool operator!=(const FileText &a, const FileText &b)
{
    return !(a == b);
}

bool operator>(const FileText &a, const FileText &b)
{
    return a.charactersCount() > b.charactersCount();
}

bool operator<(const FileText &a, const FileText &b)
{
    return a.charactersCount() < b.charactersCount();
}

bool operator>=(const FileText &a, const FileText &b)
{
    return a.charactersCount() >= b.charactersCount();
}

bool operator<=(const FileText &a, const FileText &b)
{
    return a.charactersCount() <= b.charactersCount();
}
