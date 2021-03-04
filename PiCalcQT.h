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
	PiCalcQT(QWidget* parent = nullptr);
private slots:
	void backspace();
	void bracketClicked();
	void changeSign();
	void clear();
	void clearAll();
	void digitClicked();
	void equalsClicked();
	void operatorClicked();
	void pointClicked();
	void unaryOperatorClicked();
private:
	//UI
	Button* addButton(const QString& text, const char* member);
	enum
	{
		Numpad = 10
	};
	Button* digitButtons[Numpad];
	QGridLayout* advlayout;
	QGridLayout* displaylayout;
	QGridLayout* mainlayout;
	QGridLayout* taxlayout;
	QGridLayout* windowlayout;
	QLabel* display;
	QLabel* display_h;
	QLabel* status;
	QMenu* fileMenu;
	QMenu* layoutMenu;
	QMenu* trigonometryMenu;
	QString measureUnit = "RAD";
	QString queuedOperator;
	QString queuedUnaryOperator;
	QWidget* advmodule;
	QWidget* displaymodule;
	QWidget* mainmodule;
	QWidget* mainwindow;
	QWidget* taxmodule;
	//Functions
	bool bracketClosed();
	long double getDisplayData(QLabel& displayname);
	QString lastChar(QLabel& displayname);
	QString toQString(long double& number);
	void abort();
	void about();
	void setFloatPrecision();
	void setupUI();
	void switchToAdv();
	void switchToDef();
	void switchToTax();
	void toRAD();
	void toDEG();
	void toGRAD();
	void updateDisplayData(const QString& text);
	//Variables
	bool errorFlag = false;
	bool resetFlag = false;
	float tax = 20;
	int displayLimit = 16;
	int floatPrecision = 5;
	long double memory;
};
#endif