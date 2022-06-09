#ifndef FILETEXT_H
#define FILETEXT_H

#include <QString>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QByteArray>

//FileText Version 1.14 (Patch 2)
//Need Qt 5.14 or more
//Made by the Super Penguin Team

class FileText
{
public:
    FileText();
    FileText(QString file);
    FileText(const FileText& copy);
    ~FileText();
    void write(const QString &addText);
    void write(const QString* &addText);
    void write(const QVector<QString> &addText);
    void write(const QVector<QString*> &addText);
    void write(const QVector<QString>* &addText);
    void write(const QVector<QString*>* &addText);
    void write(const QList<QString> &addText);
    void write(const QList<QString*> &addText);
    void write(const QList<QString>* &addText);
    void write(const QList<QString*>* &addText);
    void write(const FileText &addText);
    void write(const FileText* &addText);
    void insert(int l, const QString &addText);
    void insert(int l, const QString* &addText);
    void insert(int l, const QVector<QString> &addText);
    void insert(int l, const QVector<QString*> &addText);
    void insert(int l, const QVector<QString>* &addText);
    void insert(int l, const QVector<QString*>* &addText);
    void insert(int l, const QList<QString> &addText);
    void insert(int l, const QList<QString*> &addText);
    void insert(int l, const QList<QString>* &addText);
    void insert(int l, const QList<QString*>* &addText);
    void insert(int l, const FileText &addText);
    void insert(int l, const FileText* &addText);
    void replace(int l, const QString &addText);
    void replace(int l, const QString* &addText);
    void replaceMultiple(int start, int end, const QVector<QString> &addText);
    void replaceMultiple(int start, int end, const QVector<QString*> &addText);
    void replaceMultiple(int start, int end, const QVector<QString>* &addText);
    void replaceMultiple(int start, int end, const QVector<QString*>* &addText);
    void replaceMultiple(int start, int end, const QList<QString> &addText);
    void replaceMultiple(int start, int end, const QList<QString*> &addText);
    void replaceMultiple(int start, int end, const QList<QString>* &addText);
    void replaceMultiple(int start, int end, const QList<QString*>* &addText);
    void remove(int l, int count = 1);
    void removeList(QList<int> l);
    void removeList(QVector<int> l);
    void removeLast(int n = 1);
    void removeFront(int n = 1);
    void removeAll();
    void moveItem(int from, int to);
    void swapItems(int l1, int l2);
    QVector<QString*> getAll() const;
    std::vector<QString*> getStdAll() const;
    QVector<QString> copy() const;
    std::vector<QString> copyToStd() const;
    QString read(int l) const;
    QString readAll() const;
    QString currentFile() const;
    void setCurrentFile(QString file);
    int expand();
    int size() const;
    bool isEmpty() const;
    int charactersCount() const;
    int searchLine(const QString line, int start = 0) const;
    QList<int> searchAllLines(const QString line) const;
    int searchSentence(const QString sentence, Qt::CaseSensitivity cs = Qt::CaseSensitive, int start = 0) const;
    QList<int> searchAllSentences(const QString sentence, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool searchAndReplaceLine(const QString line, const QString replacement, int start = 0);
    int searchAndReplaceAllLines(const QString line, const QString replacement);
    bool searchAndReplaceSentence(const QString sentence, const QString replacement, Qt::CaseSensitivity cs = Qt::CaseSensitive, int start = 0);
    int searchAndReplaceAllSentences(const QString sentence, const QString replacement, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QList<int> sort(Qt::CaseSensitivity cs = Qt::CaseInsensitive, bool reversed = false, bool returnOldToNew = false);
    bool load(QString file);
    bool loadCompressed(QString file, int* getError = nullptr);
    bool save(QString file = "") const;
    bool saveWithDirectories(QString file = "") const;
    bool saveAndCompress(QString file = "", int compressionLevel = -1, int* getError = nullptr) const;
    bool saveWithDirectoriesAndCompress(QString file = "", int compressionLevel = -1, int* getError = nullptr) const;
    static FileText fuse(FileText &file1, FileText &file2);
    static bool compress(QString file, int compressionLevel = -1);
    static bool uncompress(QString file);
    static bool isCompressed(QString file, int* getError = nullptr);
    static bool exists(QString file);
    static bool deleteFile(QString file);
    static bool copyFile(QString from, QString to);
    static quint64 fileSize(QString file);
    static bool rename(QString oldPath, QString newName);
    void operator+=(const QString& addText);
    void operator+=(const QString*& addText);
    void operator+=(const QVector<QString>& addText);
    void operator+=(const QVector<QString*>& addText);
    void operator+=(const QVector<QString>*& addText);
    void operator+=(const QVector<QString*>*& addText);
    void operator+=(const QList<QString>& addText);
    void operator+=(const QList<QString*>& addText);
    void operator+=(const QList<QString>*& addText);
    void operator+=(const QList<QString*>*& addText);
    void operator+=(const FileText& addText);
    void operator+=(const FileText*& addText);
    QString operator[](int l) const;
    FileText operator=(const FileText& copy);
    static int ErrorNone;
    static int ErrorSaving;
    static int ErrorLoading;
    static int ErrorCompression;
    static int ErrorName;
    static int ErrorCopy;
protected:
    virtual void onLoaded();
    virtual QString onWrite(QString written);
private:
    QVector<QString*> File;
    QString fileLoaded;
    friend bool operator==(FileText const& a, FileText const& b);
    friend bool operator!=(FileText const& a, FileText const& b);
    friend bool operator>(FileText const& a, FileText const& b);
    friend bool operator<(FileText const& a, FileText const& b);
    friend bool operator>=(FileText const& a, FileText const& b);
    friend bool operator<=(FileText const& a, FileText const& b);
    friend FileText operator+(const FileText& a, const QString& b);
    friend FileText operator+(const FileText& a, const QString*& b);
    friend FileText operator+(const FileText& a, const QVector<QString>& b);
    friend FileText operator+(const FileText& a, const QVector<QString*>& b);
    friend FileText operator+(const FileText& a, const QVector<QString>*& b);
    friend FileText operator+(const FileText& a, const QVector<QString*>*& b);
    friend FileText operator+(const FileText& a, const QList<QString>& b);
    friend FileText operator+(const FileText& a, const QList<QString*>& b);
    friend FileText operator+(const FileText& a, const QList<QString>*& b);
    friend FileText operator+(const FileText& a, const QList<QString*>*& b);
    friend FileText operator+(const FileText& a, const FileText& b);
    friend FileText operator+(const FileText& a, const FileText*& b);
};

#endif // FILETEXT_H
