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
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();
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
    void abortOperation();
    void updateText(const QString &text);
    bool calculate(double rightOperand, const QString &pendingOperator);
    void switchToTax();
    void switchToAdv();
    void switchToDef();
    void about();
    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    float tax = 20;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;
    bool usingRadians = true;
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
