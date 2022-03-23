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
	MidiEncoder(Scale scale) : m_scale(scale) {};

	void encodeGenomeToMidi(const Genetic::Genome& genome, const std::string& path);

private:
	Scale m_scale;
	std::vector<unsigned char> midiInfo;

	unsigned char header[22]{0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
							0x00, 0x00, 0x00, 0x01, 0x00, 0x60, 0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x05, 0xff};

	unsigned char tail[14]{0x00, 0xff, 0x2f, 0x00};

};

// strm.write(reinterpret_cast<const char*>(&vec[0]), vec.size()*sizeof(T));




#endif // !OMEGA_MIDI