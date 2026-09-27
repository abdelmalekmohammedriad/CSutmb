# include <iostream>
# include <iomanip>
# include <fstream>
# include <list>
# include <vector>

using namespace std;

int main() {

   unsigned n; // le nombre de Sommets du graphe

   ifstream fichier1("Graphe_In.txt"); // ouverture du fichier en lecture
   ofstream fichier2("Graphe_out.txt"); // ouverture du fichier en ecriture

   if (!fichier1.fail()) // si l'ouverture a réussi
   {
      if (fichier1) fichier1 >> n;

      vector < list < int >> Graphe(n), Graphe_Inverse(n);

      fichier2 << n;

      int org, des;
      while (!fichier1.eof()) {
         fichier1 >> org >> des;
         Graphe[org].push_back(des);
         Graphe_Inverse[des].push_back(org);
         fichier2 << endl << des << " " << org;
      }

      //      tri des listes d'adjacences et éliminations des doublons
      for (int i = 0; i < n; i++) {
         Graphe[i].sort();
         Graphe[i].unique();
         Graphe_Inverse[i].sort();
         Graphe_Inverse[i].unique();
      }

      //      fermeture des fichiers
      fichier1.close();
      fichier2.close();

      list < int > ::iterator iter;
      int compteur1 = 0;

      //  impression sur ecran des listes d'adjacences du graphe initial
      cout << "Le Graphe représenter par les listes d'adjacence :\n\n";
      for (int i = 0; i < n; ++i) {
         
         cout << "\t" << setw(2) << i << " : ";

         for (iter = Graphe[i].begin(); iter != Graphe[i].end(); ++iter) {
            cout << "" << setw(2) << * iter << " ";
            compteur1++;
         }
         cout << endl;
      }

      cout << "\nNombre d'arc      : " << compteur1 << "\n";
      cout << "Nombre de sommets : " << size(Graphe) << "\n\n";

      //  impression sur ecran des listes d'adjacences du graphe transposé
      cout << "Le Graphe inverse représenter par les listes d'adjacence :\n\n";
      for (int i = 0; i < n; ++i) {
         cout << "\t" << setw(2) << i << " : ";

         for (iter = Graphe_Inverse[i].begin(); iter != Graphe_Inverse[i].end(); ++iter) {
            cout << "" << setw(2) << * iter << "" << " ";

            compteur1++;
         }
         cout << "" << endl;
      }

   } else {
              // on cas d'erreur d'ouverture du fichier
              cerr << "Impossible d'ouvrir le fichier !" << endl;
              return -1;
          }
return 0;
}