#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMdiArea>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include "ui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openFile();
    void newFile();
    bool maybeSave();
    bool Save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    void Exit();
    void About();

    void Copy();
    void Cut();
    void Paste();
    void Undo();
    void Redo();
private:
    bool isUnsave;
    QString nowFile;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
