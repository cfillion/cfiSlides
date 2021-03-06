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

#include "rectelement.h"
#include "propertymanager.h"
#include "configuration.h"

RectElement::RectElement() : SlideElement()
{
	setValue(QStringLiteral("size"), QSize(100, 100));
	setValue(QStringLiteral("bgStyle"), 1);
	setValue(QStringLiteral("borderSize"), 1);
}

QGraphicsItem *RectElement::render(const bool interactive)
{
	if(!getValue(QStringLiteral("visible")).toBool())
		return 0;

	QPen pen(penStyle());
	pen.setColor(getValue(QStringLiteral("borderColor")).value<QColor>());
	pen.setWidth(getValue(QStringLiteral("borderSize")).toInt());

	GraphicsRectItem *item = new GraphicsRectItem(interactive, this);
	item->setBrush(QBrush(getValue(QStringLiteral("color")).value<QColor>(), brushStyle()));
	item->setPen(pen);
	item->setRect(QRect(QPoint(), getValue(QStringLiteral("size")).toSize()));
	item->setPos(getValue(QStringLiteral("position")).toPoint());

	return item;
}

PropertyList RectElement::getProperties() const
{
	EnumPropertyManager *enumManager = new EnumPropertyManager;
	connect(enumManager, &PropertyManager::modified, this, &RectElement::propertyChanged);

	ColorPropertyManager *colorManager = new ColorPropertyManager;
	connect(colorManager, &PropertyManager::modified, this, &RectElement::propertyChanged);

	IntPropertyManager *intManager = new IntPropertyManager;
	connect(intManager, &PropertyManager::modified, this, &RectElement::propertyChanged);

	Property *background = new Property(0, tr("Remplissage"));

	Property *bgStyle = new Property(enumManager, tr("Style"), QStringLiteral("bgStyle"));
	bgStyle->setToolTip(tr("Mode de remplissage"));
	bgStyle->setValue(this->getValue(QStringLiteral("bgStyle")));
	enumManager->setEnumNames(QStringLiteral("bgStyle"),
		QStringList()
			<< tr("Aucun remplissage")
			<< tr("Uni")
			<< tr("Dense 1")
			<< tr("Dense 2")
			<< tr("Dense 3")
			<< tr("Dense 4")
			<< tr("Clairsemé 1")
			<< tr("Clairsemé 2")
			<< tr("Clairsemé 3")
			<< tr("Horizontal")
			<< tr("Vertical")
			<< tr("Croisé")
			<< tr("Diagonal")
			<< tr("Diagonal inversé")
			<< tr("Diagonal croisé")
	);
	background->addProperty(bgStyle);

	Property *color = new Property(colorManager, tr("Couleur"), QStringLiteral("color"));
	color->setToolTip(tr("Couleur de remplissage"));
	color->setValue(this->getValue(QStringLiteral("color")));
	background->addProperty(color);

	Property *border = new Property(0, tr("Bordure"));

	Property *borderStyle = new Property(enumManager, tr("Style"), QStringLiteral("borderStyle"));
	borderStyle->setToolTip(tr("Style de la bordure"));
	enumManager->setEnumNames(QStringLiteral("borderStyle"),
		QStringList()
			<< tr("Aucune bordure")
			<< tr("Ligne")
			<< tr("Pointillés")
			<< tr("Points")
			<< tr("Point-tiret")
			<< tr("Point-point-tiret")
	);
	borderStyle->setValue(this->getValue(QStringLiteral("borderStyle")));
	border->addProperty(borderStyle);

	Property *borderSize = new Property(intManager, tr("Taille"), QStringLiteral("borderSize"));
	borderSize->setToolTip(tr("Épaisseur de la bordure"));
	borderSize->setValue(this->getValue(QStringLiteral("borderSize")));
	intManager->setMinimum(QStringLiteral("borderSize"), 1);
	intManager->setMaximum(QStringLiteral("borderSize"), MAXIMUM_THICKNESS);
	intManager->setSuffix(QStringLiteral("borderSize"), tr(" px"));
	border->addProperty(borderSize);

	Property *borderColor = new Property(colorManager, tr("Couleur"), QStringLiteral("borderColor"));
	borderColor->setToolTip(tr("Couleur de la bordure"));
	borderColor->setValue(this->getValue(QStringLiteral("borderColor")));
	border->addProperty(borderColor);

	return PropertyList()
		<< SlideElement::getProperties()
		<< background
		<< border;
}

Qt::PenStyle RectElement::penStyle() const
{
	Qt::PenStyle penStyle;
	switch(getValue(QStringLiteral("borderStyle")).toInt())
	{
		case 1:
			penStyle = Qt::SolidLine;
			break;
		case 2:
			penStyle = Qt::DashLine;
			break;
		case 3:
			penStyle = Qt::DotLine;
			break;
		case 4:
			penStyle = Qt::DashDotLine;
			break;
		case 5:
			penStyle = Qt::DashDotDotLine;
			break;
		default:
			penStyle = Qt::NoPen;
			break;
	}
	return penStyle;
}

Qt::BrushStyle RectElement::brushStyle() const
{
	Qt::BrushStyle brushStyle;
	switch(getValue("bgStyle", 1).toInt())
	{
		case 1:
			brushStyle = Qt::SolidPattern;
			break;
		case 2:
			brushStyle = Qt::Dense1Pattern;
			break;
		case 3:
			brushStyle = Qt::Dense2Pattern;
			break;
		case 4:
			brushStyle = Qt::Dense3Pattern;
			break;
		case 5:
			brushStyle = Qt::Dense4Pattern;
			break;
		case 6:
			brushStyle = Qt::Dense5Pattern;
			break;
		case 7:
			brushStyle = Qt::Dense6Pattern;
			break;
		case 8:
			brushStyle = Qt::Dense7Pattern;
			break;
		case 9:
			brushStyle = Qt::HorPattern;
			break;
		case 10:
			brushStyle = Qt::VerPattern;
			break;
		case 11:
			brushStyle = Qt::CrossPattern;
			break;
		case 12:
			brushStyle = Qt::BDiagPattern;
			break;
		case 13:
			brushStyle = Qt::FDiagPattern;
			break;
		case 14:
			brushStyle = Qt::DiagCrossPattern;
			break;
		default:
			brushStyle = Qt::NoBrush;
			break;
	}
	return brushStyle;
}
