#include "calculator.h"
#include "button.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>
#include <QMainWindow>
#include <QtWidgets>


Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("PiCalcQT"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    mainmodule = new QWidget;
    defaultlayout = new QGridLayout;
    advlayout = new QGridLayout;
    advmodule = new QWidget;
    taxlayout = new QGridLayout;
    taxmodule = new QWidget;
    mainwindow = new QWidget;
    mainwindow->setLayout(layout);
    setCentralWidget(mainwindow);
    fileMenu = menuBar()->addMenu(tr("File"));
    layoutMenu = menuBar()->addMenu(tr("Layout"));
    auto *quit = new QAction("&Quit", this);
    auto *tax = new QAction("&Tax", this);
    auto *adv = new QAction("&Advanced", this);
    auto *def = new QAction("&Default", this);
    auto *abt = new QAction("&About", this);
    fileMenu->addAction(quit);
    fileMenu->addAction(abt);
    layoutMenu->addAction(tax);
    layoutMenu->addAction(adv);
    layoutMenu->addAction(def);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(tax, &QAction::triggered, this, &Calculator::switchToTax);
    connect(adv, &QAction::triggered, this, &Calculator::switchToAdv);
    connect(def, &QAction::triggered, this, &Calculator::switchToDef);
    connect(abt, &QAction::triggered, this, &Calculator::about);
    status_1 = new QLabel("Status_1", this);
    status_1->setText("RAD");
    status_1->setStyleSheet("margin-right: 2px; border-top: none; ");
    statusBar()->addPermanentWidget(status_1);
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setStyleSheet("background-color: white; color: black; border-top: 1px solid lightgray; ");
    statusBar()->showMessage("Ready", 2000);

    display = new QLabel("");
    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setAlignment(Qt::AlignRight);
    display->setFont(font);
    display->setStyleSheet("QLabel { background-color: white; color: black; padding: 5px; border-radius: 0.25em; border: 1px solid lightgray; }");

    setupButtons();
}
void Calculator::setupButtons() {
    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
    Button *changeSignButton = createButton(tr("\302\261"), SLOT(changeSignClicked()));
    Button *backspaceButton = createButton(tr("←"), SLOT(backspaceClicked()));
    Button *clearButton = createButton(tr("C"), SLOT(clear()));
    Button *clearAllButton = createButton(tr("CL"), SLOT(clearAll()));
    Button *trigonomeryButton = createButton(tr("R/D"), SLOT(trigonomerySwitch()));
    Button *clearMemoryButton = createButton(tr("MC"), SLOT(clearMemory()));
    Button *readMemoryButton = createButton(tr("MR"), SLOT(readMemory()));
    Button *setMemoryButton = createButton(tr("MS"), SLOT(setMemory()));
    Button *addToMemoryButton = createButton(tr("M+"), SLOT(addToMemory()));
    Button *divisionButton = createButton(tr("\303\267"), SLOT(multiplicativeOperatorClicked()));
    Button *timesButton = createButton(tr("\303\227"), SLOT(multiplicativeOperatorClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    Button *squareRootButton = createButton(tr("√"), SLOT(unaryOperatorClicked()));
    Button *powerButton = createButton(tr("x^y"), SLOT(multiplicativeOperatorClicked()));
    Button *reciprocalButton = createButton(tr("1/x"), SLOT(unaryOperatorClicked()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));
    Button *factorialButton = createButton(tr("n!"), SLOT(unaryOperatorClicked()));
    Button *logButton = createButton(tr("ln"), SLOT(unaryOperatorClicked()));
    Button *sinButton = createButton(tr("sin"), SLOT(trigonometryOperatorClicked()));
    Button *cosButton = createButton(tr("cos"), SLOT(trigonometryOperatorClicked()));
    Button *tanButton = createButton(tr("tan"), SLOT(trigonometryOperatorClicked()));
    Button *TAXsetButton = createButton(tr("SET"), SLOT(unaryOperatorClicked()));
    Button *TAXaddButton = createButton(tr("TAX+"), SLOT(unaryOperatorClicked()));
    Button *TAXremoveButton = createButton(tr("TAX-"), SLOT(unaryOperatorClicked()));
    Button *TAXshowButton = createButton(tr("TAX"), SLOT(unaryOperatorClicked()));
    //LAYOUT SETUP
    mainmodule->setLayout(defaultlayout);
    layout->addWidget(mainmodule, 0, 0, 5, 5);
    advmodule->setLayout(advlayout);
    taxmodule->setLayout(taxlayout);
    //DEFAULT
    defaultlayout->addWidget(display, 0, 0, 1, 5);
    defaultlayout->addWidget(backspaceButton, 1, 0);
    defaultlayout->addWidget(clearButton, 1, 1);
    defaultlayout->addWidget(clearAllButton, 1, 2);
    defaultlayout->addWidget(squareRootButton, 1, 3);
    defaultlayout->addWidget(clearMemoryButton, 2, 0);
    defaultlayout->addWidget(readMemoryButton, 3, 0);
    defaultlayout->addWidget(setMemoryButton, 4, 0);
    defaultlayout->addWidget(addToMemoryButton, 5, 0);
    defaultlayout->addWidget(divisionButton, 4, 4);
    defaultlayout->addWidget(timesButton, 3, 4);
    defaultlayout->addWidget(minusButton, 1, 4);
    defaultlayout->addWidget(plusButton, 2, 4);
    defaultlayout->addWidget(digitButtons[0], 5, 1);
    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        defaultlayout->addWidget(digitButtons[i], row, column);
    }
    defaultlayout->addWidget(pointButton, 5, 2);
    defaultlayout->addWidget(changeSignButton, 5, 3);
    defaultlayout->addWidget(equalButton, 5, 4);
    //ADVANCED
    advlayout->addWidget(factorialButton, 0, 0);
    advlayout->addWidget(logButton, 0, 1);
    advlayout->addWidget(powerButton, 0, 2);
    advlayout->addWidget(reciprocalButton, 0, 3);
    advlayout->addWidget(trigonomeryButton, 1, 0);
    advlayout->addWidget(sinButton, 1, 1);
    advlayout->addWidget(cosButton, 1, 2);
    advlayout->addWidget(tanButton, 1, 3);
    //TAXES
    taxlayout->addWidget(TAXsetButton, 0, 0);
    taxlayout->addWidget(TAXaddButton, 0, 1);
    taxlayout->addWidget(TAXremoveButton, 0, 2);
    taxlayout->addWidget(TAXshowButton, 0, 3);
}
void Calculator::switchToTax() {
    advmodule->hide();
    layout->removeWidget(advmodule);
    layout->addWidget(taxmodule, 6, 0, 1, 5);
    taxmodule->show();
    adjustSize();
}
void Calculator::switchToAdv() {
    taxmodule->hide();
    layout->removeWidget(taxmodule);
    layout->addWidget(advmodule, 6, 0, 2, 5);
    advmodule->show();
    adjustSize();
}
void Calculator::switchToDef() {
    advmodule->hide();
    taxmodule->hide();
    layout->removeWidget(advmodule);
    layout->removeWidget(taxmodule);
    mainwindow->adjustSize();
    mainmodule->adjustSize();
    adjustSize();
}

void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    updateText(display->text() + QString::number(digitValue));
    display->setAlignment(Qt::AlignRight);
}

