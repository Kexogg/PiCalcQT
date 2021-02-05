#include "PiCalcQT.h"
#include "button.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>
#include <QMainWindow>
#include <QtWidgets>


PiCalcQT::PiCalcQT(QWidget *parent)
    : QMainWindow(parent)
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
    windowlayout->setSpacing(0);
    mainwindow->setLayout(windowlayout);
    setCentralWidget(mainwindow);
    //TOP MENU SETUP
    fileMenu = menuBar()->addMenu(tr("File"));
    layoutMenu = menuBar()->addMenu(tr("Layout"));
    auto *quit = new QAction("&Quit", this);
    auto *tax = new QAction("&Tax", this);
    auto *adv = new QAction("&Advanced", this);
    auto *def = new QAction("&Default", this);
    auto *abt = new QAction("&About", this);
    tri = new QAction("&Swicth to DEG", this);
    fileMenu->addAction(quit);
    fileMenu->addAction(abt);
    fileMenu->addAction(tri);
    tri->setVisible(false);
    layoutMenu->addAction(tax);
    layoutMenu->addAction(adv);
    layoutMenu->addAction(def);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(abt, &QAction::triggered, this, &PiCalcQT::about);
    connect(tax, &QAction::triggered, this, &PiCalcQT::switchToTax);
    connect(adv, &QAction::triggered, this, &PiCalcQT::switchToAdv);
    connect(def, &QAction::triggered, this, &PiCalcQT::switchToDef);
    connect(tri, &QAction::triggered, this, &PiCalcQT::trigonomerySwitch);
    //STATUS BAR SETUP
    status_1 = new QLabel("Status_1", this);
    status_1->setStyleSheet("margin-right: 2px; border-top: none; border: none;");
    status_1->hide();
    statusBar()->addPermanentWidget(status_1);
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setStyleSheet("background-color: white; color: black; border-top: 1px solid lightgray; ");
    statusBar()->showMessage("Ready", 2000);
    //MAIN DISPLAY
    display = new QLabel("");
    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setAlignment(Qt::AlignRight);
    display->setFont(font);
    display->setStyleSheet("QLabel { background-color: white; color: black; padding: 5px; border-radius: 0.25em; border: 1px solid lightgray; }");
    //HISTORY DISPLAY
    display_h = new QLabel("");
    display_h->setAlignment(Qt::AlignRight);
    display_h->setStyleSheet("QLabel { background-color: white; color: black; padding: 5px; border-radius: 0.25em; border: 1px solid lightgray; }");

    setupUI();
}
void PiCalcQT::setupUI() {
    //NUMBERS
    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = addButton(QString::number(i), SLOT(digitClicked()));
    //MEMORY
    Button *clearMemoryButton = addButton(tr("MC"), SLOT(unaryOperatorClicked()));
    Button *readMemoryButton = addButton(tr("MR"), SLOT(unaryOperatorClicked()));
    Button *setMemoryButton = addButton(tr("MS"), SLOT(unaryOperatorClicked()));
    Button *addToMemoryButton = addButton(tr("M+"), SLOT(unaryOperatorClicked()));
    //DEFAULT
    Button *backspaceButton = addButton(tr("←"), SLOT(backspace()));
    Button *clearButton = addButton(tr("C"), SLOT(clear()));
    Button *clearAllButton = addButton(tr("CL"), SLOT(clearAll()));
    Button *divisionButton = addButton(tr("÷"), SLOT(operatorClicked()));
    Button *timesButton = addButton(tr("×"), SLOT(operatorClicked()));
    Button *minusButton = addButton(tr("-"), SLOT(operatorClicked()));
    Button *plusButton = addButton(tr("+"), SLOT(operatorClicked()));
    Button *squareRootButton = addButton(tr("√"), SLOT(unaryOperatorClicked()));
    Button *equalButton = addButton(tr("="), SLOT(equalClicked()));
    Button *pointButton = addButton(tr("."), SLOT(pointClicked()));
    Button *changeSignButton = addButton(tr("±"), SLOT(changeSign()));
    //ADVANCED
    Button *nthRootButton = addButton(tr("y√x"), SLOT(operatorClicked()));
    Button *powerButton = addButton(tr("↑"), SLOT(operatorClicked()));
    Button *ModButton = addButton(tr("Mod"), SLOT(operatorClicked()));
    Button *reciprocalButton = addButton(tr("1/x"), SLOT(unaryOperatorClicked()));
    Button *factorialButton = addButton(tr("n!"), SLOT(unaryOperatorClicked()));
    Button *logButton = addButton(tr("ln"), SLOT(unaryOperatorClicked()));
    Button *trigonomeryButton = addButton(tr("e"), SLOT(unaryOperatorClicked()));
    Button *sinButton = addButton(tr("sin"), SLOT(trigonometryOperatorClicked()));
    Button *cosButton = addButton(tr("cos"), SLOT(trigonometryOperatorClicked()));
    Button *tanButton = addButton(tr("tan"), SLOT(trigonometryOperatorClicked()));
    //TAXES
    Button *TAXsetButton = addButton(tr("SET"), SLOT(unaryOperatorClicked()));
    Button *TAXaddButton = addButton(tr("TAX+"), SLOT(unaryOperatorClicked()));
    Button *TAXremoveButton = addButton(tr("TAX-"), SLOT(unaryOperatorClicked()));
    Button *TAXshowButton = addButton(tr("TAX"), SLOT(unaryOperatorClicked()));
    //LAYOUT SETUP
    mainmodule->setLayout(mainlayout);
    displaymodule->setLayout(displaylayout);
    advmodule->setLayout(advlayout);
    taxmodule->setLayout(taxlayout);
    windowlayout->addWidget(displaymodule, 0, 0, 1, 5);
    windowlayout->addWidget(mainmodule, 1, 0, 5, 5);
    windowlayout->setContentsMargins(0, 0, 0, 0);
    //DISPLAY
    displaylayout->addWidget(display, 1, 0, 1, 5);
    displaylayout->addWidget(display_h, 0, 0, 1, 5);
    displaylayout->setContentsMargins(10, 10, 10, 0);
    //DEFAULT
    mainlayout->setContentsMargins(10, 10, 10, 10);
    mainlayout->addWidget(backspaceButton, 0, 0);
    mainlayout->addWidget(clearButton, 0, 1);
    mainlayout->addWidget(clearAllButton, 0, 2);
    mainlayout->addWidget(squareRootButton, 0, 3);
    mainlayout->addWidget(clearMemoryButton, 1, 0);
    mainlayout->addWidget(readMemoryButton, 2, 0);
    mainlayout->addWidget(setMemoryButton, 3, 0);
    mainlayout->addWidget(addToMemoryButton, 4, 0);
    mainlayout->addWidget(divisionButton, 3, 4);
    mainlayout->addWidget(timesButton, 2, 4);
    mainlayout->addWidget(minusButton, 0, 4);
    mainlayout->addWidget(plusButton, 1, 4);
    mainlayout->addWidget(digitButtons[0], 4, 1);
    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 1;
        int column = ((i - 1) % 3) + 1;
        mainlayout->addWidget(digitButtons[i], row, column);
    }
    mainlayout->addWidget(pointButton, 4, 2);
    mainlayout->addWidget(changeSignButton, 4, 3);
    mainlayout->addWidget(equalButton, 4, 4);
    //ADVANCED
    advlayout->setContentsMargins(10, 6, 10, 10);
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
    taxlayout->setContentsMargins(10, 6, 10, 10);
    taxlayout->addWidget(TAXsetButton, 0, 0);
    taxlayout->addWidget(TAXaddButton, 0, 1);
    taxlayout->addWidget(TAXremoveButton, 0, 2);
    taxlayout->addWidget(TAXshowButton, 0, 3);
}
Button *PiCalcQT::addButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
void PiCalcQT::switchToTax() {
    mainlayout->setContentsMargins(10, 10, 10, 0);
    advmodule->hide();
    tri->setVisible(false);
    windowlayout->removeWidget(advmodule);
    windowlayout->addWidget(taxmodule, 6, 0, 1, 5);
    taxmodule->show();
    status_1->show();
    status_1->setText("Tax rate: " + QString::number(tax) + "%");
    adjustSize();
}
void PiCalcQT::switchToAdv() {
    mainlayout->setContentsMargins(10, 10, 10, 0);
    taxmodule->hide();
    windowlayout->removeWidget(taxmodule);
    windowlayout->addWidget(advmodule, 6, 0, 2, 5);
    advmodule->show();
    status_1->show();
    tri->setVisible(true);
    status_1->setText(measureUnit);
    adjustSize();
}
void PiCalcQT::switchToDef() {
    mainlayout->setContentsMargins(10, 10, 10, 10);
    advmodule->hide();
    status_1->hide();
    taxmodule->hide();
    tri->setVisible(false);
    windowlayout->removeWidget(advmodule);
    windowlayout->removeWidget(taxmodule);
    mainwindow->adjustSize();
    mainmodule->adjustSize();
    adjustSize();
}

