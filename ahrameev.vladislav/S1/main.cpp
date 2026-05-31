#include "BiList.h"

#include <iostream>
#include <string>
#include <limits>

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

    while (std::cin.peek() == ' ')
    {
      long long value;

      if (!(std::cin >> value))
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
    if (!firstName)
    {
      std::cout << " ";
    }

    std::cout << (*it).first;
    firstName = false;
  }

  std::cout << "\n";

  bool more = true;
  bool printed = false;

  BiList<int> sums;

  while (more)
  {
    more = false;
    int sum = 0;

    bool firstNum = true;

    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      BiList<int>& numbers = (*it).second;

      if (!numbers.empty())
      {
        more = true;

        int value = numbers.front();

        if (!firstNum)
        {
          std::cout << " ";
        }

        std::cout << value;
        firstNum = false;

        if (sum > std::numeric_limits<int>::max() - value)
        {
          std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
          return 1;
        }

        sum += value;

        numbers.pop_front();
      }
    }

    if (more)
    {
      printed = true;
      std::cout << "\n";
      sums.push_back(sum);
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
    if (!firstSum)
    {
      std::cout << " ";
    }

    std::cout << *it;
    firstSum = false;
  }

  std::cout << "\n";

  return 0;
}