void Calculator::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;

    if (clickedOperator == tr("√")) {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        result = std::sqrt(operand);
    }
    if (clickedOperator == tr("ln")) {
        result = std::log(operand);
    }
    if (clickedOperator == tr("TAX")) {
        result = operand - (operand / (tax * 0.01 + 1));
    }
    if (clickedOperator == tr("TAX+")) {
        result = operand * (tax * 0.01 + 1);
    }
    if (clickedOperator == tr("TAX-")) {
        result = operand / (tax * 0.01 + 1);
    }
    if (clickedOperator == tr("SET")) {
        tax = operand;
        result = operand;
        statusBar()->showMessage("TAX rate set! | Ready", 2000);
    }
    if (clickedOperator == tr("n!")) { //Factorial
            if (operand < 0) {
                abortOperation();
                return;
            } else if (operand == 0) {
                result = 1;
            } else {
                result = 1;
                for(int i = 1; i <= operand; ++i)
                    {
                        result *= i;
                    }
            }
    } else if (clickedOperator == tr("x^y")) {
        result = std::pow(operand, 2.0);
    } else if (clickedOperator == tr("1/x")) {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }
    updateText(QString::number(result));
    statusBar()->showMessage("Ready", 2000);
    waitingForOperand = true;
}

void Calculator::trigonometryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;
    if (usingRadians == false) {
        operand = (operand * M_PI) / 180;
    }
    if (clickedOperator == tr("sin")) {
        result = std::sin(operand);
    }
    if (clickedOperator == tr("cos")) {
        result = std::cos(operand);
    }
    if (clickedOperator == tr("tan")) {
        if (usingRadians == false and (operand * 180) / M_PI == 90) {
            abortOperation();
            return;
        }
        result = std::tan(operand);
    }
    updateText(QString::number(result));
    waitingForOperand = true;
}

