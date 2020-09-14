#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <QWidget>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <math.h>
#include <QPainter>
#include <QLabel>
#include <QFont>
#include <QTextBrowser>
#include <QKeyEvent>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QFrame>
#include <QLine>
#include <QImage>
#include <QPalette>
#include <QMovie>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //限制QLineEdit输入格式
    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->NUM);
    ui->NUM->setValidator(validator);

    ui->warning->setTextFormat(Qt::RichText);

    ui->warning->hide();
    ui->voteBox->hide();
    ui->prompt->hide();
    ui->textBrowser->hide();
    ui->textEdit->hide();
    ui->submit->hide();
    ui->voteSubmit->hide();
    ui->eyeOpen->hide();
    ui->skip->hide();
    ui->frame->hide();
    ui->title->hide();

    //移动光标
    connect(ui->textBrowser, &QTextBrowser::textChanged, [=](){ui->textBrowser->moveCursor(QTextCursor::End);});

    ui->frame->setPixmap(QPixmap("../frame.png"));
    ui->frame->setScaledContents(true);

    //设置提示文字参数
    ui->prompt->setFont(QFont("华文中宋", 17, -1, false));
    ui->prompt->setStyleSheet("color:#3f77c4");
    ui->prompt->setAlignment(Qt::AlignHCenter);

    QMovie *mov = new QMovie("../rect.gif");
    ui->decoration->setMovie(mov);
    mov->start();
    ui->decoration->setScaledContents(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)//绘制
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,1294,648,QPixmap("../GOD_LIE.png"));
    if(flag==0){
        painter.end();
        return;
    }

    if(pShow==1)
    ui->prompt->setText(QString::number(currentNO)+"号玩家开始发言");

    int px=350;
    int py=320;
    int R=240;
    int r=45;
    double angle=360.0/n;

    //QPainter painter;
    //painter.begin(this);
    painter.setFont(QFont("Microsoft YaHei", 18, QFont::Bold, false));
    painter.setPen(QPen(QBrush(Qt::black), 3, Qt::SolidLine));

    //painter.drawPixmap(0,0,1294,648,QPixmap("../GOD_LIE.png"));

    for(int i=1;i<=n;i++)
    {

        if(p[i].state==0)
        {
            painter.setBrush(Qt::darkGray);
        }
        else
        {
            painter.setBrush(QColor(93, 164, 252));//248, 142, 70
            if(i==currentNO)painter.setBrush(QColor(164,224,225));
        }
        int x=int(px+sin(angle*(i-1)/180*3.14)*R);
        int y=int(py-cos(angle*(i-1)/180*3.14)*R);
        painter.drawEllipse(QPoint(x,y),r,r);
        if(p[i].state==0)
        {
            painter.drawLine(QPoint(x-32,y+32),QPoint(x+32,y-32));
        }else{
            if(p[i].identity==0&&i==currentNO)
                painter.drawPixmap(x+50, y-20, 40, 40, QPixmap("../blackMask.png"));
            else if(p[i].identity==1&&i==currentNO){
                 painter.drawPixmap(x+50, y-20, 40, 40, QPixmap("../redMask.png"));
            }
            if(i==head&&head!=0)
                 painter.drawPixmap(x-32, y-63, 64, 64, QPixmap("../head.png"));
        }

        painter.drawText(QRectF(int(x-r/2),int(y-r/2),r,r),QString::number(i),QTextOption(Qt::AlignCenter));

    }
    painter.end();
}

void Widget::Vote()//投票
{
    ui->voteBox->clear();
    ui->voteBox->show();

    if(chFlag==1)//决定方向
    {
        ui->voteBox->addItem("Left");
        ui->voteBox->addItem("Right");
        return;
    }
    for(int i=1; i<=n; i++)//投出 移交
    {
        if(p[i].state==1)
        {
            ui->voteBox->addItem(QString::number(i));
        }
    }
    if(chFlag==0)ui->voteBox->addItem("0");//弃选项
}

