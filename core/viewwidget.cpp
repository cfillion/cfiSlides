/**
 * Copyright (C) 2013  Christian Fillion
 * This file is part of cfiSlides.
 *
 * cfiSlides is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cfiSlides is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cfiSlides.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QGraphicsView>
#include <QProgressDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QTimer>
#include <QMenu>

#include "viewwidget.h"
#include "ui_viewwidget.h"
#include "slideshow.h"
#include "slide.h"
#include "icon_t.h"
#include "configuration.h"

ViewWidget::ViewWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ViewWidget)
{
	ui->setupUi(this);

	contextMenu = new QMenu(this);
	contextMenu->addAction(ICON_T("go-next"), tr("Diapositive suivante"), this, SLOT(next()), QKeySequence(Qt::Key_Return));
	contextMenu->addAction(ICON_T("go-previous"), tr("Diapositive précédente"), this, SLOT(prev()), QKeySequence(Qt::Key_Left));
	contextMenu->addAction(ICON_T("go-jump"), tr("Atteindre..."), this, SLOT(goTo()), QKeySequence(Qt::Key_G));
	contextMenu->addSeparator();
	contextMenu->addAction(ICON_T("media-playback-start"), tr("Lecture"), this, SLOT(play()), QKeySequence(Qt::Key_P));
	contextMenu->addAction(ICON_T("media-playback-pause"), tr("Pause"), this, SLOT(pause()), QKeySequence(Qt::Key_P));
	contextMenu->addAction(ICON_T("media-skip-backward"), tr("Redémarrer"), this, SLOT(restart()), QKeySequence(Qt::Key_R));
	contextMenu->addAction(ICON_T("audio-volume-muted"), tr("Muet"), this, SLOT(toggleMute()), QKeySequence(Qt::Key_M));
	contextMenu->addSeparator();
	contextMenu->addAction(ICON_T("view-restore"), tr("Fermer"), this, SLOT(close()), QKeySequence(Qt::Key_Escape));

	new QShortcut(QKeySequence(QStringLiteral("Ctrl+Q")), this, SLOT(close()));
	new QShortcut(QKeySequence(Qt::Key_Q), this, SLOT(close()));
	new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(close()));
	new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(close()));

	new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_Enter), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_Tab), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_Forward), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_MediaNext), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_L), this, SLOT(next()));
	new QShortcut(QKeySequence(Qt::Key_J), this, SLOT(next()));

	new QShortcut(QKeySequence(Qt::Key_Backspace), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_Back), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_MediaPrevious), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_H), this, SLOT(prev()));
	new QShortcut(QKeySequence(Qt::Key_K), this, SLOT(prev()));

	new QShortcut(QKeySequence(Qt::Key_Home), this, SLOT(first()));
	new QShortcut(QKeySequence(Qt::Key_PageUp), this, SLOT(first()));

	new QShortcut(QKeySequence(Qt::Key_End), this, SLOT(last()));
	new QShortcut(QKeySequence(Qt::Key_PageDown), this, SLOT(last()));
	new QShortcut(QKeySequence(Qt::Key_MediaLast), this, SLOT(last()));

	new QShortcut(QKeySequence(Qt::Key_MediaPlay), this, SLOT(play()));

	new QShortcut(QKeySequence(Qt::Key_MediaPause), this, SLOT(pause()));

	new QShortcut(QKeySequence(Qt::Key_P), this, SLOT(togglePlayPause()));
	new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(togglePlayPause()));
	new QShortcut(QKeySequence(Qt::Key_MediaTogglePlayPause), this, SLOT(togglePlayPause()));

	new QShortcut(QKeySequence(Qt::Key_S), this, SLOT(stop()));
	new QShortcut(QKeySequence(Qt::Key_MediaStop), this, SLOT(stop()));

	new QShortcut(QKeySequence(Qt::Key_M), this, SLOT(toggleMute()));
	new QShortcut(QKeySequence(Qt::Key_VolumeMute), this, SLOT(toggleMute()));

	new QShortcut(QKeySequence(Qt::Key_R), this, SLOT(restart()));
	new QShortcut(QKeySequence(Qt::Key_Refresh), this, SLOT(restart()));
	new QShortcut(QKeySequence(Qt::Key_Reload), this, SLOT(restart()));

	new QShortcut(QKeySequence(Qt::Key_G), this, SLOT(goTo()));
	new QShortcut(QKeySequence(Qt::Key_Go), this, SLOT(goTo()));

	new QShortcut(QKeySequence(Qt::Key_B), this, SLOT(toggleBlack()));
}

ViewWidget::~ViewWidget()
{
	delete ui;
}

void ViewWidget::displayContextMenu(const QPoint &pos)
{
	contextMenu->exec(this->mapToGlobal(pos));
}

void ViewWidget::setSlideshow(Slideshow *slideshow, const int startIndex)
{
	this->slideshow = slideshow;
	const QRect sceneRect = QRect(QPoint(), slideshow->getValue(QStringLiteral("size")).toSize());

	int index = 0;
	foreach(Slide *slide, slideshow->getSlides())
	{
		QGraphicsScene *scene = new QGraphicsScene(this);
		scene->setSceneRect(sceneRect);

		if(startIndex == index)
			slide->render(scene, false);

		QGraphicsView *view = new QGraphicsView(scene, this);
		view->setFrameShape(QFrame::NoFrame);
		view->setCursor(Qt::BlankCursor);
		view->setFocusPolicy(Qt::NoFocus);
		view->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
		view->setFixedSize(sceneRect.size());

		QWidget *container = new QWidget(this);
		QHBoxLayout *layout = new QHBoxLayout(container);
		layout->setMargin(0);
		layout->addWidget(view);

		ui->stackedWidget->addWidget(container);
		index++;
	}

	ui->stackedWidget->setCurrentIndex(startIndex);
	play();
}

void ViewWidget::prev()
{
	if(ui->stackedWidget->currentIndex() == 0)
		return;

	stop();
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
	play();
}

void ViewWidget::next()
{
	if(ui->stackedWidget->currentIndex() == ui->stackedWidget->count() - 1)
		return;

	stop();
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
	play();
}

void ViewWidget::first()
{
	if(ui->stackedWidget->currentIndex() == 0)
		return;

	stop();
	ui->stackedWidget->setCurrentIndex(0);
	play();
}

void ViewWidget::last()
{
	if(ui->stackedWidget->currentIndex() == ui->stackedWidget->count() - 1)
		return;

	stop();
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
	play();
}

void ViewWidget::play()
{
	const int index = ui->stackedWidget->currentIndex();
	const QGraphicsView *view = ui->stackedWidget->widget(index)->findChild<QGraphicsView *>();
	if(view->scene()->items().size() == 0)
		lazyLoad();
	else
		QTimer::singleShot(REFRESH_INTERVAL, this, SLOT(lazyLoad()));

	paused = false;
	slideshow->getSlide(index)->play();
}

void ViewWidget::pause()
{
	paused = true;
	slideshow->getSlide(ui->stackedWidget->currentIndex())->pause();
}

void ViewWidget::togglePlayPause()
{
	paused ? play() : pause();
}

void ViewWidget::stop()
{
	paused = true;
	slideshow->getSlide(ui->stackedWidget->currentIndex())->stop();
}

void ViewWidget::toggleMute()
{
	slideshow->getSlide(ui->stackedWidget->currentIndex())->toggleMute();
}

void ViewWidget::destroy()
{
	slideshow->getSlide(ui->stackedWidget->currentIndex())->destroy();
}

void ViewWidget::restart()
{
	stop();
	play();
}

void ViewWidget::goTo()
{
	pause();
	bool ok;
	QStringList slideNames;
	foreach(Slide *slide, slideshow->getSlides())
		slideNames << slide->getValue(QStringLiteral("name")).toString();

	QString name = QInputDialog::getItem(this, tr("Sélectionner une diapositive"), tr("Afficher la diapositive :"), slideNames, ui->stackedWidget->currentIndex(), false, &ok);
	if(!ok || name.isEmpty() || slideNames.indexOf(name) == ui->stackedWidget->currentIndex())
		return play();

	stop();
	ui->stackedWidget->setCurrentIndex(slideNames.indexOf(name));
	play();
}

void ViewWidget::toggleBlack()
{
	ui->stackedWidget->setVisible(!ui->stackedWidget->isVisible());
}

void ViewWidget::closeEvent(QCloseEvent *)
{
	const int slideCount = slideshow->getSlides().size();
	for(int index = 0; index < slideCount; index++)
	{
		const QGraphicsView *view = ui->stackedWidget->widget(index)->findChild<QGraphicsView *>();
		if(view->scene()->items().size() == 0)
			continue;

		Slide *slide = this->slideshow->getSlide(index);
		slide->stop();
		slide->destroy();
	}
	emit closed(ui->stackedWidget->currentIndex());
}

void ViewWidget::lazyLoad()
{
	const int currentIndex = ui->stackedWidget->currentIndex();
	const int slideCount = ui->stackedWidget->count();

	const int keepStart = currentIndex - (MAX_LOADED_SLIDES / 2);
	const int keepEnd = currentIndex + (MAX_LOADED_SLIDES / 2);
	for(int index = 0; index < slideCount; index++)
	{
		const QGraphicsView *view = ui->stackedWidget->widget(index)->findChild<QGraphicsView *>();
		Slide *slide = this->slideshow->getSlide(index);

		if(index < keepStart || index > keepEnd)
		{
			if(view->scene()->items().size() > 0)
			{
				slide->stop();
				slide->destroy();
				view->scene()->clear();
			}
		}
		else if(view->scene()->items().size() == 0)
			slide->render(view->scene(), false);
	}
}
