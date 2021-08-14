#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"
#include <cmath>
#include <QDebug>
#include <QColorDialog>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <DSP.hpp>
#include <QString>

SetupWindow::SetupWindow(MppParameters *parameters, PlayerWindow *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow)
{
	this->playerWindow = parent;
	this->parameters = parameters;
	initAudioIcons();

	ui->setupUi(this);
	load();
	connect(ui->pushButton_TitleBar_Active, SIGNAL(colorChanged()), this, SLOT(onActiveTitleBarTextColorChanged()));
	connect(ui->pushButton_TitleBar_Inactive, SIGNAL(colorChanged()), this, SLOT(onInactiveTitleBarTextColorChanged()));
	connect(ui->pushButton_ButtonLights_Active, SIGNAL(colorChanged()), this, SLOT(onActiveButtonLightColorChanged()));
	connect(ui->pushButton_ButtonLights_Inactive, SIGNAL(colorChanged()), this, SLOT(onInactiveButtonLightColorChanged()));
	connect(ui->pushButton_PlayerBody_Text, SIGNAL(colorChanged()), this, SLOT(onPlayerBodyTextColorChanged()));
	connect(ui->pushButton_PlayerBody_Background, SIGNAL(colorChanged()), this, SLOT(onPlayerBodyBackgroundColorChanged()));
	connect(ui->pushButton_LCDDisplay_Foreground, SIGNAL(colorChanged()), this, SLOT(onLcdDisplayForegroundColorChanged()));
	connect(ui->pushButton_LCDDisplay_Background, SIGNAL(colorChanged()), this, SLOT(onLcdDisplayBackgroundColorChanged()));
	initAudioInterfaceList();
	ui->pages->setCurrentIndex(0);
    ui->treeMenu->expandAll();
    ui->treeMenu->hideColumn(1);
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::onActiveTitleBarTextColorChanged(){
	parameters->activeTitlebarTextColor = RGB(ui->pushButton_TitleBar_Active->getColor());
	parameters->save();
}

void SetupWindow::onInactiveTitleBarTextColorChanged(){
	parameters->inactiveTitlebarTextColor = RGB(ui->pushButton_TitleBar_Inactive->getColor());
	parameters->save();
}

void SetupWindow::onActiveButtonLightColorChanged(){
	parameters->activeButtonLightColor = RGB(ui->pushButton_ButtonLights_Active->getColor());
	parameters->save();
}

void SetupWindow::onInactiveButtonLightColorChanged(){
	parameters->inactiveButtonLightColor = RGB(ui->pushButton_ButtonLights_Inactive->getColor());
	parameters->save();
}

void SetupWindow::onPlayerBodyTextColorChanged(){
	parameters->playerBodyTextColor = RGB(ui->pushButton_PlayerBody_Text->getColor());
	parameters->save();
}

void SetupWindow::onPlayerBodyBackgroundColorChanged(){
	parameters->playerBodyBackgroundColor = RGB(ui->pushButton_PlayerBody_Background->getColor());
	parameters->save();
}

void SetupWindow::onLcdDisplayForegroundColorChanged(){
	parameters->lcdDisplayForegroundColor = RGB(ui->pushButton_LCDDisplay_Foreground->getColor());
	parameters->save();
}

void SetupWindow::onLcdDisplayBackgroundColorChanged(){
	parameters->lcdDisplayBackgroundColor = RGB(ui->pushButton_LCDDisplay_Background->getColor());
	parameters->save();
}

void SetupWindow::on_stereoSeparationSlider_valueChanged(int value)
{
	ui->labelStereoSeparation->setText(QString::number(value) + "%");
}

void SetupWindow::on_checkBoxEnableSystemTray_toggled(bool checked)
{
	if(!checked) {
		ui->checkBoxMinimizeToSystemTray->setChecked(false);
		ui->checkBoxHideApplicationIcon->setChecked(false);
	}
	ui->groupBoxSystemTray->setEnabled(checked);
}

void SetupWindow::on_buttonBox_clicked(QAbstractButton *button) {
	if(button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
		qDebug()<<"ok";
		save();
		playerWindow->loadSettings();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Cancel)) {
		qDebug()<<"cancel";
		//load();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Apply)) {
		qDebug()<<"apply";
		save();
		playerWindow->loadSettings();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Reset)) {
		qDebug()<<"reset";
		load();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		qDebug()<<"restore defaults";
		restoreDefaults();
	}

}

