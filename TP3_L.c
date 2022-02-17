/************************************************************
 *     __/\\\_____________________/\\\\\\\\\_               *
 *      _\/\\\__________________/\\\////////__              *
 *       _\/\\\________________/\\\/___________             *
 *        _\/\\\_______________/\\\_____________            *
 *         _\/\\\______________\/\\\_____________           *
 *          _\/\\\______________\//\\\____________          *
 *           _\/\\\_______________\///\\\__________         *
 *            _\/\\\\\\\\\\\\\\\_____\////\\\\\\\\\_        *
 *             _\///////////////_________\/////////__       *
 *                                                          *
 *                                                          *
 *      Auteur : Ludovic Corcos                             *
 *                                                          *
 *      Université Clermont Auvergne | L2 Informatique      *
 *                                                          *
 *      Date : 06/03/2020                                   *   
 *                                                          *
 *      Programme : TP3.c                                   *
 *                                                          *
 *      Description :                                       *
 *      Valeurs approchées du chiffre PI                    *   
 *      et visualisation statistiques                       *
 *                                                          *
 ************************************************************/

//  Résultats en fin de fichier

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "mt19937ar.c"  //  Code de Matsumoto

#define NB_EXP_TOT 1000
#define NB_EXP_PI 1000000

/************************************************************
 *                                                          *
 *                  Prototypes des fonctions                *
 *                                                          *
 ************************************************************/

double SimuPi (long int exp);

double MoyArithme(double *Tableau_Result, int nombre_ExpTot);

double Variance(double *Tableau_Result, double Moyenne_Arithmetique, int nombre_ExpTot);

double RayonConfiance(double Variance, int nombre_ExpTot);

double EcartType (double variance);

/************************************************************
 *                                                          *
 *                Fonction 'main' principale                *
 *                                                          *
 ************************************************************/

int main() {

    int i;
    long double sum = 0;

    //  Le tableau tab_result va nous permetre de calculer l'écart type de l'échantillion
    //  En effet, il contient la série statistique des nombres PI générés
    double tab_result[NB_EXP_TOT];

    //  Permet l'effaçage de l'écran
    system("clear");

    //  On execute ici une boucle pour calculer une approximation de Pi
    //  Et on affiche une chargement pour indiquer que le programme tourne toujours
    //  et ne subit pas de bugs
    for (i = 0; i < NB_EXP_TOT; i++){
        
        tab_result[i] = SimuPi(NB_EXP_PI);
        printf("\rChargement : %d %% ", (i+1)/10);
        fflush(stdout);
    }

    system("clear");


    //-----   Gestion de l'affichage dans le terminal   -----//

    printf("\e[1m");
    printf("\t\t\tEtape 1 :\n");
    printf("\tCalcul de la moyenne arithmétique\n\n");
    printf("\e[0m");

    double moyenneArith_pi = MoyArithme(tab_result, NB_EXP_TOT);
    printf ("Moyenne arithmétique de Pi = %f\n\n\n",  moyenneArith_pi);

    printf("\e[1m");
    printf("\t\t\tEtape 2 :\n");
    printf("\tCalcul de la variance S^2\n\n");
    printf("\e[0m");

    double variance_pi = Variance(tab_result, moyenneArith_pi, NB_EXP_TOT);
    printf ("Variance de Pi = %f\n\n\n",  variance_pi);

    printf("\e[1m");
    printf("\t\t\tEtape 3 :\n");
    printf("\tCalcul de l'Ecart-type\n\n");
    printf("\e[0m");

    double ecart_type_pi = EcartType(variance_pi);
    printf ("Ecart-type de Pi = %f\n\n\n",  ecart_type_pi);

    printf("\e[1m");
    printf("\t\t\tEtape 4 :\n");
    printf("\tCalcul du rayon de confiance de Pi pour alpha = 0.05\n\n");
    printf("\e[0m");

    double rayon_conf_pi = RayonConfiance(variance_pi, NB_EXP_TOT);
    printf ("Rayon de confiance de Pi = %f\n\n\n",  ecart_type_pi);
    
    printf("\e[1m");
    printf("\t\t\tEtape 5 :\n");
    printf("\tCalcul de l'intervalle de confiance de Pi à 95 %%\n\n");
    printf("\e[0m");

    printf("\nIntervalle de confiance de Pi = ");
    printf("[%f ; %f]\n\n\n", moyenneArith_pi - rayon_conf_pi, moyenneArith_pi + rayon_conf_pi);

    printf("On vient de générer %d fois une simulation de Pi avec %d nombres aléatoires\n\n", NB_EXP_TOT, NB_EXP_PI);

    printf("\e[1m");
    printf("\tComparaison avec la veleur de Pi définie\n\n");
    printf("\e[0m");

    printf("On a généré une valeur approché de ");
    printf("\e[1m");
    printf("Pi = %f\n", moyenneArith_pi);
    printf("\e[0m");
    printf("Et on compare avec la valeur de Pi de la librairie math.h\n");
    printf("Vraie valeur de ");
    printf("\e[1m");
    printf("Pi = %f\n\n", M_PI);
    printf("\e[0m");
    
    printf("Il y a donc une erreur absolue de %f\n\n", fabs(moyenneArith_pi - M_PI));

    double err_realtive = fabs((moyenneArith_pi - M_PI) / M_PI) * 100;
    printf("Il y a aussi une erreur relative de %f %%\n\n", err_realtive);


    return 0;
}

