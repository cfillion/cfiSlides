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

#include "slideelementtype.h"

SlideElementType::SlideElementType(const int typeId, const QString &label, const QIcon &icon)
{
	this->typeId = typeId;
	this->label = label;
	this->icon = icon;
}

int SlideElementType::getId() const
{
	return typeId;
}

QString SlideElementType::getLabel() const
{
	return label;
}

QIcon SlideElementType::getIcon() const
{
	return icon;
}