void SetupWindow::load()
{
	qDebug()<<parameters->volume;
	ui->pushButton_TitleBar_Active->setColor(parameters->activeTitlebarTextColor);
	ui->pushButton_TitleBar_Inactive->setColor(parameters->inactiveTitlebarTextColor);
	ui->pushButton_ButtonLights_Active->setColor(parameters->activeButtonLightColor);
	ui->pushButton_ButtonLights_Inactive->setColor(parameters->inactiveButtonLightColor);
	ui->pushButton_PlayerBody_Text->setColor(parameters->playerBodyTextColor);
	ui->pushButton_PlayerBody_Background->setColor(parameters->playerBodyBackgroundColor);
	ui->pushButton_LCDDisplay_Foreground->setColor(parameters->lcdDisplayForegroundColor);
	ui->pushButton_LCDDisplay_Background->setColor(parameters->lcdDisplayBackgroundColor);

	qDebug()<<"load";
}

void SetupWindow::save()
{
	qDebug()<<"save";
	int r,g,b;
	ui->pushButton_TitleBar_Active->getColor().getRgb(&r, &g, &b);
	parameters->activeTitlebarTextColor = RGB(r,g,b);
	//parameters->save();
}

void SetupWindow::restoreDefaults()
{
	qDebug()<<"restore defaults";
}

void SetupWindow::initAudioIcons()
{
	iconCoreAudio.addPixmap(QPixmap(":/Graphics/Raster/CoreAudio.png"));
	iconWdmAudio.addPixmap(QPixmap(":/Graphics/Raster/WDM.png"));
	iconDirectXAudio.addPixmap(QPixmap(":/Graphics/Raster/DirectX.png"));
	iconAlsaAudio.addPixmap(QPixmap(":/Graphics/Raster/alsa.png"));
	iconOssAudio.addPixmap(QPixmap(":/Graphics/Raster/oss.png"));
	iconAsioAudio.addPixmap(QPixmap(":/Graphics/Raster/ASIO.png"));
	iconJackAudio.addPixmap(QPixmap(":/Graphics/Raster/JackAudio.png"));
}

void SetupWindow::initAudioInterfaceList()
{
	portaudio::System &sys = portaudio::System::instance();
	int numDevices = sys.deviceCount();

	try {
		for (portaudio::System::DeviceIterator i = sys.devicesBegin(); i != sys.devicesEnd(); ++i){
			std::cout << "--------------------------------------- device #" << (*i).index() << std::endl;

			// Mark global and API specific default devices:
			bool defaultDisplayed = false;

			if ((*i).isSystemDefaultInputDevice())
			{
				std::cout << "[ Default Input";
				defaultDisplayed = true;
			}
			else if ((*i).isHostApiDefaultInputDevice())
			{
				std::cout << "[ Default " << (*i).hostApi().name() << " Input";
				defaultDisplayed = true;
			}

			if ((*i).isSystemDefaultOutputDevice())
			{
				std::cout << (defaultDisplayed ? "," : "[");
				std::cout << " Default Output";
				defaultDisplayed = true;
			}
			else if ((*i).isHostApiDefaultOutputDevice())
			{
				std::cout << (defaultDisplayed ? "," : "[");
				std::cout << " Default " << (*i).hostApi().name() << " Output";
				defaultDisplayed = true;
			}

			if (defaultDisplayed)
				std::cout << " ]" << std::endl;
			if(!(*i).isInputOnlyDevice()){
				QString devStr = QString("%1 - %2").arg((*i).hostApi().name(), (*i).name());
				if((*i).isSystemDefaultOutputDevice() || (*i).isHostApiDefaultOutputDevice())
					devStr += " (Default)";
				ui->comboBoxSoundDevices->addItem(iconCoreAudio, devStr, (*i).index());
			}
			// Print device info:
			std::cout << "Name                        = " << (*i).name() << std::endl;
			std::cout << "Host API                    = " << (*i).hostApi().name() << std::endl;
			std::cout << "Max inputs = " << (*i).maxInputChannels() << ", Max outputs = " << (*i).maxOutputChannels() << std::endl;

			std::cout << "Default low input latency   = " << (*i).defaultLowInputLatency() << std::endl; // 8.3
			std::cout << "Default low output latency  = " << (*i).defaultLowOutputLatency() << std::endl; // 8.3
			std::cout << "Default high input latency  = " << (*i).defaultHighInputLatency() << std::endl; // 8.3
			std::cout << "Default high output latency = " << (*i).defaultHighOutputLatency() << std::endl; // 8.3

		}
	}
	catch (const portaudio::PaException &e)
	{
		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
	}
	catch (const portaudio::PaCppException &e)
	{
		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "A generic exception occured: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "An unknown exception occured." << std::endl;
	}
}

