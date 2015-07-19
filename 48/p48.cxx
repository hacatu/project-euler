#include <iostream>
int main()
{
  unsigned long long sol = 0;
  for (unsigned u = 1; u <= 1000; ++u)
    {
      unsigned long long tmp = u;
      for (unsigned v = 1; v < u; ++v)
        (tmp *= u) %= 10000000000ULL;
      sol += tmp;
    }
  std::cout << "The last ten digits of the "
            << "series, 1^1 + 2^2 + 3^3 + "
            << "... + 1000^1000 are: "
            << sol % 10000000000ULL << '\n';
}