void Calculator::trigonomerySwitch() {
    if (usingRadians) {
        usingRadians = false;
        status_1->setText("DEG");
    }
    else {
        usingRadians = true;
        status_1->setText("RAD");
    }

}
void Calculator::additiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    if (!clickedButton)
      return;
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        updateText(QString::number(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        updateText(QString::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }
    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::multiplicativeOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    if (!clickedButton)
      return;
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        updateText(QString::number(factorSoFar));
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    updateText(QString::number(sumSoFar));
    statusBar()->showMessage("Ready", 2000);
    sumSoFar = 0.0;
    waitingForOperand = true;
}
void Calculator::pointClicked()
{
    if (waitingForOperand)
        updateText("0");
    if (!display->text().contains('.'))
        updateText(display->text() + tr("."));
    waitingForOperand = false;
}
void Calculator::changeSignClicked()
{
    QString text = display->text();
    double value = text.toDouble();

    if (value > 0.0) {
        text.prepend(tr("-"));
    } else if (value < 0.0) {
        text.remove(0, 1);
    }
    updateText(text);
}

void Calculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    updateText(text);
}
void Calculator::clear()
{
    if (waitingForOperand)
        return;

    updateText("0");
    statusBar()->showMessage("Ready", 2000);
    waitingForOperand = true;
}
void Calculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    updateText("0");
    statusBar()->showMessage("Ready", 2000);
    waitingForOperand = true;
}
void Calculator::clearMemory()
{
    sumInMemory = 0.0;
}
void Calculator::readMemory()
{
    updateText(QString::number(sumInMemory));
    waitingForOperand = true;
}
void Calculator::setMemory()
{
    equalClicked();
    sumInMemory = display->text().toDouble();
}
void Calculator::addToMemory()
{
    equalClicked();
    sumInMemory += display->text().toDouble();
}
Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
void Calculator::abortOperation()
{
    clearAll();
    updateText("Error");
    statusBar()->showMessage("Ready", 2000);
}
void Calculator::updateText(const QString &text) {
    display->setText(tr(text.toUtf8().constData()));
    display->setAlignment(Qt::AlignRight);
}
bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == tr("+")) {
        sumSoFar += rightOperand;
    } else if (pendingOperator == tr("-")) {
        sumSoFar -= rightOperand;
    } else if (pendingOperator == tr("\303\227")) {
        factorSoFar *= rightOperand;
    } else if (pendingOperator == tr("\303\267")) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar /= rightOperand;
    } else if (pendingOperator == tr("x^y")) {
        if (rightOperand == 0.0)
            factorSoFar = 1;
        else {
            factorSoFar = std::pow(factorSoFar, rightOperand);
        }
    }
    return true;
}
void Calculator::about()
{
   QMessageBox::about(this, tr("About PiCalcQT"),
            tr("<b>PiCalcQT</b> is a simple calculator for Raspberry PI writtern in C++ and QT. © 2021 Alexander Mazhirin"));
}
