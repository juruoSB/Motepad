#include "mainwindow.h"
#include "ui.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QFontMetrics>
#include "syntaxClassic.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mainTextEdit);
    isUnsave=true;
    nowFile="未命名";
    setWindowTitle(nowFile);
    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::Save);
    connect(ui->actionSaveAs,&QAction::triggered,this,&MainWindow::saveAs);
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::Exit);
    connect(ui->actionNew,&QAction::triggered,this,&MainWindow::newFile);
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::About);

    connect(ui->actionCopy,&QAction::triggered,this,&MainWindow::Copy);
    connect(ui->actionCut,&QAction::triggered,this,&MainWindow::Cut);
    connect(ui->actionPaste,&QAction::triggered,this,&MainWindow::Paste);
    connect(ui->actionUndo,&QAction::triggered,this,&MainWindow::Undo);
    connect(ui->actionRedo,&QAction::triggered,this,&MainWindow::Redo);
    Highlighter *h = new Highlighter(ui->mainTextEdit->document());
    const int fontWidth = QFontMetrics(ui->mainTextEdit->font()).averageCharWidth();
    ui->mainTextEdit->setTabStopWidth(4*fontWidth );
}

void MainWindow::openFile(){
    QString fileName =
    QFileDialog::getOpenFileName(this,"打开文件",QDir::currentPath());
    if(!fileName.isEmpty()){
        QFile *file=new QFile;
        file->setFileName(fileName);
        bool ok=file->open(QIODevice::ReadOnly);
        nowFile=fileName;
        setWindowTitle(nowFile);
        if(ok)
        {
            if(isUnsave)
                maybeSave();
            QTextStream in(file);
            in.setCodec("utf-8");
            QString codeStr=in.readAll();
            ui->mainTextEdit->setText(codeStr.toUtf8());
            file->close();
            delete file;
        }
        else
        {
            QMessageBox::information(this,"好像有哪不对？！","Open File:"+file->errorString());
            return;
        }
    }
}


void MainWindow::Exit(){
    if(isUnsave)
        maybeSave();
    QApplication::exit();
}

void MainWindow::newFile()
{
   if (maybeSave()) {
       isUnsave = true;
       nowFile = "未命名";
       setWindowTitle(nowFile);
       ui->mainTextEdit->clear();
       ui->mainTextEdit->setVisible(true);
   }
}

bool MainWindow::maybeSave()
{
    if (ui->mainTextEdit->document()->isModified()) {
           QMessageBox box;
           box.setWindowTitle("警告");
           box.setIcon(QMessageBox::Warning);
           box.setText(nowFile+" 尚未保存，是否保存？");
           QPushButton *yesBtn = box.addButton("是(&Y)",
                            QMessageBox::YesRole);
           box.addButton("否(&N)", QMessageBox::NoRole);
           QPushButton *cancelBut = box.addButton("取消",
                            QMessageBox::RejectRole);
           box.exec();
           if (box.clickedButton() == yesBtn)
                return Save();
           else if (box.clickedButton() == cancelBut)
                return false;
       }

       return true;
}

bool MainWindow::Save()
{
   if (isUnsave)
       return saveAs();
   else return saveFile(nowFile);
}

bool MainWindow::saveAs()
{
   QString fileName = QFileDialog::getSaveFileName(this,
                                          "另存为",
                                           nowFile
                                          );
   if (fileName.isEmpty()) return false;
   return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
   QFile file(fileName);

   if (!file.open(QFile::WriteOnly | QFile::Text)) {
       QMessageBox::warning(this, "好像有哪不对？！",
                   tr("无法写入文件 %1：/n %2")
                  .arg(fileName).arg(file.errorString()));
       return false;
   }
   QTextStream out(&file);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   out << ui->mainTextEdit->toPlainText();
   QApplication::restoreOverrideCursor();
   isUnsave = false;
   nowFile = QFileInfo(fileName).canonicalFilePath();
   setWindowTitle(nowFile);
   return true;
}

void MainWindow::Copy(){ui->mainTextEdit->copy();}
void MainWindow::Cut(){ui->mainTextEdit->cut();}
void MainWindow::Paste(){ui->mainTextEdit->paste();}
void MainWindow::Undo(){ui->mainTextEdit->undo();}
void MainWindow::Redo(){ui->mainTextEdit->redo();}


void MainWindow::About(){
    QMessageBox aboutMB(
    QMessageBox::Information,"关于 Motepad",
    "制作中…\n发布 Beta1.0 后打算以 GPL3.0 协议开源于 Github"
    ,QMessageBox::Ok);
    aboutMB.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

