#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ApplyFriendItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    ui->addBtn->SetState("normal","hover", "press");
    ui->addBtn->hide();
    connect(ui->addBtn, &ClickedBtn::clicked,  [this](){ //认证好友信号，在applyFriendPage里接收
           emit this->sig_auth_friend(_apply_info);
       });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::SetInfo(std::shared_ptr<ApplyInfo> apply_info)
{
    _apply_info=apply_info;
    QPixmap pixmap(_apply_info->_icon);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);//内容自动缩放
    ui->user_name_lb->setText(_apply_info->_name);
    ui->user_chat_lb->setText(_apply_info->_desc);
}

void ApplyFriendItem::ShowAddBtn(bool bshow)
{
    if(bshow){
        ui->addBtn->show();
        ui->already_add_lb->hide();
        _added=false;
    }else{
        ui->addBtn->hide();
        ui->already_add_lb->show();
        _added=true;
    }
}

QSize ApplyFriendItem::sizeHint() const {
    return QSize(250, 80); // 返回自定义的尺寸
}

int ApplyFriendItem::GetUid()
{
    return _apply_info->_uid;
}