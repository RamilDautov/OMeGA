#include "algorithm/genetic.h"
#include <spdlog/spdlog.h>
#include <random>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <iterator>
#include <thread>

using namespace GeneticAlgorithm;

static WeightedPopulation g_weightedPopulation{};
static uint16_t g_maxWeight;

Genome Genetic::generateGenome(size_t genomeLength)
{
    Genome result(genomeLength, 0);

    double p = 0.5; // we can control probability of 1

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::bernoulli_distribution d(p);

    for (size_t n = 0; n < genomeLength; ++n) {
        result[n] = d(generator);
    }

    return result;
}

Population Genetic::generatePopulation(size_t populationSize, size_t genomeLength)
{
    Population result(populationSize);

    for (size_t n = 0; n < populationSize; ++n)
    {
        result[n] = generateGenome(genomeLength);
    }
    spdlog::info("Population generated");
    return result;
}

std::tuple<Genome, Genome> Genetic::singlePointCrossover(Genome first, Genome second)
{
    if(first.size() != second.size())
        throw std::runtime_error("Genetic::singlePointCrossover error: Both genomes must have the same length");

    auto length = first.size();
    if (length < 2)
        return { first, second };

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, length - 1);

    auto p = distr(generator);

    Genome a;
    Genome b;

    std::merge(first.begin(), first.begin() + p, second.begin()+p, second.end(), std::back_inserter(a));
    std::merge(second.begin(), second.begin() + p, first.begin()+p, first.end(), std::back_inserter(b));
    spdlog::info("Crossover passed: a = {}, b = {}", a.size(), b.size());
    return std::make_tuple( a, b );
}

void Genetic::mutation(Genome& genome, size_t num, float probability)
{
    auto length = genome.size();

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<> randDouble(0, 1);
    std::uniform_int_distribution<int>  randInt(0, length - 1);

    for (size_t i = 0; i < num; ++i)
    {
        auto p = randDouble(generator);
        auto idx = randInt(generator);

        if (p < probability)
            genome[idx] = static_cast<bool>(genome[idx]) ^ 1;
    }
	spdlog::info("Mutation passed");
}

int Genetic::populationFitness()
{
    int sum = 0;

    for(const auto& [weight, _] : g_weightedPopulation)
		sum += weight;

    return sum;
}

std::tuple<Genome, Genome> Genetic::selectionPair(const Population& population)
{
    std::array<Genome, 2> result{};

    int sumWeights = populationFitness();

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  randInt(0, sumWeights);

    for (size_t i = 0; i < 2; ++i)
    {
        int rnd = randInt(generator);
        for (const auto& [geneIdx, weight] : g_weightedPopulation)
        {
            if (rnd < weight)
            {
				result[i] = population[geneIdx];
                break;
            }
            rnd -= weight;
        }
    }
    spdlog::info("Selection pair returned");
    return std::make_tuple(result[0], result[1]);
}

void Genetic::generateWeightedDistribution(const Population& population, FitnessFunc fitnessFunc)
{
    for (size_t i = 0; i < population.size(); ++i)
    {
        auto gene = population[i];
        int weight = fitnessFunc(gene);
        g_weightedPopulation[i] = std::make_pair(i, weight);
    }

    // sorted in ascending order
	std::sort(g_weightedPopulation.begin(), g_weightedPopulation.end(), [&](auto& a, auto& b) { return a.second < b.second; });

	spdlog::info("Weighted population generated.");
}

Population Genetic::sortPopulation(const Population& population, FitnessFunc fitnessFunc, bool reversed)
{
    Population result{};

    generateWeightedDistribution(population, fitnessFunc);

    // size should be equal
    /*if (wp.size() != result.size())
        throw std::runtime_error("Genetic::sortPopulation error: size discrepancy");*/

    if (reversed)
    {
        // descending order
		for(auto it = g_weightedPopulation.rbegin(); it != g_weightedPopulation.rend(); ++it)
            result.push_back(population[it->first]);
    }
    else
    {
		// ascending order
		for(auto it = g_weightedPopulation.begin(); it != g_weightedPopulation.end(); ++it)
			result.push_back(population[it->first]);
    }

	g_maxWeight = g_weightedPopulation.rbegin()->second;
	spdlog::info("Population sorted. Max weight: {}", g_maxWeight);

    return result;
}

uint16_t Genetic::getMaxWeight()
{
	return g_maxWeight;
}

//Population Genetic::runEvolution(FitnessFunc fitnessFunc, uint32_t fitnessLimit, uint32_t generationLimit)
//{
//    Population population = generatePopulation();
//
//    for (size_t genNum = 0; genNum < generationLimit; ++genNum)
//    {
//        Population nextGeneration;
//        Population sortedPopulation = sortPopulation(population, fitnessFunc);
//
//        if(g_maxWeight >= fitnessLimit)
//            break;
//
//        nextGeneration.push_back(sortedPopulation[0]);
//        nextGeneration.push_back(sortedPopulation[1]);
//
//        for (size_t i = 0; i < static_cast<size_t>((sortedPopulation.size() / 2) - 1); ++i)
//        {
//            auto parents = selectionPair(sortedPopulation);
//
//            Genome offspringA, offspringB;
//
//            try
//			{
//				std::tie(offspringA, offspringB) = singlePointCrossover(std::get<0>(parents), std::get<1>(parents));
//
//                mutation(offspringA);
//				mutation(offspringB);
//
//				nextGeneration.push_back(offspringA);
//				nextGeneration.push_back(offspringB);
//			}
//			catch(std::exception& e)
//			{
//				spdlog::error("Exception: {}", e.what());
//            }
//
//            
//        }
//        population = nextGeneration;
//    }
//    spdlog::info("Evolution is finished");
//
//    
//    return population;
//}