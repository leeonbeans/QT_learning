#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;
    ui->rbDowntoSearch->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btnFindNext_clicked()
{
    QString target = ui->leSearchText->text();

    if(target == "" || pTextEdit == nullptr)
        return;

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();

    int index = -1;

    if (ui->rbDowntoSearch->isChecked()){
        index = text.indexOf(target, c.position(), ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if (index>=0)
        {
            c.setPosition(index);
            c.setPosition(index +target.length(), QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    else if (ui->rbUptoSearch->isChecked())
    {
        index = text.lastIndexOf(target,c.position()-text.length()-1, ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if (index>=0)
        {
            c.setPosition(index +target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if (index < 0)
    {
        QMessageBox msg(this);
        msg.setWindowTitle("提示");
        msg.setText(tr("找不到 ")+target);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }
}


void ReplaceDialog::on_btnReplace_clicked()
{
    QString target = ui->leSearchText->text();
    QString to = ui->leReplaceText->text();

    if(target == "" || pTextEdit == nullptr)
        return;

    QString text = pTextEdit->toPlainText();

    QString selText = pTextEdit->textCursor().selectedText();

    if (ui->cbCaseSensitive->isChecked())
    {
        if (selText == target)
            pTextEdit->insertPlainText(to);
    }
    else if (selText.toLower() == target.toLower())
    {
        pTextEdit->insertPlainText(to);
    }

    on_btnFindNext_clicked();
}


void ReplaceDialog::on_btnReplaceAll_clicked()
{
    QString target = ui->leSearchText->text();
    QString to = ui->leReplaceText->text();

    if(target == "" || pTextEdit == nullptr)
        return;

    QString text = pTextEdit->toPlainText();
    text.replace(target, to, ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    pTextEdit->clear();
    pTextEdit->insertPlainText(text);

}


void ReplaceDialog::on_btnCancel_clicked()
{
    accept();
}

