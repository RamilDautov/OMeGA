#ifndef OMEGA_MIDI
#define OMEGA_MIDI

// https://intuitive-theory.com/midi-from-scratch/

#include <fstream>
#include <vector>
#include "algorithm/genetic.h"
#include "scale.h"

class MidiEncoder
{
public:
	MidiEncoder(Scale scale, uint8_t tempo);

	void encodeGenomeToMidi(const GeneticAlgorithm::Genome& genome, const std::string& path);
	void writeGenomeToMidiBytes(const GeneticAlgorithm::Genome& genome, std::vector<unsigned char>& midiBytes);

private:
	Scale m_scale;
	std::vector<unsigned char> m_tempoBytes; // tempo is encoded into 3 bytes

	// initialize with midi header
	std::vector<unsigned char> m_midiBytes{
		0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
		0x00, 0x01, 0x00, 0x02, 0x00, 0x60, 0x4D, 0x54,
		0x72, 0x6B, 0x00, 0x00, 0x00, 0x13, 0x00, 0xff,
		0x51
	};
};

// strm.write(reinterpret_cast<const char*>(&vec[0]), vec.size()*sizeof(T));




#endif // !OMEGA_MIDI