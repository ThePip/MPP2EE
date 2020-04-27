#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>


#include <cmath>
#include <cassert>
#include <cstddef>

#include <libopenmpt/libopenmpt.hpp>
#include "ModulePlayerThread.hpp"
#include "ModulePlayer.hpp"
#include "BeeperWIthCallback.hpp"
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    this->settings = new QSettings("ModPlug","ModPlug Player");
    portaudio::System::initialize();
    ui->setupUi(this);
    ui->menubar->hide();
    this->setupWindow = new SetupWindow(this);
    this->setStyleSheet("#PlayerWindow{background-color:#c0c0c0}");
    #ifndef Q_OS_MACOS
        //ui->titleBarPlaceHolder->hide();
    #endif
    //portaudio::AutoSystem autoSys;

    mpThread = new ModulePlayerThread;

    BeeperWIthCallback b;

//    QObject::connect(&b, SIGNAL(b.timech(int)), this, SLOT(PlayerWindow::updateTime(int)));

//    void (PlayerControlButtons::* open)(QString) = &PlayerControlButtons::open;

    //ModulePlayerThread Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this->mpThread, &ModulePlayerThread::open);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this->mpThread, &ModulePlayerThread::stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this->mpThread, &ModulePlayerThread::pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this->mpThread, &ModulePlayerThread::play);
//    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, this->mpThread, &ModulePlayerThread::resume);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::setupClicked);

    //PlayerWindow Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::on_open);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this, &PlayerWindow::on_play);


    QObject::connect(&mpThread->mp, &ModulePlayer::timeChanged, this, &PlayerWindow::updateTime);
    QObject::connect(&mpThread->mp, &ModulePlayer::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);
    QObject::connect(&mpThread->mp, &ModulePlayer::spectrumAnalyzerData, this, &PlayerWindow::onSpetctrumAnalyzerData);

    timer = new QTimer(this);
    scrubTimer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &PlayerWindow::updateTime);
    QObject::connect(scrubTimer, &QTimer::timeout, this, &PlayerWindow::scrubTime);
    timer->start(timerTimeoutValue);

    mpThread->start();

    //mp.play();
    //portaudio::System::instance().sleep(NUM_SECONDS*1000);

    //portAudioSystem.sleep(NUM_SECONDS * 1000);

//    mp.stream.stop();

//    mp.stream.close();


    //mp.play();
    //mp.stream.start();

    //mp.play();
    //MPP mpp;
    //mpp.play();
    //BeeperWIthCallback b;
    //b.open(this->portAudioSystem);
}

void PlayerWindow::updateTime() {
    TimeInfo timeInfo = mpThread->mp.getTimeInfo();
    ui->timeScrubber->setValue(timeInfo.globalRowIndex);
    ui->lcdPanel->updateTime(mpThread->mp.getTimeInfo().seconds);
}

void PlayerWindow::setTimeScrubberTicks(int amount) {
    if(amount == 0)
        amount = 1;
    ui->timeScrubber->setMaximum((amount-1));
}

void PlayerWindow::setupClicked()
{
    setupWindow->show();
}

void PlayerWindow::onSpetctrumAnalyzerData(int amount, double *amplitudes)
{
    qDebug()<<amplitudes[0];
    delete amplitudes;
}

PlayerWindow::~PlayerWindow()
{
    portaudio::System::terminate();
    delete this->setupWindow;
    delete ui;
}

void PlayerWindow::on_timeScrubber_sliderMoved(int position)
{
    scrubberClickedPosition = position;
    //mp->mp.scrubTime(position);
}

void PlayerWindow::scrubTime(){
    if(scrubberClickedPosition != scrubberPreviousValue)
        mpThread->mp.scrubTime(scrubberClickedPosition);
    scrubberPreviousValue = scrubberClickedPosition;
}

void PlayerWindow::on_timeScrubber_sliderPressed()
{
    timer->stop();
    scrubberClickedPosition = ui->timeScrubber->value();
    scrubTimer->start(scrubTimerTimeoutValue);
    mpThread->mp.scrubTime(scrubberClickedPosition);
}

void PlayerWindow::on_timeScrubber_sliderReleased()
{
//    updateTime();
    scrubTimer->stop();
    timer->start(timerTimeoutValue);
}

double PlayerWindow::getExponentialVolume(double &linearVolume){
    if(linearVolume == 0)
        return 0;
    if(linearVolume == 1)
        return 1;
    return exp(pow(6.908, linearVolume)) / 1000.0f;
}

void PlayerWindow::on_volumeControl_valueChanged(int value)
{
    double linearVolume = ((double)value)/100.0f;
    double exponentialVolume = getExponentialVolume(linearVolume);
    mpThread->mp.setVolume(exponentialVolume);
    qDebug()<<"Linear Volume: "<<linearVolume;
    qDebug()<<"Exponential Volume "<<exponentialVolume;
}

void PlayerWindow::on_open(QString filePath)
{
}

void PlayerWindow::on_stop()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Stop";
}

void PlayerWindow::on_play()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Play";
}
void PlayerWindow::on_pause()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Pause";
}
