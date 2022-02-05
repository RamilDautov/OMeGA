#include "genetic.h"

#include <random>
#include <stdexcept>
#include <algorithm>
#include <iterator>

using Genome = Genetic::Genome;
using Population = Genetic::Population;
using WeightedPopulation = Genetic::WeightedPopulation;
using FitnessFunc = Genetic::FitnessFunc;

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
    Population result{};

    for (size_t n = 0; n < populationSize; ++n)
    {
        result[n] = generateGenome(genomeLength);
    }

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

    return wp;
}

Population Genetic::sortPopulation(Population population, FitnessFunc fitnessFunc, bool reversed)
{
    Population result{};

    WeightedPopulation wp = generateWeightedDistribution(population, fitnessFunc);

    // size should be equal
    if (wp.size() != result.size())
        throw std::runtime_error("Genetic::sortPopulation error: size discrepancy");

    int i = 0;

    if (reversed)
    {
        for (auto it = wp.rbegin(); it != wp.rend(); ++it)
            result[i++] = it->second;
    }
    else
    {
        for (auto it = wp.begin(); it != wp.end(); ++it)
            result[i++] = it->second;
    }

    return result;
}