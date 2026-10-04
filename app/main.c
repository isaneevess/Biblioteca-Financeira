#include <stdio.h>
#include "Juros.h"

int main(void) {
    printf("Juros simples: %.2f\n", juros_simples(1000.0, 0.05, 3));
    return 0;
}
