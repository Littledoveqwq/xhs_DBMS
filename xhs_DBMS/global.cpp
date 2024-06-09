#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};

QMap<QString, QString> columnMapping = {
    {"blogger_id", "编号"},
    {"blogger_nickname", "姓名"},
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
    {"project_update_time","最近更新时间"}
};
