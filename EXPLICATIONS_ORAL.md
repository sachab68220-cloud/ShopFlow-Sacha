# ShopFlow — fiche pour l'oral

## 1. Le projet en une phrase
Un programme en C qui gère une boutique **en mémoire** : produits, clients, commandes et statistiques.
Tout est rangé dans des **tableaux dynamiques** (`malloc` / `realloc`), et tout est libéré à la fin (`detruireBoutique`).

## 2. L'organisation des fichiers (« architecture modulaire »)
| Fichier | Rôle |
|---|---|
| `types.h` | Toutes les structures (Date, Adresse, Produit, LigneCommande, Commande, Client, Boutique). Inclus par tout le monde. |
| `produits.c/.h` | Catalogue : afficher, ajouter, modifier, supprimer, rechercher (référence ou mot-clé) |
| `clients.c/.h` | Clients : liste, fiche avec historique, ajouter, modifier, supprimer |
| `commandes.c/.h` | Commandes : calcul des montants, créer, afficher, ajouter/supprimer une ligne, statut, annuler |
| `statistiques.c/.h` | Chiffre d'affaires, palmarès produits et clients, alertes de stock, stats par catégorie |
| `initialisation.c/.h` | Données de départ (8 produits, 5 clients, 6 commandes) + `detruireBoutique` |
| `utils.c/.h` | Petits outils : `color`, `VerifChiffres`, `lireEntier`, `lireReel`, `dateValide` |
| `Main.c` | Le menu, et le mode démonstration |

**Pourquoi des `.h` ?** Le `.h` contient seulement les **prototypes** (la « carte de visite » de la fonction). Un autre fichier qui fait `#include "produits.h"` peut alors appeler les fonctions de `produits.c`. On n'inclut **jamais** un `.c`.
**`#pragma once`** : empêche qu'un même `.h` soit inclus deux fois.

## 3. Comment les données sont rangées
```
Boutique
 ├── produits[]  (tableau dynamique de Produit)
 └── clients[]   (tableau dynamique de Client)
      └── commandes[]   (chaque client a son tableau de commandes)
           └── lignes[] (chaque commande a son tableau de lignes)
```
C'est les **4 niveaux d'allocation dynamique** demandés : Boutique → Client → Commande → LigneCommande.

Pour chaque tableau on garde **deux compteurs** :
- `nbXxx` = combien de cases sont **remplies**
- `capaciteXxx` = combien de cases sont **réservées**

Quand `nb == capacite`, l'étagère est pleine : on fait `realloc` pour **ajouter 5 cases**.
On passe par un **pointeur temporaire** (`nouveauTableau`) : si `realloc` renvoie `NULL`, on n'a pas perdu l'ancien tableau.

