/*
 * Structures de Recette et Stock
 * Mehdi Bennis - BENM80120206
 * INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
*/
#ifndef STOCK_H
#define STOCK_H

#include <iostream>

#include "arbremap.h"
#include <vector>


class Recette {
public:
    Recette() : nomrecette(""){}
    Recette(const std::string& nom) : nomrecette(nom){}
    Recette(const Recette& autre) : nomrecette(autre.nomrecette), ingredients(autre.ingredients) {}

    void ajout_ingredient(const std::string& nomingredient, int quantite);
    const ArbreMap<std::string, int>& get_ingredients() const;

    friend std::istream& operator >> (std::istream& is, Recette& recette);

private:
    std::string nomrecette;
    ArbreMap<std::string, int> ingredients;
};


class Stock
{

public:
    Stock();

    void ajout(std::string nomingredient, std::string date, int nombre);

    void retrait(std::string nomingredient, int nombre);

    void affichage();

    std::string dateExpiration(const Recette& recette) const;

    bool datePlusProche(const std::string& date1, const std::string& date2) const;

    bool realisable(const Recette& recette) const;

    bool utilisation(const Recette& recette);

    void ajout_recette(const std::string& nomrecette, const Recette& recette);

    std::string recommander_recette() const;

    bool utiliser_recette(const std::string& nomrecette);

private:
    ArbreMap<std::string, ArbreMap<std::string, int>> inventaire;
    ArbreMap<std::string, Recette> recettes;
};




#endif // STOCK_H