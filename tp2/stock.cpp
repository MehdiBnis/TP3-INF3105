/*
 * Code des méthodes de Stock et Recette
 * Mehdi Bennis - BENM80120206
 * INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
*/

#include "stock.h"

Stock::Stock() {}

void Recette::ajout_ingredient(const std::string &nomingredient, int quantite) {
    if (ingredients.contient(nomingredient)) {
        ingredients[nomingredient] += quantite;
    } else {
        ingredients[nomingredient] = quantite;
    }
}

const ArbreMap<std::string, int> &Recette::get_ingredients() const {
    return ingredients;
}


void Stock::ajout(std::string nomingredient, std::string date, int nombre) {
    inventaire[nomingredient][date] += nombre;
}

void Stock::retrait(std::string nomingredient, int nombre) {
    auto entrees = inventaire[nomingredient].debut();

    while (entrees) {
        if (nombre <= 0) break;
        if (entrees.valeur() <= nombre) {
            nombre -= entrees.valeur();
            entrees.valeur() = 0;
        } else {
            entrees.valeur() -= nombre;
            nombre = 0;
        }
        ++entrees;
    }
}

void Stock::affichage() {
    for (auto it = inventaire.debut(); it != inventaire.fin(); ++it) {
        std::cout << it.cle() << " ";
        int total = 0;
        auto v = it.valeur().debut();
        while (v) {
            total += v.valeur();
            ++v;
        }
        std::cout << total << std::endl;
    }
    std::cout << ";" << std::endl;
}

std::string Stock::dateExpiration(const Recette &recette) const {
    std::string date_proche;
    auto it = recette.get_ingredients().debut();

    while (it) {
        if (inventaire.contient(it.cle())) {
            auto sous_it = inventaire[it.cle()].debut();
            while (sous_it) {
                auto date = sous_it.cle();
                if (date_proche.empty() || datePlusProche(date, date_proche)) {
                    date_proche = date;
                }
                ++sous_it;
            }
        }
        ++it;
    }
    return date_proche;
}

bool Stock::datePlusProche(const std::string &date1, const std::string &date2) const {
    int annee1 = (date1[0] - '0') * 1000 + (date1[1] - '0') * 100 + (date1[2] - '0') * 10 + (date1[3] - '0');
    int mois1 = (date1[5] - '0') * 10 + (date1[6] - '0');
    int jour1 = (date1[8] - '0') * 10 + (date1[9] - '0');

    int annee2 = (date2[0] - '0') * 1000 + (date2[1] - '0') * 100 + (date2[2] - '0') * 10 + (date2[3] - '0');
    int mois2 = (date2[5] - '0') * 10 + (date2[6] - '0');
    int jour2 = (date2[8] - '0') * 10 + (date2[9] - '0');

    if (annee1 != annee2) {
        return annee1 < annee2;
    }
    if (mois1 != mois2) {
        return mois1 < mois2;
    }
    return jour1 < jour2;
}

bool Stock::realisable(const Recette &recette) const {
    for (auto it = recette.get_ingredients().debut(); it != recette.get_ingredients().fin(); ++it) {
        const auto& nomingredient = it.cle();
        int qte_necessaire = it.valeur();
        int qte_disponible = 0;

        if (!inventaire.contient(nomingredient)) {
            return false;
        }

        auto sous_it = inventaire[nomingredient].debut();

        while (sous_it) {
            qte_disponible += sous_it.valeur();
            if (qte_disponible >= qte_necessaire) break;
            ++sous_it;
        }

        if (qte_disponible < qte_necessaire) {
            return false;
        }
    }
    return true;
}

bool Stock::utilisation(const Recette &recette) {
    auto it = recette.get_ingredients().debut();
    while (it) {
        const auto& nomingredient = it.cle();
        int qte_necessaire = it.valeur();
        auto sous_it = inventaire[nomingredient].debut();

        while (sous_it) {
            if (qte_necessaire <= 0) break;

            if (sous_it.valeur() >= qte_necessaire) {
                sous_it.valeur() -= qte_necessaire;
                qte_necessaire = 0;
            } else {
                qte_necessaire -= sous_it.valeur();
                sous_it.valeur() = 0;
            }
            ++sous_it;
        }

        ++it;
    }
    return true;
}

void Stock::ajout_recette(const std::string &nomrecette, const Recette &recette) {
    Recette nouvelle_recette(nomrecette);
    for (auto it = recette.get_ingredients().debut(); it != recette.get_ingredients().fin(); ++it) {
        nouvelle_recette.ajout_ingredient(it.cle(), it.valeur());
    }
    recettes[nomrecette] = nouvelle_recette;
}

std::string Stock::recommander_recette() const {
    std::string date_proche;
    std::vector<std::string> recettes_recommandees;

    for (auto it = recettes.debut(); it != recettes.fin(); ++it) {
        const std::string& nomrecette = it.cle();
        const Recette& recette = it.valeur();

        if (!realisable(recette)) {
            continue;
        }

        std::string date = dateExpiration(recette);

        if (date_proche.empty() || date < date_proche) {
            date_proche = date;
            recettes_recommandees.clear();
            recettes_recommandees.push_back(nomrecette);
        } else if (date == date_proche) {
            recettes_recommandees.push_back(nomrecette);
        }
    }

    std::string resultat;
    for (const auto& recette : recettes_recommandees) {
        if (!resultat.empty()) {
            resultat += " ";
        }
        resultat += recette;
    }

    return resultat;
}

bool Stock::utiliser_recette(const std::string &nomrecette) {
    if (!recettes.contient(nomrecette)) return false;

    const Recette& recette = recettes[nomrecette];

    if (!realisable(recette)) return false;

    return utilisation(recette);
}


std::istream& operator >> (std::istream& is, Recette& recette){
    std::string chaine;
    int quantite;

    for(;;) {
        is >> chaine;
        if(chaine == ";")
            break;
        is >> quantite;

        if (recette.ingredients.contient(chaine)) {
            recette.ingredients[chaine] += quantite;
        } else {
            recette.ingredients[chaine] = quantite;
        }
    }
    return is;
}

