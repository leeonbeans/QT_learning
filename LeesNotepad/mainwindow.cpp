#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //添加信息至状态栏
    statusLable.setMaximumWidth(150);
    statusLable.setText("Length: " + QString::number(0) + " Lines: " + QString::number(1) + "  ");
    ui->statusbar->addPermanentWidget(&statusLable);

    statusCursorLable.setMaximumWidth(150);
    statusCursorLable.setText("Ln: " + QString::number(0) + " Col: " + QString::number(1) + "  ");
    ui->statusbar->addPermanentWidget(&statusCursorLable);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("Leeon"));
    ui->statusbar->addPermanentWidget(author);

    this->setWindowTitle(tr("LeesNotepad"));
    textChanged = false;

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if (mode == QTextEdit::NoWrap)
    {
        ui->actionAutoWrap->setChecked(false);
    }
    else
    {
        ui->actionAutoWrap->setChecked(true);
    }

    ui->actionStatusbar->setChecked(true);
    ui->actionToolbar->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog searchDialog(this, ui->textEdit);
    searchDialog.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog replaceDialog(this, ui->textEdit);
    replaceDialog.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if (!changedConfirm())
        return;

    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件"));
    textChanged = false;
    filePath = "";
}


void MainWindow::on_actionOpen_triggered()
{
    if (!changedConfirm())
        return;

    QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"..", tr("无法打开文件"));
        return;
    }

    filePath = filename;

    QTextStream textin(&file);
    QString text = textin.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("Text files (*.txt)"));

        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this,"..", tr("无法保存文件"));
            return;
        }
        filePath = filename;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}


void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("Text files (*.txt)"));

    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"..", tr("无法保存文件"));
        return;
    }
    filePath = filename;

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}


void MainWindow::on_textEdit_textChanged()
{
    if (!textChanged)
    {
        this->setWindowTitle("*"+this->windowTitle());
        textChanged = true;
    }
}

bool MainWindow::changedConfirm()  //用户确认返回true 用户取消返回false
{
    if (textChanged)
    {
        QString path = (filePath != "") ? "\""+filePath+"\"" : "当前文件";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(tr("是否保存更改到\n") + path + "?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
            break;
        default:
            return false;
            break;
        }
    }
    return true;
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, tr("选择颜色"));
    if (color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditorColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, tr("选择颜色"));
    if (color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackgroundColor_triggered()
{
    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle("无法设置字体背景色");
    msg.setWindowFlag(Qt::Drawer);
    msg.setText(tr("不支持捏\n"));
    msg.setStandardButtons(QMessageBox::Yes);
    msg.exec();
}


void MainWindow::on_actionAutoWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if (mode == QTextEdit::NoWrap)
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWrap->setChecked(true);
    }
    else
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);

    if (ok)
    {
        ui->textEdit->setFont(font);
    }
}


void MainWindow::on_actionToolbar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);
}


void MainWindow::on_actionStatusbar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusbar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(changedConfirm())
        exit(0);
}

