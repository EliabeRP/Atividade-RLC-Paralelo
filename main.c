#include <stdio.h>
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

    // Exibir as soluções
    if (sub) {
        B1 = A1;
        B2 = A2;
        printf("B1 = %.2f, ", B1);
        printf("B2 = %.2f\n", B2);
        return;
    }

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

   
    // Resistencia, Capacitancia, Indutância  

    printf("Digite o valor da Resistencia: ");
    scanf("%lf", &resistencia);
    printf("Indique a unidade (1 - Ohm - 2 - nOhm, 3 - microOhm, 4 - mOhm, 5 - kOhm): \n");
    scanf("%d", &unidade);
    resistencia *= valores[unidade-1];

    printf("Digite o valor da Capacitancia: ");
    scanf("%lf", &capacitancia);
    printf("Indique a unidade (1 - F, 2 - nF, 3 - microF, 4 - mF, 5 - kF): \n");
    scanf("%d", &unidade);
    capacitancia *= valores[unidade-1];
    
    printf("Digite o valor da Indutancia: ");
    scanf("%lf", &indutancia);
    printf("Indique a unidade (1 - H, 2 - nH, 3 - microH, 4 - mH, 5 - kH): \n");
    scanf("%d", &unidade);
    indutancia *= valores[unidade-1];

    
    // Condições Iniciais

    printf("Qual o valor da Tensao Inicial no capacitor? ");
    scanf("%lf", &tensaoC);
    printf("Indique a unidade (1 - V, 2 - nV, 3 - microV, 4 - mV, 5 - kV): \n");
    scanf("%d", &unidade);
    tensaoC *= valores[unidade-1];
    
    printf("Qual o valor da Corrente Inicial no indutor? ");
    scanf("%lf", &correnteI);
    printf("Indique a unidade (1 - A, 2 - nA, 3 - microA, 4 - mA, 5 - kA): \n");
    scanf("%d", &unidade);
    correnteI *= valores[unidade-1];


    // Variaveis iniciais determinadas:

    double Ir = 0, Ic = 0;

    Ir = (tensaoC/resistencia);

    Ic = - (correnteI + Ir);
    

    // Cálculo de Sigma e Omega

    sigma = (1 / (2 * resistencia * capacitancia));

    omega = (1 / sqrt(capacitancia * indutancia));


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
        solveEquation(1, 1, tensaoC, s1, s2, Ic/capacitancia, 0);

        tm = (log(fabs(s1*A1)) - log(fabs(s2*A2)))/(s2 - s1);
        printf("tm: %.10lf s\n", tm);

        vtm = A1*exp(s1*tm) + A2*exp(s2*tm);
        printf("vtm: %.2lf V\n", vtm);

        return 0;
    }

    // Circuito Criticamente Amortecido

    if (sigma == omega) {
        printf("\nCircuito Criticamente amortecido\n");
        printf("Sigma: %.2lf s^-1\n", sigma);
        printf("Omega: %.2lf rad/s\n", omega);
        solveEquation(0, 1, tensaoC, 1, -1*(sigma), Ic/capacitancia, 0);
                
        tm = ((A2/A1*sigma) - 1)/(-1*(sigma));
        printf("tm: %.10lf s\n", tm);

        vtm = (A1*tm + A2) * exp(-1 * sigma * tm);
        printf("vtm: %.2lf V\n", vtm);

        return 0;
    }

    // Circuito Subamortecido
    
    omegad = sqrt(pow(omega, 2) - pow(sigma, 2));

    printf("\nCircuito Subamortecido\n");
    printf("Sigma: %.2lf s^-1\n", sigma);
    printf("Omega: %.2lf rad/s\n", omega);
    printf("Omegad: %.2lf rad/s\n", omegad);
    solveEquation(1, 0, tensaoC, -1*(sigma), omegad, Ic/capacitancia, 1);

    tm = (atan((B2 * omegad - B1 * sigma) / (B1 * omegad + B2 * sigma)))/omegad;
    printf("tm: %.10lf s\n", tm);

    vtm = exp( -1 * sigma * tm) * (B1 * cos(omegad * tm) + B2 * sin(omegad * tm)); 
    printf("vtm: %.2lf V\n", vtm);

    return 0;
}
