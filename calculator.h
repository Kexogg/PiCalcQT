#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QStatusBar>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class Button;

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);

private slots:
    void digitClicked();
    void unaryOperatorClicked();
    void trigonometryOperatorClicked();
    void operatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void trigonomerySwitch();
private:
    QWidget *mainWidget;
    QWidget *fileMenu;
    QWidget *layoutMenu;
    QWidget *mainmodule;
    QWidget *advmodule;
    QWidget *taxmodule;
    QWidget *mainwindow;
    QString currentLayout = "default";
    Button *createButton(const QString &text, const char *member);
    void setupButtons();
    void abort();
    void updateText(const QString &text);
    void switchToTax();
    void switchToAdv();
    void switchToDef();
    void about();
    bool calculate(double rightOperand, const QString &pendingOperator);
    bool statusBarBusy = false;
    double memory;
    //double sumSoFar;
    //double factorSoFar;
    double resultSoFar;
    float tax = 20;
    QString pendingAdditiveOperator;
    QString pendingOperator;
    QString measureUnit = "RAD";
    bool waitingForOperand;
    QLabel *display;
    QLabel *status_1;
    QLabel *status_2;
    QStatusBar *bar = nullptr;
    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
    QGridLayout *layout;
    QGridLayout *defaultlayout;
    QGridLayout *advlayout;
    QGridLayout *taxlayout;
};


#endif