//  Permet de générer une valeur approché de Pi en fonction du nombre 
//  de tirages saisis dans long int exp, et renvoi une valeur approché de Pi
double SimuPi (long int exp){
    long int      i;
    long int      compteur = 0; 
    long double   x, y;

    for (i = 0; i < exp; i++) {
        x = genrand_real1();
        y = genrand_real1();

        if ((x*x + y*y) <= 1.0) {
            compteur ++;
        }
    }

    return ((long double) compteur / (long double) exp) * 4.0;
}

//  Permet le calcul de la moyenne arithmétique
//  Prend en entrée un tableau contenant des valeurs approchées du chiffre Pi
//  Et en retourne sa moyenne arithmétique
double MoyArithme(double *Tableau_Result, int nombre_ExpTot) {
    int     i;
    double  sum = 0;

    for (i = 0; i < nombre_ExpTot; i++) {
        sum += Tableau_Result[i];
    }

    return (sum / nombre_ExpTot);
}


//  Permet le calcul de la variance de l'échantillon
//  Prend en entrée un tableau contenant des valeurs approchées du chiffre Pi
//  Ainsi que la moyenne arithmétique et le nombre d'expériences totales
//  Renvoi la variance 
double Variance(double *Tableau_Result, double Moyenne_Arithmetique, int nombre_ExpTot) {
    double  new_sum = 0;
    double  result;
    int     i;

    for (i = 0; i < nombre_ExpTot; i++) {
        Tableau_Result[i] = pow((Tableau_Result[i] - Moyenne_Arithmetique), 2);
        new_sum += Tableau_Result[i];
    }

    result = new_sum / (Moyenne_Arithmetique - 1);
    return result;
}

//  Permet le calcul de l'écart type
//  Prend en entrée la variance 
//  Renvoi la racine carré de la variance, soit l'écart type
double EcartType (double variance) {
    return sqrt(variance);
}

//  Permet le calcul du rayon de confiance pour alpha = 1.96
//  Utilisé lors du calcul de l'intervalle de confiance
//  Prend en entrée l'écart type et le nombre d'expériences totales
//  Renvoi le rayon de confiance
double RayonConfiance(double ecart_Type, int nombre_ExpTot) {
    return (double)(1,96 * (ecart_Type / sqrt(nombre_ExpTot)));
}

/************************************************************
 *                                                          *
 *                Présentation des résultats                *
 *                                                          *
 ************************************************************/
