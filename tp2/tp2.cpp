/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas obligés de l'utiliser.
*/

#include <fstream>
#include "stock.h"


int main(int argc, const char** argv) {
    std::istream* entree;
    std::ifstream* entree_fichier = NULL;

    if(argc>1){
         entree = entree_fichier = new std::ifstream(argv[1]);
         if(entree->fail())
             std::cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << std::endl;
    } else {
         entree = &std::cin;
    }

    Stock stock;

    while(*entree){
        std::string commande;
        *entree >> commande;
        if(!(*entree)) break;
        if(commande=="recette"){
            std::string nomrecette;
            *entree >> nomrecette;
            Recette recette;
            *entree >> recette;
            stock.ajout_recette(nomrecette, recette);

        } else if(commande=="ajout") {
            std::string nomingredient;
            *entree >> nomingredient;
            int nombre = 0;
            *entree >> nombre;
            if( nombre <= 0) {
                std::cerr << "Erreur sur le nombre d'ingrédient." << std::endl;
                return -1;
            }
            std::string date;
            *entree >> date;

            stock.ajout(nomingredient, date, nombre);

        } else if(commande=="retrait") {
            std::string nomingredient;
            *entree >> nomingredient;
            std::cerr << "retrait " << nomingredient << std::endl;
            int nombre = 0;
            *entree >> nombre;
            if( nombre <= 0) {
                std::cerr << "Erreur sur le nombre d'ingrédient." << std::endl;
                return -1;
            }

            stock.retrait(nomingredient, nombre);

        } else if(commande=="recommandation") {
            std::string recommandation = stock.recommander_recette();
            if(!recommandation.empty()) {
                std::cout << recommandation << " ;" << std::endl;
            } else {
                std::cout << "Aucune recommandation disponible." << std::endl;
            }

        } else if(commande=="utilisation") {
            std::string nomrecette;
            *entree >> nomrecette;
            bool utilisation = stock.utiliser_recette(nomrecette);
            std::cout << nomrecette << " : " << (utilisation ? "1" : "0") << std::endl;

        } else if(commande=="affichage") {
            stock.affichage();
        } else {
            std::cerr << "commande \"" << commande << " non reconnue !" << std::endl; 
        }
    }
    delete entree_fichier;


    return 0;
}
