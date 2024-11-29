#include "chatitembase.h"

ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent):m_role(role),QWidget(parent)
{
       m_pNameLabel    = new QLabel();
       m_pNameLabel->setObjectName("chat_user_name");
       QFont font("Microsoft YaHei");
       font.setPointSize(9);
       m_pNameLabel->setFont(font);
       m_pNameLabel->setFixedHeight(20);
       m_pIconLabel    = new QLabel();
       m_pIconLabel->setScaledContents(true);
       m_pIconLabel->setFixedSize(42, 42);

       m_pBubble=new QWidget();
       QGridLayout *pGLayout=new QGridLayout();
       pGLayout->setVerticalSpacing(3);//设置垂直和水平布局
       pGLayout->setHorizontalSpacing(3);
       pGLayout->setMargin(3);
       QSpacerItem*pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
       if(m_role==ChatRole::Self){
           m_pNameLabel->setContentsMargins(0,0,8,0);//left,top.right,bottom
           m_pNameLabel->setAlignment(Qt::AlignRight);//靠右
           pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
           pGLayout->addWidget(m_pIconLabel, 0, 2, 2,1, Qt::AlignTop);
           pGLayout->addItem(pSpacer, 1, 0, 1, 1);
           pGLayout->addWidget(m_pBubble, 1,1, 1,1);
           pGLayout->setColumnStretch(0, 2);
           pGLayout->setColumnStretch(1, 3);
       }else{
           m_pNameLabel->setContentsMargins(8,0,0,0);
            m_pNameLabel->setAlignment(Qt::AlignLeft);
            pGLayout->addWidget(m_pIconLabel, 0, 0, 2,1, Qt::AlignTop);
            pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
            pGLayout->addWidget(m_pBubble, 1,1, 1,1);
            pGLayout->addItem(pSpacer, 2, 2, 1, 1);
            pGLayout->setColumnStretch(1, 3);
            pGLayout->setColumnStretch(2, 2);
       }
       this->setLayout(pGLayout);//设置布局
}

void ChatItemBase::setUserName(const QString &name)
{
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon)
{
    m_pIconLabel->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout *pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
      pGLayout->replaceWidget(m_pBubble, w);
      delete m_pBubble;//原来的位置指向了一个新的widget，旧的widget无法和layout一个被析构，手动删除
      m_pBubble = w;
}