/************************************************************


-----------------------------
Pour    NB_EXP_TOT 1000
        NB_EXP_PI 1000
-----------------------------


                        Etape 1 :
        Calcul de la moyenne arithmétique

Moyenne arithmétique de Pi = 3.141200


                        Etape 2 :
        Calcul de la variance S^2

Variance de Pi = 1.247973


                        Etape 3 :
        Calcul de l'Ecart-type

Ecart-type de Pi = 1.117127


                        Etape 4 :
        Calcul du rayon de confiance de Pi pour alpha = 0.05

Rayon de confiance de Pi = 1.117127


                        Etape 5 :
        Calcul de l'intervalle de confiance de Pi à 95 %


Intervalle de confiance de Pi = [-0.647380 ; 6.929780]


On vient de générer 1000 fois une simulation de Pi avec 1000 nombres aléatoires

        Comparaison avec la veleur de Pi définie

On a généré une valeur approché de Pi = 3.141200
Et on compare avec la valeur de Pi de la librairie math.h
Vraie valeur de Pi = 3.141593

Il y a donc une erreur absolue de 0.000393

Il y a aussi une erreur relative de 0.012499 %


-----------------------------
Pour    NB_EXP_TOT 1000
        NB_EXP_PI 1000000
-----------------------------


                        Etape 1 :
        Calcul de la moyenne arithmétique

Moyenne arithmétique de Pi = 3.141660


                        Etape 2 :
        Calcul de la variance S^2

Variance de Pi = 0.001226


                        Etape 3 :
        Calcul de l'Ecart-type

Ecart-type de Pi = 0.035007


                        Etape 4 :
        Calcul du rayon de confiance de Pi pour alpha = 0.05

Rayon de confiance de Pi = 0.035007


                        Etape 5 :
        Calcul de l'intervalle de confiance de Pi à 95 %


Intervalle de confiance de Pi = [3.137939 ; 3.145380]


On vient de générer 1000 fois une simulation de Pi avec 1000000 nombres aléatoires

        Comparaison avec la veleur de Pi définie

On a généré une valeur approché de Pi = 3.141660
Et on compare avec la valeur de Pi de la librairie math.h
Vraie valeur de Pi = 3.141593

Il y a donc une erreur absolue de 0.000067

Il y a aussi une erreur relative de 0.002128 %


-----------------------------
Pour    NB_EXP_TOT 100
        NB_EXP_PI 1000000000
-----------------------------


                        Etape 1 :
        Calcul de la moyenne arithmétique

Moyenne arithmétique de Pi = 3.141595


                        Etape 2 :
        Calcul de la variance S^2

Variance de Pi = 0.000000


                        Etape 3 :
        Calcul de l'Ecart-type

Ecart-type de Pi = 0.000309


                        Etape 4 :
        Calcul du rayon de confiance de Pi pour alpha = 0.05

Rayon de confiance de Pi = 0.000309


                        Etape 5 :
        Calcul de l'intervalle de confiance de Pi à 95 %


Intervalle de confiance de Pi = [3.141594 ; 3.141596]


On vient de générer 100 fois une simulation de Pi avec 1000000000 nombres aléatoires

        Comparaison avec la veleur de Pi définie

On a généré une valeur approché de Pi = 3.141595
Et on compare avec la valeur de Pi de la librairie math.h
Vraie valeur de Pi = 3.141593

Il y a donc une absolue relative de 0.000003

Il y a aussi une erreur relative de 0,000095 %


*************************************************************/

//  On remarque donc que plus on augmente le nombre de nombres aléatoires
//  utilisés pour faire la simulation du nombre Pi, plus la préision diminue
//  et ainsi, on obtiens un nombre Pi plus précis.
//  On peut aussi dire, que plus l'erreur absolue et l'erreur relative est faible,
//  meilleure est la précision du résultat

/************************************************************
 *                                                          *
 *                Bibliographie                             *
 *                                                          *
 ************************************************************/

//  https://webapps.fundp.ac.be/didactique/introTP/ch4/ch4_3.php
//  http://cybertim.timone.univ-mrs.fr/enseignement/doc-enseignement/statistiques/MasterEstimation-RG/docpeda_fichier
//  https://dpt-info.di.unistra.fr/~c.michel/Cours_Statistiques_Resume.pdf
//  https://www.statisticshowto.datasciencecentral.com/probability-and-statistics/standard-deviation/
