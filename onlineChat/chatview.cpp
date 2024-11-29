#include "chatview.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

#include <QTimer>
#include <QStyleOption>
#include <QPainter>
//ChatView::ChatView(QWidget *parent):QWidget(parent),isAppended(false)
//{
//    QVBoxLayout *pMainLayout = new QVBoxLayout();//MainLayout
//    this->setLayout(pMainLayout);
//    pMainLayout->setMargin(0);//margin=0,布局和ChatView重合

//    m_pScrollArea=new QScrollArea();
//    m_pScrollArea->setObjectName("chat_area");
//    pMainLayout->addWidget(m_pScrollArea);//设置滚动区域,添加到主布局

//    QWidget *w=new QWidget(this);
//    w->setObjectName("chat_bg");
//    w->setAutoFillBackground(true);

//    QVBoxLayout *pVLayout_1 = new QVBoxLayout();
//    pVLayout_1->addWidget(new QWidget(), 100000);//widget,stretch伸缩因子
//    w->setLayout(pVLayout_1);
//    m_pScrollArea->setWidget(w);

//    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//关掉滚动条
//    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
//    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);//滚动时触发滚动事件

//    //把垂直ScrollBar放到上边 而不是原来的并排
//       QHBoxLayout *pHLayout_2 = new QHBoxLayout();
//       pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);//滚动条放到右边，不允许伸缩
//       pHLayout_2->setMargin(0);
//       m_pScrollArea->setLayout(pHLayout_2);
//       pVScrollBar->setHidden(true);

//       m_pScrollArea->setWidgetResizable(true);
//       m_pScrollArea->installEventFilter(this);
//       initStyleSheet();

//}

ChatView::ChatView(QWidget *parent)
   : QWidget(parent)
   , isAppended(false)
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setMargin(0);

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_pScrollArea);

    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);
    QVBoxLayout *pHLayout_1 = new QVBoxLayout();
    pHLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pHLayout_1);
    m_pScrollArea->setWidget(w);    //应该时在QSCrollArea构造后执行 才对

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);
    //把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setMargin(0);
    m_pScrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->installEventFilter(this);
    initStyleSheet();
}
void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());
    vl->insertWidget(vl->count()-1, item);//每条聊天信息都是一个widget，插入到count()-1位置后
    isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}
void ChatView::removeAllItem()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());

   int count = layout->count();

    for (int i = 0; i < count - 1; ++i) {
        QLayoutItem *item = layout->takeAt(0); // 始终从第一个控件开始删除
        if (item) {
            if (QWidget *widget = item->widget()) {
                delete widget;
            }
            delete item;
        }
    }

}
bool ChatView::eventFilter(QObject *o, QEvent *e)
{
       if(e->type() == QEvent::Enter && o == m_pScrollArea)
        {
            m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
            //滚动条大小为0隐藏
        }
        else if(e->type() == QEvent::Leave && o == m_pScrollArea)
        {
             m_pScrollArea->verticalScrollBar()->setHidden(true);
        }
        return QWidget::eventFilter(o, e);
}

void ChatView::paintEvent(QPaintEvent *event)
{
     QStyleOption opt;
      opt.init(this);
      QPainter p(this);
      style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended){
        QScrollBar *pVScrollbar=m_pScrollArea->verticalScrollBar();
        pVScrollbar->setSliderPosition(pVScrollbar->maximum());
        //500毫秒内可能调用多次
        QTimer::singleShot(500, [this]()
          {
             isAppended = false;
           });
    }
}

void ChatView::initStyleSheet()
{

}
