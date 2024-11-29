#include "searchlist.h"
#include<QScrollBar>
#include "adduseritem.h"
//#include "invaliditem.h"
#include "findsuccessdlg.h"
//#include "findfaildlg.h"
#include "tcpmgr.h"
#include "customizeedit.h"
#include "loadingdlg.h"
#include "userdata.h"
#include "usermgr.h"
#include<QJsonDocument>
SearchList::SearchList(QWidget *parent):QListWidget(parent),_send_pending(false),_search_edit(nullptr),_find_dlg(nullptr)
{
    Q_UNUSED(parent);//是一个宏，用于标记一个未使用的变量，避免编译器发出未使用变量的警告。
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);//需要定义事件过滤器
    connect(this,&QListWidget::itemClicked,this,&SearchList::slot_item_clicked);//连接点击的信号和槽
    //添加条目
     addTipItem();
     //连接搜索条目
     connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    if(_find_dlg){
        _find_dlg->hide();
        _find_dlg=nullptr;//引用计数减一
    }
}

void SearchList::SetSearchEdit(QWidget *edit)
{
    _search_edit=edit;
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
{

        // 检查事件是否是鼠标悬浮进入或离开
        if (watched == this->viewport()) {
            if (event->type() == QEvent::Enter) {
                // 鼠标悬浮，显示滚动条
                this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            } else if (event->type() == QEvent::Leave) {
                // 鼠标离开，隐藏滚动条
                this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            }
        }

        // 检查事件是否是鼠标滚轮事件
        if (watched == this->viewport() && event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            int numDegrees = wheelEvent->angleDelta().y() / 8;
            int numSteps = numDegrees / 15; // 计算滚动步数

            // 设置滚动幅度
            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

            return true; // 停止事件传递
        }

        return QListWidget::eventFilter(watched, event);

}

void SearchList::waitPending(bool pending)
{
    if(pending){//在等待，显示加载对话框，屏蔽对其他对话框访问
        _loadingDialog=new LoadingDlg(this);
        _loadingDialog->setModal(true);
        _loadingDialog->show();
        _send_pending=pending;
    }else{
        _loadingDialog->hide();
        _loadingDialog->deleteLater();
        _send_pending=pending;
    }
}

void SearchList::addTipItem()
{
    auto *invalid_item=new QWidget();//SearchList继承了ListWidget
    QListWidgetItem *item_tmp=new QListWidgetItem();//创建 invalid_item 的目的是为了为 item_tmp 设置一个小部件，以便在 QListWidget 中显示。
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp,invalid_item);
    auto *add_user_item = new AddUserItem();
       QListWidgetItem *item = new QListWidgetItem;
       //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
       item->setSizeHint(add_user_item->sizeHint());
       this->addItem(item);
       this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget=this->itemWidget(item);//获取自定义的widget对象
    if(!widget){
        return;
    }
    //对自定义的widget操作，item转为基类ListItemBase
    ListItemBase *customItem=qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        return;
    }
    auto itemType=customItem->GetItemType();
    if(itemType==ListItemType::INVALID_ITEM){
        return;
    }
    if(itemType==ListItemType::ADD_USER_TIP_ITEM){
        if(_send_pending){
            return;//搜索延迟，
        }
        if(!_search_edit){
            return;
        }
        waitPending(true);
        auto search_edit = dynamic_cast<CustomizeEdit*>(_search_edit);
      auto uid_str = search_edit->text();
      //此处发送请求给server
      QJsonObject jsonObj;
      jsonObj["uid"] = uid_str;//通过uid访问

      QJsonDocument doc(jsonObj);
      QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

      //发送tcp请求给chat server,返回搜索用户的回包
      emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonData);
      return;
    }
    CloseFindDlg();//清除弹出框
}
/**
 * @brief SearchList::slot_user_search
 * @param si
 * 搜索用户信息时触发
 */
void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);//搜索完成，不阻塞
       if(si == nullptr){
           _find_dlg = std::make_shared<FindFailDlg>(this);
       }else{
           auto self_uid= UserMgr::GetInstance()->GetUid();//自己的uid
           if(si->_uid==self_uid)return;
           //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
           //查找是否已经是好友 todo...
           bool bExist=UserMgr::GetInstance()->CheckFriendById(si->_uid);
           if(bExist){
               //处理已经添加的好友，实现页面跳转
               emit sig_jump_chat_item(si);
               return;
           }
           _find_dlg = std::make_shared<FindSuccessDlg>(this);//不是自己的好友，显示添加界面
           std::dynamic_pointer_cast<FindSuccessDlg>(_find_dlg)->SetSearchInfo(si);
       }

       _find_dlg->show();
}