## 4. Les règles métier et où elles sont codées
| Règle | Où |
|---|---|
| Remise 0 / 5 / 10 / 15 % selon le montant brut HT | `calculCommande` (if / else if dans l'ordre) |
| TVA 20 % sur le HT **après** remise | `calculCommande` |
| Port 5,90 € offert dès 60 € brut HT | `calculCommande` |
| 1 point de fidélité par tranche de 10 € TTC | `rattacherCommande` : `(int)(montantTTC / 10)` coupe les décimales |
| Points retirés si on annule | `annulerCommande` |
| Statuts EN_COURS → EXPEDIEE → LIVREE, sans retour | `changerStatut` |
| Annuler seulement si EN_COURS, et le stock revient | `annulerCommande` + `remettreStock` |
| Ajouter / supprimer une ligne seulement si EN_COURS | `ajouterLigneCommande`, `supprimerLigneCommande` |
| Pas deux lignes pour le même produit | `saisirLigne` : si le produit est déjà là, on **augmente la quantité** |
| Contrôle du stock sur la quantité cumulée | `saisirLigne` retire le stock **tout de suite**, donc la 2ᵉ fois on compare au stock déjà diminué |
| Suppression **logique** (`actif = 0`) | `supprimerProduit`, `supprimerClient` |
| Refuser de supprimer un produit présent dans une commande non annulée | `produitDansCommande` |
| Refuser de supprimer un client avec des commandes non annulées | `clientACommandeActive` |
| Unicité référence / email, même sur les supprimés | boucles avec `strcmp` sur **tout** le tableau |
| Numéros de commande et ID client automatiques | `numeroSuivant`, `idClientSuivant` dans la Boutique |

**Exemple de calcul à savoir refaire** (commande 1001) :
brut = 79,90 + 2×24,50 + 3×9,90 = **158,60** → palier 100-300 → **5 %** → remise 7,93
HT = 150,67 → TVA = 30,13 → port 0 (≥ 60) → **TTC = 180,80** → **18 points**.

## 5. Les fonctions importantes expliquées simplement
- **`rechercherProduit` / `rechercheClient`** : boucle sur le tableau, `return i` (la **case**) si trouvé, `return -1` sinon. Après chaque appel on teste `-1`.
- **`indexProduit` / `positionClient`** : la même chose mais **sans rien afficher** (utilisées dans les commandes).
- **`trouverCommande(b, numero)`** : double boucle (clients puis leurs commandes). Renvoie **l'adresse** de la commande (`&b->clients[i].commandes[j]`) ou `NULL`, comme `malloc`. Avec cette adresse on modifie la vraie commande.
- **`creerCommande`** : client → date (vérifiée par `dateValide`) → `malloc` des lignes → boucle « ajouter un autre produit ? » → si 0 ligne on `free` et on annule → `rattacherCommande` → `numeroSuivant + 1`.
- **`supprimerLigneCommande`** : on remet le stock, puis on **décale** les lignes suivantes d'une case vers la gauche pour boucher le trou, puis `nbLignes - 1`.
- **Palmarès** : on calcule un nombre par produit (ou client) dans un tableau `malloc`, puis on cherche **le plus grand**, on l'affiche, on le met à **-1** pour ne pas le reprendre, et on recommence (5 fois pour le top 5). À la fin, `free`.
- **`lireEntier`** : si l'utilisateur tape des lettres, `scanf` renvoie 0. On vide la saisie avec `while (getchar() != '\n');` et on redemande → le programme ne part jamais en boucle infinie.
- **`dateValide`** : tableau des jours par mois, février à 29 si l'année est bissextile (`annee % 4 == 0` et pas `% 100`, sauf `% 400`).
- **`detruireBoutique`** : libère **du plus petit au plus grand** : les lignes de chaque commande, puis les commandes de chaque client, puis les clients, puis les produits. Si on libérait d'abord les clients, on perdrait l'adresse de leurs commandes → fuite mémoire.

## 6. Les deux fonctionnalités créatives
1. **Statistiques par catégorie** (menu Statistiques → 5) : pour chaque catégorie, nombre de produits, articles vendus, ventes HT et valeur du stock. Astuce : on saute un produit si un produit **avant lui** a déjà la même catégorie, comme ça chaque catégorie n'est affichée qu'une fois.
2. **Mode démonstration** (menu principal → 5) : enchaîne automatiquement catalogue, clients, 3 commandes (remise 5 %, remise 15 %, frais de port), palmarès, alertes et catégories. Pratique pour montrer tout le projet en 1 minute.

## 7. Les données de test (exigées par le sujet)
- 8 produits, 4 catégories, **2 en alerte** (Souris 8 ≤ 15, Hub USB-C 4 ≤ 10)
- 5 clients
- 6 commandes : **LIVREE** (1001, 1002), **EXPEDIEE** (1003, > 600 € → 15 %), **EN_COURS** (1004 avec frais de port, 1006), **ANNULEE** (1005). Deux commandes ont 3 lignes.

## 8. Questions probables du prof
- **Différence entre `.` et `->` ?** `.` sur une structure, `->` sur un **pointeur** vers une structure (`b->nbProduits` = `(*b).nbProduits`).
- **Pourquoi `Boutique* b` en paramètre ?** Pour modifier la **vraie** boutique et pas une copie.
- **Pourquoi `&` dans `scanf("%d", &x)` mais pas pour un texte ?** Un tableau de `char` est déjà une adresse.
- **Pourquoi `strcmp` / `strcpy` ?** On ne peut pas comparer ou copier des textes avec `==` ou `=`.
- **Pourquoi `" %59[^\n]"` ?** Lire un texte **avec des espaces** jusqu'à Entrée ; l'espace au début saute le Entrée restant ; 59 = taille − 1 (place du `\0`).
- **Que se passe-t-il si `malloc` renvoie NULL ?** On affiche une erreur et on arrête proprement (`exit(1)` à l'initialisation, `return` ailleurs).
- **Pourquoi recopier le prix et le nom dans la ligne de commande ?** Pour que la commande garde le prix **du jour de la vente**, même si on modifie le produit après.
- **Pourquoi les stocks ne baissent pas avec les commandes de test ?** Ce sont des ventes passées : les stocks de départ sont déjà « après » ces ventes.
