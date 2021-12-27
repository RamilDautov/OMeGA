#ifndef OMEGA_SCALE
#define OMEGA_SCALE

#include <string>
#include <array>
#include <SDL2/SDL.h>

enum ScaleType
{
	MAJOR,
	MINOR,
	DORIAN,
	PHRYGIAN,
	LYDIAN,
	MIXOLYDIAN,
	MAJOR_BLUES,
	MINOR_BLUES
};

class Scale
{
public:
	using scale_t = std::array<float, 14>;

	Scale(ScaleType scaleType, std::string Key) : m_scaleType(scaleType), m_key(Key) {};

	scale_t getScale() { return m_scale; };
	void setScheme(ScaleType scaleType);
	void constructScale();
private:
	ScaleType m_scaleType;
	std::string m_key;

	// two octaves -> 14 notes
	scale_t m_scale;
	std::array<int, 14> m_scheme;
};
#endif // !OMEGA_SCALE

