#include "bloggerrevisedialog.h"
#include "ui_bloggerrevisedialog.h"
#include "global.h"

BloggerReviseDialog::BloggerReviseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BloggerReviseDialog)
    , _blogger(nullptr)
{
    ui->setupUi(this);
}

BloggerReviseDialog::~BloggerReviseDialog()
{
    delete ui;
}

void BloggerReviseDialog::setInfo(BloggerInfo bloggerInfo)
{
    _blogger = new BloggerInfo(bloggerInfo);
    ui->edt_name->setText(_blogger->blogger_nickname);
    ui->edt_type->setText(_blogger->blogger_type);
    ui->edt_fans->setText(QString::number(_blogger->blogger_fans));
    ui->edt_noteprice->setText(QString::number(_blogger->blogger_noteprice));
    ui->edt_contact->setText(_blogger->blogger_wechat);
    ui->edt_id->setText(_blogger->blogger_id);
    ui->edt_likes->setText(QString::number(_blogger->blogger_likes));
    ui->edt_videoprice->setText(QString::number(_blogger->blogger_videoprice));
    ui->edt_homelink->setText(_blogger->blogger_homelink);
}

void BloggerReviseDialog::on_btn_submit_clicked()
{
    if(!_blogger)
        return;

    // 创建新的 BloggerInfo 来存储修改后的数据
    BloggerInfo revisedBlogger;
    revisedBlogger.blogger_nickname = ui->edt_name->text();
    revisedBlogger.blogger_type = ui->edt_type->text();
    revisedBlogger.blogger_fans = ui->edt_fans->text().toInt();
    revisedBlogger.blogger_noteprice = ui->edt_noteprice->text().toInt();
    revisedBlogger.blogger_wechat = ui->edt_contact->text();
    revisedBlogger.blogger_id = ui->edt_id->text();
    revisedBlogger.blogger_likes = ui->edt_likes->text().toInt();
    revisedBlogger.blogger_videoprice = ui->edt_videoprice->text().toInt();
    revisedBlogger.blogger_homelink = ui->edt_homelink->toPlainText();

    QString revise_json = revisedBlogger.toJsonStr();
    _blogger->revise_json = revise_json;
    // 打印 JSON 字符串以供调试
    qDebug() << _blogger->revise_json;

    // 插入或更新到数据库
    QSqlQuery query;
    query.prepare("UPDATE bloggers_info SET revise_json = :revise_json, to_review = 'true' WHERE blogger_id = :blogger_id");
    query.bindValue(":revise_json", _blogger->revise_json); // 使用完整的 reviseInfo JSON 字符串
    query.bindValue(":blogger_id", _blogger->blogger_id);
    if (!query.exec()) {
        qDebug() << "Database update error:" << query.lastError().text();
    }

    accept();
}


void BloggerReviseDialog::on_btn_delete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(nullptr, "警告", "确认删除id:" + _blogger->blogger_id + "的博主吗",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM bloggers_info WHERE blogger_id = :blogger_id");
        query.bindValue(":blogger_id", _blogger->blogger_id);

        if (!query.exec()) {
            qDebug() << "Error deleting record from bloggers_info:" << query.lastError();
        } else {
            qDebug() << "Record deleted successfully from bloggers_info.";
        }
        accept();
    }
}