void Widget::Deal(int k)//放逐+判断游戏是否结束
{
    //ui->textBrowser->insertPlainText("\n"+QString::number(k)+"号玩家得到最多票数 将被驱逐\n");

    QMessageBox::about(this, "Prompt", QString::number(k)+"号玩家被驱逐\t\n");
    ui->prompt->setText(QString::number(k)+"号玩家被驱逐");
    ui->textBrowser->insertPlainText(QString::number(k)+"号玩家被驱逐！身份为：");

    p[k].state=0;
    for(int i=1;i<=n;i++)//票数清零
        p[i].voteNum=0;

    if(p[k].identity==0){//黑牌
        black--;
        ui->textBrowser->insertPlainText("黑牌！\n\n");
    }
    else{//红牌
        red--;
        ui->textBrowser->insertPlainText("红牌！\n\n");
    }
    if(red==0||black==0)//游戏结束
    {
        QMessageBox MSG;
        MSG.setTextFormat(Qt::RichText);
        if(black==0)
            MSG.about(this, "GameOver", "游戏结束<br><font color='red'><b>红方</b></font></br>获胜");
        else
            MSG.about(this, "GameOver", "游戏结束<br><font color='black'><b>黑方</b></font></br>获胜");
        this->close();
        return;
    }

    ui->textBrowser->insertPlainText("游戏继续！\n\n");

    if(k!=head)//平民
    {
        ui->voteBox->hide();
        ui->voteSubmit->hide();
        currentNO=head;
        update();
        double t=((black+red)%2==1?0.5:1.5);
        p[head].vote=t;
        ui->textBrowser->insertPlainText("村长的票权数（可能）被改变！投票时拥有："+QString::number(t)+"票！\n");
        night=NightShift();
        return;

    }else{//村长
        ui->textBrowser->insertPlainText("被驱逐是村长！村长移交下一任村长！\n\n");
        ui->prompt->setText("请村长移交下一任村长");
        currentNO=head;
        chFlag=2;
        Vote();
        update();
    }
}

void Widget::on_start_clicked()//开始游戏
{

    n = ui->NUM->text().toInt();
    if(n<3)
    {
        ui->warning->setText("<font color='red' size='5'><b>至少需要3人参与哦！！！</b></font>");
        ui->warning->show();
        ui->NUM->clear();
        return ;
    }
    else if(n>20)
    {
        ui->warning->setText("<font color='red' size='5'><b>人数太多啦，不要超过20人哦！！！</b></font>");
        ui->warning->show();
        ui->NUM->clear();
    }
    else
    {
        black=n/2;
        red=n-black;
        SetCard();//发牌
        ui->start->hide();
        ui->txt->hide();
        ui->NUM->hide();
        ui->warning->hide();
        ui->decoration->hide();

        QMessageBox MSG;
        MSG.setTextFormat(Qt::RichText);
        MSG.about(this, "游戏规则", "<div style=\"width: 485px;height: 300px\">"
                                        "<p style=\"text-align: center;\">--<b><font color=\"red\">红</font><font color=\"black\">黑</font></b>夜--</p>"
                                            "<ol>"
                                            "<li>游戏分为红牌方和黑牌方，红牌方不少于黑牌方且最多比黑牌方多一人；</li>"
                                            "<li>白天时按指定顺序进行发言，发言结束时进行进行投票（投0号即弃票）驱逐；</li>"
                                            "<li>白天结束后可能进入红夜或黑夜，红牌或黑牌可以在任意夜选择睁眼；</li>"
                                            "<li>在开局时需要选出（选0号即弃票）村长，村长被驱逐后也需要移交下一任村长，天亮时若场上玩家数为奇数则村长投票时拥有0.5票，否则为1.5票；黑夜时黑牌睁眼数为偶数，或红夜时红方睁眼数为偶数，也能将天亮后村长投票时的票数更改为1票；新任村长需要在任职当天选择白天发言方向;</li>"
                                            "<li>当红牌或黑牌玩家全部被驱逐后，游戏结束，场上留下的一方胜利</li>"
                                            "</ol>"
                                   "</div>");
        ui->textBrowser->insertPlainText("开始游戏！！！\n从1号玩家开始轮流发言 选出村长！\n\n");

        ui->prompt->show();
        ui->textBrowser->show();
        ui->textEdit->show();
        ui->submit->show();
        ui->frame->show();
        ui->title->show();

        flag=1;//切换为选村长状态
        pShow=1;
        currentNO=1;

        update();
        return ;
    }

}