QIcon SetupWindow::getAudioIcon(std::string &hostApiName)
{
	if(hostApiName == "Core Audio")
		return iconCoreAudio;
	return QIcon();
}

void SetupWindow::on_checkBoxSaveSettingsImmediately_toggled(bool checked)
{
	if(checked) {
		ui->buttonBox->hide();
	}
	else {
		ui->buttonBox->show();
	}
}

void SetupWindow::on_checkBoxHideTitleBar_toggled(bool checked)
{
	playerWindow->onHideTitleBarRequested(checked);
}

void SetupWindow::on_pushButton_SetBufferLengthToDefaultValue_clicked()
{
	ui->spinBox_BufferLength->setValue(1024);
}

void SetupWindow::on_treeMenu_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current->text(1) == "General")
        ui->pages->setCurrentWidget(ui->generalPage);
    else if(current->text(1) == "SoundCard")
        ui->pages->setCurrentWidget(ui->soundCardPage);
    else if(current->text(1) == "Player")
        ui->pages->setCurrentWidget(ui->playerPage);
    else if(current->text(1) == "Directories")
        ui->pages->setCurrentWidget(ui->directoriesPage);
    else if(current->text(1) == "Visualization")
        ui->pages->setCurrentWidget(ui->visualizationPage);
    else if(current->text(1) == "Oscilloscope")
        ui->pages->setCurrentWidget(ui->oscilloscopePage);
    else if(current->text(1) == "SpectrumAnalyzer")
        ui->pages->setCurrentWidget(ui->spectrumAnalyzerPage);
    else if(current->text(1) == "VUMeter")
        ui->pages->setCurrentWidget(ui->vuMeterPage);
    else if(current->text(1) == "Display")
        ui->pages->setCurrentWidget(ui->displayPage);
    else if(current->text(1) == "Window")
        ui->pages->setCurrentWidget(ui->windowPage);
    else if(current->text(1) == "Colors")
        ui->pages->setCurrentWidget(ui->colorsPage);
    else if(current->text(1) == "PlugIns")
        ui->pages->setCurrentWidget(ui->pluginsPage);

}


void SetupWindow::on_horizontalSlider_ledAmount_sliderMoved(int position)
{
    ui->label_ledAmount->setText(QString::number(position));
}


void SetupWindow::on_comboBox_spectrumAnalyzerType_currentIndexChanged(int index)
{
    if(index == 0) {
        ui->label_ledAmount->show();
        ui->label_ledAmount_Text->show();
        ui->horizontalSlider_ledAmount->show();
        ui->label_ledSize->show();
        ui->label_ledSize_Text->show();
        ui->horizontalSlider_ledSize->show();

        ui->label_peakHeight->hide();
        ui->label_peakHeight_text->hide();
        ui->horizontalSlider_peakHeight->hide();
    }
    else if(index == 1) {
        ui->label_ledAmount->hide();
        ui->label_ledAmount_Text->hide();
        ui->horizontalSlider_ledAmount->hide();
        ui->label_ledSize->hide();
        ui->label_ledSize_Text->hide();
        ui->horizontalSlider_ledSize->hide();

        if(ui->checkBox_showPeaks->isChecked()) {
            ui->label_peakHeight->show();
            ui->label_peakHeight_text->show();
            ui->horizontalSlider_peakHeight->show();
        }
    }
}


void SetupWindow::on_checkBox_showPeaks_stateChanged(int checked)
{
    if(checked) {
        ui->label_peakTimeout->show();
        ui->label_peakTimeout_text->show();
        ui->horizontalSlider_peakTimeout->show();
        if(ui->comboBox_spectrumAnalyzerType->currentIndex() == 1) {
            ui->label_peakHeight->show();
            ui->label_peakHeight_text->show();
            ui->horizontalSlider_peakHeight->show();
        }
    }
    else {
        ui->label_peakTimeout->hide();
        ui->label_peakTimeout_text->hide();
        ui->horizontalSlider_peakTimeout->hide();
        ui->label_peakHeight->hide();
        ui->label_peakHeight_text->hide();
        ui->horizontalSlider_peakHeight->hide();
    }
}

void SetupWindow::on_horizontalSlider_peakTimeout_valueChanged(int value)
{
    ui->label_peakTimeout->setText(QString::number(value*250) + " ms");
}

