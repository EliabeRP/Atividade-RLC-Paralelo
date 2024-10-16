#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define QUILO 1e3
#define MILI 1e-3
#define MICRO 1e-6
#define NANO 1e-9

double A1 , A2 , B1, B2;

// Função para calcular o determinante de uma matriz 2x2
double determinant(double a, double b, double c, double d) {
    return (a * d) - (b * c);
}

void solveEquation(double a1, double b1, double c1, double a2, double b2, double c2, int sub) {
    // Determinante da matriz dos coeficientes
    double D = determinant(a1, b1, a2, b2);

    // Determinantes das matrizes modificadas
    double D1 = determinant(c1, b1, c2, b2);
    double D2 = determinant(a1, c1, a2, c2);

    // Soluções usando a regra de Cramer
    A1 = D1 / D;
    A2 = D2 / D;

    // Exibir as soluções (caso subamortecido)
    if (sub) {
        B1 = A1;
        B2 = A2;
        printf("B1 = %.2f, ", B1);
        printf("B2 = %.2f\n", B2);
        return;
    }

    // Soluções A1 e A2 para os demais casos
    printf("A1 = %.2f, ", A1);
    printf("A2 = %.2f\n", A2);
    return;
}

//Circuito RLC paralelo

int main(){

    // Array das unidades
    double valores[] = { 1, NANO, MICRO, MILI, QUILO };

    // Declaração das Variáveis

    double resistencia, capacitancia, indutancia;
    double tensaoC, correnteI;
    double sigma = 0, omega = 0;
    double omegad = 0;
    double tm, vtm;
    int unidade = 0;

    // Créditos

    printf("\n");
    printf("\033[1;92mPROGRAMA CRIADO POR: \n");
    printf("Arthur Maximus Cavalcante Beuttenmuller\n");
    printf("Eliabe Rodrigues Pereira\033[m\n");
    printf("\n");
  
    int aux = 1;
    while(aux){

        printf("\033[1;31mATENCAO: DIGITE APENAS NUMEROS\n");
        printf("DIGITE UM NUMERO ENTRE 1 E 5 AO INDICAR A UNIDADE\n");
        printf("DIGITE QUALQUER VALOR (NUMERO) PARA A UNIDADE DO COMPONENTE, CASO SEU VALOR SEJA 0 \033[m\n");
        printf("\n");

        // Resistencia, Capacitancia, Indutância

        printf("Digite o valor da Resistencia: ");
        scanf("%lf", &resistencia);
        printf("Indique a unidade (1 - Ohm - 2 - nOhm, 3 - microOhm, 4 - mOhm, 5 - kOhm): ");
        scanf("%d", &unidade);
        resistencia *= valores[unidade-1];

        printf("\nDigite o valor da Capacitancia: ");
        scanf("%lf", &capacitancia);
        printf("Indique a unidade (1 - F, 2 - nF, 3 - microF, 4 - mF, 5 - kF): ");
        scanf("%d", &unidade);
        capacitancia *= valores[unidade-1];
        
        printf("\nDigite o valor da Indutancia: ");
        scanf("%lf", &indutancia);
        printf("Indique a unidade (1 - H, 2 - nH, 3 - microH, 4 - mH, 5 - kH): ");
        scanf("%d", &unidade);
        indutancia *= valores[unidade-1];

        
        // Condições Iniciais

        printf("\nQual o valor da Tensao Inicial no capacitor? ");
        scanf("%lf", &tensaoC);
        printf("Indique a unidade (1 - V, 2 - nV, 3 - microV, 4 - mV, 5 - kV): ");
        scanf("%d", &unidade);
        tensaoC *= valores[unidade-1];
        
        printf("\nQual o valor da Corrente Inicial no indutor? ");
        scanf("%lf", &correnteI);
        printf("Indique a unidade (1 - A, 2 - nA, 3 - microA, 4 - mA, 5 - kA): ");
        scanf("%d", &unidade);
        correnteI *= valores[unidade-1];


        // Variaveis iniciais determinadas:

        double Ir = 0, Ic = 0;

        Ir = (tensaoC/resistencia); // Corrente no resistor

        Ic = (correnteI - Ir); // Corrente no capacitor seguindo o circuito pré-determinado
        

        // Cálculo de Sigma e Omega

        sigma = (1 / (2 * resistencia * capacitancia));

        omega = (1 / sqrt(capacitancia * indutancia));


        printf("\n\033[1;92mResultados da resposta natural de Circuitos RLC PARALELO\n\033[m"); // Cor verde

        // Circuito Superamortecido

        if (sigma > omega) {
            // Cálculo de s1 e s2
            double s1 = 0, s2 = 0;

            s1 = -sigma + sqrt(pow(sigma, 2) - pow(omega, 2));
            s2 = -sigma - sqrt(pow(sigma, 2) - pow(omega, 2));
                
            printf("\nCircuito Superamortecido\n");
            printf("Sigma: %.2lf s^-1\n", sigma);
            printf("Omega: %.2lf rad/s\n", omega);
            printf("s1: %.2lf, s2: %.2lf\n", s1, s2);
            solveEquation(1, 1, tensaoC, s1, s2, Ic/capacitancia, 0); // Resolvendo o sistema 2x2

            tm = (log(fabs(s1*A1)) - log(fabs(s2*A2)))/(s2 - s1); // Fórmula para tm do Circuito Superamortecido
            printf("tm: %.10lf s\n", tm);

            vtm = A1*exp(s1*tm) + A2*exp(s2*tm); // Substituição de tm em v(t) para descobrir o vtm
            printf("vtm: %.10lf V\n", vtm);
        }

        // Circuito Criticamente Amortecido

        if (sigma == omega) {
            printf("\nCircuito Criticamente amortecido\n");
            printf("Sigma: %.2lf s^-1\n", sigma);
            printf("Omega: %.2lf rad/s\n", omega);
            solveEquation(0, 1, tensaoC, 1, -1*(sigma), Ic/capacitancia, 0); // Resolvendo o sistema 2x2
                    
            tm = ((A2/A1*sigma) - 1)/(-1*(sigma)); // Fórmula para tm do Circuito Superamortecido
            printf("tm: %.10lf s\n", tm);

            vtm = (A1*tm + A2) * exp(-1 * sigma * tm); // Substituição de tm em v(t) para descobrir o vtm
            printf("vtm: %.10lf V\n", vtm);
        }

        // Circuito Subamortecido
        
        if (sigma < omega){
            omegad = sqrt(pow(omega, 2) - pow(sigma, 2));

            printf("\nCircuito Subamortecido\n");
            printf("Sigma: %.2lf s^-1\n", sigma);
            printf("Omega: %.2lf rad/s\n", omega);
            printf("Omegad: %.2lf rad/s\n", omegad);
            solveEquation(1, 0, tensaoC, -1*(sigma), omegad, Ic/capacitancia, 1); // Resolvendo o sistema 2x2

            tm = (atan((B2 * omegad - B1 * sigma) / (B1 * omegad + B2 * sigma)))/omegad; // Fórmula para tm do Circuito Superamortecido
            
            if (tm < 0) tm += M_PI/omegad; // Caso tm seja negativo, soma-se meio período. M_PI refere-se a PI (3,141592).
            
            printf("tm: %.10lf s\n", tm);

            vtm = exp( -1 * sigma * tm) * (B1 * cos(omegad * tm) + B2 * sin(omegad * tm));  // Substituição de tm em v(t) para descobrir o vtm
            printf("vtm: %.10lf V\n", vtm);
        }
        
        // Verificação de novo cálculo
        char verif;
        printf("\n");
        printf("\033[1;33mVOCE DESEJA REALIZAR UM NOVO CALCULO? (DIGITE APENAS s OU n) \033[m");
        scanf(" %c", &verif);
        if (verif == 'n') aux = 0;
        printf("\n");
    }  

    printf("\033[1;36mPrograma encerrado!\033[m");

    return 0;
}