void Widget::on_submit_clicked()//提交发言
{
    ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家："+"\n");
    ui->textBrowser->insertPlainText(ui->textEdit->toPlainText()+"\n");
    ui->textEdit->clear();
    if(flag==1)//选村长
    {
        currentNO++;
        update();
        if(currentNO==n+1)//发言结束->开始投票
        {
            currentNO=1;
            pShow=0;
            ui->prompt->setText("请开始投票选出村长");
            ui->textBrowser->insertPlainText("开始投票选出村长！\n\n");
            ui->voteSubmit->show();
            ui->submit->hide();
            Vote();
        }
    }
    else if(flag==2)//投出
    {
        if(currentNO==head)//发言结束->开始投票
        {
            pShow=0;
            ui->prompt->setText("请开始投票进行驱逐");
            ui->textBrowser->insertPlainText("开始投票进行驱逐！\n\n");
            ui->voteSubmit->show();
            ui->submit->hide();
            Vote();
            return;
        }
        currentNO=NOShift(currentNO);
        update();
    }

}

void Widget::on_voteSubmit_clicked()//提交选择
{
    if(chFlag==1)//村长选择发言方向
    {
        if(ui->voteBox->currentText()=="Left")
        {
            right=0;
            ui->textBrowser->insertPlainText("从村长左边开始发言！\n\n");
        }else
        {
            right=1;
            ui->textBrowser->insertPlainText("从村长右边开始发言！\n\n");
        }
        currentNO=NOShift(head);

        ui->voteBox->hide();
        ui->voteSubmit->hide();
        ui->submit->show();
        flag=2;
        chFlag=0;
        pShow=1;
        update();
        return;
    }else if(chFlag==2){//移交村长
        head=ui->voteBox->currentText().toInt();
        double t=((black+red)%2==1?0.5:1.5);
        p[head].vote=t;
        ui->textBrowser->insertPlainText(QString::number(head)+"号玩家被移交为村长！ 投票时拥有"+QString::number(t)+"票！\n");
        ui->voteSubmit->hide();
        ui->voteBox->hide();
        currentNO=head;
        night=NightShift();
        return;
    }

    if(flag==1)//选村长
    {
        if(ui->voteBox->currentText()=="0")
        {
            //ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            //ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum++;
        }
        update();
        currentNO++;
        Vote();
        if(currentNO==n+1)//选村长结束->进入红/黑夜
        {
            //平票
            head=Tie();
            p[head].vote=(n%2==1?0.5:1.5);
            /*ui->textBrowser->insertPlainText("\n"+QString::number(maxIndex)+"号玩家得到最多票数："+QString::number(maxVote)
                                             +"票 成为村长 投票时拥有"+QString::number(n%2==1?0.5:1.5)+"票\n");*/
            ui->textBrowser->insertPlainText(QString::number(head)+"号玩家成为村长 投票时拥有"+QString::number(n%2==1?0.5:1.5)+"票\n");
            for(int i=0;i<=n;i++)p[i].voteNum=0;//清空票数
            currentNO=head;
            ui->voteBox->hide();
            ui->voteSubmit->hide();
            night=NightShift();//选出村长后 进入黑/红夜
            return;
        }
    }
    else if(flag==2)//投出
    {
        if(ui->voteBox->currentText()=="0")
        {
            //ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            //ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum+=p[currentNO].vote;
        }

        currentNO=NOShift(currentNO);
        update();
        Vote();

        if(currentNO==head)//结束投票->开始放逐
        {
            Deal(Tie());
        }
    }
}

int Widget::Tie()//平票函数
{    
    int i=2;
    double maxVote=p[1].voteNum;
    int maxIndex=1;
    while(i<=n)
    {
        if(maxVote<p[i].voteNum)
        {
            maxVote=p[i].voteNum;
            maxIndex=i;
        }
        i++;
    }

    memset(temp, 0, sizeof (temp));
    int cnt=0;
    i=1;
    while(i<=n){
        if(fabs(p[i].voteNum-maxVote)<0.001)
            temp[cnt++]=i;
        i++;
    }
    if(cnt==1)return maxIndex;//未出现平票

    i=0;
    while(i<cnt){
        ui->textBrowser->insertPlainText(QString::number(temp[i])+"号 ");
        i++;
    }
    ui->textBrowser->insertPlainText("出现了平票\n\n");

    QMessageBox::about(this, "Prompt", "系统将为平票玩家抽取随机数\n\n");

    ui->textBrowser->insertPlainText("系统为平票玩家抽取了随机数：\n\n");

    i=0;
    int maxRandom=0;
    while(i<cnt){

        //srand((unsigned)time(NULL));
        int t=rand()%1024;
        ui->textBrowser->insertPlainText(QString::number(temp[i])+"号玩家："+QString::number(t)+"\n\n");
        if(t>maxRandom){
            maxRandom=t;
            maxIndex=temp[i];
        }
        i++;
    }
    return maxIndex;
}