void PiCalcQT::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digit = clickedButton->text().toInt();
    if (display->text() == "0" && digit == 0)
        return;

    if (noOperand) {
        display->clear();
        noOperand = false;
    }
    updateText(display->text() + QString::number(digit));
    display->setAlignment(Qt::AlignRight);
}

void PiCalcQT::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0;
    bool statusbarlock = false;
    bool historylock = false;
    if (clickedOperator == tr("√")) {
        if (operand < 0) {
            abort();
            return;
        }
        result = std::sqrt(operand);
    }
    else if (clickedOperator == tr("ln")) {
        result = std::log(operand);
        historylock = true;
        display_h->setText(clickedOperator + "(" + QString::number(operand, 'g' ,10) + ") = " + QString::number(result, 'g' ,10));
    }
    else if (clickedOperator == tr("n!")) {
            if (operand < 0 or operand == INFINITY) {
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
            display_h->setText(QString::number(operand, 'g' ,10) + "! = " + QString::number(result, 'g' ,10));
    }
    else if (clickedOperator == tr("1/x")) {
        if (operand == 0) {
            abort();
            return;
        }
        result = 1.0 / operand;
        historylock = true;
        display_h->setText("1/" + QString::number(operand, 'g' ,10) + " = " + QString::number(result, 'g' ,10));
    }
    else if (clickedOperator == tr("e")) {
        result = M_E;
        historylock = true;
    }
    //TAXES
    else if (clickedOperator == tr("TAX")) {
        result = operand - (operand / (tax * 01 + 1));
        historylock = true;
    }
    else if (clickedOperator == tr("TAX+")) {
        result = operand * (tax * 01 + 1);
        historylock = true;
    }
    else if (clickedOperator == tr("TAX-")) {
        result = operand / (tax * 01 + 1);
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
    updateText(QString::number(result, 'g' ,10));
    if (!historylock) {
    display_h->setText(clickedOperator + QString::number(operand, 'g' ,10) + " = " + QString::number(result, 'g' ,10));
    }
    if (!statusbarlock) {
        statusBar()->showMessage("Ready", 2000);
    }
    noOperand = true;
}

void PiCalcQT::trigonometryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    long double operand = display->text().toDouble();
    long double result = 0;
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
    display_h->setText(clickedOperator + "(" + QString::number(operand, 'g' ,10) + ") = " + QString::number(result, 'g' ,10));
    updateText(QString::number(result, 'g' ,10));
    noOperand = true;
}

