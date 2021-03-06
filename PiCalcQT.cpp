#include "PiCalcQT.h"
#include "button.h"
#include "tinyexpr.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>
#include <QMainWindow>
#include <QtWidgets>
#include <sstream>
#include <iomanip>
#include <math.h>
PiCalcQT::PiCalcQT(QWidget* parent) : QMainWindow(parent)
{
	setWindowTitle(tr("PiCalcQT"));
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	//DECLARE WIDGETS AND LAYOUTS
	advlayout = new QGridLayout;
	advmodule = new QWidget;
	displaylayout = new QGridLayout;
	displaymodule = new QWidget;
	mainlayout = new QGridLayout;
	mainmodule = new QWidget;
	mainwindow = new QWidget;
	taxlayout = new QGridLayout;
	taxmodule = new QWidget;
	windowlayout = new QGridLayout;
	//TOP MENU SETUP
	fileMenu = menuBar()->addMenu(tr("File"));
	layoutMenu = menuBar()->addMenu(tr("Layout"));
	QActionGroup* layoutList = new QActionGroup(this);
	QActionGroup* trigonometryList = new QActionGroup(this);
	QAction* abt = fileMenu->addAction(tr("About"), this, &PiCalcQT::about);
	QAction* abtQT = fileMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
	QAction* adv = layoutMenu->addAction(tr("Advanced"), this, &PiCalcQT::switchToAdv);
	QAction* def = layoutMenu->addAction(tr("Default"), this, &PiCalcQT::switchToDef);
	QAction* fpp = fileMenu->addAction(tr("Set precision"), this, &PiCalcQT::setFloatPrecision);
	trigonometryMenu = fileMenu->addMenu(tr("Trigonometry"));
	trigonometryMenu->setEnabled(false);
	QAction* quit = fileMenu->addAction(tr("Quit"), qApp, &QApplication::quit);
	QAction* tax = layoutMenu->addAction(tr("Taxes"), this, &PiCalcQT::switchToTax);
	QAction* toDEG = trigonometryMenu->addAction(tr("Swicth to DEG"), this, &PiCalcQT::toDEG);
	QAction* toRAD = trigonometryMenu->addAction(tr("Swicth to RAD"), this, &PiCalcQT::toRAD);
	QAction* toGRAD = trigonometryMenu->addAction(tr("Swicth to GRAD"), this, &PiCalcQT::toGRAD);
	layoutList->addAction(adv);
	layoutList->addAction(def);
	layoutList->addAction(tax);
	trigonometryList->addAction(toDEG);
	trigonometryList->addAction(toRAD);
	trigonometryList->addAction(toGRAD);
	toDEG->setCheckable(true);
	toRAD->setCheckable(true);
	toRAD->setChecked(true);
	toGRAD->setCheckable(true);
	adv->setCheckable(true);
	def->setCheckable(true);
	def->setChecked(true);
	tax->setCheckable(true);
	abt->setStatusTip("About this application");
	abtQT->setStatusTip("About QT");
	adv->setStatusTip("Switch to advanced mode");
	def->setStatusTip("Switch to default mode");
	fpp->setStatusTip("Set precision for floating point");
	quit->setStatusTip("Quit this application");
	tax->setStatusTip("Switch to taxes mode");
	//STATUS BAR SETUP
	status = new QLabel("status", this);
	status->setStyleSheet("margin-right: 2px; border-top: none; border: none;");
	status->hide();
	statusBar()->addPermanentWidget(status);
	statusBar()->setSizeGripEnabled(false);
	statusBar()->setStyleSheet("background-color: white; color: black; border-top: 1px solid lightgray;");
	statusBar()->showMessage("Ready", 2000);
	//MAIN DISPLAY SETUP
	display = new QLabel("0");
	QFont font = display->font();
	font.setPointSize(font.pointSize() + 8);
	display->setFont(font);
	display->setAlignment(Qt::AlignRight);
	displaymodule->setStyleSheet( //FIXME: css parse errors
		".QWidget {background-color: white; border-radius: 0.25em; border: 1px solid lightgray; margin: 5px 10px 0;}"
		"QLabel {color: black; padding: 0 5px 5px;}");
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
	Button* addToMemory = addButton("M+", SLOT(unaryOperatorClicked()));
	Button* clearMemory = addButton("MC", SLOT(unaryOperatorClicked()));
	Button* readMemory = addButton("MR", SLOT(unaryOperatorClicked()));
	Button* setMemory = addButton("MS", SLOT(unaryOperatorClicked()));
	//DEFAULT
	Button* backspace = addButton("←", SLOT(backspace()));
	Button* changeSign = addButton("±", SLOT(changeSign()));
	Button* clear = addButton("CE", SLOT(clear()));
	Button* clearAll = addButton("C", SLOT(clearAll()));
	Button* divide = addButton("÷", SLOT(operatorClicked()));
	Button* equal = addButton("=", SLOT(equalsClicked()));
	Button* minus = addButton("-", SLOT(operatorClicked()));
	Button* multiply = addButton("×", SLOT(operatorClicked()));
	Button* plus = addButton("+", SLOT(operatorClicked()));
	Button* point = addButton(".", SLOT(pointClicked()));
	Button* squareRoot = addButton("√", SLOT(unaryOperatorClicked()));
	//ADVANCED
	Button* abs = addButton("|𝑥|", SLOT(unaryOperatorClicked()));
	Button* bracketL = addButton("(", SLOT(bracketClicked()));
	Button* bracketR = addButton(")", SLOT(bracketClicked()));
	Button* cos = addButton("cos", SLOT(unaryOperatorClicked()));
	Button* factorial = addButton("n!", SLOT(unaryOperatorClicked()));
	Button* inserte = addButton("e", SLOT(unaryOperatorClicked()));
	Button* ln = addButton("ln", SLOT(unaryOperatorClicked()));
	Button* log = addButton("log", SLOT(unaryOperatorClicked()));
	Button* mod = addButton("Mod", SLOT(operatorClicked()));
	Button* multiplicativeinverse = addButton("1/𝑥", SLOT(unaryOperatorClicked()));
	Button* pi = addButton("π", SLOT(unaryOperatorClicked()));
	Button* power = addButton("↑", SLOT(operatorClicked()));
	Button* sin = addButton("sin", SLOT(unaryOperatorClicked()));
	Button* tan = addButton("tan", SLOT(unaryOperatorClicked()));
	//TAXES
	Button* TAXadd = addButton("TAX+", SLOT(unaryOperatorClicked()));
	Button* TAXremove = addButton("TAX-", SLOT(unaryOperatorClicked()));
	Button* TAXset = addButton("RATE", SLOT(unaryOperatorClicked()));
	Button* TAXshow = addButton("TAX", SLOT(unaryOperatorClicked()));
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
	advlayout->addWidget(pi, 0, 1);
	advlayout->addWidget(power, 0, 2);
	advlayout->addWidget(multiplicativeinverse, 0, 3);
	advlayout->addWidget(mod, 0, 4);
	advlayout->addWidget(inserte, 1, 0);
	advlayout->addWidget(abs, 1, 1);
	advlayout->addWidget(sin, 1, 2);
	advlayout->addWidget(cos, 1, 3);
	advlayout->addWidget(tan, 1, 4);
	advlayout->addWidget(bracketL, 2, 0);
	advlayout->addWidget(bracketR, 2, 1);
	advlayout->addWidget(ln, 2, 2);
	advlayout->addWidget(log, 2, 3);
	//TAXES
	taxlayout->setContentsMargins(10, 6, 10, 10);
	taxlayout->addWidget(TAXset, 0, 0);
	taxlayout->addWidget(TAXadd, 0, 1);
	taxlayout->addWidget(TAXremove, 0, 2);
	taxlayout->addWidget(TAXshow, 0, 3);
}
Button* PiCalcQT::addButton(const QString& label, const char* member)
{
	Button* button = new Button(label);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}
