
// Exemple : tableaux de taille variables avec malloc et free

#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int* tableau;
    int i;

    tableau = (int*) malloc (5 * sizeof(int));

    if (tableau == NULL)
        cout << "pas assez de mémoire" << endl;

    else {

        	for (i = 0; i < 5; i++)  tableau[i] = i * i;
        	for (i = 0; i < 5; i++)  cout << " " << tableau[i] << " ";
        	cout << endl;
        	free(tableau);
    }

    tableau = (int*) malloc (10 * sizeof(int));
    
    if (tableau == NULL)
        cout << "pas assez de mémoire" << endl;

    else {
        	for (i = 0; i < 10; i++)  tableau[i] = i * i;
        	for (i = 0; i < 10; i++)  cout << " " << tableau[i] << " ";
        	cout << endl;
        	free(tableau);
    }

    return 0;
}