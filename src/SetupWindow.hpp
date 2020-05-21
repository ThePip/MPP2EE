#ifndef SETUPWINDOW_HPP
#define SETUPWINDOW_HPP

#include <QDialog>
#include "MppParameters.hpp"
#include <QAbstractButton>

namespace Ui {
class SetupWindow;
}

class SetupWindow : public QDialog
{
    Q_OBJECT

public:
	explicit SetupWindow(MppParameters *mppParameters, QWidget *parent = nullptr);
    ~SetupWindow();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

	void on_stereoSeparationSlider_valueChanged(int value);

	void on_checkBoxEnableSystemTray_toggled(bool checked);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_checkBoxSaveSettingsImmediately_toggled(bool checked);

	void on_pushButtonSaveSettingsToFile_clicked();

	void on_pushButtonLoadSettingsFromFile_clicked();

	private:
    Ui::SetupWindow *ui;
	MppParameters *mppParameters;
	void load();
	void save();
	void restoreDefaults();
};

#endif // SETUPWINDOW_HPP
