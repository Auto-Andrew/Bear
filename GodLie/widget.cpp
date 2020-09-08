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

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //限制QLineEdit输入格式
    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->NUM);
    ui->NUM->setValidator(validator);

    ui->warning->hide();
    ui->voteBox->hide();
    ui->prompt->hide();
    ui->textBrowser->hide();
    ui->textEdit->hide();
    ui->submit->hide();
    ui->voteSubmit->hide();
    ui->eyeOpen->hide();
    ui->noUse->hide();
    ui->skip->hide();
    ui->HlineLeft->hide();
    ui->HlineRight->hide();
    ui->VlineDown->hide();
    ui->VlineUp->hide();
    //移动光标
    connect(ui->textBrowser, &QTextBrowser::textChanged, [=](){ui->textBrowser->moveCursor(QTextCursor::End);});
    //设置提示文字参数
    ui->prompt->setFont(QFont("Microsoft YaHei", 17, -1, false));
    ui->prompt->setStyleSheet("color:#ca0be1");
    ui->prompt->setAlignment(Qt::AlignHCenter);

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
            painter.setBrush(QColor(193,12,177));
            if(i==currentNO)painter.setBrush(QColor(164,224,225));
        }
        int x=px+sin(angle*(i-1)/180*3.14)*R;
        int y=py-cos(angle*(i-1)/180*3.14)*R;
        painter.drawEllipse(QPoint(x,y),r,r);
        if(p[i].state==0)
        {
            painter.drawLine(QPoint(x-32,y+32),QPoint(x+32,y-32));
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
    ui->textBrowser->insertPlainText("\n"+QString::number(k)+"号玩家得到最多票数："+QString::number(p[k].voteNum)+"票 将被驱逐\n");
    //Tie();
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
    if(n<4)
    {
        ui->warning->show();
        ui->NUM->clear();
        return ;
    }
    else
    {
        black=n/2;
        red=n-black;
        SetCard();
        ui->start->hide();
        ui->txt->hide();
        ui->NUM->hide();
        ui->warning->hide();

        ui->prompt->show();
        ui->textBrowser->show();
        ui->textEdit->show();
        ui->submit->show();
        ui->noUse->show();
        ui->HlineLeft->show();
        ui->HlineRight->show();
        ui->VlineDown->show();
        ui->VlineUp->show();

        QMessageBox::about(this, "GameStart", "\n开始游戏\n");
        ui->textBrowser->insertPlainText("开始游戏！！！\n从1号玩家开始轮流发言 选出村长！\n\n");

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
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum++;
        }
        currentNO++;
        Vote();
        update();
        if(currentNO==n+1)
        {
            int i=2;
            int maxVote=p[1].voteNum;
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
            head=maxIndex;
            p[head].vote=(n%2==1?0.5:1.5);
            ui->textBrowser->insertPlainText("\n"+QString::number(maxIndex)+"号玩家得到最多票数："+QString::number(maxVote)
                                             +"票 成为村长 投票时拥有"+QString::number(n%2==1?0.5:1.5)+"票\n");

            for(i=0;i<=n;i++)p[i].voteNum=0;//清空票数
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
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum+=p[currentNO].vote;
        }

        currentNO=NOShift(currentNO);
        update();
        Vote();

        if(currentNO==head)//结束投票->开始放逐
        {
            int i=2;
            int maxVote=p[1].voteNum;
            int maxIndex=1;
            while(i<=n)
            {
                //被驱逐的玩家的voteNum一定是0，可以不考虑
                if(maxVote<p[i].voteNum)
                {
                    maxVote=p[i].voteNum;
                    maxIndex=i;
                }
                i++;
            }
            Deal(maxIndex);

        }
    }
}

void Widget::lastWords(int k)//遗言
{

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

        if(flag==1)//选村长阶段
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
    if(t==0){
        QMessageBox::about(this, "Prompt", "进入黑夜\n\n");
        ui->prompt->setText("黑夜");
        ui->textBrowser->insertPlainText("\n进入黑夜！\n\n");

    }else{
        QMessageBox::about(this, "Prompt", "进入红夜\n");
        ui->prompt->setText("红夜");
        ui->textBrowser->insertPlainText("\n进入红夜！\n\n");

    }
    ui->eyeOpen->show();
    ui->skip->show();
    return t;
}

void Widget::SetCard()//发牌
{
    int a[100];
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