void Widget::on_eyeOpen_clicked()//睁眼
{
    p[currentNO].eye=1;
    update();
    currentNO=NOShift(currentNO);
    if(currentNO==head)//结束夜晚
    {

        ui->eyeOpen->hide();
        ui->skip->hide();
        QMessageBox::about(this, "Prompt", "天亮了\n");

        int redEye=0, blackEye=0;
        for(int i=1;i<=n;i++)
        {
            if(p[i].eye==1){
                if(p[i].identity==1)
                    redEye++;
                else blackEye++;
            }
        }
        if((blackEye!=0&&blackEye%2==0&&night==0)||(redEye!=0&&redEye%2==0&&night==1))
        {
            ui->textBrowser->insertPlainText("村长的票权数（可能）被改变！投票时拥有："+QString::number(1)+"票！\n\n");
            p[head].vote=1;
        }

        for(int i=1;i<=n;i++)
            p[i].eye=0;
        night=-1;

        if(flag==1||chFlag==2)//选村长阶段+村长被移交
        {
          ui->prompt->setText("请村长决定发言顺序");
          chFlag=1;
          Vote();
          ui->voteSubmit->show();
          update();
          return;
        }

        pShow=1;
        currentNO=NOShift(currentNO);
        ui->submit->show();
        update();
    }
}

void Widget::on_skip_clicked()//跳过/不睁眼
{
    update();
    currentNO=NOShift(currentNO);
    if(currentNO==head)//结束夜晚
    {
        ui->eyeOpen->hide();
        ui->skip->hide();
        QMessageBox::about(this, "Prompt", "天亮了\n");

        int redEye=0, blackEye=0;
        for(int i=1;i<=n;i++)
        {
            if(p[i].eye==1){
                if(p[i].identity==1)
                    redEye++;
                else blackEye++;
            }
        }
        if((blackEye!=0&&blackEye%2==0&&night==0)||(redEye!=0&&redEye%2==0&&night==1))
        {
            ui->textBrowser->insertPlainText("村长的票权数（可能）被改变！投票时拥有："+QString::number(1)+"票！\n\n");
            p[head].vote=1;
        }

        for(int i=1;i<=n;i++)
            p[i].eye=0;
        night=-1;

        if(flag==1||chFlag==2)//选村长阶段+村长被移交
        {
          ui->prompt->setText("请村长决定发言顺序");
          chFlag=1;
          Vote();
          ui->voteSubmit->show();
          update();
          return;
        }

        pShow=1;
        currentNO=NOShift(currentNO);
        ui->submit->show();
        update();
    }
}

int Widget::NightShift()//进入红/黑夜
{
    srand((unsigned)time(NULL));
    int t=rand()%2;
    QMessageBox MSG(this);
    MSG.setTextFormat(Qt::RichText);
    if(t==0){//黑夜
        MSG.about(this, "Prompt", "<div>进入<b><font color='black'>黑夜</font></b></div>");
        ui->prompt->setText("黑夜");
        ui->textBrowser->insertPlainText("\n进入黑夜！\n\n");
    }else{//红夜
        MSG.about(this, "Prompt", "<div>进入<b><font color='red'>红夜</font></b></div>");
        ui->prompt->setText("红夜");
        ui->textBrowser->insertPlainText("\n进入红夜！\n\n");
    }
    ui->eyeOpen->show();
    ui->skip->show();
    return t;
}

void Widget::SetCard()//发牌
{
    int a[30];
    for(int i=1;i<=n;i++)a[i]=i;
    for(int i=n;i>=1;i--)
    {
        srand((unsigned)time(NULL));
        //swap(a[i], a[rand()%i+1]);
        int t=rand()%i+1;
        int tt=a[i];
        a[i]=a[t];
        a[t]=tt;
    }
    for(int i=1;i<=n;i++)p[i].identity=a[i]%2;
}

int Widget::NOShift(int k)
{
    int i;
    if(right==1)
    {
        if(k==1)i=n;
        else i=k-1;
        while(p[i].state==0)
        {
            if(i==1)i=n;
            else i--;
        }
    }
    else
    {
        if(k==n)i=1;
        else i=k+1;
        while(p[i].state==0){
            if(i==n)i=1;
            else i++;
        }
    }
    return i;
}

//DOWN
