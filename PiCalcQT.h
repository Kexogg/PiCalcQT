#ifndef PiCalcQT_H
#define PiCalcQT_H
#include <QStatusBar>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QToolButton>
QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class Button;

class PiCalcQT : public QMainWindow
{
    Q_OBJECT

public:
    PiCalcQT(QWidget *parent = nullptr);

private slots:
    void digitClicked();
    void operatorClicked();
    void unaryOperatorClicked();
    void trigonometryOperatorClicked();
    void equalsClicked();
    void pointClicked();
    void changeSign();
    void backspace();
    void clear();
    void clearAll();

private:
    //UI
    Button *addButton(const QString &text, const char *member);
    enum
    {
        Numpad = 10
    };
    Button *digitButtons[Numpad];
    QGridLayout *windowlayout;
    QGridLayout *displaylayout;
    QGridLayout *mainlayout;
    QGridLayout *advlayout;
    QGridLayout *taxlayout;
    QWidget *mainwindow;
    QWidget *displaymodule;
    QWidget *mainmodule;
    QWidget *advmodule;
    QWidget *taxmodule;
    QMenu *fileMenu;
    QMenu *layoutMenu;
    QLabel *display;
    QLabel *display_h;
    QLabel *status;
    QAction *tri;
    QStatusBar *bar = nullptr;
    QString queuedOperator;
    QString measureUnit = "RAD";
    //Functions
    QString toQString(long double &number);
    long double getDisplayData(QLabel &displayname);
    bool calculate(long double operand, const QString &queuedOperator);
    void updateDisplayData(const QString &text);
    void setupUI();
    void abort();
    void trigonometrySwitch();
    void switchToTax();
    void switchToAdv();
    void switchToDef();
    void about();
    //Variables
    int displayLimit = 15;
    bool noOperand;
    long double memory;
    long double result;
    float tax = 20; //VAT 20%
};

#endif
