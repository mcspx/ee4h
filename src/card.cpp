/************************************************************************\
| Card file for EE4H Assignment											 |
|																		 |
| Authors: Yousef Amar and Chris Lewis									 |
| Last Modified: 05/02/2014												 |
|																		 |
\************************************************************************/

#include "../include/stdafx.h"

using namespace std;

//#define CORNER_H_PERC 0.12F
//#define CORNER_V_PERC 0.28F

// Initialise corner Rects
const cv::Rect Card::TOP_CORNER_RECT = cv::Rect(2, 2, 0.13F * Card::WIDTH - 2, 0.28F * Card::HEIGHT - 2);
const cv::Rect Card::BOTTOM_CORNER_RECT = cv::Rect(Card::WIDTH - 0.13F * Card::WIDTH, Card::HEIGHT - 0.28F * Card::HEIGHT, 0.13F * Card::WIDTH, 0.28F * Card::HEIGHT);
const int Card::CORNER_AREA = Card::TOP_CORNER_RECT.area();

// Initialise colours
const cv::Scalar Card::LINE_COLOUR = cv::Scalar(0, 255, 0), Card::LINE_COLOUR_ALT = cv::Scalar(255, 0, 0);
const cv::Scalar Card::TEXT_COLOUR = cv::Scalar(255, 255, 0);

int Card::card_window_count = 0;

/**
  * Initialise all the data items in the results
  */
Card::Card()
{
	detected_suit = UNKNOWN_SUIT;
	detected_colour = UNKNOWN_COLOUR;
	detected_value = -1;
	detected_rank = UNKNOWN_RANK;

	/*stringstream title_stream;
	title_stream << WINDOW_TITLE;
	cv::destroyWindow(title_stream.str());*/
}

Card::Card(cv::Mat mat)
{
	Card();
	set_mat(mat);
}

void Card::set_mat(cv::Mat mat)
{
	this->mat = mat;

	// Convert card mat to grey
	mat_clahe = cv::Mat(mat.size(), CV_8U);
	cv::cvtColor(mat, mat_clahe, CV_BGR2GRAY);

	
	//CLAHE (Contrast Limited Adaptive Histogram Equalization)
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
	clahe->apply(mat_clahe, mat_clahe);
	cv::blur(mat_clahe, mat_clahe, cv::Size(4, 4));

	mat_bin = mat_clahe >= 140;
}

/**
  * Show the results data
  */
void Card::show()
{
	//Finally
	stringstream title_stream;
	title_stream << WINDOW_TITLE;
	cv::imshow(title_stream.str(), results_to_mat());
}

void Card::show_with_card(cv::Mat card)
{
	cv::Mat results_mat = results_to_mat();

	cv::Mat final(card.rows + results_mat.rows, max(card.cols, results_mat.cols), CV_8UC3);

	card.copyTo(final(cv::Rect(0, 0, card.cols, card.rows)));
	results_mat.copyTo(final(cv::Rect(0, card.rows, results_mat.cols, results_mat.rows)));

	//Finally
	stringstream title_stream;
	title_stream << WINDOW_TITLE << ++Card::card_window_count;
	cv::imshow(title_stream.str(), final);
}

