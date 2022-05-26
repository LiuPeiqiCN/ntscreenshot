#include "SystemTray.h"
#include <QtWidgets>
#include "controller/WindowManager.h"

SystemTray::SystemTray(QWidget* parent)
	: QSystemTrayIcon(parent)
{
	this->setIcon(QIcon(":/images/ntscreenshot.ico"));
	menu_ = new QMenu(parent);
	screenshotAction_ = menu_->addAction(QStringLiteral("截屏"));
	connect(screenshotAction_, &QAction::triggered, this, &SystemTray::onScreenshot);

	pinAction_ = menu_->addAction(QStringLiteral("贴图"));
	connect(pinAction_, &QAction::triggered, this, &SystemTray::onPin);

	QAction* settingAction = menu_->addAction(QStringLiteral("设置"));
	connect(settingAction, &QAction::triggered, this, &SystemTray::onSetting);

	menu_->addSeparator();

	QAction* exitAction = menu_->addAction(QStringLiteral("退出"));
	connect(exitAction, &QAction::triggered, this, &SystemTray::onExit);

	QAction* testAction = menu_->addAction(QStringLiteral("test"));
	connect(testAction, &QAction::triggered, this, &SystemTray::onTest);
	testAction->setVisible(false);

	this->setContextMenu(menu_);

	onUpdate();
	connect(WindowManager::instance(), &WindowManager::sigSettingChanged, this, &SystemTray::onUpdate, Qt::QueuedConnection);
	connect(WindowManager::instance(), &WindowManager::sigStickerCountChanged, this, &SystemTray::onUpdate, Qt::QueuedConnection);
}

SystemTray::~SystemTray()
{
}

void SystemTray::onScreenshot()
{
	WindowManager::instance()->openWidget(WidgetID::SCREENSHOT);
}

void SystemTray::onPin()
{
	WindowManager::instance()->showAllSticker();
}

void SystemTray::onSetting()
{
	WindowManager::instance()->openWidget(WidgetID::SETTINGS);
}

void SystemTray::onExit()
{
	qApp->exit();
}

void SystemTray::onTest()
{
	WindowManager::instance()->openWidget(WidgetID::MASK);
}

void SystemTray::onUpdate()
{
	QStringList tips;
	tips << "ntscreenshot";
	tips << QStringLiteral("版本v1.0.0");
	tips << QStringLiteral("截图快捷键：%1").arg(WindowManager::instance()->setting()->screenhotGlobalKey());
	tips << QStringLiteral("贴图快捷键：%1").arg(WindowManager::instance()->setting()->pinGlobalKey());
	tips << QStringLiteral("贴图数目：%1").arg(WindowManager::instance()->allStickerCount());
	this->setToolTip(tips.join("\r\n"));

	screenshotAction_->setText(QStringLiteral("截屏 %1").arg(WindowManager::instance()->setting()->screenhotGlobalKey()));
	pinAction_->setText(QStringLiteral("贴图 %1").arg(WindowManager::instance()->setting()->pinGlobalKey()));
}
