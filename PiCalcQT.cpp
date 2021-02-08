#include "PiCalcQT.h"
#include "button.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>
#include <QMainWindow>
#include <QtWidgets>
#include <sstream>
#include <math.h>
//TODO: Translate to Russian, implement global historylock
PiCalcQT::PiCalcQT(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("PiCalcQT"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //DECLARE WIDGETS AND LAYOUTS
    windowlayout = new QGridLayout;
    displaylayout = new QGridLayout;
    mainlayout = new QGridLayout;
    advlayout = new QGridLayout;
    taxlayout = new QGridLayout;
    mainwindow = new QWidget;
    displaymodule = new QWidget;
    mainmodule = new QWidget;
    advmodule = new QWidget;
    taxmodule = new QWidget;
    //TOP MENU SETUP
    fileMenu = menuBar()->addMenu(tr("File"));
    layoutMenu = menuBar()->addMenu(tr("Layout"));
    QActionGroup *layoutList = new QActionGroup(this);
    QAction *abt = fileMenu->addAction(tr("&About"), this, &PiCalcQT::about);
    QAction *abtQT = fileMenu->addAction(tr("&About Qt"), qApp, &QApplication::aboutQt);
    QAction *quit = fileMenu->addAction(tr("&Quit"), qApp, &QApplication::quit);
    QAction *tax = layoutMenu->addAction(tr("&Taxes"), this, &PiCalcQT::switchToTax);
    QAction *adv = layoutMenu->addAction(tr("&Advanced"), this, &PiCalcQT::switchToAdv);
    QAction *def = layoutMenu->addAction(tr("&Default"), this, &PiCalcQT::switchToDef);
    tri = fileMenu->addAction(tr("&Swicth to DEG"), this, &PiCalcQT::trigonometrySwitch);
    tri->setVisible(false);
    layoutList->addAction(tax);
    layoutList->addAction(adv);
    layoutList->addAction(def);
    tax->setCheckable(true);
    adv->setCheckable(true);
    def->setCheckable(true);
    def->setChecked(true);
    quit->setStatusTip("Quit this application");
    tax->setStatusTip("Switch to taxes mode");
    adv->setStatusTip("Switch to advanced mode");
    def->setStatusTip("Switch to default mode");
    abt->setStatusTip("About this application");
    abtQT->setStatusTip("About QT");
    //STATUS BAR SETUP
    status = new QLabel("status", this);
    status->setStyleSheet("margin-right: 2px; border-top: none; border: none;");
    status->hide();
    statusBar()->addPermanentWidget(status);
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setStyleSheet("background-color: white; color: black; border-top: 1px solid lightgray; ");
    statusBar()->showMessage("Ready", 2000);
    //MAIN DISPLAY SETUP
    display = new QLabel("");
    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);
    display->setAlignment(Qt::AlignRight);
    displaymodule->setStyleSheet(
    ".QWidget {background-color: white; border-radius: 0.25em; border: 1px solid lightgray; margin: 5px 10px 0} "
    "QLabel {color: black; padding: 0 5px 5px;} "
    "QLabel#display_h {padding: 5px 5px; 0}");
    //HISTORY DISPLAY SETUP
    display_h = new QLabel("");
    display_h->setAlignment(Qt::AlignRight);
    setupUI();
}
void PiCalcQT::setupUI()
{
    //NUMBERS
    for (int i = 0; i < Numpad; ++i)
        digitButtons[i] = addButton(QString::number(i), SLOT(digitClicked()));
    //MEMORY
    Button *clearMemory = addButton("MC", SLOT(unaryOperatorClicked()));
    Button *readMemory = addButton("MR", SLOT(unaryOperatorClicked()));
    Button *setMemory = addButton("MS", SLOT(unaryOperatorClicked()));
    Button *addToMemory = addButton("M+", SLOT(unaryOperatorClicked()));
    //DEFAULT
    Button *backspace = addButton(tr("←"), SLOT(backspace()));
    Button *clear = addButton("C", SLOT(clear()));
    Button *clearAll = addButton("CL", SLOT(clearAll()));
    Button *divide = addButton("÷", SLOT(operatorClicked()));
    Button *multiply = addButton("×", SLOT(operatorClicked()));
    Button *minus = addButton("-", SLOT(operatorClicked()));
    Button *plus = addButton("+", SLOT(operatorClicked()));
    Button *squareRoot = addButton("√", SLOT(unaryOperatorClicked()));
    Button *equal = addButton(tr("="), SLOT(equalsClicked()));
    Button *point = addButton(tr("."), SLOT(pointClicked()));
    Button *changeSign = addButton(tr("±"), SLOT(changeSign()));
    //ADVANCED
    Button *nthRoot = addButton("y√x", SLOT(operatorClicked()));
    Button *power = addButton("↑", SLOT(operatorClicked()));
    Button *mod = addButton("Mod", SLOT(operatorClicked()));
    Button *multiplicativeinverse = addButton("1/x", SLOT(unaryOperatorClicked()));
    Button *factorial = addButton("n!", SLOT(unaryOperatorClicked()));
    Button *log = addButton("ln", SLOT(unaryOperatorClicked()));
    Button *inserte = addButton("e", SLOT(unaryOperatorClicked()));
    Button *sin = addButton("sin", SLOT(trigonometryOperatorClicked()));
    Button *cos = addButton("cos", SLOT(trigonometryOperatorClicked()));
    Button *tan = addButton("tan", SLOT(trigonometryOperatorClicked()));
    //TAXES
    Button *TAXset = addButton("RATE", SLOT(unaryOperatorClicked()));
    Button *TAXadd = addButton("TAX+", SLOT(unaryOperatorClicked()));
    Button *TAXremove = addButton("TAX-", SLOT(unaryOperatorClicked()));
    Button *TAXshow = addButton("TAX", SLOT(unaryOperatorClicked()));
    //LAYOUT SETUP
    windowlayout->setSpacing(0);
    mainwindow->setLayout(windowlayout);
    setCentralWidget(mainwindow);
    mainmodule->setLayout(mainlayout);
    displaymodule->setLayout(displaylayout);
    advmodule->setLayout(advlayout);
    taxmodule->setLayout(taxlayout);
    windowlayout->addWidget(displaymodule, 0, 0, 1, 5);
    windowlayout->addWidget(mainmodule, 1, 0, 5, 5);
    windowlayout->setContentsMargins(0, 0, 0, 0);
    //DISPLAY
    displaymodule->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    displaylayout->setSpacing(0);
    displaylayout->addWidget(display, 1, 0, 1, 5);
    displaylayout->addWidget(display_h, 0, 0, 1, 5);
    displaylayout->setContentsMargins(10, 10, 10, 0);
    //DEFAULT
    mainlayout->setContentsMargins(10, 10, 10, 10);
    mainlayout->addWidget(backspace, 0, 0);
    mainlayout->addWidget(clear, 0, 1);
    mainlayout->addWidget(clearAll, 0, 2);
    mainlayout->addWidget(squareRoot, 0, 3);
    mainlayout->addWidget(minus, 0, 4);
    mainlayout->addWidget(clearMemory, 1, 0);
    mainlayout->addWidget(plus, 1, 4);
    mainlayout->addWidget(readMemory, 2, 0);
    mainlayout->addWidget(multiply, 2, 4);
    mainlayout->addWidget(setMemory, 3, 0);
    mainlayout->addWidget(divide, 3, 4);
    mainlayout->addWidget(addToMemory, 4, 0);
    mainlayout->addWidget(digitButtons[0], 4, 1);
    mainlayout->addWidget(point, 4, 2);
    mainlayout->addWidget(changeSign, 4, 3);
    mainlayout->addWidget(equal, 4, 4);
    for (int i = 1; i < Numpad; ++i)
    {
        int row = ((9 - i) / 3) + 1;
        int column = ((i - 1) % 3) + 1;
        mainlayout->addWidget(digitButtons[i], row, column);
    }
    //ADVANCED
    advlayout->setContentsMargins(10, 6, 10, 10);
    advlayout->addWidget(factorial, 0, 0);
    advlayout->addWidget(log, 0, 1);
    advlayout->addWidget(power, 0, 2);
    advlayout->addWidget(multiplicativeinverse, 0, 3);
    advlayout->addWidget(mod, 0, 4);
    advlayout->addWidget(inserte, 1, 0);
    advlayout->addWidget(sin, 1, 1);
    advlayout->addWidget(cos, 1, 2);
    advlayout->addWidget(tan, 1, 3);
    advlayout->addWidget(nthRoot, 1, 4);
    //TAXES
    taxlayout->setContentsMargins(10, 6, 10, 10);
    taxlayout->addWidget(TAXset, 0, 0);
    taxlayout->addWidget(TAXadd, 0, 1);
    taxlayout->addWidget(TAXremove, 0, 2);
    taxlayout->addWidget(TAXshow, 0, 3);
}
Button *PiCalcQT::addButton(const QString &label, const char *member)
{
    Button *button = new Button(label);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
QString PiCalcQT::toQString(long double &number)
{
    return QString::number(number, 'g', displayLimit);
}
long double PiCalcQT::getDisplayData(QLabel &displayname)
{
    long double p1 = 0;
    std::stringstream p2(displayname.text().toStdString());
    p2>>p1;
    return p1;
}
void PiCalcQT::updateDisplayData(const QString &text)
{
    if (text == "nan") {
        display->setText(QString("Not a number").toUtf8().constData());
        return;
    }
    else if (text == "inf") {
        display->setText(QString("Infinity/Overflow").toUtf8().constData());
        return;
    }
    display->setText(text.toUtf8().constData());
}
void PiCalcQT::switchToTax()
{
    mainlayout->setContentsMargins(10, 10, 10, 0);
    advmodule->hide();
    tri->setVisible(false);
    windowlayout->removeWidget(advmodule);
    windowlayout->addWidget(taxmodule, 6, 0, 1, 5);
    taxmodule->show();
    status->show();
    status->setText("Tax rate: " + QString::number(tax) + "%");
    adjustSize();
}
void PiCalcQT::switchToAdv()
{
    mainlayout->setContentsMargins(10, 10, 10, 0);
    taxmodule->hide();
    windowlayout->removeWidget(taxmodule);
    windowlayout->addWidget(advmodule, 6, 0, 2, 5);
    advmodule->show();
    status->show();
    tri->setVisible(true);
    status->setText(measureUnit);
    adjustSize();
}
void PiCalcQT::switchToDef()
{
    mainlayout->setContentsMargins(10, 10, 10, 10);
    advmodule->hide();
    status->hide();
    taxmodule->hide();
    tri->setVisible(false);
    windowlayout->removeWidget(advmodule);
    windowlayout->removeWidget(taxmodule);
    mainwindow->adjustSize();
    mainmodule->adjustSize();
    adjustSize(); //AdjustSize() MUST be called 3 times here.
}
void PiCalcQT::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digit = clickedButton->text().toInt();
    if (noOperand)
    {
        display->clear();
        noOperand = false;
    }
    if ((display->text() == "0" and digit == 0) or (display->text().length() == displayLimit))
        return;
    updateDisplayData(display->text() + QString::number(digit));
}
void PiCalcQT::pointClicked()
{
    if (noOperand)
        updateDisplayData("0");
    if (!display->text().contains('.'))
        updateDisplayData(display->text() + tr("."));
    noOperand = false;
}
void PiCalcQT::changeSign()
{
    QString text = display->text();
    double value = text.toDouble();
    if (value > 0)
    {
        text.prepend("-");
    }
    else if (value < 0)
    {
        text.remove(0, 1);
    }
    updateDisplayData(text);
}
void PiCalcQT::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    long double operand = getDisplayData(*display);
    long double result = 0;
    bool statusbarlock = false;
    bool historylock = false;
    if (clickedOperator == "√")
    {
        if (operand < 0)
        {
            abort();
            return;
        }
        result = std::sqrt(operand);
    }
    else if (clickedOperator == "ln")
    {
        result = std::log(operand);
        historylock = true;
        display_h->setText(clickedOperator + "(" + toQString(operand) + ") = " + toQString(result));
    }
    else if (clickedOperator == "n!")
    {
        if (operand < 0 or operand == INFINITY)
        {
            abort();
            return;
        }
        else if (operand == 0)
        {
            result = 1;
        }
        else
        {
            result = 1;
            for (int i = 1; i <= operand; ++i)
            {
                if (result == INFINITY) //Break on overflow
                        break;
                result *= i;

            }
        }
        historylock = true;
        display_h->setText(toQString(operand) + "! = " + toQString(result));
    }
    else if (clickedOperator == "1/x")
    {
        if (operand == 0)
        {
            abort();
            return;
        }
        result = 1.0 / operand;
        historylock = true;
        display_h->setText("1/" + toQString(operand) + " = " + toQString(result));
    }
    else if (clickedOperator == "e")
    {
        result = M_E;
        historylock = true;
    }
    //TAXES
    else if (clickedOperator == "TAX")
    {
        result = operand - (operand / (tax * 0.01 + 1));
        historylock = true;
    }
    else if (clickedOperator == "TAX+")
    {
        result = operand * (tax * 0.01 + 1);
        historylock = true;
    }
    else if (clickedOperator == "TAX-")
    {
        result = operand / (tax * 0.01 + 1);
        historylock = true;
    }
    else if (clickedOperator == "RATE")
    {
        tax = operand;
        result = operand;
        statusBar()->showMessage("Tax rate set | Ready", 2000);
        statusbarlock = true;
        status->setText("Tax rate: " + QString::number(tax) + "%");
        historylock = true;
    }

    //MEMORY
    else if (clickedOperator == "MC")
    {
        result = operand;
        memory = 0;
        statusbarlock = true;
        statusBar()->showMessage("Memory cleared | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == "MR")
    {
        result = memory;
        statusbarlock = true;
        statusBar()->showMessage("Memory recalled | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == "MS")
    {
        memory = operand;
        result = operand;
        statusbarlock = true;
        statusBar()->showMessage("Memory set | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == "M+")
    {
        memory += operand;
        result = operand;
        statusbarlock = true;
        statusBar()->showMessage("Memory updated | Ready", 2000);
        historylock = true;
    }
    updateDisplayData(toQString(result));
    if (!historylock)
    {
        display_h->setText(clickedOperator + toQString(operand) + " = " + toQString(result));
    }
    if (!statusbarlock)
    {
        statusBar()->showMessage("Ready", 2000);
    }
    noOperand = true;
}
void PiCalcQT::operatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    if (!clickedButton)
        return;
    QString clickedOperator = clickedButton->text();
    long double operand = getDisplayData(*display);
    long double displaybuffer = result;
    bool historylock = false;
    if (!queuedOperator.isEmpty())
    {
        if (!calculate(operand, queuedOperator))
        {
            abort();
            return;
        }
        display_h->setText(toQString(displaybuffer) + " " + queuedOperator + " " + toQString(operand) + " = " + toQString(result));
        historylock = true;
        updateDisplayData(toQString(result));
    }
    else
    {
        result = operand;
    }
    queuedOperator = clickedOperator;
    if (!historylock)
    {
        display_h->setText(toQString(operand) + " " + queuedOperator);
    }
    noOperand = true;
}
void PiCalcQT::trigonometrySwitch()
{
    if (measureUnit == "RAD")
    {
        measureUnit = "DEG";
        status->setText(measureUnit);
        tri->setText("Switch to RAD");
    }
    else
    {
        measureUnit = "RAD";
        status->setText(measureUnit);
        tri->setText("Switch to DEG");
    }
}
void PiCalcQT::trigonometryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    long double operand = getDisplayData(*display);
    long double result = 0;
    if (measureUnit == "DEG")
    {
        operand = (operand * M_PI) / 180;
    }
    if (clickedOperator == "sin")
    {
        result = std::sin(operand);
    }
    if (clickedOperator == "cos")
    {
        result = std::cos(operand);
    }
    if (clickedOperator == "tan")
    {
        if (measureUnit == "DEG" and (operand * 180) / M_PI == 90)
        {
            abort();
            return;
        }
        result = std::tan(operand);
    }
    display_h->setText(clickedOperator + "(" + toQString(operand) + ") = " + toQString(result));
    updateDisplayData(toQString(result));
    noOperand = true;
}
void PiCalcQT::equalsClicked()
{
    long double operand = getDisplayData(*display);
    long double displaybuffer = result;
    if (!queuedOperator.isEmpty())
    {
        if (!calculate(operand, queuedOperator))
        {
            abort();
            return;
        }
        display_h->setText(toQString(displaybuffer) + " " + queuedOperator + " " + toQString(operand) + " = " + toQString(result));
        statusBar()->showMessage("Ready", 2000);
        operand = result;
        queuedOperator.clear();
    }
    else
    {
        result = operand;
    }

    updateDisplayData(toQString(result));
    result = 0;
    noOperand = true;
}
void PiCalcQT::backspace()
{
    if (noOperand)
        return;
    QString text = display->text();
    text.chop(1);
    if (text.isEmpty())
    {
        text = "0";
        noOperand = true;
    }
    updateDisplayData(text);
}
void PiCalcQT::clear()
{
    if (noOperand)
        return;
    updateDisplayData("0");
    statusBar()->showMessage("Ready", 2000);
    noOperand = true;
}
void PiCalcQT::clearAll()
{
    result = 0;
    display_h->clear();
    updateDisplayData("0");
    statusBar()->showMessage("Ready", 2000);
    noOperand = true;
    queuedOperator.clear();
}
bool PiCalcQT::calculate(long double operand, const QString &queuedOperator)
{
    if (queuedOperator == "+")
    {
        result += operand;
    }
    else if (queuedOperator == "×")
    {
        result *= operand;
    }
    else if (queuedOperator == "-")
    {
        result -= operand;
    }
    else if (queuedOperator == "÷")
    {
        if (operand == 0)
            return false;
        result /= operand;
    }
    else if (queuedOperator == "↑")
    {
        if (operand == 0)
            result = 1;
        else
        {
            result = std::pow(result, operand);
        }
    }
    else if (queuedOperator == "Mod")
    {
        result = fmod(result, operand);
    }
    else if (queuedOperator == "y√x")
    {
        result = pow(result, 1 / operand);
    }
    return true;
}
void PiCalcQT::abort()
{
    clearAll();
    updateDisplayData("Error");
}
void PiCalcQT::about()
{
    QMessageBox::about(this, tr("About PiCalcQT"), tr("<p><b>PiCalcQT</b> is a calculator for Raspberry PI written in C++ and QT.<br>Build DEV01080221 - ALPHA5.<br>© 2021 Alexander Mazhirin</p>"));
}
