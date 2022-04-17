#include "midi.h"
#include <fstream>
#include <sstream>
#include <array>
#include <bitset>
#include <spdlog/spdlog.h>

using namespace GeneticAlgorithm;

static constexpr unsigned char defaultVelocity = 0x64; // default velocity of a note

static constexpr std::array<unsigned char, 34> g_headerPartAfterTempo = {
	0xe2, 0x00, 0xff, 0x58,
	0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xff, 0x2f,
	0x00, 0x4d, 0x54, 0x72, 0x6b, 0x00,
	0x00, 0x01,	0x11, // num of bytes
	0x00, 0xff, 0x03, 0x09, 0x53, 0x61, 0x6d,
	0x70, 0x6c, 0x65, 0x72, 0x20, 0x31
};

static constexpr std::array<unsigned char, 4> g_tailPart = { 0x00, 0xFF, 0x2F, 0x00 };

static std::vector<unsigned char> decimalTempoToBytes(uint8_t tempo)
{
	std::vector<unsigned char> arrayOfBytes(3);

	for (int i = 0; i < 3; i++)
		arrayOfBytes[2 - i] = (tempo >> (i * 8));

	return arrayOfBytes;
}

MidiEncoder::MidiEncoder(Scale scale, uint8_t tempo) : m_scale(scale), m_tempoBytes(decimalTempoToBytes(tempo))
{
}

void MidiEncoder::writeGenomeToMidiBytes(const Genome& genome, std::vector<unsigned char>& midiBytes)
{
	std::bitset<4> note{};
	uint8_t prevIdx{0};
	unsigned char velocity;
	auto scale = m_scale.getScale();

	size_t i = 0;
	size_t j = 0;

	for (const auto& bit : genome)
	{
		note[i] = bit;
		i = (i + 1) % 4;
		if (i == 0)
		{
			velocity = defaultVelocity;
			auto idx = note.to_ulong();

			if (idx == 0)
			{
				velocity = 0x00;
				idx = prevIdx;
			}

			if (idx == 15 && prevIdx != 0)
			{
				if (prevIdx != 0)
					idx = prevIdx;
				else
					idx = 1; // root note
			}

			// note on
			midiBytes.push_back(0x00);
			midiBytes.push_back(0x90);
			midiBytes.push_back(scale.at(idx - 1));
			midiBytes.push_back(velocity); // loudness of a note 13+8*64+4

			// note off
			midiBytes.push_back(0x60);
			midiBytes.push_back(0x80);
			midiBytes.push_back(scale.at(idx - 1));
			midiBytes.push_back(0x40);

			prevIdx = idx;
		}
	}
}

void MidiEncoder::encodeGenomeToMidi(const Genome& genome, const std::string& path)
{
	// write tempo
	/*for (const auto& byte : m_tempoBytes)
		m_midiBytes.push_back(byte);*/
	m_midiBytes.push_back(0x03);
	m_midiBytes.push_back(0x07);
	m_midiBytes.push_back(0x0a);

	// writing the part after tempo
	for (const auto& byte : g_headerPartAfterTempo)
		m_midiBytes.push_back(byte);

	// writing genome part
	writeGenomeToMidiBytes(genome, m_midiBytes);

	// writing the tail
	for (const auto& byte : g_tailPart)
		m_midiBytes.push_back(byte);

	//Writing to midi file
	std::ofstream midiFile;
	midiFile.open(path, std::ios::binary);

	midiFile.write(reinterpret_cast<const char*>(&m_midiBytes[0]), m_midiBytes.size() * sizeof(unsigned char));

	midiFile.close();
}