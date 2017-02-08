#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <string>

class GuiStyle
{
public:

	sf::Color bodyCol;
	sf::Color bodyHighlightCol;
	sf::Color borderColor;
	sf::Color borderHighlightColor;
	sf::Color textCol;
	sf::Color textHighlightCol;
	sf::Font *font;

	float borderSize;

	GuiStyle(sf::Font *font, float borderSize,
		sf::Color bodyCol, sf::Color borderCol, sf::Color textCol,
		sf::Color bodyHighlightCol, sf::Color borderHighlightCol, sf::Color textHilightCol)
	{
		this->bodyCol = bodyCol;
		this->borderColor = borderCol;
		this->textCol = textCol;
		this->bodyHighlightCol = bodyHighlightCol;
		this->borderHighlightColor = borderHighlightCol;
		this->textHighlightCol = textHighlightCol;
		this->font = font;
		this->borderSize = borderSize;
	}

	GuiStyle() { }
};

class GuiEntry
{
public:

	// handles appearance of the entry
	sf::RectangleShape shape;

	// string returned when activated
	std::string message;

	// text displayed on the entry
	sf::Text text;

	GuiEntry(const std::string &message, sf::RectangleShape shape, sf::Text text)
	{
		this->message = message;
		this->shape = shape;
		this->text = text;
	}

	GuiEntry() { }
};

class Gui : public sf::Transformable, public sf::Drawable
{
private:

	// if true, menu entries horizontally (not vertically) adjacent
	bool horizontal;

	GuiStyle style;

	sf::Vector2f dimensions;

	int padding;

public:

	std::vector<GuiEntry> entries;

	bool visible;

	// constructor
	Gui(sf::Vector2f dimensions, int padding, bool horizontal,
		GuiStyle &style, std::vector<std::pair<std::string, std::string>> entries)
	{
		visible = false;
		this->horizontal = horizontal;
		this->style = style;
		this->dimensions = dimensions;
		this->padding = padding;

		// construct background shape
		sf::RectangleShape shape;
		shape.setSize(dimensions);
		shape.setFillColor(style.bodyCol);
		shape.setOutlineThickness(-style.borderSize);
		shape.setOutlineColor(style.borderColor);

		// construct each gui entry
		for (auto entry : entries)
		{
			// construct the text
			sf::Text text;
			text.setString(entry.first);
			text.setFont(*style.font);
			text.setFillColor(style.textCol);
			text.setCharacterSize(dimensions.y - style.borderSize - padding);

			this->entries.push_back(GuiEntry(entry.second, shape, text));
		}
	}

	sf::Vector2f getSize();

	// return entry that mouse is hovering over.  Returns -1 if outside of GUI
	int getEntry(const sf::Vector2f mousePos);

	// change text of an entry
	void setEntryText(int entry, std::string text);

	// change entry dimensions
	void setDimensions(sf::Vector2f dimensions);

	// draw the menu
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void show();

	void hide();

	// highlight an entry of the menu
	void highlight(const int entry);

	// return message bound to entry
	std::string activate(const int entry);
	std::string activate(const sf::Vector2f mousePos);
};