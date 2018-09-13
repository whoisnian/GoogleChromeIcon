#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent)
    : QWindow(parent)
    , m_backingStore(new QBackingStore(this))
{
    // 窗口大小
    int w, h;
    w = h = 451;

    // 窗口居中，固定大小
    setGeometry((QApplication::desktop()->width()-w)/4, (QApplication::desktop()->height()-h)/2, (QApplication::desktop()->width())*2/3, h);
    setMaximumSize(QSize(width(), height()));
    setMinimumSize(QSize(width(), height()));

    // 初始化角度，开始计时
    angle = 0;
    startTimer(5);

    // 窗口标题
    setTitle("Google Chrome icon");
    setIcon(QIcon::fromTheme("google-chrome"));
}

void MainWindow::exposeEvent(QExposeEvent *)
{
    // 窗口显示时进行渲染
    if(isExposed())
        renderNow();
}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    // 窗口调整大小后重新渲染
    m_backingStore->resize(resizeEvent->size());
    if(isExposed())
        renderNow();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    // 旋转速度
    angle = (angle+1)%720;

    if(!isExposed())
        return;
    renderNow();
}

void MainWindow::renderNow()
{
    // 窗口隐藏时不渲染
    if(!isExposed())
        return;

    // 创建画布，大小为窗口大小
    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    // 在画布上创建画笔
    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    // 灰色背景
    painter.fillRect(0, 0, width(), height(), QColor("#31363b"));

    // 计算位置偏移
    painter.translate((1-cos(angle*1.0*acos(-1)/360))*(width()-height())/2+height()/2, height()/2);

    // 计算角度偏移
    painter.rotate((1-cos(angle*1.0*acos(-1)/360))*180);

    // 定义需要的颜色
    QColor red("#db4437");
    QColor green("#0f9d58");
    QColor yellow("#ffcd40");
    QColor white("#f1f1f1");
    QColor blue("#4285f4");

    // 画 Chrome 图标
    int l = qMin(width(), height());
    painter.setPen(Qt::NoPen);
    painter.setBrush(red);
    painter.drawChord(-l/3, -l/3, l*2/3, l*2/3, 30*16, 120*16);
    painter.setBrush(green);
    painter.drawChord(-l/3, -l/3, l*2/3, l*2/3, 150*16, 120*16);
    painter.setBrush(yellow);
    painter.drawChord(-l/3, -l/3, l*2/3, l*2/3, 270*16, 120*16);
    painter.setBrush(red);
    painter.drawPie(-int(l*sqrt(3)/3), -int(l*(1+sqrt(3))/6), int(l*sqrt(3)/3), int(l*sqrt(3)/3), 300*16, 60*16);
    painter.setBrush(green);
    painter.drawPie(-int(l*sqrt(3))/6, int(l*(2-sqrt(3))/6), int(l*sqrt(3)/3), int(l*sqrt(3)/3), 60*16, 60*16);
    painter.setBrush(yellow);
    painter.drawPie(0, -int(l*(1+sqrt(3))/6), int(l*sqrt(3)/3), int(l*sqrt(3)/3), 180*16, 60*16);
    painter.setBrush(white);
    painter.drawEllipse(-l/6, -l/6, l/3, l/3);
    painter.setBrush(blue);
    painter.drawEllipse(-l/8, -l/8, l/4, l/4);

    // 渲染结束
    painter.end();
    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}
