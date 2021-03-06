//Card file to show output that can be global

#include "../include/stdafx.h"

//Include guard
#ifndef CARD_H
#define CARD_H

//Configuration
#define window_width 250
#define window_height 120

//Constants
#define WINDOW_TITLE "Processing Card"

class Card {
public:
	//Suit enumeration
	enum Suit {
		CLUBS,
		DIAMONDS,
		HEARTS,
		SPADES,
		UNKNOWN_SUIT
	};

	//Picture card rank enumeration
	enum PictureRank {
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		UNKNOWN_RANK
	};

	//Suit colour enumeration
	enum Colour {
		BLACK,
		RED,
		UNKNOWN_COLOUR
	};

	static const int WIDTH = 250, HEIGHT = 350, AREA = 250*350, CORNER_AREA;
	static const cv::Rect TOP_CORNER_RECT;
	static const cv::Rect BOTTOM_CORNER_RECT;
	static const cv::Scalar LINE_COLOUR, LINE_COLOUR_ALT;
	static const cv::Scalar TEXT_COLOUR;

	static int card_window_count;

	//Data
	cv::Mat mat, mat_clahe, mat_bin, mat_sym, mat_rank;
	Suit detected_suit;
	Colour detected_colour;
	PictureRank detected_rank;
	int detected_value;
	bool is_picture_card;
	cv::Rect _last_aabb, _rank_aabb;

	//Constructor declarations
	Card();
	Card(cv::Mat);

	//Functions
	void set_mat(cv::Mat mat);
	void init();
	void show();
	void show_with_card(cv::Mat card);
	cv::Mat results_to_mat();
	cv::Mat as_mat_with_results();
};

void show_cascade(std::vector<Card> cards);

#endif