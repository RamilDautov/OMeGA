#include "algorithm/genetic.h"
#include <spdlog/spdlog.h>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <thread>

using namespace GeneticAlgorithm;

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

    std::merge(first.begin(), first.begin() + p, second.begin()+p+1, second.end(), std::back_inserter(a));
    std::merge(second.begin(), second.begin() + p, first.begin()+p+1, first.end(), std::back_inserter(b));
    spdlog::info("Crossover passed");
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

int Genetic::populationFitness(Population population, FitnessFunc fitnessFunc)
{
    int sum = 0;

    for (size_t i = 0; i < population.size(); ++i)
    {
        auto genome = population[i];
        sum += fitnessFunc(genome);
    }

    return sum;
}

std::tuple<Genome, Genome> Genetic::selectionPair(Population population, FitnessFunc fitnessFunc)
{
    std::array<Genome, 2> result{};

    int sumWeights = 0;

    auto weightedPopulation = generateWeightedDistribution(population, fitnessFunc);
    for (const auto& [weight, _] : weightedPopulation)
        sumWeights += weight;

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  randInt(0, sumWeights);

    for (size_t i = 0; i < 2; ++i)
    {
        int rnd = randInt(generator);
        for (const auto& [weight, gene] : weightedPopulation)
        {
            if (rnd < weight)
            {
                result[i] = gene;
                break;
            }
            rnd -= weight;
        }
    }
    spdlog::info("Selection pair returned");
    return std::make_tuple(result[0], result[1]);
}

WeightedPopulation Genetic::generateWeightedDistribution(Population population, FitnessFunc fitnessFunc)
{
    WeightedPopulation wp{};

    for (size_t i = 0; i < population.size(); ++i)
    {
        auto gene = population[i];
        int weight = fitnessFunc(gene);
        wp[weight] = gene;
    }
    spdlog::info("Weighted population generated");
    return wp;
}

Population Genetic::sortPopulation(Population population, FitnessFunc fitnessFunc, bool reversed)
{
    Population result{};

    WeightedPopulation wp = generateWeightedDistribution(population, fitnessFunc);

    // size should be equal
    /*if (wp.size() != result.size())
        throw std::runtime_error("Genetic::sortPopulation error: size discrepancy");*/

    if (reversed)
    {
        for (auto it = wp.rbegin(); it != wp.rend(); ++it)
            result.push_back(it->second);
    }
    else
    {
        for (auto it = wp.begin(); it != wp.end(); ++it)
            result.push_back(it->second);
    }
    spdlog::info("Population sorted");
    return result;
}

Population Genetic::runEvolution(FitnessFunc fitnessFunc, uint32_t fitnessLimit, uint32_t generationLimit)
{
    spdlog::info("!!! run evolution 1");
    Population population = generatePopulation();

    for (size_t genNum = 0; genNum < generationLimit; ++genNum)
    {
        Population nextGeneration;
        spdlog::info("!!! run evolution");
        Population sortedPopulation = sortPopulation(population, fitnessFunc);

        if (fitnessFunc(population[0]) >= fitnessLimit)
            break;

        nextGeneration.push_back(sortedPopulation[0]);
        nextGeneration.push_back(sortedPopulation[1]);

        for (size_t i = 0; i < static_cast<size_t>((sortedPopulation.size() / 2) - 1); ++i)
        {
            auto parents = selectionPair(sortedPopulation, fitnessFunc);

            Genome offspringA, offspringB;

            std::tie(offspringA, offspringB) = singlePointCrossover(std::get<0>(parents), std::get<1>(parents));

            mutation(offspringA);
            mutation(offspringB);

            nextGeneration.push_back(offspringA);
            nextGeneration.push_back(offspringB);
        }
        population = nextGeneration;
    }
    spdlog::info("Evolution is finished");

    
    return population;
}