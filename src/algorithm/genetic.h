#ifndef OMEGA_GENETIC
#define OMEGA_GENETIC

#include <vector>
#include <map>
#include <array>
#include <tuple>
#include <functional>

constexpr uint16_t GENOME_LENGTH = 256;
constexpr uint8_t GENOMES_IN_POPULATION = 6;

struct Genetic
{
	using Genome = std::vector<bool>;
	using Population = std::array<Genome, GENOMES_IN_POPULATION>;
	using WeightedPopulation = std::map<int,Genome>;
	using FitnessFunc = std::function<int(Genome)>;

	static Genome generateGenome(size_t genomeLength);
	static Population generatePopulation(size_t populationSize, size_t genomeLength);
	static std::tuple<Genome, Genome> singlePointCrossover(Genome first, Genome second);
	static void mutation(Genome& genome, size_t num = 1, float probability = 0.5);
	static int populationFitness(Population population, FitnessFunc fitnessFunc);
	static std::tuple<Genome, Genome> selectionPair(Population population, FitnessFunc fitnessFunc);
	static WeightedPopulation generateWeightedDistribution(Population population, FitnessFunc fitnessFunc);
	static Population sortPopulation(Population population, FitnessFunc fitnessFunc, bool reversed = true);
};

#endif // !OMEGA_GENETIC