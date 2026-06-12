#include "BiList.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace ahrameev;

struct Pair
{
  std::string first;
  BiList<int> second;
};

int main()
{
  BiList<Pair> sequences;
  std::string name;

  while (std::cin >> name)
  {
    Pair p;
    p.first = name;

    while (std::cin.peek() != EOF && std::cin.peek() != '\n' && std::cin.peek() != '\r')
    {
      if (std::isspace(std::cin.peek()))
      {
        std::cin.get();
        continue;
      }

      std::string raw_number;
      while (std::cin.peek() != EOF && !std::isspace(std::cin.peek()))
      {
        raw_number += static_cast<char>(std::cin.get());
      }

      if (raw_number.empty())
      {
        continue;
      }

      long long value = 0;
      try
      {
        std::size_t pos = 0;
        value = std::stoll(raw_number, &pos);

        if (pos != raw_number.length())
        {
          std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
          return 1;
        }
      }
      catch (...)
      {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
      }

      if (value > std::numeric_limits<int>::max() ||
          value < std::numeric_limits<int>::min())
      {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
      }

      p.second.push_back(static_cast<int>(value));
    }

    if (std::cin.peek() == '\n' || std::cin.peek() == '\r')
    {
      std::cin.get();
    }

    sequences.push_back(p);
  }

  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  bool firstName = true;
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    if (!firstName) std::cout << " ";
    std::cout << (*it).first;
    firstName = false;
  }
  std::cout << "\n";

  bool more = true;
  bool printed = false;
  BiList<int> sums;

  struct IterState {
    LIter<int> current;
    LIter<int> end;
  };

  BiList<IterState> states;
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    states.push_back({(*it).second.begin(), (*it).second.end()});
  }

  while (more)
  {
    more = false;
    long long current_level_sum = 0;
    bool firstNum = true;

    for (auto it = states.begin(); it != states.end(); ++it)
    {
      if ((*it).current != (*it).end)
      {
        more = true;
        int value = *((*it).current);

        if (!firstNum) std::cout << " ";
        std::cout << value;
        firstNum = false;

        current_level_sum += value;
        if (current_level_sum > std::numeric_limits<int>::max() ||
            current_level_sum < std::numeric_limits<int>::min())
        {
          std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
          return 1;
        }

        ++((*it).current);
      }
    }

    if (more)
    {
      printed = true;
      std::cout << "\n";
      sums.push_back(static_cast<int>(current_level_sum));
    }
  }

  if (!printed)
  {
    std::cout << "0\n";
    return 0;
  }

  bool firstSum = true;
  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    if (!firstSum) std::cout << " ";
    std::cout << *it;
    firstSum = false;
  }
  std::cout << "\n";

  return 0;
}