void PiCalcQT::trigonomerySwitch() {
    if (measureUnit == "RAD") {
        measureUnit = "DEG";
        status_1->setText(measureUnit);
        tri->setText("Switch to RAD");
    }
    else {
        measureUnit = "RAD";
        status_1->setText(measureUnit);
        tri->setText("Switch to DEG");
    }

}

void PiCalcQT::operatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    if (!clickedButton)
      return;
    QString clickedOperator = clickedButton->text();
    long double operand = display->text().toDouble();
    long double displaybuffer = result;
    bool historylock = false;
    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abort();
            return;
        }
        display_h->setText(QString::number(displaybuffer, 'g' ,10) + " " + pendingOperator + " " + QString::number(operand, 'g' ,10) + " = " + QString::number(result, 'g' ,10));
        historylock = true;
        updateText(QString::number(result, 'g' ,10));
    } else {
        result = operand;
    }
    pendingOperator = clickedOperator;
    if (!historylock) {
        display_h->setText(QString::number(operand, 'g' ,10) + " " + pendingOperator);
    }
    noOperand = true;
}

void PiCalcQT::equalClicked()
{
    double operand = display->text().toDouble();
    double displaybuffer = result;
    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abort();
            return;
        }
        display_h->setText(QString::number(displaybuffer, 'g' ,10) + " " + pendingOperator + " " + QString::number(operand, 'g' ,10) + " = " + QString::number(result, 'g' ,10));
        statusBar()->showMessage("Ready", 2000);
        operand = result;
        pendingOperator.clear();
    }
    else {
        result = operand;
    }

    updateText(QString::number(result, 'g' ,10));
    result = 0;
    noOperand = true;
}
void PiCalcQT::pointClicked()
{
    if (noOperand)
        updateText("0");
    if (!display->text().contains('.'))
        updateText(display->text() + tr("."));
    noOperand = false;
}
void PiCalcQT::changeSign()
{
    QString text = display->text();
    double value = text.toDouble();

    if (value > 0) {
        text.prepend(tr("-"));
    } else if (value < 0) {
        text.remove(0, 1);
    }
    updateText(text);
}

