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
    displaylayout = new QGridLayout;
    advlayout = new QGridLayout;
    advmodule = new QWidget;
    taxlayout = new QGridLayout;
    taxmodule = new QWidget;
    displaymodule = new QWidget;
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
    status_1->setStyleSheet("margin-right: 2px; border-top: none; border: none;");
    status_1->hide();
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

    display_h = new QLabel("");
    display_h->setAlignment(Qt::AlignRight);
    display_h->setStyleSheet("QLabel { background-color: white; color: black; padding: 5px; border-radius: 0.25em; border: 1px solid lightgray; }");

    setupUI();
}
void Calculator::setupUI() {
    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    Button *backspaceButton = createButton(tr("←"), SLOT(backspaceClicked()));
    Button *clearButton = createButton(tr("C"), SLOT(clear()));
    Button *clearAllButton = createButton(tr("CL"), SLOT(clearAll()));
    Button *trigonomeryButton = createButton(tr("R/D"), SLOT(trigonomerySwitch()));
    //MEMORY
    Button *clearMemoryButton = createButton(tr("MC"), SLOT(unaryOperatorClicked()));
    Button *readMemoryButton = createButton(tr("MR"), SLOT(unaryOperatorClicked()));
    Button *setMemoryButton = createButton(tr("MS"), SLOT(unaryOperatorClicked()));
    Button *addToMemoryButton = createButton(tr("M+"), SLOT(unaryOperatorClicked()));
    //DEFAULT
    Button *divisionButton = createButton(tr("÷"), SLOT(operatorClicked()));
    Button *timesButton = createButton(tr("×"), SLOT(operatorClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(operatorClicked()));
    Button *plusButton = createButton(tr("+"), SLOT(operatorClicked()));
    Button *squareRootButton = createButton(tr("√"), SLOT(unaryOperatorClicked()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));
    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
    Button *changeSignButton = createButton(tr("±"), SLOT(changeSignClicked()));
    //ADVANCED
    Button *nthRootButton = createButton(tr("y√x"), SLOT(operatorClicked()));
    Button *powerButton = createButton(tr("↑"), SLOT(operatorClicked()));
    Button *ModButton = createButton(tr("Mod"), SLOT(operatorClicked()));
    Button *reciprocalButton = createButton(tr("1/x"), SLOT(unaryOperatorClicked()));
    Button *factorialButton = createButton(tr("n!"), SLOT(unaryOperatorClicked()));
    Button *logButton = createButton(tr("ln"), SLOT(unaryOperatorClicked()));
    Button *sinButton = createButton(tr("sin"), SLOT(trigonometryOperatorClicked()));
    Button *cosButton = createButton(tr("cos"), SLOT(trigonometryOperatorClicked()));
    Button *tanButton = createButton(tr("tan"), SLOT(trigonometryOperatorClicked()));
    //TAXES
    Button *TAXsetButton = createButton(tr("SET"), SLOT(unaryOperatorClicked()));
    Button *TAXaddButton = createButton(tr("TAX+"), SLOT(unaryOperatorClicked()));
    Button *TAXremoveButton = createButton(tr("TAX-"), SLOT(unaryOperatorClicked()));
    Button *TAXshowButton = createButton(tr("TAX"), SLOT(unaryOperatorClicked()));
    //LAYOUT SETUP
    mainmodule->setLayout(defaultlayout);
    displaymodule->setLayout(displaylayout);
    advmodule->setLayout(advlayout);
    taxmodule->setLayout(taxlayout);
    layout->addWidget(displaymodule, 0, 0, 1, 5);
    layout->addWidget(mainmodule, 1, 0, 5, 5);
    //DISPLAY
    displaylayout->addWidget(display, 1, 0, 1, 5);
    displaylayout->addWidget(display_h, 0, 0, 1, 5);
    //DEFAULT
    defaultlayout->addWidget(backspaceButton, 0, 0);
    defaultlayout->addWidget(clearButton, 0, 1);
    defaultlayout->addWidget(clearAllButton, 0, 2);
    defaultlayout->addWidget(squareRootButton, 0, 3);
    defaultlayout->addWidget(clearMemoryButton, 1, 0);
    defaultlayout->addWidget(readMemoryButton, 2, 0);
    defaultlayout->addWidget(setMemoryButton, 3, 0);
    defaultlayout->addWidget(addToMemoryButton, 4, 0);
    defaultlayout->addWidget(divisionButton, 3, 4);
    defaultlayout->addWidget(timesButton, 2, 4);
    defaultlayout->addWidget(minusButton, 0, 4);
    defaultlayout->addWidget(plusButton, 1, 4);
    defaultlayout->addWidget(digitButtons[0], 4, 1);
    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 1;
        int column = ((i - 1) % 3) + 1;
        defaultlayout->addWidget(digitButtons[i], row, column);
    }
    defaultlayout->addWidget(pointButton, 4, 2);
    defaultlayout->addWidget(changeSignButton, 4, 3);
    defaultlayout->addWidget(equalButton, 4, 4);
    //ADVANCED
    advlayout->addWidget(factorialButton, 0, 0);
    advlayout->addWidget(logButton, 0, 1);
    advlayout->addWidget(powerButton, 0, 2);
    advlayout->addWidget(reciprocalButton, 0, 3);
    advlayout->addWidget(ModButton, 0, 4);
    advlayout->addWidget(nthRootButton, 1, 4);
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
    status_1->show();
    status_1->setText("Tax rate: " + QString::number(tax) + "%");
    adjustSize();
}
void Calculator::switchToAdv() {
    taxmodule->hide();
    layout->removeWidget(taxmodule);
    layout->addWidget(advmodule, 6, 0, 2, 5);
    advmodule->show();
    status_1->show();
    status_1->setText(measureUnit);
    adjustSize();
}
void Calculator::switchToDef() {
    advmodule->hide();
    status_1->hide();
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
    bool statusbarlock = false;
    bool historylock = false;
    if (clickedOperator == tr("√")) {
        if (operand < 0.0) {
            abort();
            return;
        }
        result = std::sqrt(operand);
    }
    else if (clickedOperator == tr("ln")) {
        result = std::log(operand);
        historylock = true;
        display_h->setText(clickedOperator + "(" + QString::number(operand) + ") = " + QString::number(result));
    }
    else if (clickedOperator == tr("n!")) {
            if (operand < 0) {
                abort();
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
            historylock = true;
            display_h->setText(QString::number(operand) + "! = " + QString::number(result));
    }
    else if (clickedOperator == tr("1/x")) {
        if (operand == 0.0) {
            abort();
            return;
        }
        result = 1.0 / operand;
        historylock = true;
        display_h->setText("1/" + QString::number(operand) + " = " + QString::number(result));
    }

    //TAXES
    else if (clickedOperator == tr("TAX")) {
        result = operand - (operand / (tax * 0.01 + 1));
        historylock = true;
    }
    else if (clickedOperator == tr("TAX+")) {
        result = operand * (tax * 0.01 + 1);
        historylock = true;
    }
    else if (clickedOperator == tr("TAX-")) {
        result = operand / (tax * 0.01 + 1);
        historylock = true;
    }
    else if (clickedOperator == tr("SET")) {
        tax = operand;
        result = operand;
        statusBar()->showMessage("Tax rate set | Ready", 2000);
        statusbarlock = true;
        status_1->setText("Tax rate: " + QString::number(tax) + "%");
        historylock = true;
    }

    //MEMORY
    else if (clickedOperator == tr("MC")) {
        result = operand;
        memory = 0;
        statusbarlock = true;
        statusBar()->showMessage("Memory cleared | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == tr("MR")) {
        result = memory;
        statusbarlock = true;
        statusBar()->showMessage("Memory recalled | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == tr("MS")) {
        memory = operand;
        result = operand;
        statusbarlock = true;
        statusBar()->showMessage("Memory set | Ready", 2000);
        historylock = true;
    }
    else if (clickedOperator == tr("M+")) {
        memory += operand;
        result = operand;
        statusbarlock = true;
        statusBar()->showMessage("Memory updated | Ready", 2000);
        historylock = true;
    }
    updateText(QString::number(result));
    if (!historylock) {
    display_h->setText(clickedOperator + QString::number(operand) + " = " + QString::number(result));
    }
    if (!statusbarlock) {
        statusBar()->showMessage("Ready", 2000);
    }
    waitingForOperand = true;
}

void Calculator::trigonometryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;
    if (measureUnit == "DEG") {
        operand = (operand * M_PI) / 180;
    }
    if (clickedOperator == tr("sin")) {
        result = std::sin(operand);
    }
    if (clickedOperator == tr("cos")) {
        result = std::cos(operand);
    }
    if (clickedOperator == tr("tan")) {
        if (measureUnit == "DEG" and (operand * 180) / M_PI == 90) {
            abort();
            return;
        }
        result = std::tan(operand);
    }
    updateText(QString::number(result));
    waitingForOperand = true;
}

void Calculator::trigonomerySwitch() {
    if (measureUnit == "RAD") {
        measureUnit = "DEG";
        status_1->setText(measureUnit);
    }
    else {
        measureUnit = "RAD";
        status_1->setText(measureUnit);
    }

}

void Calculator::operatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    if (!clickedButton)
      return;
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double displaybuffer = resultSoFar;
    bool historylock = false;
    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abort();
            return;
        }
        display_h->setText(QString::number(displaybuffer) + " " + pendingOperator + " " + QString::number(operand) + " = " + QString::number(resultSoFar));
        historylock = true;
        updateText(QString::number(resultSoFar));
    } else {
        resultSoFar = operand;
    }
    pendingOperator = clickedOperator;
    if (!historylock) {
        display_h->setText(QString::number(operand) + " " + pendingOperator);
    }
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    double operand = display->text().toDouble();
    double displaybuffer = resultSoFar;
    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abort();
            return;
        }
        display_h->setText(QString::number(displaybuffer) + " " + pendingOperator + " " + QString::number(operand) + " = " + QString::number(resultSoFar));
        statusBar()->showMessage("Ready", 2000);
        operand = resultSoFar;
        pendingOperator.clear();
    }
    else {
        resultSoFar = operand;
    }

    updateText(QString::number(resultSoFar));
    resultSoFar = 0.0;
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
    resultSoFar = 0.0;
    pendingOperator.clear();
    display_h->clear();
    updateText("0");
    statusBar()->showMessage("Ready", 2000);
    waitingForOperand = true;
}
Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
void Calculator::abort()
{
    clearAll();
    updateText("Error");
}
void Calculator::updateText(const QString &text) {
    display->setText(tr(text.toUtf8().constData()));
    display->setAlignment(Qt::AlignRight);
}
bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == tr("+")) {
        resultSoFar += rightOperand;
    }
    else if (pendingOperator == tr("-")) {
        resultSoFar -= rightOperand;
    }
    else if (pendingOperator == tr("×")) {
        resultSoFar *= rightOperand;
    }
    else if (pendingOperator == tr("÷")) {
        if (rightOperand == 0.0)
            return false;
        resultSoFar /= rightOperand;
    }
    else if (pendingOperator == tr("↑")) {
        if (rightOperand == 0.0)
            resultSoFar = 1;
        else {
            resultSoFar = std::pow(resultSoFar, rightOperand);
        }
    }
    else if (pendingOperator == tr("Mod")) {
        resultSoFar = fmod(resultSoFar,rightOperand);
    }
    else if (pendingOperator == tr("y√x")) {
        resultSoFar = pow(resultSoFar,1/rightOperand);
    }
    return true;
}
void Calculator::about()
{
   QMessageBox::about(this, tr("About PiCalcQT"),
            tr("<b>PiCalcQT</b> is a simple calculator for Raspberry PI written in C++ and QT.<br>Build DEV01050221 - ALPHA2.<br>© 2021 Alexander Mazhirin"));
}