QString PiCalcQT::toQString(long double& number)
{
	return QString::number(number, 'g', displayLimit);
}
QString PiCalcQT::lastChar(QLabel& displayname)
{
	if (!displayname.text().isEmpty())
		return QString(displayname.text().back());
	else
		return "";
}
long double PiCalcQT::getDisplayData(QLabel& displayname)
{
	long double p1 = 0;
	std::stringstream p2(displayname.text().toStdString());
	p2 >> p1;
	return p1;
}
void PiCalcQT::updateDisplayData(const QString& text)
{
	if (text == "nan") {
		display->setText(QString("Error").toUtf8().constData());
		errorFlag = true;
		return;
	}
	else if (text == "inf") {
		display->setText(QString("∞").toUtf8().constData());
		errorFlag = true;
		return;
	}
	else if (text == "-inf") {
		display->setText(QString("-∞").toUtf8().constData());
		errorFlag = true;
		return;
	}
	display->setText(text.toUtf8().constData());
}
bool PiCalcQT::bracketClosed() {
	if (!display_h->text().isEmpty()) {
		if (lastChar(*display_h) == ")")
			return true;
	}
	return false;
}
void PiCalcQT::switchToTax()
{
	mainlayout->setContentsMargins(10, 10, 10, 0);
	advmodule->hide();
	trigonometryMenu->setEnabled(false);
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
	trigonometryMenu->setEnabled(true);
	status->setText(measureUnit);
	adjustSize();
}
void PiCalcQT::switchToDef()
{
	mainlayout->setContentsMargins(10, 10, 10, 10);
	advmodule->hide();
	status->hide();
	taxmodule->hide();
	trigonometryMenu->setEnabled(false);
	windowlayout->removeWidget(advmodule);
	windowlayout->removeWidget(taxmodule);
	mainwindow->adjustSize();
	mainmodule->adjustSize();
	adjustSize(); //AdjustSize() MUST be called 3 times here.
}
void PiCalcQT::toRAD()
{
	measureUnit = "RAD";
	status->setText(measureUnit);
}
void PiCalcQT::toDEG()
{
	measureUnit = "DEG";
	status->setText(measureUnit);
}
void PiCalcQT::toGRAD()
{
	measureUnit = "GRAD";
	status->setText(measureUnit);
}
void PiCalcQT::about()
{
	QMessageBox about;
	QPixmap pixmapIcon = QIcon(":/Logo.png").pixmap(48);
	about.setParent(0);
	about.setText("<b>About PiCalcQT</b>");
	about.setInformativeText("<p>PiCalcQT is a calculator for Raspberry PI written in C++ and QT." \
		"<br>See <a href=\"https://github.com/Kexogg/PiCalcQT\">my GitHub page</a> for updates or more information" \
		"<br>Build " + QStringLiteral(__DATE__) + " " + QStringLiteral(__TIME__) + ".<br>© 2021 Alexander Mazhirin" \
		"<br>This application uses modified version of <a href=\"https://codeplea.com/tinyexpr\">tinyexpr</a> by Lewis Van Winkle</p>");
	about.setStandardButtons(QMessageBox::Ok);
	about.setDefaultButton(QMessageBox::Ok);
	about.setWindowTitle("About");
	about.setIconPixmap(pixmapIcon);
	about.exec();
}
void PiCalcQT::setFloatPrecision()
{
	floatPrecision = QInputDialog::getInt(this, tr("Set floating point precision"), tr("Precision"), floatPrecision, 0, 16, 1);
}
void PiCalcQT::digitClicked()
{
	Button* clickedButton = qobject_cast<Button*>(sender());
	int digit = clickedButton->text().toInt();
	if (resetFlag)
		clearAll();
	if (!queuedOperator.isEmpty())
	{
		display->clear();
		queuedOperator.clear();
	}
	if (display->text() == "0" && digit == 0)
		return;
	else if (display->text().length() == displayLimit)
		return;
	else if (display->text() == "0")
		updateDisplayData(QString::number(digit));
	else
		updateDisplayData(display->text() + QString::number(digit));
	queuedUnaryOperator.clear();
}
void PiCalcQT::pointClicked()
{
	if (resetFlag) {
		clearAll();
		updateDisplayData("0.");
	}
	if (!display->text().contains('.'))
		updateDisplayData(display->text() + ".");
}
void PiCalcQT::changeSign()
{
	if (!resetFlag && display->text() != "0")
	{
		long double value = getDisplayData(*display) / -1;
		updateDisplayData(toQString(value));
	}
}
void PiCalcQT::backspace()
{
	if (resetFlag)
		return;
	QString text = display->text();
	text.chop(1);
	if (text.isEmpty())
		text = "0";
	updateDisplayData(text);
}
void PiCalcQT::clear()
{
	if (resetFlag)
		return;
	updateDisplayData("0");
	statusBar()->showMessage("Ready", 2000);
}
void PiCalcQT::clearAll()
{
	display_h->clear();
	updateDisplayData("0");
	statusBar()->showMessage("Ready", 2000);
	resetFlag = false;
	errorFlag = false;
	queuedOperator.clear();
	queuedUnaryOperator.clear();
}
void PiCalcQT::unaryOperatorClicked()
{
	if (errorFlag)
		clearAll();
	Button* clickedButton = qobject_cast<Button*>(sender());
	queuedUnaryOperator = clickedButton->text();
	long double operand = getDisplayData(*display);
	long double result = 0;
	bool statusbarlock = false;
	bool historylock = false;
	bool bracketlock = false;
	//TAXES
	if (queuedUnaryOperator == "TAX")
	{
		result = operand - (operand / (tax * 0.01 + 1));
		historylock = true;
	}
	else if (queuedUnaryOperator == "TAX+")
	{
		result = operand * (tax * 0.01 + 1);
		historylock = true;
	}
	else if (queuedUnaryOperator == "TAX-")
	{
		result = operand / (tax * 0.01 + 1);
		historylock = true;
	}
	else if (queuedUnaryOperator == "RATE")
	{
		tax = operand;
		result = operand;
		statusBar()->showMessage("Tax rate set | Ready", 2000);
		statusbarlock = true;
		status->setText("Tax rate: " + QString::number(tax) + "%");
		historylock = true;
	}
	//ADVANCED
	if (queuedUnaryOperator == "n!")
	{
		queuedUnaryOperator = "!";
	}
	else if (queuedUnaryOperator == "1/𝑥")
	{
		if (operand == 0)
		{
			abort();
			return;
		}
		result = 1 / operand;
		if (resetFlag) {
			display_h->clear();
			resetFlag = false;
		}
		historylock = true;
	}
	else if (queuedUnaryOperator == "e" || queuedUnaryOperator == "π")
		bracketlock = true;
	else if (queuedUnaryOperator == "|𝑥|")
		queuedUnaryOperator = "abs";
	//MEMORY
	else if (queuedUnaryOperator == "MC")
	{
		result = operand;
		memory = 0;
		statusbarlock = true;
		statusBar()->showMessage("Memory cleared | Ready", 2000);
		historylock = true;
	}
	else if (queuedUnaryOperator == "MR")
	{
		result = memory;
		statusbarlock = true;
		statusBar()->showMessage("Memory recalled | Ready", 2000);
		historylock = true;
	}
	else if (queuedUnaryOperator == "MS")
	{
		memory = operand;
		result = operand;
		statusbarlock = true;
		statusBar()->showMessage("Memory set | Ready", 2000);
		historylock = true;
	}
	else if (queuedUnaryOperator == "M+")
	{
		memory += operand;
		result = operand;
		statusbarlock = true;
		statusBar()->showMessage("Memory updated | Ready", 2000);
		historylock = true;
	}
	if (historylock)
	{
		updateDisplayData(toQString(result));
		queuedUnaryOperator.clear();
	}
	else
	{
		if (!bracketlock)
			queuedUnaryOperator = queuedUnaryOperator + "(";
		if (resetFlag)
		{
			display_h->clear();
			resetFlag = false;
			if (bracketlock)
				display_h->setText(display->text() + "×" + queuedUnaryOperator);
			else
				display_h->setText(queuedUnaryOperator + display->text());
			display->setText("0");
			equalsClicked();
			return;
		}
		else if (lastChar(*display_h) == "e" || lastChar(*display_h) == "π" || lastChar(*display_h) == ")")
		{
			if (getDisplayData(*display) == 0)
				display_h->setText(display_h->text() + "×" + queuedUnaryOperator);
			else
				display_h->setText(display_h->text() + "×" + display->text() + "×" + queuedUnaryOperator);
		}
		else if (getDisplayData(*display) == 0)
			display_h->setText(display_h->text() + queuedUnaryOperator);
		else
			display_h->setText(display_h->text() + display->text() + "×" + queuedUnaryOperator);
		queuedOperator.clear();
		display->setText("0");
	}
	if (!statusbarlock)
		statusBar()->showMessage("Ready", 2000);
}
void PiCalcQT::bracketClicked()
{ //FIXME: else if hell
	if (errorFlag)
		clearAll();
	Button* clickedButton = qobject_cast<Button*>(sender());
	if (clickedButton->text() == "(")
	{
		if (display_h->text().isEmpty() && getDisplayData(*display) == 0)
			display_h->setText(clickedButton->text());
		else if (lastChar(*display_h) == "(" || (!queuedOperator.isEmpty() && getDisplayData(*display) == 0))
			display_h->setText(display_h->text() + clickedButton->text());
		else if (lastChar(*display_h) == ")")
		{
			if (getDisplayData(*display) == 0)
				display_h->setText(display_h->text() + "×" + clickedButton->text());
			else
				display_h->setText(display_h->text() + "×" + display->text() + "×" + clickedButton->text());
		}
		else if (getDisplayData(*display) != 0 && queuedOperator.isEmpty())
			display_h->setText(display_h->text() + display->text() + "×" + clickedButton->text());
		else if (queuedOperator.isEmpty())
			display_h->setText(display_h->text() + "×" + clickedButton->text());
	}
	else if (clickedButton->text() == ")" && !display_h->text().isEmpty())
	{
		std::string expression_str = display_h->text().toStdString(); //If we use std::count with display_h->text().toStdString() program will crash
		if (std::count(expression_str.begin(), expression_str.end(), '(') > std::count(expression_str.begin(), expression_str.end(), ')'))
		{
			if (getDisplayData(*display) != 0 || lastChar(*display_h) == "(")
				display_h->setText(display_h->text() + display->text() + clickedButton->text());
			else
				display_h->setText(display_h->text() + clickedButton->text());
		}
	}
	display->setText("0");
}
void PiCalcQT::operatorClicked()
{
	Button* clickedButton = qobject_cast<Button*>(sender());
	if (!clickedButton || errorFlag)
		return;
	if (resetFlag)
	{
		resetFlag = false;
		display_h->setText(display->text());
		display->clear();
	}
	if (!queuedOperator.isEmpty())
	{
		QString text = display_h->text();
		text.chop(queuedOperator.length());
		queuedOperator = clickedButton->text();
		display_h->setText(text + queuedOperator);
	}
	else if (queuedUnaryOperator.isEmpty() || queuedOperator.isEmpty())
	{
		queuedOperator = clickedButton->text();
		if (bracketClosed() || queuedUnaryOperator == "π" || queuedUnaryOperator == "e")
			display_h->setText(display_h->text() + queuedOperator);
		else
			display_h->setText(display_h->text() + display->text() + queuedOperator);
		display->setText("0");
	}
}
std::string replaceSymbols(std::string& text, const std::string& find, const std::string& replace) {
	size_t start_pos = 0;
	while ((start_pos = text.find(find, start_pos)) != std::string::npos)
	{
		text.replace(start_pos, find.length(), replace);
		start_pos += replace.length();
	}
	return(text);
}
std::string convertTrigonometry(std::string& text, const QString& replace)
{
	size_t start_pos = 0;
	size_t br_pos;
	int nested_brackets;
	std::string function[3] = { "sin(", "cos(", "tan(" };
	std::string replace_std;
	if (replace == "DEG")
		replace_std = "d2r(";
	else
		replace_std = "g2r(";
	for (int i = 0; i < 3; i++)
	{
		while ((start_pos = text.find(function[i], start_pos)) != std::string::npos)
		{
			text.replace(start_pos, function[i].length(), function[i] + replace_std);
			start_pos += replace_std.length();
			br_pos = start_pos;
			nested_brackets = 0;
			while (text.at(br_pos - 1) != ')' || nested_brackets != 0)
			{
				if (text.at(br_pos) == '(')
					nested_brackets += 1;
				else if (text.at(br_pos) == ')' && nested_brackets == 1)
				{
					text.insert(br_pos, ")");
					nested_brackets = 0;
				}
				else if (text.at(br_pos) == ')' && nested_brackets != 0)
					nested_brackets -= 1;
				br_pos += 1;
			}
		}
		start_pos = 0;
	}
	return(text);
}
void PiCalcQT::equalsClicked()
{
	if (errorFlag)
	{
		clearAll();
		return;
	}
	if (lastChar(*display_h) == "÷" || lastChar(*display_h) == "×" || lastChar(*display_h) == "-" || lastChar(*display_h) == "+" || lastChar(*display_h) == "d" || lastChar(*display_h) == "(")
		display_h->setText(display_h->text() + display->text());
	else if ((lastChar(*display_h) == "e" || lastChar(*display_h) == "π") && getDisplayData(*display) != 0)
		display_h->setText(display_h->text() + "×" + display->text());
	else if (getDisplayData(*display) != 0)
		display_h->setText(display_h->text() + display->text());
	if (resetFlag)
		display_h->setText(display->text());
	/* Close brackets */
	std::string expression_str = display_h->text().toStdString();
	int clb = std::count(expression_str.begin(), expression_str.end(), '(');
	int crb = std::count(expression_str.begin(), expression_str.end(), ')');
	if (clb > crb) {
		for (int var = 0; var < clb - crb; ++var)
			expression_str += ")";
	}
	display_h->setText(QString::fromStdString(expression_str));
	/* Convert special symbols to normal symbols so tinyexpr can parse expression*/
	replaceSymbols(expression_str, std::string("!"), std::string("fac"));
	replaceSymbols(expression_str, std::string("×"), std::string("*"));
	replaceSymbols(expression_str, std::string("÷"), std::string("/"));
	replaceSymbols(expression_str, std::string("√"), std::string("sqrt"));
	replaceSymbols(expression_str, std::string("↑"), std::string("^"));
	replaceSymbols(expression_str, std::string("Mod"), std::string("%"));
	replaceSymbols(expression_str, std::string("π"), std::string("pi"));
	/* Convert GRAD or DEG to RAD if needed */
	if (measureUnit != "RAD" && (expression_str.find("sin") != std::string::npos || expression_str.find("cos") != std::string::npos || expression_str.find("tan") != std::string::npos))
		convertTrigonometry(expression_str, measureUnit);
	display_h->setText(display_h->text() + "=");
	std::stringstream stream;
	long double result = te_interp(expression_str.c_str(), 0);
	if (result != NAN && result != INFINITY && result != -INFINITY)
	{
		stream << std::fixed << std::setprecision(floatPrecision) << result;
		stream >> result;
	}
	updateDisplayData(toQString(result));
	resetFlag = true;
	queuedOperator.clear();
	queuedOperator.clear();
}
void PiCalcQT::abort()
{
	clearAll();
	updateDisplayData("Error");
}