cv::Mat Card::results_to_mat()
{
	//Create canvas
	cv::Mat canvas(window_height + 20, window_width + 10, CV_8UC3, cv::Scalar(255, 255, 255));

	//Title
	cv::putText(canvas, "Results:", cv::Point(10, 35), CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 0, 0), 1, 8, false);
	
	//Colour
	switch(detected_colour) {
	case RED:
		cv::putText(canvas, "Colour: RED", cv::Point(10, 60), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	case BLACK:
		cv::putText(canvas, "Colour: BLACK", cv::Point(10, 60), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	default:
		cv::putText(canvas, "Colour: UNKNOWN", cv::Point(10, 60), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	}

	//Suit
	switch(detected_suit) {
	case CLUBS:
		cv::putText(canvas, "Suit: CLUBS", cv::Point(10, 85), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	case DIAMONDS:
		cv::putText(canvas, "Suit: DIAMONDS", cv::Point(10, 85), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	case HEARTS:
		cv::putText(canvas, "Suit: HEARTS", cv::Point(10, 85), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	case SPADES:
		cv::putText(canvas, "Suit: SPADES", cv::Point(10, 85), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	default:
		cv::putText(canvas, "Suit: UNKNOWN", cv::Point(10, 85), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
		break;
	}

	// Small hack; equivalent of != null
	if (mat_sym.size().width)
		mat_sym.copyTo(canvas(cv::Rect(window_width - mat_sym.size().width, window_height - mat_sym.size().height, mat_sym.size().width, mat_sym.size().height)));
	if (mat_rank.size().width)
		mat_rank.copyTo(canvas(cv::Rect(window_width - mat_rank.size().width - mat_sym.size().width, window_height - mat_rank.size().height, mat_rank.size().width, mat_rank.size().height)));

	//Detected value
	stringstream val_stream;
	val_stream << "Value: " << detected_value;
	cv::putText(canvas, val_stream.str(), cv::Point(10, 110), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);

	//Detected picture card?
	if(is_picture_card == true)
	{
		switch(detected_rank) {
		case RANK_JACK:
			cv::putText(canvas, "Rank: JACK", cv::Point(10, 135), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
			break;
		case RANK_QUEEN:
			cv::putText(canvas, "Rank: QUEEN", cv::Point(10, 135), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
			break;
		case RANK_KING:
			cv::putText(canvas, "Rank: KING", cv::Point(10, 135), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
			break;
		case RANK_ACE:
			cv::putText(canvas, "Rank: ACE", cv::Point(10, 135), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
			break;
		default:
			cv::putText(canvas, "Rank: UNKNOWN", cv::Point(10, 135), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
			break;
		}
	}
	else
	{
		cv::putText(canvas, "Rank: N/A", cv::Point(10, 130), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 0), 1, 8, false);
	}

	return canvas;
}

cv::Mat Card::as_mat_with_results()
{
	cv::Mat results_mat = results_to_mat();
	cv::Mat final(mat.rows + results_mat.rows, max(mat.cols, results_mat.cols), CV_8UC3);
	mat.copyTo(final(cv::Rect(0, 0, mat.cols, mat.rows)));
	results_mat.copyTo(final(cv::Rect(0, mat.rows, results_mat.cols, results_mat.rows)));

	//Show regions searched on output window
	cv::rectangle(final, Card::TOP_CORNER_RECT, Card::LINE_COLOUR);
	cv::rectangle(final, Card::BOTTOM_CORNER_RECT, Card::LINE_COLOUR);
	cv::rectangle(final, _last_aabb, Card::LINE_COLOUR_ALT);
	cv::rectangle(final, _rank_aabb, Card::LINE_COLOUR_ALT);

	return final;
}

/**
 * NOTE: Assumes constant dims à la GridLayout
 */
void show_cascade(vector<Card> cards)
{
	if (cards.size() < 1)
		return;

	// Max 6 cards per row; could put this elsewhere as a constant/define/static/global
	int cards_per_row = 6;

	for (size_t i = 0; i < cards.size(); ++i)
		cards[i].mat = cards[i].as_mat_with_results();

	cv::Mat final((cards.size()/cards_per_row + 1) * cards[0].mat.rows, min(cards.size() * cards[0].mat.cols, cards_per_row * cards[0].mat.cols), CV_8UC3);

	for (size_t i = 0; i < cards.size(); ++i)
		cards[i].mat.copyTo(final(cv::Rect(cards[0].mat.cols * (i%cards_per_row), cards[0].mat.rows * (i/cards_per_row), cards[i].mat.cols, cards[i].mat.rows)));

	//Finally
	stringstream title_stream;
	title_stream << WINDOW_TITLE;
	cv::imshow(title_stream.str(), final);

	//Nice CMD summary
	cout << endl << "============== Detected Cards ==============" << endl << endl;

	for(size_t i = 0; i < cards.size(); i++)
	{
		cout << "Card " << (i + 1) << ": The ";

		if(cards[i].is_picture_card)
		{
			switch(cards[i].detected_rank)
			{
			case Card::RANK_JACK:
				cout << "Jack";
				break;
			case Card::RANK_QUEEN:
				cout << "Queen";
				break;
			case Card::RANK_KING:
				cout << "King";
				break;
			case Card::RANK_ACE:
				cout << "Ace";
				break;
			}
		}
		else
		{
			cout << cards[i].detected_value;
		}

		cout << " of ";

		switch(cards[i].detected_suit)
		{
		case Card::CLUBS:
			cout << "Clubs";
			break;
		case Card::DIAMONDS:
			cout << "Diamonds";
			break;
		case Card::HEARTS:
			cout << "Hearts";
			break;
		case Card::SPADES:
			cout << "Spades";
			break;
		}

		cout << endl;
	}

	cout << endl << "============================================" << endl << endl;
}