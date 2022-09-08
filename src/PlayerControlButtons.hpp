/*
PlayerControlButtons class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PLAYERCONTROLBUTTONS_HPP
#define PLAYERCONTROLBUTTONS_HPP

#include <QWidget>
#include <QFileDialog>
#include <RGB.hpp>
#include "Enums.hpp"
#include "SVGIcon.hpp"
#include "LEDColorProperties.hpp"
#include "TextColorProperties.hpp"
#include <vector>

namespace Ui {
class PlayerControlButtons;
}

class PlayerControlButtons : public QWidget, public LEDColorProperties, public TextColorProperties
{
    Q_OBJECT

public:
    explicit PlayerControlButtons(QWidget *parent = nullptr);
    ~PlayerControlButtons();
    void setActiveButtonLightColor(const RGB &color);
    void setInactiveButtonLightColor(const RGB &color);
    void setBackgroundColor(const RGB &color);
    void setTextColor(const RGB &color);
    void setState(const PlayerState &state);
	void refresh();
signals:
    void open();
    void play();
    void pause();
    void stop();
    void setup();
    void rewind();
    void fastForward();
    void previous();
    void next();

public slots:
	void on_playerState_changed(PlayerState playerState);

private:

	Ui::PlayerControlButtons *ui;

	PlayerState state;

	RGB activeButtonLightColor, inactiveButtonLightColor;

	SVGIcon *iconOpen, *iconPlay, *iconPause, *iconStop,
            *iconRewind, *iconFastForward, *iconPrevious, *iconNext;
protected:
    void refreshStyleSheet() override;
    std::vector<QPushButton *> buttons;
    std::vector<SVGIcon *> icons;
};

#endif // PLAYERCONTROLBUTTONS_HPP
