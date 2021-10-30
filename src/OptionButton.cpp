#include "OptionButton.hpp"
#include <QPainter>
#include "SVGIconEngine.hpp"
#include "ResourceUtil.hpp"

OptionButton::OptionButton(QWidget* parent):QPushButton(parent)
{
    setActiveButtonLightColor(RGB(std::string("#00FF00")));
    setInactiveButtonLightColor(RGB(std::string("#00FFFF")));
    buttonLightSize = 7;
    iconButtonLight = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/toprightcorner.svg"), "#00ff00");
}

void OptionButton::setActiveButtonLightColor(const RGB & color)
{
    this->activeButtonLightColor = color;
    active = QColor(color.red, color.green, color.blue);
    //iconButtonLight->setActiveColor(color);
}

void OptionButton::setInactiveButtonLightColor(const RGB & color)
{
    this->inactiveButtonLightColor = color;
    inactive = QColor(color.red, color.green, color.blue);
    //iconButtonLight->setInactiveColor(color);
}

void OptionButton::paintEvent(QPaintEvent * event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::SmoothPixmapTransform);
    painter.setPen(QPen(Qt::black, 0.5));
    int shiftingAmount;
    if(QPushButton::isDown()) {
        shiftingAmount = 1;
        painter.setBrush(active);
    }
    else {
        shiftingAmount = 0;
        painter.setBrush(inactive);
    }
    QPolygon pp;
    pp << QPoint(width() - buttonLightSize - 3 - shiftingAmount, 3 + shiftingAmount) << QPoint(width() - 3 - shiftingAmount, 3 + shiftingAmount) <<QPoint(width()-3 - shiftingAmount, buttonLightSize + 3 + shiftingAmount);
    painter.drawPolygon(pp, Qt::OddEvenFill);
    painter.restore();
}
