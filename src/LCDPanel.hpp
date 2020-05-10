#ifndef LCDPANEL_HPP
#define LCDPANEL_HPP

#include <QWidget>

namespace Ui {
class LCDPanel;
}

class LCDPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LCDPanel(QWidget *parent = nullptr);
    void updateTime(int seconds);
    ~LCDPanel();

private:
    Ui::LCDPanel *ui;
    QFont *SevenSegment;
};

#endif // LCDPANEL_HPP