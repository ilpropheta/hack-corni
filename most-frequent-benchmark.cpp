#include <random>
#include <tuple>

static size_t FreqTable(const std::vector<int>& v)
{
    std::array<int, 100000+1> histogram{}; // we know i is within [0, 10^5]
    for (auto i : v)
      histogram[i]++;
    auto maxElem = std::max_element(begin(histogram), end(histogram));
    return std::distance(begin(histogram), maxElem);
}

std::vector<int> GenInput(size_t size)
{
  std::default_random_engine re;
  std::uniform_int_distribution id{1, 100000};
  std::vector<int> input(size);
  std::generate(begin(input), end(input), [&]{
    return id(re);
  });
  return input;
}

static void MeasureFreqTable(benchmark::State& state) 
{
  const auto input = GenInput(1000000);
  for (auto _ : state) 
  {
    const auto i = FreqTable(input);
    benchmark::DoNotOptimize(i);
  }
}
BENCHMARK(MeasureFreqTable);

static int Map(const std::vector<int>& v)
{
    std::map<int, int> histogram; 
    for (auto i : v)
      histogram[i]++;
    auto maxElem = std::max_element(begin(histogram), end(histogram), [](auto p1, auto p2){
        return std::tie(p1.second, p2.first) < std::tie(p2.second, p1.first);
    });
    return maxElem->first;
}

static void MeasureMap(benchmark::State& state) 
{
  const auto input = GenInput(1000000);
  for (auto _ : state) 
  {
    const auto i = Map(input);
    benchmark::DoNotOptimize(i);
  }
}
BENCHMARK(MeasureMap);


static int UnorderedMap(const std::vector<int>& v)
{
    std::unordered_map<int, int> histogram; 
    for (auto i : v)
      histogram[i]++;
    auto maxElem = std::max_element(begin(histogram), end(histogram), [](auto p1, auto p2){
        return std::tie(p1.second, p2.first) < std::tie(p2.second, p1.first);
    });
    return maxElem->first;
}

static void MeasureUnorderedMap(benchmark::State& state) 
{
  const auto input = GenInput(1000000);
  for (auto _ : state) 
  {
    const auto i = UnorderedMap(input);
    benchmark::DoNotOptimize(i);
  }
}
BENCHMARK(MeasureUnorderedMap);

static int Sort(std::vector<int>& v)
{
    std::sort(begin(v), end(v));
    
    auto head = begin(v);
    std::pair<int, int> maxv;
    while (head != end(v))
    {
        auto where = std::find_if_not(head, end(v), [&](auto i){
            return i == *head;
        });
        maxv = std::max(maxv, {*head, distance(head, where)}, [](auto p1, auto p2){
            return std::tie(p1.second, p2.first) < std::tie(p2.second, p1.first);
        });
        if (where == end(v))
            break;
        
        head = where;
    }
    
    return maxv.first;
}

static void MeasureSort(benchmark::State& state) 
{
  auto input = GenInput(1000000);
  for (auto _ : state) 
  {
    const auto i = Sort(input);
    benchmark::DoNotOptimize(i);
  }
}
BENCHMARK(MeasureSort);
