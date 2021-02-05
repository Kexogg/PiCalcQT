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
    void unaryOperatorClicked();
    void trigonometryOperatorClicked();
    void operatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSign();
    void backspace();
    void clear();
    void clearAll();
    void trigonomerySwitch();
private:
    QWidget *mainWidget;
    QWidget *fileMenu;
    QWidget *layoutMenu;
    QWidget *displaymodule;
    QWidget *mainmodule;
    QWidget *advmodule;
    QWidget *taxmodule;
    QWidget *mainwindow;
    Button *addButton(const QString &text, const char *member);
    void setupUI();
    void abort();
    void updateText(const QString &text);
    void switchToTax();
    void switchToAdv();
    void switchToDef();
    void about();
    bool calculate(double rightOperand, const QString &pendingOperator);
    bool noOperand;
    long double memory;
    long double result;
    float tax = 20;
    QString pendingAdditiveOperator;
    QString pendingOperator;
    QString measureUnit = "RAD";
    QLabel *display;
    QLabel *display_h;
    QLabel *status_1;
    QAction *tri;
    QStatusBar *bar = nullptr;
    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
    QGridLayout *windowlayout;
    QGridLayout *displaylayout;
    QGridLayout *mainlayout;
    QGridLayout *advlayout;
    QGridLayout *taxlayout;
};


#endif
