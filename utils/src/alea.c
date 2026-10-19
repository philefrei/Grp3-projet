/* alea.c : module fourni, ne pas modifier */
#include "alea.h"

static uint64_t etat = 0x123456789ABCDEFULL;

void randomInit(uint64_t graine)
{
    etat = graine;
}

static uint64_t random64Bits(void)
{
    uint64_t z = (etat += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

double randomReal(void)    /* dans [0 ; 1[ */
{
    return (random64Bits() >> 11) * (1.0 / 9007199254740992.0);
}

double randomBetween(double a, double b)    /* dans [a ; b[ */
{
    return a + (b - a) * randomReal();
}

int randomInt(int n)    /* dans {0, 1, ..., n-1} */
{
    uint64_t un = (uint64_t) n, seuil = (0 - un) % un, r;
    do
    {
        r = random64Bits();
    } while (r < seuil);
    return (int) (r % un);
}
