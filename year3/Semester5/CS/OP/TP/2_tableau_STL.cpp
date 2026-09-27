// Exemple : tableaux de taille variables avec vector de la STL

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int main()
{
    vector<int> tableau;

    for (unsigned i = 0; i < 5; i++)
        tableau.push_back(i * i);

    cout << " Size t = " << tableau.size() << endl;

    for (unsigned i = 0; i < tableau.size(); i++)
        cout << " " << tableau[i] << " ";

    cout << endl;

    tableau.clear();

    cout << " Size tableau = " << tableau.size() << endl;

    for (unsigned i = 0; i < 10; i++)
        tableau.push_back(i * i);

    cout << " Size tableau = " << tableau.size() << endl;

    for (unsigned i = 0; i < tableau.size(); i++)
        cout << " " << tableau[i] << " ";

    cout << endl;

    vector<int>::iterator it;

    for (it = tableau.begin(); it != tableau.end(); ++it)
        cout << " " << *it << " ";

    cout << endl;

    vector<int>::reverse_iterator rit;

    for (rit = tableau.rbegin(); rit != tableau.rend(); ++rit)
        cout << " " << *rit << " ";

    cout << endl;

    return 0;
}