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

#ifndef MOVIEELEMENT_H
#define MOVIEELEMENT_H

#include <QGraphicsProxyWidget>
#include <phonon/VideoPlayer>
#include <phonon/VideoWidget>
#include <phonon/AudioOutput>
#include <phonon/MediaObject>
#include <QPainter>
#include <QTimer>

#include "slideelement.h"
#include "graphicspixmapitem.h"
#include "filepathmanager.h"
#include "fileeditfactory.h"
#include "configuration.h"
#include "shared.h"

class CFISLIDES_DLLSPEC MovieElement : public SlideElement
{
	Q_OBJECT
public:
	explicit MovieElement();
	virtual QString previewUrl() const;
	virtual void render(QGraphicsScene *scene, const bool interactive);
	virtual void bindProperties(QtTreePropertyBrowser *browser) const;

public slots:
	virtual void play();
	virtual void pause();
	virtual void stop();
	virtual void toggleMute();
	virtual void destroy();

private slots:
	void interactiveVideoLoaded(bool);
	void takeSnapshot();
	void restart();

protected:
	Phonon::VideoPlayer *player;
	QGraphicsProxyWidget *proxy;
	QPixmap snapshot;
	QString loadedFile;
	bool playbackFinished;
};

Q_DECLARE_METATYPE(MovieElement)

#endif // MOVIEELEMENT_H
