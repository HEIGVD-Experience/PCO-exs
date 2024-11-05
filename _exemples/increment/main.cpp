/** @file main.cpp
 *  @brief Tentative de protocole d'exclusion mutuelle
 *
 *  Cet exemple illustre une tentative de mettre en place de l'exclusion
 *  mutuelle par attente active. Il ne fonctionne effectivement pas
 *  correctement.
 *
 *  @author Yann Thoma
 *  @date 17.02.2017
 *  @bug Pas de bug connu
 */

#include <iostream>
#include <pcosynchro/pcothread.h>

static int counter = 0;
const int NB_ITERATIONS = 1000000;

void run() {
    for(int i = 0; i < NB_ITERATIONS; i++) {
        counter = counter + 1;
    }
}

int main(int /*argc*/, char* /*argv*/[])
{
    std::vector<PcoThread*> threads;
    for(int i = 0; i < 2; i++)
        threads.push_back(new PcoThread(run));
    for(size_t i = 0; i < 2; i++)
        threads[i]->join();

    std::cout << "Fin des taches : counter = " << counter
              << " (" << 2 * NB_ITERATIONS << ")" << std::endl;

    return 0;
}
