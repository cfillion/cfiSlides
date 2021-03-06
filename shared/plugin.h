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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtPlugin>
#include <QMainWindow>

#include "slideelementtype.h"

class MainWindow;

class Plugin : public QObject
{
	Q_OBJECT

public:
	Plugin() {}
	virtual SlideElementTypeList getElementTypes() const
	{
		return SlideElementTypeList();
	}

	void setWindow(QMainWindow *window)
	{
		this->window = window;
		initialize();
	}

protected:
	QMainWindow *window;
	virtual void initialize() = 0;
};

Q_DECLARE_INTERFACE(Plugin, "cfiSlides.Plugin")

#endif // PLUGIN_H
