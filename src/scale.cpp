#include "scale.h"
#include <unordered_map>
#include <cmath>

using namespace std;

// mapping notes to frequencies
static unordered_map<string, float> keyboard_layout = {
	{"C",  523.25},
	{"C#", 554.37},
	{"Db", 554.37},
	{"D",  587.33},
	{"D#", 622.25},
	{"Eb", 622.25},
	{"E",  659.25},
	{"F",  698.46},
	{"F#", 739.99},
	{"Gb", 739.99},
	{"G",  783.99},
	{"G#", 830.61},
	{"Ab", 830.61},
	{"A",  880.00},
	{"A#", 932.33},
	{"Bb", 932.33},
	{"B",  987.77}
};

static float calculteFrequency(uint16_t idx, float root)
{
	return root * pow(2, float(idx) / 12);
}

void Scale::setScheme(ScaleType scaleType)
{
	switch(m_scaleType)
	{
	case ScaleType::MAJOR:
		{
			// major formula: {whole, whole, half, whole, whole, whole, half}
			// this pattern repeated 2 times because we work with 2 octaves
			m_scheme = { 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23 };
		}
		break;
		case ScaleType::MINOR:
		{
			// minor formula: {whole, half, whole, whole, half, whole, whole}
			m_scheme = { 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22 };
		}
		break;
		case ScaleType::DORIAN:
		{
			// dorian formula: {whole, half, whole, whole, whole, half, whole}
			m_scheme = { 0, 2, 3, 5, 7, 9, 10, 12, 14, 15, 17, 19, 21, 22 };
		}
		break;
		case ScaleType::PHRYGIAN:
		{
			// phrygian formula: {half, whole, whole, whole, half, whole, whole}
			m_scheme = { 0, 1, 3, 5, 7, 8, 10, 12, 13, 15, 17, 19, 20, 22 };
		}
		break;
		case ScaleType::LYDIAN:
		{
			// lydian formula: {whole, whole, whole, half, whole, whole, half}
			m_scheme = { 0, 2, 4, 6, 7, 9, 10, 11, 13, 15, 17, 18, 20, 22 };
		}
		break;
		case ScaleType::MIXOLYDIAN:
		{
			// mixolydian formula: {whole, whole, half, whole, whole, half, whole,}
			m_scheme = { 0, 2, 4, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22 };
		}
		break;
		case ScaleType::MAJOR_BLUES:
		{
			// major blues formula: {whole, half, half, 3*half, whole, 3*half}
			m_scheme = { 0, 2, 3, 4, 7, 9, 12, 14, 15, 16, 19, 21, 24, 26 };
		}
		break;
		case ScaleType::MINOR_BLUES:
		{
			// minor blues formula: {3*half, whole, half, half, 3*half, whole}
			m_scheme = { 0, 3, 5, 6, 7, 10, 12, 15, 17, 18, 19, 22, 24, 27 };
		}
		break;
		default:
			break;
	}
}

void Scale::constructScale()
{
	auto root = keyboard_layout.at(m_key);

	for (size_t i = 0; i < 14; ++i)
		m_scale[i] = calculteFrequency(m_scheme[i], root);
}