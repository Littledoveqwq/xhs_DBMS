#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};

std::function<void(QWidget*)> moveToCenter = [](QWidget* w){
    // 获取屏幕的中心点坐标
    QRect primaryScreenGeometry = qApp->primaryScreen()->geometry();
    QPoint centerPoint = primaryScreenGeometry.center();

    // 获取窗口的大小
    QSize windowSize = w->size();

    // 计算窗口左上角的坐标
    int x = centerPoint.x() - windowSize.width() / 2;
    int y = centerPoint.y() - windowSize.height() / 2;

    // 将窗口移动到计算出的坐标位置
    w->move(x, y);
};

std::function<void(QSqlQueryModel*)> updateHeaderToChinese = [](QSqlQueryModel* model){
    for (int col = 0; col < model->columnCount(); ++col) {
        QString englishHeader = model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }
};

// 全局函数，用于设置表头数据
std::function<void(MySqlQueryModel*, const QStringList&)> setTableHeader = [](MySqlQueryModel* model, const QStringList& headers) {
    QStringList col = headers;
    col.push_front("");
    for (int i = 0; i < col.size(); i++) {
        model->setHeaderData(i, Qt::Horizontal, col[i]);
    }
    updateHeaderToChinese(model);
};

QMap<QString, QString> columnMapping = {
    {"blogger_id", "编号"},
    {"blogger_nickname", "昵称"},
    {"blogger_type", "类型"},
    {"blogger_homelink", "主页链接"},
    {"blogger_fans", "粉丝数"},
    {"blogger_likes", "点赞数"},
    {"blogger_noteprice", "笔记价格"},
    {"blogger_videoprice", "视频价格"},
    {"blogger_wechat", "微信"},
    {"blogger_Remark", "备注"},
    {"project_name","项目名称"},
    {"project_manager","项目管理人"},
    {"project_remark", "项目备注"},
    {"project_update_time","最近更新时间"},
    {"note_link","笔记链接"},
    {"note_likes","笔记点赞"},
    {"note_collection","笔记收藏"},
    {"note_remarks","笔记评论"},
    {"repay","返款"},
    {"note_title","笔记标题"}
};

MySqlQueryModel* initSingleTable(const QString &queryStr, const QStringList &headers, QTableView *table)
{
    MySqlQueryModel *model = new MySqlQueryModel;

    // 执行查询并将结果加载到 TableView 中
    model->setQuery(queryStr);
    if (model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    // 更新表头显示为中文
    setTableHeader(model, headers);

    // 设置自定义模型到表格视图中
    table->setModel(model);

    // 取消自动换行
    table->setWordWrap(false);

    // 超出文本不显示省略号
    table->setTextElideMode(Qt::ElideNone);

    // 设置复选框列的宽度为50
    table->setColumnWidth(CHECK_BOX_COLUMN, 10);

    return model;
}
