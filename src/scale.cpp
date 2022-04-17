#include "scale.h"
#include <unordered_map>
#include <cmath>
#include <spdlog/spdlog.h>

using namespace std;

// mapping notes to bytes of MIDI protocol
static unordered_map<string, unsigned char> g_keyboardLayout = {
	{"C",  0x3c},
	{"C#", 0x3d},
	{"Db", 0x3e},
	{"D",  0x3f},
	{"D#", 0x40},
	{"Eb", 0x41},
	{"E",  0x42},
	{"F",  0x43},
	{"F#", 0x44},
	{"Gb", 0x45},
	{"G",  0x46},
	{"G#", 0x47},
	{"Ab", 0x48},
	{"A",  0x49},
	{"A#", 0x4a},
	{"Bb", 0x4b},
	{"B",  0x4c}
};

Scale::Scale(ScaleType scaleType, const std::string& Key) : m_scaleType(scaleType), m_key(Key)
{
	setScheme(scaleType);
	constructScale();
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
			spdlog::info("Major scale scheme is initialized");
		}
		break;
		case ScaleType::MINOR:
		{
			// minor formula: {whole, half, whole, whole, half, whole, whole}
			m_scheme = { 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22 };
			spdlog::info("Minor scale scheme is initialized");
		}
		break;
		case ScaleType::DORIAN:
		{
			// dorian formula: {whole, half, whole, whole, whole, half, whole}
			m_scheme = { 0, 2, 3, 5, 7, 9, 10, 12, 14, 15, 17, 19, 21, 22 };
			spdlog::info("Dorian scale scheme is initialized");
		}
		break;
		case ScaleType::PHRYGIAN:
		{
			// phrygian formula: {half, whole, whole, whole, half, whole, whole}
			m_scheme = { 0, 1, 3, 5, 7, 8, 10, 12, 13, 15, 17, 19, 20, 22 };
			spdlog::info("Phrygian scale scheme is initialized");
		}
		break;
		case ScaleType::LYDIAN:
		{
			// lydian formula: {whole, whole, whole, half, whole, whole, half}
			m_scheme = { 0, 2, 4, 6, 7, 9, 10, 11, 13, 15, 17, 18, 20, 22 };
			spdlog::info("Lydian scale scheme is initialized");
		}
		break;
		case ScaleType::MIXOLYDIAN:
		{
			// mixolydian formula: {whole, whole, half, whole, whole, half, whole,}
			m_scheme = { 0, 2, 4, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22 };
			spdlog::info("Mixolydian scale scheme is initialized");
		}
		break;
		case ScaleType::MAJOR_BLUES:
		{
			// major blues formula: {whole, half, half, 3*half, whole, 3*half}
			m_scheme = { 0, 2, 3, 4, 7, 9, 12, 14, 15, 16, 19, 21, 24, 26 };
			spdlog::info("Major blues scale scheme is initialized");
		}
		break;
		case ScaleType::MINOR_BLUES:
		{
			// minor blues formula: {3*half, whole, half, half, 3*half, whole}
			m_scheme = { 0, 3, 5, 6, 7, 10, 12, 15, 17, 18, 19, 22, 24, 27 };
			spdlog::info("Minor blues scale scheme is initialized");
		}
		break;
		default:
			break;
	}
}

void Scale::constructScale()
{
	auto root = g_keyboardLayout.at(m_key);

	for (size_t i = 0; i < 14; ++i)
		m_scale[i] = static_cast<unsigned char>(m_scheme[i] + root);
}