void PiCalcQT::backspace()
{
    if (noOperand)
        return;
    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        noOperand = true;
    }
    updateText(text);
}
void PiCalcQT::clear()
{
    if (noOperand)
        return;

    updateText("0");
    statusBar()->showMessage("Ready", 2000);
    noOperand = true;
}
void PiCalcQT::clearAll()
{
    result = 0;
    pendingOperator.clear();
    display_h->clear();
    updateText("0");
    statusBar()->showMessage("Ready", 2000);
    noOperand = true;
}

void PiCalcQT::abort()
{
    clearAll();
    updateText("Error");
}
void PiCalcQT::updateText(const QString &text) {
    display->setText(tr(text.toUtf8().constData()));
    display->setAlignment(Qt::AlignRight);
}
bool PiCalcQT::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == tr("+")) {
        result += rightOperand;
    }
    else if (pendingOperator == tr("-")) {
        result -= rightOperand;
    }
    else if (pendingOperator == tr("×")) {
        result *= rightOperand;
    }
    else if (pendingOperator == tr("÷")) {
        if (rightOperand == 0)
            return false;
        result /= rightOperand;
    }
    else if (pendingOperator == tr("↑")) {
        if (rightOperand == 0)
            result = 1;
        else {
            result = std::pow(result, rightOperand);
        }
    }
    else if (pendingOperator == tr("Mod")) {
        result = fmod(result,rightOperand);
    }
    else if (pendingOperator == tr("y√x")) {
        result = pow(result,1/rightOperand);
    }
    return true;
}
void PiCalcQT::about()
{
   QMessageBox::about(this, tr("About PiCalcQT"),
            tr("<b>PiCalcQT</b> is a calculator for Raspberry PI written in C++ and QT.<br>Build DEV02050221 - ALPHA3.<br>© 2021 Alexander Mazhirin"));
}
